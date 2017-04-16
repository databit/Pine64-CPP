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
#ifndef PINE64_SPI_H
#define PINE64_SPI_H

namespace Pine64 {
    typedef struct {
        uint8_t mode;
        uint8_t bits_per_word;
        uint32_t speed;
        uint16_t delay;
    } spi_config_t;

    class SPI {
        private : 
            int _fd;
            
        public : 
            SPI(void);
            ~SPI(void);
            int setup(char *device, spi_config_t config);
            int dispose();
            int xfer(uint8_t *tx_buffer, uint8_t tx_len, uint8_t *rx_buffer, uint8_t rx_len);
            int recv(uint8_t *rx_buffer, uint8_t rx_len);
            int send(uint8_t *tx_buffer, uint8_t tx_len);
    };
}
#endif
