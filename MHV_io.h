/*
 * MHV_io.h
 *
 *  Created on: 21/02/2010
 *      Author: deece
 */

#ifndef MHV_IO_H_
#define MHV_IO_H_

#define mhv_setOutput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	*mhvDir |= _BV(mhvBit)

#define mhv_setInput(mhvDir,mhvOutput,mhvInput,mhvBit) \
	{ \
		*mhvDir |= _BV(mhvBit); \
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
