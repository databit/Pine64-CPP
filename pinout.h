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
#ifndef PINE64_PINOUT_H
#define PINE64_PINOUT_H

// Pinout configuration
// http://joey.hazlett.us/pine64/pine64_pins.html

namespace Pine64 {
    const short PINE_A13 =  13; // ????
    const short PINE_B0 =   32;
    const short PINE_B1 =   33;
    const short PINE_B2 =   34;
    const short PINE_B3 =   35;
    const short PINE_B4 =   36;
    const short PINE_B5 =   37;
    const short PINE_B6 =   38;
    const short PINE_B7 =   39;
    const short PINE_B8 =   40;
    const short PINE_B9 =   41;
    const short PINE_B13 =  45; // ???? 
    const short PINE_C0 =   64;
    const short PINE_C1 =   65;
    const short PINE_C2 =   66;
    const short PINE_C3 =   67;
    const short PINE_C4 =   68;
    const short PINE_C5 =   69;
    const short PINE_C6 =   70;
    const short PINE_C7 =   71;
    const short PINE_C8 =   72;
    const short PINE_C9 =   73;
    const short PINE_C10 =  74;
    const short PINE_C11 =  75;
    const short PINE_C12 =  76;
    const short PINE_C13 =  77;
    const short PINE_C14 =  78;
    const short PINE_C15 =  79;
    const short PINE_C16 =  80;
    const short PINE_D0 =   96;
    const short PINE_D1 =   97;
    const short PINE_D2 =   98;
    const short PINE_D3 =   99;
    const short PINE_D4 =   100;
    const short PINE_D5 =   101;
    const short PINE_D6 =   102;
    const short PINE_D7 =   103;
    const short PINE_H2 =   226;
    const short PINE_H3 =   227;
    const short PINE_H5 =   229;
    const short PINE_H6 =   230;
    const short PINE_H7 =   231;
    const short PINE_H8 =   104;
    const short PINE_H9 =   233;
    const short PINE_L10 =  362;
    const short PINE_L11 =  363;

    // PI 2 BUS
    const short PI_GPIO_2 =  PINE_H3;
    const short PI_GPIO_3 =  PINE_H2;
    const short PI_GPIO_4 =  PINE_L10;
    const short PI_GPIO_5 =  PINE_H5;
    const short PI_GPIO_6 =  PINE_H6;
    const short PI_GPIO_7 =  PINE_H7;
    const short PI_GPIO_8 =  PINE_C3;
    const short PI_GPIO_9 =  PINE_C1;
    const short PI_GPIO_10 = PINE_C0;
    const short PI_GPIO_11 = PINE_C2;
    const short PI_GPIO_12 = PINE_C4;
    const short PI_GPIO_13 = PINE_C5;
    const short PI_GPIO_14 = PINE_B0;
    const short PI_GPIO_15 = PINE_B1;
    const short PI_GPIO_16 = PINE_C6;
    const short PI_GPIO_17 = PINE_C7;
    const short PI_GPIO_18 = PINE_C8;
    const short PI_GPIO_19 = PINE_C9;
    const short PI_GPIO_20 = PINE_C10;
    const short PI_GPIO_21 = PINE_C11;
    const short PI_GPIO_22 = PINE_C12;
    const short PI_GPIO_23 = PINE_C13;
    const short PI_GPIO_24 = PINE_C14;
    const short PI_GPIO_25 = PINE_C15;
    const short PI_GPIO_26 = PINE_C16;
    const short PI_GPIO_27 = PINE_H9;

    // Euler BUS
    const short IR_RX =     PINE_L11;
    const short OWA_OUT =   PINE_H8;
    const short SPDIF =     PINE_H8;
    const short EAROUTP =   PINE_B13;
    const short EAROUTN =   PINE_A13;
    const short I2S_MCK =   PINE_B3;
    const short I2S_BCK =   PINE_B5;
    const short I2S_DIN =   PINE_B7;
    const short I2S_DOUT =  PINE_B6;
    const short I2S_SYNC =  PINE_B4;
    const short SPI1_MOSI = PINE_D2;
    const short SPI1_MISO = PINE_D3;
    const short SPI1_CLK =  PINE_D1;
    const short SPI1_CS =   PINE_D0;
    const short UART0_TX =  PINE_B8;
    const short UART0_RX =  PINE_B9;
    const short UART3_TX =  PINE_D0;
    const short UART3_RX =  PINE_D1;
    const short UART4_TX =  PINE_D2;
    const short UART4_RX =  PINE_D3;
    const short UART4_RTS = PINE_D4;
    const short UART4_CTS = PINE_D5;
}
#endif
