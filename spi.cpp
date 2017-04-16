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
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

using namespace Pine64;

SPI::SPI() { 
    this->_fd = -1;
}

SPI::~SPI(void){
    this->dispose();
}

int SPI::setup(char *device, spi_config_t config) {
    /* Open block device */
    this->_fd = open(device, O_RDWR);
    if (this->_fd < 0) {
        return this->_fd;
    }

    /* Set SPI_POL and SPI_PHA */
    if (ioctl(this->_fd, SPI_IOC_WR_MODE, &config.mode) < 0) {
        return -1;
    }
    if (ioctl(this->_fd, SPI_IOC_RD_MODE, &config.mode) < 0) {
        return -1;
    }

    /* Set bits per word*/
    if (ioctl(this->_fd, SPI_IOC_WR_BITS_PER_WORD, &config.bits_per_word) < 0) {
        return -1;
    }
    if (ioctl(this->_fd, SPI_IOC_RD_BITS_PER_WORD, &config.bits_per_word) < 0) {
        return -1;
    }

    /* Set SPI speed*/
    if (ioctl(this->_fd, SPI_IOC_WR_MAX_SPEED_HZ, &config.speed) < 0) {
        return -1;
    }
    if (ioctl(this->_fd, SPI_IOC_RD_MAX_SPEED_HZ, &config.speed) < 0) {
        return -1;
    }

    /* Return file descriptor */
    return this->_fd;
}

int SPI::dispose() {
    return close(this->_fd);
}

int SPI::xfer(uint8_t *tx_buffer, uint8_t tx_len, uint8_t *rx_buffer, uint8_t rx_len){
    struct spi_ioc_transfer spi_message[1];
    memset(spi_message, 0, sizeof(spi_message));

    spi_message[0].rx_buf = (unsigned long)rx_buffer;
    spi_message[0].tx_buf = (unsigned long)tx_buffer;
    spi_message[0].len = tx_len;

    return ioctl(this->_fd, SPI_IOC_MESSAGE(1), spi_message);
}

int SPI::recv(uint8_t *rx_buffer, uint8_t rx_len){
    struct spi_ioc_transfer spi_message[1];
    memset(spi_message, 0, sizeof(spi_message));

    spi_message[0].rx_buf = (unsigned long)rx_buffer;
    spi_message[0].len = rx_len;

    return ioctl(this->_fd, SPI_IOC_MESSAGE(1), spi_message);
}

int SPI::send(uint8_t *tx_buffer, uint8_t tx_len){
    struct spi_ioc_transfer spi_message[1];
    memset(spi_message, 0, sizeof(spi_message));

    spi_message[0].tx_buf = (unsigned long)tx_buffer;
    spi_message[0].len = tx_len;

    return ioctl(this->_fd, SPI_IOC_MESSAGE(1), spi_message);
}
