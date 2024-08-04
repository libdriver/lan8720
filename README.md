[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LAN8720

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/lan8720/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The LAN8720A/LAN8720Ai is a low-power 10BASE-T/100BASE-TX physical layer (PHY) transceiver with variable I/O voltage that is compliant with the IEEE 802.3-2005 standards.The LAN8720A/LAN8720Ai supports communication with an Ethernet MAC via a standard RMII interface. It contains a full-duplex 10-BASE-T/100BASE-TX transceiver and supports 10Mbps (10BASE-T) and 100Mbps (100BASE-TX) operation. The LAN8720A/LAN8720Ai implements auto-negotiation to automatically determine the best possible speed and duplex mode of operation. HP Auto-MDIX support allows the use of direct connect or cross-over LAN cables.

LibDriver LAN8720 is the full function driver of LAN8720 launched by LibDriver.LibDriver LAN8720 provides basic configuration, auto negotiation and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver LAN8720 source files.

/interface includes LibDriver LAN8720 SMI, RMII platform independent template.

/test includes LibDriver LAN8720 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver LAN8720 sample code.

/doc includes LibDriver LAN8720 offline document.

/datasheet includes LAN8720 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface SMI, RMII platform independent template and finish your platform SMI, RMII driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_lan8720_basic.h"

uint8_t res;
lan8720_speed_indication_t speed_indication;

/* basic init */
res = lan8720_basic_init(0x01);
if (res != 0)
{
    return 1;
}

...
    
/* check auto negotiation */
if (lan8720_basic_auto_negotiation(&speed_indication) != 0)
{
    (void)lan8720_basic_deinit();
    
    return 1;
}

...

/* basic deinit */    
(void)lan8720_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/lan8720/index.html](https://www.libdriver.com/docs/lan8720/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.