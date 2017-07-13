/*
 * Copyright (c) 2017 Scansorial Inc. All rights reserved.
 */

#ifndef ASF_H
#define ASF_H

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

// From module: Common SAM0 compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: EXTINT - External Interrupt (Callback APIs)
#include <extint.h>
#include <extint_callback.h>

// From module: Generic board support
#include <board.h>

// From module: Interrupt management - SAM implementation
#include <interrupt.h>

// From module: PORT - GPIO Pin Control
#include <port.h>

// From module: Part identification macros
#include <parts.h>

// From module: SAM D21/R21/D11/L21/DA1/L22 USB Dual role interface
#include <usb_dual.h>

// From module: SYSTEM - Clock Management for SAMD21
#include <clock.h>
#include <gclk.h>

// From module: SYSTEM - Core System Driver
#include <system.h>

// From module: SYSTEM - I/O Pin Multiplexer
#include <pinmux.h>

// From module: SYSTEM - Interrupt Driver
#include <system_interrupt.h>

// From module: SYSTEM - Power Management for SAM D20/D21/R21/D09/D10/D11/DA/HA
#include <power.h>

// From module: SYSTEM - Reset Management for SAM D20/D21/R21/D09/D10/D11/DA/HA
#include <reset.h>

// From module: USB - Universal Serial Bus
#include <usb.h>

// From module: USB CDC Protocol
#include <usb_protocol_cdc.h>

// From module: USB Device CDC (Single Interface Device)
#include <udi_cdc.h>

// From module: USB Device Stack Core (Common API)
#include <udc.h>
#include <udd.h>

#endif // ASF_H
