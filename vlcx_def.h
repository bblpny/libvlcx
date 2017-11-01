#pragma once
#ifndef HEADER_LIBVLCX_DEF
#	include <cstdint>

#	define HEADER_LIBVLCX_DEF 1

#	ifndef LIBVLCX_SKIP
#		define LIBVLCX_SKIP(...)
#	endif

#	ifndef VLCX_INLINE
#		if _MSC_VER
#			define VLCX_INLINE __forceinline
#		else
#			define VLCX_INLINE inline
#		endif
#	endif

#	ifndef LIBVLCX_API
#		define LIBVLCX_API
#	endif

#	ifndef LIBVLCX_NAMESPACE

#		ifndef LIBVLCX_NAMESPACE_START
#			define LIBVLCX_NAMESPACE_START namespace LibVLCX {
#		endif

#		ifndef LIBVLCX_NAMESPACE_END
#			define LIBVLCX_NAMESPACE_END };
#		endif

#		define LIBVLCX_NAMESPACE(...) LIBVLCX_NAMESPACE_##__VA_ARGS__

#	endif


#endif