#ifndef LASSIE_TYPES_H
#define LASSIE_TYPES_H

#if !defined(_MSC_VER)
#	include <stdint.h>
#endif

namespace modri
{
#if defined(_MSC_VER)
	typedef __int8  sint8;
	typedef __int16 sint16;
	typedef __int32 sint32;
	typedef __int64 sint64;
	typedef unsigned __int8  uint8;
	typedef unsigned __int16 uint16;
	typedef unsigned __int32 uint32;
	typedef unsigned __int64 uint64;
#else
	typedef int8_t   sint8;
	typedef int16_t  sint16;
	typedef int32_t  sint32;
	typedef int64_t  sint64;
	typedef uint8_t  uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
#endif
}

#endif // LASSIE_TYPES_H
