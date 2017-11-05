#pragma once
#ifndef HEADER_LIBVLCX_MEDIA_STRING
#	define HEADER_LIBVLCX_MEDIA_STRING 1
#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

enum VLCX_ENUM_CLASS VLCMediaUse VLCX_ENUM_INT {
	Path,
	Location,
	NodeName,
};

#	define Macro_LIBVLCX_Enumerate_VLCMediaUse(X,...)\
	X(Path)__VA_ARGS__ \
	X(Location)__VA_ARGS__ \
	X(NodeName)

template<VLCMediaUse Use>
struct VLCMediaString {
	const char *const Sz;

	VLCX_EXPRESSION VLCMediaString(
		const char *const& sz)
		: Sz(sz) { }

	VLCX_EXPRESSION operator
		const char *const &() const {
		return Sz;
	}

private:
	friend struct VLC<VLCKind::Media>;

	// implementation is specific!
	static VLCX_INLINE libvlc_media_t *Open(
		libvlc_instance_t*const& instance,
		const char *const& sz);

	VLCX_INLINE libvlc_media_t *Open(
		libvlc_instance_t*const& instance) const {
		return Open(instance, Sz);
	}
};

#	define Macro_LIBVLCX_BindMediaString(Usage,.../*operation*/)	\
using VLCMedia##Usage##String = VLCMediaString<VLCMediaUse::Usage>;	\
VLCX_INLINE libvlc_media_t *VLCMedia##Usage##String::Open(			\
	libvlc_instance_t*const& instance,								\
	const char *const& sz){											\
	return (nullptr==instance||nullptr==sz)?nullptr:__VA_ARGS__;	\
}

template<VLCMediaUse Use>
VLCX_EXPRESSION VLCMediaString<Use> MediaString(const char *const v) { return v; }

LIBVLCX_NAMESPACE(END)
#endif