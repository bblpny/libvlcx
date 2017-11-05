#pragma once
#ifndef HEADER_LIBVLCX_OBJECT
#	define HEADER_LIBVLCX_OBJECT 1

#	include "vlcx_def.h"

LIBVLCX_NAMESPACE(START)

// all possible types of vlc objects.
enum VLCX_ENUM_CLASS VLCKind VLCX_ENUM_INT {
	Null = 0,
	Instance = 1,
	Media = 2,
	MediaPlayer = 3,
};

// return the number of named entries in VLCKind (including Null)
VLCX_EXPRESSION uint8_t VLCKindNum() { return 4; }

// calls the first parameter with each valid name inside of VLCKind.
// any additional arguments are placed between the calls.
#define Macro_LIBVLCX_Enumerate_VLCKind(valid,...)	\
	valid(Instance)__VA_ARGS__						\
	valid(Media)__VA_ARGS__							\
	valid(MediaPlayer)

// return true for any VLCKind that has an event manager.
VLCX_EXPRESSION bool VLCHasEventManager(VLCKind Kind) {
	return Kind != VLCKind::Null && (uint8_t)Kind < VLCKindNum();
}

// each valid type of VLCKind must explicitly implement this.
template<typename Struct> VLCX_INLINE void VLCStructRelease(Struct *const&);

// each valid type of VLCKind must explicitly implement this.
template<typename Struct> VLCX_INLINE void VLCStructRetain(Struct *const&);

// base wrapper struct that should not be explicitly specialized.
template<typename Struct> struct VLCWrapper;

// final wrapper struct that should always be explicitly specialized.
template<VLCKind Kind> struct VLC;

// event manager type.
template<VLCKind Kind> struct VLCEventManager;

// meta type for VLCKind, every valid VLCKind should implement this explicitly through the use of Macro_LIBVLCX_BindKind
template<VLCKind Kind> struct VLCKindDef { using TStruct = void; };

// meta function for VLCKind, every valid VLCKind should implement this explicitly through the use of Macro_LIBVLCX_BindKind
template<typename Struct> VLCX_EXPRESSION VLCKind VLCStructKindOf() { return VLCKind::Null; }

// utility type definition.
template<VLCKind Kind> using VLCStruct = typename VLCKindDef<Kind>::TStruct;

// use this to bind a kind to a type.
#define Macro_LIBVLCX_BindKind(name,type,retain,release)							\
template<> VLCX_EXPRESSION VLCKind VLCStructKindOf<type>(){return VLCKind::name;}	\
template<> struct VLCKindDef<VLCKind::name> { using TStruct = type; };				\
template<> VLCX_INLINE void VLCStructRetain(type*const&name){retain(name);}			\
template<> VLCX_INLINE void VLCStructRelease(type*const&name){release(name);}		\
using VLC##name##EventManager = VLCWrapper<type>::EventManager;						\
using VLC##name = VLC<VLCKind::name>
// 128 bit (on 32 bit) or 256 bit (on 64 bit) structure that acts as the base to all vlc types covered in VLCKind.
// Purpose: holds pointer to the vlc structure abstractly, counts references and holds the kind being wrapped.
//
// no implementations of VLCWrapper<> nor VLC<> shall add any fields, this will be the size for all wrapped types in VLCKind.
LIBVLCX_API struct VLCObject {
private:
	// the pointer to the vlc C structure.
	void *const Ptr;
	// one or more if either retain had been called once or the object constructor made the object being addressed by Ptr.
	mutable volatile size_t RefCount;
	// one if alive (not released) zero if dead (null or released)
	mutable volatile size_t Alive;

public:
	// the kind of this object.
	alignas(size_t) const VLCKind Kind;

protected:
	// a dummy enum used to control ref-counts initial setting.
	// since vlc objects come retained once when they are created, we do not want to invoke the retain method when the constructor creates an object.
	enum EConstructorRetain {
		ConstructorRetain,
	};

private:/*EXTREMELY PRIVATE*/

	// extremely private. invoked from other constructor.
	VLCX_EXPRESSION VLCObject(void*const& ptr, const VLCKind kind)
		: Kind(kind)
		, Ptr(ptr)
		, RefCount(0)
		, Alive(nullptr == ptr ? 0 : 1) {}

	// extremely private. invoked from other constructor.
	VLCX_EXPRESSION VLCObject(void*const& ptr, const EConstructorRetain, const VLCKind kind)
		: Kind(kind)
		, Ptr(ptr)
		, RefCount(nullptr == ptr ? 0 : 1)
		, Alive(nullptr == ptr ? 0 : 1) {}

private:/*PROTECTED TO VLCWrapper<>*/

	// creates private friend structures for each vlc structure type.
	// I intend only the specific managed types to utilize the constructors of this class.
	template<typename T> friend struct VLCWrapper;
	
	// returns true when the object at ptr should actually be retained.
	// VLCWrapper<> should only invoke VLCRetain<> when a call to this returns true.
	VLCX_INLINE bool DoRetain() const {
		return nullptr != Ptr && 0u != Alive && 0u == RefCount++;
	}

	// returns true when the object at ptr should actually be released.
	// VLCWrapper<> should only invoke VLCRelease<> when a call to this returns true.
	VLCX_INLINE bool DoRelease() const {
		return nullptr != Ptr && 0u != RefCount && 0u == --RefCount && 0u == --Alive;
	}

	// starts the refcount at zero.
	// meaning you must call Retain within the implementation constructor.
	// if ptr is non-nullptr then Alive is initialized to 1.
	template<typename Struct>
	VLCX_EXPRESSION VLCObject(Struct*const&ptr)
		: VLCObject(ptr, VLCStructKindOf<Struct>()) {}

	// if ptr is non-nullptr then both alive and recount is initialized to 1.
	// this should only be used for things that Create the ptr object.
	// Retain should NOT be called within the implementations constructor.
	template<typename Struct> 
	VLCX_EXPRESSION VLCObject(EConstructorRetain, Struct*const&ptr)
		: VLCObject(ptr, ConstructorRetain, VLCStructKindOf<Struct>()) {}

	VLCX_EXPRESSION VLCObject() : Kind(VLCKind::Null), Ptr(nullptr), Alive(0), RefCount(0) {}
};

