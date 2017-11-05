#pragma once
#ifndef HEADER_LIBVLCX_ENUM
#	define HEADER_LIBVLCX_ENUM 1

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

// some vlc functions take or return integers that are documented as being values of an enum
//
// this struct template is used on those situations to make it more apparent to you the user.
template<typename TEnum, typename TInt = unsigned>
struct VLCEnum {
	
	// the integer value.
	TInt Integer;

	// the enum value.
	VLCX_EXPRESSION TEnum Enum() const { return (TEnum)Integer; }

	// initialize the enum value with an integer.
	VLCX_EXPRESSION VLCEnum(const TInt value) : Integer(value) { }
	
	// initialize the enum value with the enum.
	VLCX_EXPRESSION VLCEnum(const TEnum value) : Integer((TInt)value) {}

	// initialize the enum value with another enum.
	VLCX_EXPRESSION VLCEnum(const VLCEnum& value) : Integer(value.Integer) {}

	// check for equality (integer)
	VLCX_EXPRESSION auto operator ==(const TEnum other)const { return Integer == (TInt)other; }

	// check for inequality (integer)
	VLCX_EXPRESSION auto operator !=(const TEnum other)const { return Integer != (TInt)other; }

	// check for equality (integer)
	VLCX_EXPRESSION auto operator ==(const TInt other)const { return Integer == other; }

	// check for inequality (integer)
	VLCX_EXPRESSION auto operator !=(const TInt other)const { return Integer != other; }

	// less than operator (integer)
	VLCX_EXPRESSION auto operator <(const TInt other)const { return Integer < other; }

	// greater than operator (integer)
	VLCX_EXPRESSION auto operator >(const TInt other)const { return Integer > other; }

	// less than or equal to operator (integer)
	VLCX_EXPRESSION auto operator <=(const TInt other)const { return Integer <= other; }

	// greater than or equal to operator (integer)
	VLCX_EXPRESSION auto operator >=(const TInt other)const { return Integer >= other; }

	// check for equality (integer)
	VLCX_EXPRESSION auto operator ==(const VLCEnum& other)const { return Integer == other.Integer; }

	// check for inequality (integer)
	VLCX_EXPRESSION auto operator !=(const VLCEnum& other)const { return Integer != other.Integer; }

	// implicit cast to integer.
	VLCX_EXPRESSION operator const TInt&() const { return Integer; }

	// implicit cast to integer.
	VLCX_EXPRESSION operator TInt&() { return Integer; }

	// explicit cast to enum.
	VLCX_EXPRESSION explicit operator TEnum() const { return (TEnum)Integer; }
};

// specialize version for defaulting to a signed integer (note that this does do the less/greater equal as signed).
template<typename TEnum> using VLCEnumI = VLCEnum<TEnum, int>;

// perhaps a little out of place here but VLC returns booleans in two separate ways.
// Booleans return as normal false == 0 and Results return as true == 0.
//
// this logic is done here in case you would rather retain the int that libvlc returns you can just swap this out without external modification.
#	if !LIBVLCX_KEEP_INT_BOOLEAN
using VLCBoolean = bool;
VLCX_EXPRESSION VLCBoolean VLCBool(const int code) { return !!code; }
#	elif LIBVLCX_KEEP_INT_BOOLEAN == 1
using VLCBoolean = int;
VLCX_EXPRESSION const VLCBoolean &VLCBool(const VLCBoolean &code) { return code; }
#	endif

// Results in libvlc have a meaning of success when zero is returned.
#	if !LIBVLCX_KEEP_INT_RESULT
using VLCResult = bool;
VLCX_EXPRESSION VLCResult VLCRes(const int code) { return !code; }
VLCX_EXPRESSION bool VLCSuccess(const VLCResult r) { return r; }
#	elif LIBVLCX_KEEP_INT_RESULT == 1
using VLCResult = int;
VLCX_EXPRESSION const VLCResult &VLCRes(const VLCBoolean &code) { return code; }
VLCX_EXPRESSION bool VLCSuccess(const VLCResult r) { return !r; }
#	endif

LIBVLCX_NAMESPACE(END)
#endif