/*
                              PINE 64 - C++ lib                                            1                        
 Copyright (c) 2017 Daniele Contarino <daniele.contatino@studium.unict.it>               i111i                      
 This library is based over work by:                                                  .ft  .;  .tf.                  
     Eric Ptak <trouch@trouch.com>                                                    ft. ,fft. .ff                  
     Stefan Mavrodiev @ OLIMEX LTD <support@olimex.com>                               1fffffffffff1                  
     Kamil Trzcinski <ayufan@ayufan.eu>                                           ,LLf. ,fffffff, ,LLL,              
Permission is hereby granted, free of charge, to any person obtaining a copy of   ,LLLLLLLi   iLLLLLLL,              
this software and associated documentation files (the "Software"), to deal in     ,LLL;  1LLLLLi  ;LLL,              
the Software without restriction, including without limitation the rights to        :LLLLLLLLLLLLLLL:                
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies    .LLL;  1LLLLLLLi  ;LLL              
of the Software, and to permit persons to whom the Software is furnished to do    :LLLLLLL:   :LLLLLLL,              
so, subject to the following conditions:                                           iLLf. :LLLLL, .LLL;               
The above copyright notice and this permission notice shall be included in all       1LLLLLLLLLLLLLi                 
copies or substantial portions of the Software.                                          :LLLLL:                     
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                  L                        
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                    L      
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "gpio.h"

using namespace Pine64;

GPIO::GPIO() {
    this->initialiseEpoch();
    this->_pinea64Found = 1;
}

GPIO::~GPIO(void){ }

int GPIO::setup(void) {
    int mem_fd;
    uint8_t *gpio_mem;
    uint32_t peri_base;
    uint32_t gpio_base;
    unsigned char buf[4];
    FILE *fp;
    char buffer[1024];
    char hardware[1024];
    int found = 0;

    if(!this->_pinea64Found) {
        // try /dev/gpiomem first - this does not require root privs
        if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) > 0) {
            this->_gpioMap = (uint32_t *)mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
            return ((uint64_t)this->_gpioMap < 0) ? SETUP_MMAP_FAIL : SETUP_OK;
        }

        // revert to /dev/mem method - requires root
        // determine peri_base
        if ((fp = fopen("/proc/device-tree/soc/ranges", "rb")) != NULL) {
            // get peri base from device tree
            fseek(fp, 4, SEEK_SET);
            if (fread(buf, 1, sizeof buf, fp) == sizeof buf) {
                peri_base = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
            }
            fclose(fp);
        } else {
            // guess peri base based on /proc/cpuinfo hardware field
            if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
                return SETUP_CPUINFO_FAIL;

            while(!feof(fp) && !found) {
                fgets(buffer, sizeof(buffer), fp);
                sscanf(buffer, "Hardware	: %s", hardware);
                if (strcmp(hardware, "BCM2708") == 0 || strcmp(hardware, "BCM2835") == 0) {
                    // pi 1 hardware
                    peri_base = BCM2708_PERI_BASE_DEFAULT;
                    found = 1;
                } else if (strcmp(hardware, "BCM2709") == 0 || strcmp(hardware, "BCM2836") == 0) {
                    // pi 2 hardware
                    peri_base = BCM2709_PERI_BASE_DEFAULT;
                    found = 1;
                }
            }
            fclose(fp);
            if (!found)
                return SETUP_NOT_RPI_FAIL;
        }

        gpio_base = peri_base + GPIO_BASE_OFFSET;
    }
    
    // mmap the GPIO memory registers
    if((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
        return SETUP_DEVMEM_FAIL;

    if((gpio_mem = (uint8_t *)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL)
        return SETUP_MALLOC_FAIL;

    if((uint64_t)gpio_mem % PAGE_SIZE)
        gpio_mem += (uint64_t)(PAGE_SIZE - ((uint64_t)gpio_mem % PAGE_SIZE));

    if(this->_pinea64Found) {
        this->_gpioMap = (uint32_t *)mmap( (caddr_t)gpio_mem, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, mem_fd, SUNXI_GPIO_BASE);
        this->_pioMap = this->_gpioMap + (SUNXI_GPIO_REG_OFFSET>>2);
    } else {
        this->_gpioMap = (uint32_t *)mmap( (void *)gpio_mem, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, mem_fd, gpio_base);
    }

    if ((uint64_t)this->_gpioMap < 0)
        return SETUP_MMAP_FAIL;

    return SETUP_OK;
}

//Manage GPIO
void GPIO::pinMode(int gpio, int direction) {
    this->pinMode(gpio, direction, PUD_UP);
}

void GPIO::pinMode(int gpio, int direction, int pud) {
    if(this->_pinea64Found) {
        uint32_t regval = 0;
        int bank = GPIO_BANK(gpio); //gpio >> 5
        int index = GPIO_CFG_INDEX(gpio); // (gpio & 0x1F) >> 3
        int offset = GPIO_CFG_OFFSET(gpio); // ((gpio & 0x1F) & 0x7) << 2

        sunxiGPIO *pio = &((sunxiGPIOReg *) this->_pioMap)->gpio_bank[bank];

        this->_setPullupdn(gpio, pud);

        regval = *(&pio->CFG[0] + index);
        regval &= ~(0x7 << offset); // 0xf?
        if (INPUT == direction) {
            *(&pio->CFG[0] + index) = regval;
        } else if (OUTPUT == direction) {
            regval |=  (1 << offset);
            *(&pio->CFG[0] + index) = regval;
        } else {
            printf("line:%dgpio number error\n",__LINE__);
        }
    } else {
        int offset = FSEL_OFFSET + (gpio/10);
        int shift = (gpio%10)*3;

        this->_setPullupdn(gpio, pud);
        if (direction == OUTPUT)
            *(this->_gpioMap+offset) = (*(this->_gpioMap+offset) & ~(7<<shift)) | (1<<shift);
        else  // direction == INPUT
            *(this->_gpioMap+offset) = (*(this->_gpioMap+offset) & ~(7<<shift));
    }
}

int GPIO::digitalRead(int gpio) {
    if(this->_pinea64Found) {
        uint32_t regval = 0;
        int bank = GPIO_BANK(gpio); //gpio >> 5
        int num = GPIO_NUM(gpio); // gpio & 0x1F

        sunxiGPIO *pio = &((sunxiGPIOReg *) this->_pioMap)->gpio_bank[bank];

        regval = *(&pio->DAT);
        regval = regval >> num;
        regval &= 1;
        return regval;
    } else {
       int offset, value, mask;

       offset = PINLEVEL_OFFSET + (gpio/32);
       mask = (1 << gpio%32);
       value = *(this->_gpioMap+offset) & mask;
       return value;
    }
}

void GPIO::digitalWrite(int gpio, int value) {
    if(this->_pinea64Found) {
        int bank = GPIO_BANK(gpio); //gpio >> 5
        int num = GPIO_NUM(gpio); // gpio & 0x1F

        sunxiGPIO *pio = &((sunxiGPIOReg *) this->_pioMap)->gpio_bank[bank];

        if (value == 0)
            *(&pio->DAT) &= ~(1 << num);
        else
            *(&pio->DAT) |= (1 << num);
    } else {
        int offset, shift;

        if (value) // value == HIGH
            offset = SET_OFFSET + (gpio/32);
        else       // value == LOW
           offset = CLR_OFFSET + (gpio/32);

        shift = (gpio%32);

        *(this->_gpioMap+offset) = 1 << shift;
    }
}

int GPIO::GPIOFunction(int gpio) {
    if(this->_pinea64Found) {
        uint32_t regval = 0;
        int bank = GPIO_BANK(gpio); //gpio >> 5
        int index = GPIO_CFG_INDEX(gpio); // (gpio & 0x1F) >> 3
        int offset = GPIO_CFG_OFFSET(gpio); // ((gpio & 0x1F) & 0x7) << 2

        sunxiGPIO *pio = &((sunxiGPIOReg *) this->_pioMap)->gpio_bank[bank];

        regval = *(&pio->CFG[0] + index);
        regval >>= offset;
        regval &= 7;
        return regval; // 0=input, 1=output, 4=alt0
    } else {
        int offset = FSEL_OFFSET + (gpio/10);
        int shift = (gpio%10)*3;
        int value = *(this->_gpioMap+offset);
        value >>= shift;
        value &= 7;
        return value; // 0=input, 1=output, 4=alt0
    }
}

//Event
int GPIO::eventDetected(int gpio) {
    if(!this->_pinea64Found) {
        int offset, value, bit;

        offset = EVENT_DETECT_OFFSET + (gpio/32);
        bit = (1 << (gpio%32));
        value = *(this->_gpioMap+offset) & bit;
        if (value)
            this->_clearEventDetect(gpio);
        return value;
    }
    return -1;
}

void GPIO::setRisingEvent(int gpio, int enable) {
  if(!this->_pinea64Found) {
    int offset = RISING_ED_OFFSET + (gpio/32);
    int shift = (gpio%32);

    if (enable)
        *(this->_gpioMap+offset) |= 1 << shift;
    else
        *(this->_gpioMap+offset) &= ~(1 << shift);
    this->_clearEventDetect(gpio);
  }
}

void GPIO::setFallingEvent(int gpio, int enable) {
  if(!this->_pinea64Found) {
    int offset = FALLING_ED_OFFSET + (gpio/32);
    int shift = (gpio%32);

    if (enable) {
        *(this->_gpioMap+offset) |= (1 << shift);
        *(this->_gpioMap+offset) = (1 << shift);
    } else {
        *(this->_gpioMap+offset) &= ~(1 << shift);
    }
    this->_clearEventDetect(gpio);
  }
}

void GPIO::setHighEvent(int gpio, int enable) {
  if(!this->_pinea64Found) {
    int offset = HIGH_DETECT_OFFSET + (gpio/32);
    int shift = (gpio%32);

    if (enable)
        *(this->_gpioMap+offset) |= (1 << shift);
    else
        *(this->_gpioMap+offset) &= ~(1 << shift);
    this->_clearEventDetect(gpio);
  }
}

void GPIO::setLowEvent(int gpio, int enable) {
  if(!this->_pinea64Found) {
    int offset = LOW_DETECT_OFFSET + (gpio/32);
    int shift = (gpio%32);

    if (enable)
        *(this->_gpioMap+offset) |= 1 << shift;
    else
        *(this->_gpioMap+offset) &= ~(1 << shift);
    this->_clearEventDetect(gpio);
  }
}

void GPIO::cleanup(void) {
    munmap((void *)this->_gpioMap, BLOCK_SIZE);
}

// Time
void GPIO::initialiseEpoch (void) {
    struct timeval tv ;

    gettimeofday (&tv, NULL) ;
    this->_epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000) ;
    this->_epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec) ;
}

void GPIO::delayMicrosecondsHard (unsigned int howLong) {
    struct timeval tNow, tLong, tEnd ;

    gettimeofday (&tNow, NULL) ;
    tLong.tv_sec  = howLong / 1000000 ;
    tLong.tv_usec = howLong % 1000000 ;
    timeradd (&tNow, &tLong, &tEnd) ;

    while (timercmp (&tNow, &tEnd, <))
        gettimeofday (&tNow, NULL) ;
}

void GPIO::delayMicroseconds (unsigned int howLong) {
    struct timespec sleeper ;
    unsigned int uSecs = howLong % 1000000 ;
    unsigned int wSecs = howLong / 1000000 ;

    if (howLong == 0)
        return ;
        
    else if (howLong < 100)
        GPIO::delayMicrosecondsHard (howLong) ;
        
    else {
        sleeper.tv_sec  = wSecs ;
        sleeper.tv_nsec = (long)(uSecs * 1000L) ;
        nanosleep (&sleeper, NULL) ;
    }
}

void GPIO::delay (unsigned int howLong) {
    struct timespec sleeper, dummy ;

    sleeper.tv_sec  = (time_t)(howLong / 1000) ;
    sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

    nanosleep (&sleeper, &dummy) ;
}

unsigned int GPIO::millis (void) {
    struct timeval tv ;
    uint64_t now ;

    gettimeofday (&tv, NULL) ;
    now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

    return (uint32_t)(now - this->_epochMilli) ;
}

unsigned int GPIO::micros (void) {
    struct timeval tv ;
    uint64_t now ;

    gettimeofday (&tv, NULL) ;
    now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;

    return (uint32_t)(now - this->_epochMicro) ;
}

// Private methods
void GPIO::_shortWait(void){
    for (int i=0; i<150; i++)    // wait 150 cycles
        asm volatile("nop");
}

void GPIO::_clearEventDetect(int gpio) {
  if(!this->_pinea64Found)  {
    int offset = EVENT_DETECT_OFFSET + (gpio/32);
    int shift = (gpio%32);

    *(this->_gpioMap+offset) |= (1 << shift);
    this->_shortWait();
    *(this->_gpioMap+offset) = 0;
  }
}

uint32_t GPIO::_sunxiReadl(volatile uint32_t *addr) {
    uint32_t val = 0;
    uint32_t mmap_base = (uint64_t)addr & (~MAP_MASK);
    uint32_t mmap_seek = ((uint64_t)addr - mmap_base) >> 2;
    val = *(this->_gpioMap + mmap_seek);
    return val;
}

void GPIO::_sunxiWritel(volatile uint32_t *addr, uint32_t val) {
    uint64_t mmap_base = (uint64_t)addr & (~MAP_MASK);
    uint64_t mmap_seek =( (uint64_t)addr - mmap_base) >> 2;
    *(this->_gpioMap + mmap_seek) = val;
}

void GPIO::_setPullupdn(int gpio, int pud) {
  if(this->_pinea64Found) {
    uint32_t regval = 0;
    int bank = GPIO_BANK(gpio); //gpio >> 5
    int index = GPIO_PUL_INDEX(gpio); // (gpio & 0x1f) >> 4
    int offset = GPIO_PUL_OFFSET(gpio); // (gpio) & 0x0F) << 1

    sunxiGPIO *pio = &((sunxiGPIOReg *) this->_pioMap)->gpio_bank[bank];

    regval = *(&pio->PULL[0] + index);
    regval &= ~(3 << offset);
    regval |= pud << offset;
    *(&pio->PULL[0] + index) = regval;
  } else {
    int clk_offset = PULLUPDNCLK_OFFSET + (gpio/32);
    int shift = (gpio%32);

    if (pud == PUD_DOWN)
        *(this->_gpioMap+PULLUPDN_OFFSET) = (*(this->_gpioMap+PULLUPDN_OFFSET) & ~3) | PUD_DOWN;
    else if (pud == PUD_UP)
        *(this->_gpioMap+PULLUPDN_OFFSET) = (*(this->_gpioMap+PULLUPDN_OFFSET) & ~3) | PUD_UP;
    else  // pud == PUD_OFF
        *(this->_gpioMap+PULLUPDN_OFFSET) &= ~3;

    this->_shortWait();
    *(this->_gpioMap+clk_offset) = 1 << shift;
    this->_shortWait();
    *(this->_gpioMap+PULLUPDN_OFFSET) &= ~3;
    *(this->_gpioMap+clk_offset) = 0;
  }
}
