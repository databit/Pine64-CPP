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
#ifndef PINE64_GPIO_H
#define PINE64_GPIO_H

#define BCM2708_PERI_BASE_DEFAULT   0x20000000
#define BCM2709_PERI_BASE_DEFAULT   0x3f000000
#define GPIO_BASE_OFFSET            0x200000
#define FSEL_OFFSET                 0   // 0x0000
#define SET_OFFSET                  7   // 0x001c / 4
#define CLR_OFFSET                  10  // 0x0028 / 4
#define PINLEVEL_OFFSET             13  // 0x0034 / 4
#define EVENT_DETECT_OFFSET         16  // 0x0040 / 4
#define RISING_ED_OFFSET            19  // 0x004c / 4
#define FALLING_ED_OFFSET           22  // 0x0058 / 4
#define HIGH_DETECT_OFFSET          25  // 0x0064 / 4
#define LOW_DETECT_OFFSET           28  // 0x0070 / 4
#define PULLUPDN_OFFSET             37  // 0x0094 / 4
#define PULLUPDNCLK_OFFSET          38  // 0x0098 / 4

#define PAGE_SIZE  (4*1024)
#define BLOCK_SIZE (4*1024)

#define PINEA64_GPIO_MASK	(0xFFFFFF80)
#define SUNXI_GPIO_BASE		0x01C20000
#define SUNXI_GPIO_REG_OFFSET   0x800
#define PINEA64_GPIO_BASE	(SUNXI_GPIO_BASE + SUNXI_GPIO_REG_OFFSET)
#define SUNXI_CFG_OFFSET	0x00
#define SUNXI_DATA_OFFSET	0x10
#define SUNXI_PUD_OFFSET	0x1C
#define SUNXI_BANK_SIZE		0x24

#define MAP_SIZE        (4096*2)
#define MAP_MASK        (MAP_SIZE - 1)

#define GPIO_BANK(pin)  ((pin) >> 5)
#define GPIO_NUM(pin)   ((pin) & 0x1F)

#define GPIO_CFG_INDEX(pin)     (((pin) & 0x1F) >> 3)
#define GPIO_CFG_OFFSET(pin)    ((((pin) & 0x1F) & 0x7) << 2)

#define GPIO_PUL_INDEX(pin)     (((pin) & 0x1F )>> 4)
#define GPIO_PUL_OFFSET(pin)    (((pin) & 0x0F) << 1)

#define SETUP_OK           0
#define SETUP_DEVMEM_FAIL  1
#define SETUP_MALLOC_FAIL  2
#define SETUP_MMAP_FAIL    3
#define SETUP_CPUINFO_FAIL 4
#define SETUP_NOT_RPI_FAIL 5

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

#define HIGH 1
#define LOW  0

#define PUD_OFF  0
#define PUD_UP   1
#define PUD_DOWN 2

namespace Pine64 {
    typedef struct sunxiGPIO_t {
        unsigned int CFG[4];
        unsigned int DAT;
        unsigned int DRV[2];
        unsigned int PULL[2];
    } sunxiGPIO;

    /* gpio interrupt control */
    typedef struct sunxiGPIOInt_t {
        unsigned int CFG[3];
        unsigned int CTL;
        unsigned int STA;
        unsigned int DEB;
    } ssunxiGPIOInt;

    typedef struct sunxiGPIOReg_t {
        struct sunxiGPIO_t gpio_bank[9];
        unsigned char res[0xbc];
        struct sunxiGPIOInt_t gpio_int;
    } sunxiGPIOReg;

    class GPIO {
        private : 
            uint64_t _epochMilli, _epochMicro;
            volatile uint32_t* _pioMap;
            volatile uint32_t* _gpioMap;
            int _pinea64Found;

            static void _shortWait(void) ;
            void _clearEventDetect(int gpio);
            uint32_t _sunxiReadl(volatile uint32_t *addr);
            void _sunxiWritel(volatile uint32_t *addr, uint32_t val);
            void _setPullupdn(int gpio, int pud);

        public : 
            GPIO();
            ~GPIO(void);
            
            //Manage GPIO
            int setup(void);
            void pinMode(int gpio, int direction);
            void pinMode(int gpio, int direction, int pud);
            int digitalRead(int gpio);
            void digitalWrite(int gpio, int value);
            int GPIOFunction(int gpio);
            
            //Event
            void setRisingEvent(int gpio, int enable);
            void setFallingEvent(int gpio, int enable);
            void setHighEvent(int gpio, int enable);
            void setLowEvent(int gpio, int enable);
            int eventDetected(int gpio);
            void cleanup(void);
            
            //Time function
            void initialiseEpoch (void);
            static void delayMicrosecondsHard (unsigned int howLong);
            static void delayMicroseconds (unsigned int howLong);
            static void delay (unsigned int howLong);
            unsigned int millis (void);
            unsigned int micros (void);
    };
}

#endif