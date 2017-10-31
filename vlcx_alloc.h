#pragma once
#ifndef HEADER_LIBVLCX_ALLOC
#	define HEADER_LIBVLCX_ALLOC

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

template<typename T> VLCX_INLINE void VLCFreeSingle(T*const &adr);
template<typename T> VLCX_INLINE void VLCFreeList(T*const &adr);

template<bool IsList>
struct VLCFree {
	template<typename T>
	static VLCX_INLINE void Address(T*const &adr) { if (adr) VLCFreeList(adr); }
};

template<>
struct VLCFree<false> {
	template<typename T>
	static VLCX_INLINE void Address(T*const &adr) { if (adr) VLCFreeSingle(adr); }
};

template<typename T>
struct VLCAllocAddress {
	T*const Adr;
protected:
	explicit VLCX_INLINE VLCAllocAddress(T*const& adr) : Adr(adr) {}
	VLCAllocAddress(const VLCAllocAddress&) = delete;
};

template<typename T, bool IsList>
struct VLCAllocContainerBase : public VLCAllocAddress<T> {
	enum { IS_LIST = (IsList ? true : false), };
	VLCX_INLINE ~VLCAllocContainerBase() { VLCFree<IS_LIST>::Address(Adr); }
protected:
	explicit VLCX_INLINE VLCAllocContainerBase(T*const& adr) : VLCAllocAddress(adr) {}
	VLCAllocContainerBase(const VLCAllocContainerBase&) = delete;
};
template<typename T, bool IsList>
struct VLCAllocContainer : public VLCAllocContainerBase<T, IsList ? true : false> {
	VLCX_INLINE VLCAllocContainer(const VLCAllocContainer &) = delete;
	VLCX_INLINE VLCAllocContainer(T*const&adr) : VLCAllocContainerBase(adr) {}
};

template<typename T> using VLCAllocList = VLCAllocContainer<T, true>;
template<typename T> using VLCAlloc = VLCAllocContainer<T, false>;

LIBVLCX_NAMESPACE(END)

#endif