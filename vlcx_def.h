#pragma once
#ifndef HEADER_LIBVLCX_DEF
#	include <cstdint>

#	define HEADER_LIBVLCX_DEF 1

#	define LIBVLCX_SKIP(...)

#	ifndef VLCX_INLINE
#		if _MSC_VER
#			define VLCX_INLINE __forceinline
#		else
#			define VLCX_INLINE inline
#		endif
#	endif

#	ifndef VLCX_CONSTEXPR
#		define VLCX_CONSTEXPR constexpr
#	endif

#	ifndef VLCX_EXPR
#		define VLCX_EXPRESSION VLCX_INLINE VLCX_CONSTEXPR
#	endif

#	ifndef VLCX_TYPE_EXPR
#		define VCLX_TYPE_EXPR static VLCX_EXPR
#	endif

#	ifndef VLCX_CONSTANT
#		define VLCX_CONSTANT const VLCX_CONSTEXPR
#	endif

#	ifndef VLCX_TYPE_CONSTANT
#		define VLCX_TYPE_CONSTANT static VLCX_CONSTANT
#	endif

#	ifndef LIBVLCX_API
#		define LIBVLCX_API
#	endif

#	ifndef LIBVLCX_MEMBER_OP
#		define LIBVLCX_MEMBER_OP(OPERATION,...) __VA_ARGS__ = OPERATION
#	endif

#	ifndef LIBVLCX_MEMBER_DELETE
#		define LIBVLCX_MEMBER_DELETE(...) LIBVLCX_MEMBER_OP(delete,##__VA_ARGS__)
#	endif

#	ifndef LIBVLCX_MEMBER_DEFAULT
#		define LIBVLCX_MEMBER_DEFAULT(...) LIBVLCX_MEMBER_OP(default,##__VA_ARGS__)
#	endif

#	ifndef VLCX_NAMESPACE
#		ifndef LIBVLCX_USE_NAMESPACE
#			define LIBVLCX_USE_NAMESPACE 1
#		endif
#		if LIBVLCX_USE_NAMESPACE
#			define VLCX_NAMESPACE VLCX
#		else
#			define VLCX_NAMESPACE
#		endif
#	elif !defined(LIBVLCX_USE_NAMESPACE)
#		define LIBVLCX_USE_NAMESPACE 1
#	elif !LIBVLCX_USE_NAMESPACE
#		undef VLCX_NAMESPACE
#		define VLCX_NAMESPACE
#	endif

#	if !defined(LIBVLCX_NAMESPACE) || defined(LIBVLCX_NAMESPACE_UTILITY)
#		if !defined(Macro_LIBVLCX_Namespace_START)
#			if !defined(Macro_LIBVLCX_Namespace_END)
#				if LIBVLCX_USE_NAMESPACE
#					define Macro_LIBVLCX_Namespace_START(...) namespace VLCX_NAMESPACE { LIBVLCX_SKIP(__VA_ARGS__)
#					define Macro_LIBVLCX_Namespace_END(...) LIBVLCX_SKIP(__VA_ARGS__) };
#					ifndef Macro_LIBVLCX_Namespace_USE
#						define Macro_LIBVLCX_Namespace_USE(...) using namespace VLCX_NAMESPACE LIBVLCX_SKIP(__VA_ARGS__);
#					endif
#				else
#					define Macro_LIBVLCX_Namespace_START(...) LIBVLCX_SKIP(__VA_ARGS__)
#					define Macro_LIBVLCX_Namespace_END(...) LIBVLCX_SKIP(__VA_ARGS__)
#					ifndef Macro_LIBVLCX_Namespace_USE
#						define Macro_LIBVLCX_Namespace_USE(...) LIBVLCX_SKIP(__VA_ARGS__)
#					endif
#				endif
#			else
#				define Macro_LIBVLCX_Namespace_START(...) LIBVLCX_SKIP(__VA_ARGS__)
#			endif
#		elif !defined(Macro_LIBVLCX_Namespace_END)
#			define Macro_LIBVLCX_Namespace_END(...) LIBVLCX_SKIP(__VA_ARGS__) 
#		endif

#		ifndef Macro_LIBVLCX_Namespace_USE
#			define Macro_LIBVLCX_Namespace_USE(...) LIBVLCX_SKIP(__VA_ARGS__)
#		endif

#		if !defined(LIBVLCX_NAMESPACE)
#			define LIBVLCX_NAMESPACE(COMMAND,...) Macro_LIBVLCX_Namespace_##COMMAND(__VA_ARGS__)
#		endif

#	endif

#ifndef LIBVLCX_KEEP_INT_BOOLEAN
#	define LIBVLCX_KEEP_INT_BOOLEAN 0
#endif

#ifndef LIBVLCX_KEEP_INT_RESULT
#	define LIBVLCX_KEEP_INT_RESULT 0
#endif

#ifndef VLCX_ENUM_CLASS
#	define VLCX_ENUM_CLASS class
#endif

#ifndef VLCX_ENUM_INT
#	define VLCX_ENUM_INT : uint8_t
#endif

#endif