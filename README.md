# FreeRTOS 9.0 and FreeRTOS_CLI demo for Atmel/Microchip samd21 using ASF

A simple demonstration application using FreeRTOS for the
Atmel/Microchip samd21 xPlained Pro development board.
    
This demo blinks the built-in LED0 while reading a dummy "hello world"
command from the USB CDC serial port. It depends on FreeRTOS v9.0, the
FreeRTOS_CLI library, and ASF 3.34.2.
    
Instructions:
  - run `./install_dependencies.sh`
  - do `make flash`
  - plug a cable into the samd21 native USB port
  - connect to new USB serial port with 115200 bps, 8n1, no parity
  - enter `help` for command overview and `hello` for dummy command
  
`install_dependencies.sh` downloads and installs the ASF source code library,
the arm-none-eabi toolchain, and the `edbg` programming utility.

Currently Ubuntu/Debian Linux and Mac OS X are supported.

External dependencies:
  - https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
  - https://www.microchip.com/development-tools/atmel-studio-7/advanced-software-framework-%28asf%29
  - https://github.com/ataradov/edbg
