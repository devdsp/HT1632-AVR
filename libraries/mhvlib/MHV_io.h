/*
 * MHV_io.h
 *
 *  Created on: 21/02/2010
 *      Author: deece

Copyright (C) 2010 Alastair D'Silva

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */

#ifndef MHV_IO_H_
#define MHV_IO_H_

#define mhv_setOutput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvDir |= _BV(mhvBit)

#define mhv_setInput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	{ \
		*mhvDir &= ~_BV(mhvBit); \
		*mhvOutput &= ~_BV(mhvBit); \
	}

#define mhv_setInputPullup(mhvDir,mhvOutput,mhvInput,mhvBit) \
	{ \
		*mhvDir |= _BV(mhvBit); \
		*mhvOutput |= _BV(mhvBit); \
	}

#define mhv_pinOn(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvOutput |= _BV(mhvBit)

#define mhv_pinOff(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvOutput &= ~_BV(mhvBit)

#define mhv_pinRead(mhvDir,mhvOutput,mhvInput,mhvBit) \
	(*mhvInput & _BV(mhvBit))

#endif /* MHV_IO_H_ */