// just ensuring the size is correct.
static_assert(sizeof(VLCObject) == (sizeof(size_t) << 2), "VLCObject alignment off");

// base template for vlc types covered by VLCKind.
// each handler for the type shall publicly inherit this in their own explicit implementation of VLC<>
template<typename Struct>
struct VLCWrapper : public VLCObject {

public:
	// the vlc structure type.
	using TStruct = Struct;

	// the VLCKind for the type this wraps.
	static VLCX_CONSTANT VLCKind Kind = VLCStructKindOf<TStruct>();
	
	// the explicit implementation of the kind (VLC<>)
	using Implementation = VLC<Kind>;

	// the event manager type.
	using EventManager = VLCEventManager<Kind>;
	
	// the type of the VLCWrapper<>
	using Wrapper = VLCWrapper<Struct>;

	// returns a pointer to the TStruct off this unreleased wrapper or nullptr.
	VLCX_EXPRESSION TStruct *const GetStructure() const {
		return 0 == Alive ? nullptr : (TStruct*)Ptr;
	}

	// returns the pointer to the TStruct off this wrapper it was initialized with
	// (even if it had since been released).
	// USE WITH CAUTION.
	VLCX_EXPRESSION TStruct *const GetStructureUnchecked() const {
		return (TStruct*)Ptr;
	}
protected:
	// the VLC<> instance shall ONLY invoke this once in its deconstruction.
	VLCX_INLINE void Release() const {
		if (DoRelease()) VLCStructRelease(GetStructureUnchecked());
	}
protected:
	// VLC<> should invoke this constructor when it is being made with an existing struct reference, and not making a new one.
	explicit VLCX_INLINE VLCWrapper(TStruct *const& Ptr)
		: VLCObject(Ptr) {
		if(DoRetain()) VLCStructRetain((TStruct*)this->Ptr);
	}

	// makes a copy of an existing wrapper. retaining the underlying struct if it was not released prior.
	VLCX_INLINE VLCWrapper(const VLCWrapper &copy) : VLCWrapper(copy.GetStructure()) {}

	// VLC<> should invoke this constructor when it creates a new vlc struct instance through libvlc.
	VLCX_EXPRESSION VLCWrapper(EConstructorRetain, TStruct *const& Ptr)
		: VLCObject(ConstructorRetain, Ptr) {}

	// a constexpr constructor. initializes Kind in VLCObject but sets null as ptr.
	VLCX_EXPRESSION VLCWrapper() : VLCObject() {}
};


