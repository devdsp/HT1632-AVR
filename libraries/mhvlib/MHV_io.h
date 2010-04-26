/*
 * Copyright (c) 2010, Make, Hack, Void Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Make, Hack, Void nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAKE, HACK, VOID BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef MHV_IO_H_
#define MHV_IO_H_

#include <avr/io.h>



enum mhv_interruptMode {
	MHV_INTERRUPT_LOW,
	MHV_INTERRUPT_CHANGE,
	MHV_INTERRUPT_FALLING,
	MHV_INTERRUPT_RISING
};
typedef enum mhv_interruptMode MHV_INTERRUPTMODE;


/* We have to shadow all the macros below as the precedence of macro expansion means that
 * the multi-parmeter macros will only see a single argument if one of our MHV_PIN macros
 * is used
 */

/* See http://gcc.gnu.org/onlinedocs/gcc-3.3.6/cpp/Swallowing-the-Semicolon.html
 * to understand why we have do...while(0) on our macros
 */

#define mhv_out(mhvParms) \
	_mhv_out(mhvParms)

#define _mhv_out(mhvDir,mhvOutput,mhvInput,mhvBit) \
	mhvOutput

#define mhv_in(mhvParms) \
	_mhv_in(mhvParms)

#define _mhv_in(mhvDir,mhvOutput,mhvInput,mhvBit) \
	mhvInput

#define mhv_Bit(mhvParms) \
	_mhv_Bit(mhvParms)

#define _mhv_Bit(mhvDir,mhvOutput,mhvInput,mhvBit) \
	mhvBit

#define mhv_dir(mhvParms) \
	_mhv_dir(mhvParms)

#define _mhv_dir(mhvDir,mhvOutput,mhvInput,mhvBit) \
	mhvDir

#define mhv_setOutput(mhvParms) \
	do { \
		_mhv_setOutput(mhvParms); \
	} while(0)

#define _mhv_setOutput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvDir |= _BV(mhvBit);

#define mhv_setInput(mhvParms) \
	do { \
		_mhv_setInput(mhvParms); \
	} while(0)

#define _mhv_setInput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvDir &= ~_BV(mhvBit); \
	*mhvOutput &= ~_BV(mhvBit);

#define mhv_setInputPullup(mhvParms) \
	do { \
		_mhv_setInputPullup(mhvParms) \
	} while(0)

#define _mhv_setInputPullup(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvDir |= _BV(mhvBit); \
	*mhvOutput |= _BV(mhvBit);

#define mhv_pinOn(mhvParms) \
	do { \
		_mhv_pinOn(mhvParms) \
	} while(0)

#define _mhv_pinOn(mhvDir,mhvOutput,mhvInput,mhvBit) \
		*mhvOutput |= _BV(mhvBit);

#define mhv_pinOff(mhvParms) \
	do { \
		_mhv_pinOff(mhvParms) \
	} while(0)

#define _mhv_pinOff(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvOutput &= ~_BV(mhvBit); \

#define mhv_pinRead(mhvParms) \
	_mhv_pinRead(mhvParms)

#define _mhv_pinRead(mhvDir,mhvOutput,mhvInput,mhvBit) \
	(*mhvInput & _BV(mhvBit))

#define mhv_declareExternalInterrupt(mhvInterruptParms,mhvFunction) \
	_mhv_declareExternalInterrupt(mhvInterruptParms, mhvFunction)

#define _mhv_declareExternalInterrupt(mhvInterruptHandler,mhvModeRegister,mhvModeBitshift,mhvFunction) \
ISR(mhvInterruptHandler) mhvFunction

#define mhv_enableExternalInterrupt(mhvInterruptParms,mhvInterruptMode) \
	do { \
		_mhv_enableExternalInterrupt(mhvInterruptParms,mhvInterruptMode); \
	} while (0)

#define _mhv_enableExternalInterrupt(mhvInterruptHandler,mhvModeRegister,mhvModeBitshift,mhvInterruptMode) \
	*mhvModeRegister = (*mhvModeRegister & ~(0x03 << mhvModeBitshift)) | mhvInterruptMode << mhvModeBitshift

#endif /* MHV_IO_H_ */
