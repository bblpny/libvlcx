#pragma once
#ifndef HEADER_LIBVLCX_OBJECT
#	define HEADER_LIBVLCX_OBJECT 1

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

enum class VLCKind : uint8_t {
	Null = 0,
	Library = 1,
	Media = 2,
	MediaPlayer = 3,
};

#define Macro_LIBVLCX_Enumerate_VLCKind(x,...)\
	x(Null)__VA_ARGS__ \
	x(Library)__VA_ARGS__ \
	x(Media)__VA_ARGS__ \
	x(MediaPlayer)

#define Macro_LIBVLCX_EnumerateValid_VLCKind(x,...)\
	x(Library,libvlc_instance_t)__VA_ARGS__ \
	x(Media,libvlc_media_t)__VA_ARGS__ \
	x(MediaPlayer,libvlc_media_player_t)


VLCX_INLINE constexpr uint8_t VLCKindNum() { return 4; }

VLCX_INLINE constexpr bool VLCHasEventManager(VLCKind Kind) {
	return Kind != VLCKind::Null && (uint8_t)Kind < VLCKindNum();
}
template<typename Struct> VLCX_INLINE void VLCRelease(Struct *const&);
template<typename Struct> VLCX_INLINE void VLCRetain(Struct *const&);
template<typename Struct> struct VLCWrapper;
template<VLCKind Kind> struct VLC;
template<typename Struct> VLCX_INLINE constexpr VLCKind VLCStructKindOf() { return VLCKind::Null; }
template<VLCKind Kind> struct VLCKindDef { using TStruct = void; };
template<VLCKind Kind> using VLCStruct = typename VLCKindDef<Kind>::TStruct;

#define Macro_LIBVLCX_PopulateKind(name,...) \
template<> VLCX_INLINE constexpr VLCKind VLCStructKindOf<__VA_ARGS__>(){return VLCKind::name;} \
template<> struct VLCKindDef<VLCKind::name> { using TStruct = __VA_ARGS__; }

Macro_LIBVLCX_EnumerateValid_VLCKind(Macro_LIBVLCX_PopulateKind, ;);

#undef Macro_LIBVLCX_PopulateKind

LIBVLCX_API struct VLCTag {
	uint32_t Int;
	uint16_t Short;
	uint8_t Byte;
	const VLCKind Kind;

	VLCX_INLINE constexpr VLCTag(
		const uint32_t intTag,
		const uint16_t shortTag,
		const uint8_t byteTag,
		const VLCKind kind)
		: Int(intTag)
		, Short(shortTag)
		, Byte(byteTag)
		, Kind(kind) {}

	VLCX_INLINE constexpr VLCTag(const VLCKind kind) : VLCTag(0, 0, 0, kind) {}

	VLCX_INLINE constexpr VLCTag() : VLCTag(0, 0, 0, VLCKind::Null) {}

	VLCX_INLINE constexpr VLCTag(const VLCTag &copy)
		: Int(copy.Int)
		, Short(copy.Short)
		, Byte(copy.Byte)
		, Kind(copy.Kind) {}
	template<VLCKind ConstantKind>
	VLCX_INLINE constexpr VLCTag(const VLC<ConstantKind> &copy) : Int(copy.IntTag()), Short(copy.ShortTag()), Byte(copy.ByteTag()), Kind(copy.Kind()) {}

	VLCX_INLINE constexpr bool operator ==(const VLCKind kind) const { return Kind == kind; }
	VLCX_INLINE constexpr bool operator !=(const VLCKind kind) const { return Kind != kind; }
	VLCX_INLINE constexpr bool operator <(const VLCKind kind) const { return Kind < kind; }
	VLCX_INLINE constexpr bool operator >(const VLCKind kind) const { return Kind > kind; }
	VLCX_INLINE constexpr bool operator <=(const VLCKind kind) const { return Kind <= kind; }
	VLCX_INLINE constexpr bool operator >=(const VLCKind kind) const { return Kind >= kind; }

	VLCX_INLINE explicit constexpr operator const VLCKind&()const { return Kind; }
};


static_assert(sizeof(VLCTag) == 8, "Strange alignment");


LIBVLCX_API struct VLCEventHandler {
	const libvlc_callback_t Callback;
	void *const UserData;
	const libvlc_event_type_t EventType;

	VLCX_INLINE constexpr VLCEventHandler(
		const libvlc_event_type_t eventType,
		const libvlc_callback_t callback,
		void*const userData) : Callback(callback), EventType(eventType), UserData(userData) {}

	VLCX_INLINE constexpr VLCEventHandler(const VLCEventHandler & copy)
		: Callback(copy.Callback), UserData(copy.UserData), EventType(copy.EventType) {}

	VLCX_INLINE constexpr VLCEventHandler(const VLCEventHandler & base, void* userData)
		: Callback(base.Callback), UserData(userData), EventType(base.EventType) {}

	VLCX_INLINE explicit operator bool() const { return nullptr != Callback; }
	VLCX_INLINE bool operator !()const { return !Callback; }
};

