/* -*- Mode: C; c-basic-offset: 8; -*- */
//
// Copyright (c) 2011 Michael Smith, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

/// @file	board.h
///
/// Board-specific definitions for the bootloader.
///
/// Expected to define:
///
/// BOARD_ID
///	A unique byte identifying this board.
///
/// LED_BOOTLOADER
///	A __bit controlling an LED for the bootloader.
///
/// LED_RADIO
///     A __bit controlling an LED for the radio.
///
/// LED_ACTIVITY
///	A __bit controlling an LED that can be blinked to show activity.
///
/// LED_ON
///	The value to write to LED to turn the LED on.
///
/// LED_OFF
///	The value to write to LED to turn the LED off.
///
/// BUTTON_BOOTLOAD
///	A __bit corresponding to a button or strap that will cause the
///	bootloader to stop and wait for a download.
///
/// BUTTON_ACTIVE
///	The value that BUTTON will have when the bootloader should stop.
///
/// SYSCLK
///	Frequency of the system clock in MHz.
///
/// HW_INIT
///	A code fragment called at early startup time that configures
///	the SoC for board-specific operation.
///	- configures LED GPIO(s)
///	- configures button GPIO(s)
///	- configures INT0 for the radio interrupt
///
/// TBD:
///     strapping options for board frequency band selection
///

/// @file	board.h
///		Board definitions for the HopeRF RF50 evaluation board

#ifndef _BOARD_H
#define _BOARD_H

#include <compiler_defs.h>
#include <Si1000_defs.h>

#define BOARD_ID	0x4d	// unique board ID used to connect bootloader and upload tools

// GPIO definitions (not exported)
SBIT(LED_RED,	   SFR_P2, 0);
SBIT(LED_GREEN,	   SFR_P2, 5);
SBIT(BUTTON_ENTER, SFR_P0, 6);
SBIT(BUTTON_UP,	   SFR_P1, 5);
SBIT(BUTTON_DOWN,  SFR_P1, 6);

// Signal polarity definitions
#define LED_ON		0
#define LED_OFF		1
#define BUTTON_ACTIVE	0

// UI definitions
#define LED_BOOTLOADER	LED_RED
#define LED_RADIO	LED_GREEN
#define LED_ACTIVITY	LED_RED
#define BUTTON_BOOTLOAD	BUTTON_ENTER

// board-specific hardware config
#define HW_INIT						\
do {							\
	/* GPIO config */				\
	P0SKIP	|=  0x40;		/* button */	\
	P1SKIP  |=  0x60;		/* buttons */	\
	P2SKIP	|=  0x21;		/* LEDs */	\
	SFRPAGE	 =  CONFIG_PAGE;			\
	P2DRV	|=  0x21;		/* LEDs */	\
	SFRPAGE	 =  LEGACY_PAGE;			\
	/* INT0 is the radio interrupt, on P0.7 */	\
	IT01CF   =  (IT01CF & 0xf) | 0x7;		\
	IT0	 = 0;	/* INT0 leve triggered */	\
} while(0)

// System clock frequency
// XXX should be setting up clocks in HW_INIT to be able to guarantee this...
#define SYSCLK	245000000

// EzRadio / rtPhy definitions
// Note that the RFM50 deviates in both the oscillator cap value and the wiring
// of the RF switch
#define EZRADIOPRO_OSC_CAP_VALUE 0xb4	// Per HRF demo code
#define ENABLE_RFM50_SWITCH	1	// Per HRF demo code, verified presence of RF switch on the RFM50 module
SBIT(IRQ,  SFR_P0, 7);			// Per HRF demo code & schematic
SBIT(NSS1, SFR_P1, 4);			// SI100x Internal Connection
SBIT(SDN,  SFR_P2, 6);			// XXX not actually the case on the RFM50... HRF set it this way though

#endif // _BOARD_H