// utility structure for arguments to libvlc_event_attach/detach.
LIBVLCX_API struct VLCEventHandler {

	// the callback
	const libvlc_callback_t Callback;

	// the opaque data to pass to the callback.
	void *const UserData;

	// the type of event.
	const libvlc_event_type_t EventType;

	// initializer.
	VLCX_EXPRESSION VLCEventHandler(
		const libvlc_event_type_t eventType,
		const libvlc_callback_t callback,
		void*const userData) : Callback(callback), EventType(eventType), UserData(userData) {}

	// copy constructor.
	VLCX_EXPRESSION VLCEventHandler(const VLCEventHandler & copy)
		: Callback(copy.Callback), UserData(copy.UserData), EventType(copy.EventType) {}

	// copy constructor with user data override.
	VLCX_EXPRESSION VLCEventHandler(const VLCEventHandler & base, void* userData)
		: Callback(base.Callback), UserData(userData), EventType(base.EventType) {}

	// returns true if callback is non-nullptr.
	VLCX_EXPRESSION explicit operator bool() const { return nullptr != Callback; }

	// returns true if callback is nullptr.
	VLCX_EXPRESSION bool operator !()const { return !Callback; }
};

// base poly type for VLCKinds that expose an event manager.
// you should only access items in this type from VLCEventManager<> which inherits this as well as VLC<>.
LIBVLCX_API struct VLCEventManagerBase {
	// pointer to event manager or nullptr.
	libvlc_event_manager_t*const Manager;

protected:
	explicit VLCX_EXPRESSION VLCEventManagerBase(libvlc_event_manager_t*const& manager) : Manager(manager) {}
	explicit VLCX_EXPRESSION VLCEventManagerBase(const VLCEventManagerBase &copy) : Manager(copy.Manager) {}
public:
	// adds an event handler to the manager.
	VLCX_INLINE int Attach(libvlc_event_type_t i_event_type,
		libvlc_callback_t f_callback,
		void *const user_data)const {
		return libvlc_event_attach(Manager, i_event_type, f_callback, user_data);
	}

	// removes an event handler to the manager.
	VLCX_INLINE void Detach(libvlc_event_type_t i_event_type,
		libvlc_callback_t f_callback,
		void*const p_user_data)const {
		libvlc_event_detach(Manager, i_event_type, f_callback, p_user_data);
	}

	// adds an event handler to the manager.
	VLCX_INLINE int Attach(const VLCEventHandler &h) const { return Attach(h.EventType, h.Callback, h.UserData); }

	// removes an event handler to the manager.
	VLCX_INLINE void Detach(const VLCEventHandler &h) { return Detach(h.EventType, h.Callback, h.UserData); }

	// returns true if manager is non-nullptr.
	VLCX_EXPRESSION explicit operator bool() const { return nullptr != Manager; }

	// returns true if manager is nullptr.
	VLCX_EXPRESSION bool operator !()const { return !Manager; }
};

// event manager type specialized for each valid type covered in VLCKind.
// this inherits VLC<> and VLCEventManagerBase.
// you can optionally += and -= VLCEventHandler to it.
//
// you shall not explicitly define this type.
template<VLCKind Kind>
struct VLCEventManager : public VLCEventManagerBase, public VLC<Kind> {

	// ensure that the vlc type covered by the kind does have an event manager.
	static_assert(VLCHasEventManager(Kind), "kind does not have event manager");

	// initializes this event manager from the VLC<> kind. (retains a reference to the underlying type by copying it to this instance).
	// this also works as a copy constructor.
	VLCX_INLINE VLCEventManager(const VLC<Kind> & outer) 
		: Implementation(outer)
		, VLCEventManagerBase(outer.GetEventManagerStructure()) { }

	// allows you to add an event handler. if you need the return value of the removal use Attach instead.
	VLCX_INLINE VLCEventManager &operator +=(const VLCEventHandler & h) { Attach(h); return *this; }

	// allows you to remove an event handler. if you need the return value of the removal use Detach instead.
	VLCX_INLINE VLCEventManager &operator -=(const VLCEventHandler & h) { Detach(h); return *this; }
};

// place this macro into any VLC<> types that have a method to get the event manager from them.
// the only argument to this macro is that of the name of the libvlc function which when invoked with the TStruct pointer
// returns a libvlc_event_manager_t * (or nullptr).
//
// leaves visibility in public, terminate with semicolon
#define Macro_LIBVLCX_VLCBody(EventManagerFunction)														\
	private:																							\
		VLCX_INLINE libvlc_event_manager_t *const GetEventManagerStructure() const {					\
			if (TStruct *const m = GetStructure())														\
				if(libvlc_event_manager_t *const o = EventManagerFunction(m))							\
					return o;																			\
		}																								\
		friend struct VLCEventManager<Kind>;															\
	public:																								\
		using EventManager = VLCEventManager<Kind> /* use semicolon after insertion of macro */	


LIBVLCX_NAMESPACE(END)

#endif
