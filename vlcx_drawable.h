#pragma once
#ifndef HEADER_LIBVLCX_DRAWABLE
#	define HEADER_LIBVLCX_DRAWABLE 1
#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

enum VLCX_ENUM_CLASS VLCDrawableKind VLCX_ENUM_INT {
	Draw_NSObject = 0,
	Draw_AGL = 1,
	Draw_HWND = 2,
	Draw_XWindow = 3,
};

// note that X is called with the names Draw_ prefix removed.
#	define Macro_LIBVLCX_Enumerate_VLCDrawableKind(X,...)\
	X(NSObject)__VA_ARGS__ \
	X(AGL)__VA_ARGS__ \
	X(HWND)__VA_ARGS__ \
	X(XWindow)

template<typename T> struct VLCDrawableKindHandleInfo { using THandle = T; };
template<VLCDrawableKind Kind> struct VLCDrawableKindInfo;

#	define Macro_LIBVLCX_BindDrawable(Kind,Handle,GetF,SetF) \
template<> struct VLCDrawableKindInfo<VLCDrawableKind::Draw_##Kind> \
	: public VLCDrawableKindHandleInfo<Handle> { \
	static VLCX_INLINE void Set(libvlc_media_player_t*const m, THandle const h) { SetF(m, h); } \
	static VLCX_INLINE THandle Get(libvlc_media_player_t*const m) { return GetF(m); } \
}

LIBVLCX_NAMESPACE(END)

#endif