#pragma once
#ifndef HEADER_LIBVLCX_ALLOC
#	define HEADER_LIBVLCX_ALLOC

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

// Frees the instance. specialized for each type by use of Macro_LIBVLCX_BindAlloc
template<typename T> VLCX_INLINE void VLCFree(T*const &adr);

// base template type for things that can be freed with vlc free.
// this should not be explicitly specialized, only VLCFree should.
//
// a Steal method is provided for those special instances where you may want to do such a thing, just keep in mind to VLCFree it afterwards.
template<typename T> struct VLCAlloc {
	// the address of the item or nullptr.
	// note that when the VLCAlloc gets deleted it will VLCFree the address
	// so you should avoid passing this field around and prefer passing the VLCAlloc around by reference.
	T * volatile const Base;

	// binds the address to the instance.
	VLCX_INLINE VLCAlloc(T*const& adr) : Base(adr) {}

	// you cannot copy vlc alloc. it does not handle reference counting on purpose.
	// pass it around with reference.
	LIBVLCX_MEMBER_DELETE(VLCAlloc(const VLCAlloc&));

	// note that this is not thread safe!
	// returns a pointer to what base was prior to the call to steal while manipulating the base value to be nullptr.
	// so that it does not free with this instance.
	//
	// this does mutate the constant Base member.
	VLCX_INLINE T *Steal() {
		T*const ReadAdr = Base;
		*((T**)(&(this->Base))) = nullptr;
		return ReadAdr;
	}

	// frees the Base
	VLCX_INLINE ~VLCAlloc() { if ((VLCAlloc*)nullptr != this) if (T*const FreeBase = Steal()) VLCFree(FreeBase); }

	// note that you have to make sure that this VLCAlloc does not get deleted prior to your use of the return value.
	VLCX_INLINE T * operator *()const { return Base; }
};

// use this to define a allocation type
// First parameter is the libvlc type
// Second parameter is the libvlc free function
// Third parameter what you would like to name the Alloc<> typedef.
// Any additional args get sent tot he free call after the address.
//
// terminate with semicolon
#define Macro_LIBVLCX_BindAlloc( TypeName, FreeCall, FriendlyName, ... )		\
template<> VLCX_INLINE void VLCFree(TypeName*const &adr) {						\
	FreeCall(adr,##__VA_ARGS__);												\
}																				\
using FriendlyName = VLCAlloc<TypeName>

LIBVLCX_NAMESPACE(END)

#endif