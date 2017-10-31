#pragma once
#ifndef HEADER_LIBVLCX_ENUM
#	define HEADER_LIBVLCX_ENUM 1

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)


template<typename TEnum, typename TInt = unsigned>
struct VLCEnum {
	TInt Integer;
	VLCX_INLINE constexpr TEnum Enum() const { return (TEnum)Integer; }

	VLCX_INLINE constexpr VLCEnum(const TInt value) : Integer(value) { }
	VLCX_INLINE constexpr VLCEnum(const TEnum value) : Integer((TInt)value) {}
	VLCX_INLINE constexpr VLCEnum(const VLCEnum& value) : Integer(value.Integer) {}
	VLCX_INLINE constexpr bool operator ==(const TEnum other)const { return Integer == (TInt)other; }
	VLCX_INLINE constexpr bool operator !=(const TEnum other)const { return Integer != (TInt)other; }
	VLCX_INLINE constexpr bool operator ==(const TInt other)const { return Integer == other; }
	VLCX_INLINE constexpr bool operator !=(const TInt other)const { return Integer != other; }
	VLCX_INLINE constexpr bool operator <(const TInt other)const { return Integer < other; }
	VLCX_INLINE constexpr bool operator >(const TInt other)const { return Integer > other; }
	VLCX_INLINE constexpr bool operator <=(const TInt other)const { return Integer <= other; }
	VLCX_INLINE constexpr bool operator >=(const TInt other)const { return Integer >= other; }
	VLCX_INLINE constexpr bool operator ==(const VLCEnum& other)const { return Integer == other.Integer; }
	VLCX_INLINE constexpr bool operator !=(const VLCEnum& other)const { return Integer != other.Integer; }
	VLCX_INLINE constexpr operator const TInt&() const { return Integer; }
	VLCX_INLINE operator TInt&() { return Integer; }
	VLCX_INLINE explicit constexpr operator TEnum() const { return (TEnum)Integer; }
};

template<typename TEnum>
using VLCEnumI = VLCEnum<TEnum, int>;

using VLCBoolean = bool;
using VLCResult = bool;

VLCX_INLINE constexpr VLCBoolean VLCBool(const int code) { return !!code; }
VLCX_INLINE constexpr VLCResult VLCRes(const int code) { return !code; }


LIBVLCX_NAMESPACE(END)
#endif