LIBVLCX_API struct VLCEventManagerBase {
	libvlc_event_manager_t*const Manager;
protected:
	explicit VLCX_INLINE constexpr VLCEventManagerBase(libvlc_event_manager_t*const& manager) : Manager(manager) {}
	explicit VLCX_INLINE constexpr VLCEventManagerBase(const VLCEventManagerBase &copy) : Manager(copy.Manager) {}
public:
	VLCX_INLINE int Attach(libvlc_event_type_t i_event_type,
		libvlc_callback_t f_callback,
		void *const user_data)const {
		return libvlc_event_attach(Manager, i_event_type, f_callback, user_data);
	}
	VLCX_INLINE void Detach(libvlc_event_type_t i_event_type,
		libvlc_callback_t f_callback,
		void*const p_user_data)const {
		libvlc_event_detach(Manager, i_event_type, f_callback, p_user_data);
	}
	VLCX_INLINE int Attach(const VLCEventHandler &h) const { return Attach(h.EventType, h.Callback, h.UserData); }
	VLCX_INLINE void Detach(const VLCEventHandler &h) { return Detach(h.EventType, h.Callback, h.UserData); }
};

template<VLCKind Kind>
struct VLCEventManager : public VLCEventManagerBase, public VLC<Kind> {
	static_assert(VLCHasEventManager(Kind), "kind does not have event manager");
	VLCX_INLINE explicit VLCEventManager(const VLC<Kind> & outer) : Implementation(outer), VLCEventManagerBase(outer.GetEventManagerStructure()) {
	}
	VLCX_INLINE VLCEventManager(const VLCEventManager &copy) : Implementation(copy), VLCEventManagerBase(copy.GetEventManagerStructure()) {}

	VLCX_INLINE VLCEventManager &operator +=(const VLCEventHandler & h) { Attach(h); return *this; }
	VLCX_INLINE VLCEventManager &operator -=(const VLCEventHandler & h) { Detach(h); return *this; }
};

#define Macro_LIBVLCX_ExposeEventManager(func) \
private: \
VLCX_INLINE libvlc_event_manager_t *const GetEventManagerStructure() const { \
	libvlc_event_manager_t *o; if (auto m = GetStructure())o = func(m);else o = nullptr; return o; \
} \
friend struct VLCEventManager<ConstantKind>; \
public: \
using TEventManager = VLCEventManager<ConstantKind>


LIBVLCX_API struct VLCObject : private VLCTag {
private:
	void *const Ptr;

	mutable volatile size_t RefCount;
	mutable volatile size_t Alive;

protected:
	enum EConstructorRetain {
		ConstructorRetain,
	};
private:
#define Macro_LIBVLCX_WrapperFriends(name,...) friend struct VLCWrapper<__VA_ARGS__>;
	Macro_LIBVLCX_EnumerateValid_VLCKind(Macro_LIBVLCX_WrapperFriends)
#undef Macro_LIBVLCX_WrapperFriends
	
	VLCX_INLINE bool DoRetain() const {
		return nullptr != Ptr && 0u != Alive && 0u == RefCount++;
	}
	
	VLCX_INLINE bool DoRelease() const {
		return nullptr != Ptr && 0u != RefCount && 0u == --RefCount && 0u == --Alive;
	}

	constexpr VLCObject(void*const& ptr, const VLCTag & tag) : VLCTag(tag), Ptr(ptr), RefCount(0), Alive(nullptr == ptr ? 0 : 1) {

	}
	constexpr VLCObject(void*const& ptr, const VLCTag & tag, EConstructorRetain) 
		: VLCTag(tag), Ptr(ptr), RefCount(nullptr == ptr ? 0 : 1), Alive(nullptr == ptr ? 0 : 1) {

	}
	template<typename Struct>
	constexpr VLCObject(Struct*const&ptr) : VLCObject(ptr, VLCStructKindOf<Struct>()) {}
	template<typename Struct>
	constexpr VLCObject(EConstructorRetain, Struct*const&ptr) : VLCObject(ptr, VLCStructKindOf<Struct>()) {}
public:

	VLCX_INLINE uint32_t &IntTag() { return VLCTag::Int; }
	VLCX_INLINE uint16_t &ShortTag() { return VLCTag::Short; }
	VLCX_INLINE uint8_t &ByteTag() { return VLCTag::Byte; }
	VLCX_INLINE constexpr const VLCKind &Kind() const { return VLCTag::Kind; }

	VLCX_INLINE constexpr const uint32_t &IntTag() const { return VLCTag::Int; }
	VLCX_INLINE constexpr const uint16_t &ShortTag() const { return VLCTag::Short; }
	VLCX_INLINE constexpr const uint8_t &ByteTag() const { return VLCTag::Byte; }

};


template<typename Struct>
struct VLCWrapper : public VLCObject {
	using TStruct = Struct;
public:
	static const constexpr VLCKind ConstantKind = VLCStructKindOf<Struct>();
	using Implementation = typename VLC<ConstantKind>;
	using Wrapper = typename VLCWrapper<Struct>;
	static VLCX_INLINE constexpr VLCKind Kind() { return ConstantKind; }
private:
	friend struct VLCObject;

protected:
	VLCX_INLINE constexpr VLCWrapper(TStruct *const& Ptr) : VLCObject(Ptr) {}
	VLCX_INLINE constexpr VLCWrapper(EConstructorRetain, TStruct *const& Ptr) : VLCObject(ConstructorRetain, Ptr) {}
	VLCX_INLINE constexpr VLCWrapper() : VLCObject(nullptr, ConstantKind) {}
public:
	VLCX_INLINE constexpr TStruct *const GetStructure() const { return 0 == Alive ? nullptr : (TStruct*)Ptr; }
	VLCX_INLINE constexpr TStruct *const GetStructureUnchecked() const { return (TStruct*)Ptr; }
protected:
	VLCX_INLINE void Retain() const {
		if (DoRetain()) VLCRetain(GetStructureUnchecked());
	}

	VLCX_INLINE void Release() const {
		if (DoRelease()) VLCRelease(GetStructureUnchecked());
	}
};

LIBVLCX_NAMESPACE(END)
#endif
