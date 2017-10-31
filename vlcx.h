#pragma once
#ifndef HEADER_LIBVLCX
#	define HEADER_LIBVLCX 1

#	if !LIBVLCX_NO_VLC_H
#		include <vlc/vlc.h>
#	endif

#	include "vlcx_enum.h"
#	include "vlcx_alloc.h"
#	include "vlcx_object.h"

LIBVLCX_NAMESPACE(START)

template<> FORCEINLINE void VLCFreeList(libvlc_audio_output_t*const &adr) {
	libvlc_audio_output_list_release(adr);
}
template<> FORCEINLINE void VLCFreeSingle(libvlc_track_description_t*const &adr) {
	//libvlc_track_description_release(adr);
	libvlc_track_description_list_release(adr);
}
template<> FORCEINLINE void VLCFreeList(libvlc_track_description_t*const &adr) {
	libvlc_track_description_list_release(adr);
}
template<> FORCEINLINE void VLCFreeList(libvlc_audio_output_device_t*const &adr) {
	libvlc_audio_output_device_list_release(adr);
}
template<> FORCEINLINE void VLCFreeList(libvlc_module_description_t*const &adr) {
	libvlc_module_description_list_release(adr);
}
template<> FORCEINLINE void VLCFreeList(char*const &adr) {
	libvlc_free(adr);
}

using VLCString = VLCAllocList<char>;

template<> FORCEINLINE constexpr VLCKind VLCStructKindOf<libvlc_instance_t>() { return VLCKind::Library; }
template<> FORCEINLINE void VLCRelease(libvlc_instance_t*const&v) { libvlc_release(v); }
template<> FORCEINLINE void VLCRetain(libvlc_instance_t*const&v) { libvlc_retain(v); }

template<> FORCEINLINE constexpr VLCKind VLCStructKindOf<libvlc_media_player_t>() { return VLCKind::MediaPlayer; }
template<> FORCEINLINE void VLCRelease(libvlc_media_player_t*const&v) { libvlc_media_player_release(v); }
template<> FORCEINLINE void VLCRetain(libvlc_media_player_t*const&v) { libvlc_media_player_retain(v); }

template<> FORCEINLINE constexpr VLCKind VLCStructKindOf<libvlc_media_t>() { return VLCKind::Media; }
template<> FORCEINLINE void VLCRelease(libvlc_media_t*const&v) { libvlc_media_release(v); }
template<> FORCEINLINE void VLCRetain(libvlc_media_t*const&v) { libvlc_media_retain(v); }

template<>
struct VLC<VLCKind::Library> : public VLCWrapper<libvlc_instance_t> {
	FORCEINLINE VLC(int argc = 0, const char *const *argv = nullptr)
		: Wrapper(ConstructorRetain, libvlc_new(argc, argv)) {}
	FORCEINLINE VLC(TStruct *const& retain) : Wrapper(retain) { Retain(); }
	FORCEINLINE VLC(const VLC& copy) : Wrapper(copy.GetStructure()) { Retain(); }
	FORCEINLINE ~VLC() { Release(); }

	FORCEINLINE VLCAllocList<libvlc_audio_output_t> GetAudioOutputList() const {
		return libvlc_audio_output_list_get(GetStructure());
	}

	FORCEINLINE VLCAllocList<libvlc_audio_output_device_t> GetAudioOutputDeviceList(const char*const output) const {
		return libvlc_audio_output_device_list_get(GetStructure(), output);
	}

	FORCEINLINE VLCAllocList<libvlc_module_description_t> GetAudioFilterList()const {
		return libvlc_audio_filter_list_get(GetStructure());
	}

	FORCEINLINE VLCAllocList<libvlc_module_description_t> GetVideoFilterList()const {
		return libvlc_video_filter_list_get(GetStructure());
	}

	FORCEINLINE void SetLogCallback(const libvlc_log_cb callback, void*const data) {
		libvlc_log_set(GetStructure(), callback, data);
	}

	FORCEINLINE void SetLogFile(FILE*const stream) {
		libvlc_log_set_file(GetStructure(), stream);
	}
	FORCEINLINE void UnsetLogCallback() {
		libvlc_log_unset(GetStructure());
	}
	FORCEINLINE void SetUserAgent(
		const char *const name, const char *const http) {
		libvlc_set_user_agent(GetStructure(), name, http);
	}
	FORCEINLINE void SetAppId(const char *const id,
		const char *const version, const char *const icon) {
		libvlc_set_app_id(GetStructure(),id, version, icon);
	}
	FORCEINLINE void SetExitHandler(void(*cb) (void *), void *opaque) {
		libvlc_set_exit_handler(GetStructure(), cb, opaque);
	}
	FORCEINLINE void Wait()const {
		libvlc_wait(GetStructure());
	}
	FORCEINLINE bool AddInterface(const char*const name) {
		return libvlc_add_intf(GetStructure(), name) ? false : true;/*<--*/
	}

	/*VLM*/
	FORCEINLINE bool AddBroadcast(
		const char *const name, const char *const input,
		const char *const output, const int options,
		const char * const*const  ppsz_options,
		const bool enabled, const bool loop) {
		return libvlc_vlm_add_broadcast(GetStructure(),
			name, input,
			output, options,
			ppsz_options,
			enabled ? (int)1 : (int)0,
			loop ? (int)1 : (int)0) ? false : true;/*<--*/
	}
	FORCEINLINE bool AddVOD(
		const char *const name, const char *const input,
		const int options, const char * const* const ppsz_options,
		const bool enabled, const char *const mux) {
		return libvlc_vlm_add_vod(GetStructure(),
			name, input, options, ppsz_options, enabled ? (int)1 : (int)0, mux) ? false : true;/*<--*/
	}
	FORCEINLINE bool AddInput(
		const char *const name, const char *const input) {
		return libvlc_vlm_add_input(GetStructure(), name, input) ? false : true;/*<--*/
	}
	FORCEINLINE bool DeleteMedia(const char *const name) {
		return libvlc_vlm_del_media(GetStructure(), name) ? false : true;/*<--*/
	}
	FORCEINLINE bool SetEnabled(const char *const name, const bool enabled) {
		return libvlc_vlm_set_enabled(GetStructure(), name, enabled ? (int)1 : (int)0) ? false : true;/*<--*/
	}
	FORCEINLINE bool SetOutput(const char *const name, const char *const output) {
		return libvlc_vlm_set_output(GetStructure(), name, output) ? false : true;/*<--*/
	}
	FORCEINLINE bool SetInput(const char *const name, const char *const input) {
		return libvlc_vlm_set_input(GetStructure(), name, input) ? false : true;/*<--*/
	}
	FORCEINLINE bool SetMux(const char *const name, const char *const mux) {
		return libvlc_vlm_set_mux(GetStructure(), name, mux) ? false : true;/*<--*/
	}
	FORCEINLINE bool SetLoop(
		const char *const name, const bool enabled
	) {
		return libvlc_vlm_set_loop(GetStructure(), name, enabled ? (int)1 : (int)0) ? false : true;/*<--*/
	}
	FORCEINLINE bool ChangeMedia(
		const char *const name, const char *const input,
		const char *const output, const int options,
		const char * const*const  ppsz_options,
		const bool enabled, const bool loop) {
		return libvlc_vlm_change_media(GetStructure(),
			name, input,
			output, options,
			ppsz_options,
			enabled ? (int)1 : (int)0,
			loop ? (int)1 : (int)0) ? false : true;/*<--*/
	}
	FORCEINLINE bool PlayMedia(const char *const name) {
		return libvlc_vlm_play_media(GetStructure(), name) ? false : true;/*<--*/
	}
	FORCEINLINE bool StopMedia(const char *const name) {
		return libvlc_vlm_stop_media(GetStructure(), name) ? false : true;/*<--*/
	}
	FORCEINLINE bool PauseMedia(const char *const name) {
		return libvlc_vlm_pause_media(GetStructure(), name) ? false : true;/*<--*/
	}
	FORCEINLINE bool SeekMedia(const char *const name, const float percentage) {
		return libvlc_vlm_seek_media(GetStructure(), name, percentage) ? false : true;/*<--*/
	}
	FORCEINLINE float GetMediaInstancePosition(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_position(GetStructure(), name, instance_id);
	}
	FORCEINLINE int GetMediaInstanceTime(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_time(GetStructure(), name, instance_id);
	}
	FORCEINLINE int GetMediaInstanceLength(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_length(GetStructure(), name, instance_id);
	}
	FORCEINLINE int GetMediaInstanceRate(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_rate(GetStructure(), name, instance_id);
	}
	FORCEINLINE const char * ShowMedia(const char *const name) const {
		return libvlc_vlm_show_media(GetStructure(), name);/*<--*/
	}

	Macro_LIBVLCX_ExposeEventManager(libvlc_vlm_get_event_manager);
};
using VLCLibrary = VLC<VLCKind::Library>;


enum class VLCMediaStringUsage : uint8_t {
	Path,
	Location,
	NodeName,
};

template<VLCMediaStringUsage StrUse>
struct VLCMediaString {
	const char *const& Sz;
	FORCEINLINE constexpr VLCMediaString(const char *const& sz) : Sz(sz) { }
	FORCEINLINE constexpr operator const char *const &()const { return Sz; }
	static FORCEINLINE libvlc_media_t *Open(libvlc_instance_t*const& instance, const char *const& sz);
	FORCEINLINE libvlc_media_t *Open(libvlc_instance_t*const& instance) const { return Open(instance, Sz); }
};
using VLCMediaPathString = VLCMediaString<VLCMediaStringUsage::Path>;
using VLCMediaLocationString = VLCMediaString<VLCMediaStringUsage::Location>;
using VLCMediaNodeNameString = VLCMediaString<VLCMediaStringUsage::NodeName>;
#define Macro_VLCImplementMediaString(Usage,.../*operation*/) \
using VLCMedia##Usage##String = VLCMediaString<VLCMediaStringUsage::NodeName>; \
FORCEINLINE libvlc_media_t *VLCMedia##Usage##String::Open(libvlc_instance_t*const& instance, const char *const& sz){ \
	return (nullptr==instance||nullptr==sz)?nullptr:__VA_ARGS__; \
}
Macro_VLCImplementMediaString(Path, libvlc_media_new_path(instance, sz))
Macro_VLCImplementMediaString(Location, libvlc_media_new_location(instance, sz))
Macro_VLCImplementMediaString(NodeName, libvlc_media_new_as_node(instance, sz))
#undef Macro_VLCImplementMediaString


template<>
struct VLC<VLCKind::Media> : public VLCWrapper<libvlc_media_t> {
	enum EDuplicate { Duplicate, };

	FORCEINLINE VLC(VLCLibrary::TStruct *const &instance, const char *const& Sz, const VLCMediaStringUsage Type)
		: Wrapper(ConstructorRetain,
			Type == VLCMediaStringUsage::Location ? VLCMediaLocationString::Open(instance, Sz) :
			Type == VLCMediaStringUsage::NodeName ? VLCMediaNodeNameString::Open(instance, Sz) :
			Type == VLCMediaStringUsage::Path ? VLCMediaPathString::Open(instance, Sz) : nullptr)
	{
	}
	template<VLCMediaStringUsage Type>
	FORCEINLINE VLC(VLCLibrary::TStruct *const &instance, const VLCMediaString<Type> & Sz)
		: Wrapper(ConstructorRetain, Sz.Open(instance)) {}

	template<VLCMediaStringUsage Type>
	FORCEINLINE VLC(const VLCLibrary &instance, const VLCMediaString<Type> & Sz)
		: Wrapper(ConstructorRetain, Sz.Open(instance.GetStructure())) {}

	FORCEINLINE VLC(const VLCLibrary &instance, const char *const& Sz, const VLCMediaStringUsage Type) : VLC(instance.GetStructure(), Sz, Type) {}

	FORCEINLINE VLC(VLCLibrary::TStruct *const &instance, const int Fd)
		: Wrapper(ConstructorRetain,
			nullptr != instance ? libvlc_media_new_fd(instance, Fd) : nullptr) {}
	FORCEINLINE VLC(const VLCLibrary &instance, const int Fd) : VLC(instance.GetStructure(), Fd) {}
	explicit FORCEINLINE VLC(TStruct *const &ptr) : Wrapper(ptr) { Retain(); }
	FORCEINLINE VLC(EDuplicate, TStruct*const&copy) : Wrapper(ConstructorRetain, libvlc_media_duplicate(copy)){};
	FORCEINLINE VLC(EDuplicate, const VLC&copy) : Wrapper(ConstructorRetain, libvlc_media_duplicate(copy.GetStructure())){};
	FORCEINLINE VLC(const VLC&copy) : Wrapper(copy) {
		Retain();
	}
	FORCEINLINE ~VLC() {
		Release();
	}
	Macro_LIBVLCX_ExposeEventManager(libvlc_media_event_manager);

	FORCEINLINE libvlc_time_t GetDuration() const { return libvlc_media_get_duration(GetStructure()); }
	FORCEINLINE VLCString GetMRL() const { return libvlc_media_get_mrl(GetStructure()); }
	FORCEINLINE void Parse() { libvlc_media_parse(GetStructure()); }
	FORCEINLINE void BeginParse() { libvlc_media_parse_async(GetStructure()); }
	FORCEINLINE void Parse(bool asyncronous) { if (asyncronous)BeginParse();else Parse(); }
	FORCEINLINE VLCBoolean IsParsed() const { return VLCBool(libvlc_media_is_parsed(GetStructure())); }
	FORCEINLINE libvlc_state_t GetState() const { return libvlc_media_get_state(GetStructure()); }
	FORCEINLINE VLCString GetMeta(const libvlc_meta_t Entry)const { return libvlc_media_get_meta(GetStructure(), Entry); }
	FORCEINLINE void SetMeta(const libvlc_meta_t Entry, const char *const& Value) {
		libvlc_media_set_meta(GetStructure(), Entry, Value);
	}
	template<libvlc_meta_t Entry>
	FORCEINLINE VLCString GetMeta() const { return GetMeta(Entry); }

	template<libvlc_meta_t Entry>
	FORCEINLINE void SetMeta(const char *const& Value) { SetMeta(Entry, Value); }
	FORCEINLINE VLCBoolean SaveMeta() { return VLCBool(libvlc_media_save_meta(GetStructure())); }

	FORCEINLINE void AddOptions(const char *const& Options) { libvlc_media_add_option(GetStructure(), Options); }
	FORCEINLINE void AddOptionFlag(const char *const& Options, unsigned int Flags) { libvlc_media_add_option_flag(GetStructure(), Options, Flags); }
	FORCEINLINE VLCBoolean GetStats(libvlc_media_stats_t &stats) const { return VLCBool(libvlc_media_get_stats(GetStructure(), &stats)); }
	FORCEINLINE libvlc_media_stats_t GetStats(bool &got) const {
		libvlc_media_stats_t stats{0};
		got = GetStats(stats);
		return stats;
	}
	FORCEINLINE libvlc_media_stats_t GetStats() const {
		libvlc_media_stats_t stats{ 0 };
		GetStats(stats);
		return stats;
	}

	FORCEINLINE void *GetUserData()const { return libvlc_media_get_user_data(GetStructure()); }
	FORCEINLINE void SetUserData(void *const userData) { libvlc_media_set_user_data(GetStructure(),userData); }

};
using VLCMedia = VLC<VLCKind::Media>;

enum class VLCDrawableKind : uint8_t {
	Draw_NSObject=0,
	Draw_AGL=1,
	Draw_HWND=2,
	Draw_XWindow = 3,
};
template<typename T> struct VLCDrawableKindHandleInfo { using THandle = T; };
template<VLCDrawableKind Kind> struct VLCDrawableKindInfo;
#define Macro_VLCDrawableKind(Kind,Handle,GetF,SetF) \
template<> struct VLCDrawableKindInfo<VLCDrawableKind::Draw_##Kind> \
	: public VLCDrawableKindHandleInfo<Handle> { \
	static FORCEINLINE void Set(libvlc_media_player_t*const m, THandle const h) { Set(m, h); } \
	static FORCEINLINE THandle Get(libvlc_media_player_t*const m) { return GetF(m); } \
}
Macro_VLCDrawableKind(NSObject, void*, libvlc_media_player_get_nsobject, libvlc_media_player_set_nsobject);
Macro_VLCDrawableKind(HWND, void*, libvlc_media_player_get_hwnd, libvlc_media_player_set_hwnd);
Macro_VLCDrawableKind(XWindow, uint32_t, libvlc_media_player_get_xwindow, libvlc_media_player_set_xwindow);
Macro_VLCDrawableKind(AGL, uint32_t, libvlc_media_player_get_agl, libvlc_media_player_set_agl);
#undef Macro_VLCDrawableKind


template<VLCDrawableKind Kind>
struct VLCDrawableKindInfo<Kind, true> {
	static_assert(IsVoidPtr(Kind) && (uint8_t)Kind < VLCDrawableKindNum(), "Invalid context");
	using THandle = void*;
};

template<VLCDrawableKind Kind>
struct VLCDrawableKindInfo<Kind, false> {
	static_assert(!IsVoidPtr(Kind) && (uint8_t)Kind < VLCDrawableKindNum(), "Invalid context");
	using THandle = uint32_t;
};



template<>
struct VLC<VLCKind::MediaPlayer> : public VLCWrapper<libvlc_media_player_t> {
	explicit FORCEINLINE VLC(VLCLibrary::TStruct *const &instance)
		: Wrapper(ConstructorRetain,
			nullptr != instance ?
			libvlc_media_player_new(instance) : nullptr) {
	}
	explicit FORCEINLINE VLC(VLCMedia::TStruct*const &media)
		: Wrapper(ConstructorRetain,
			nullptr != media ? libvlc_media_player_new_from_media(media) : nullptr) {
	}
	explicit FORCEINLINE VLC(TStruct *const &ptr) : Wrapper(ptr) {}

	FORCEINLINE VLC(const VLC&copy) : Wrapper(copy) {
		Retain();
	}
	FORCEINLINE ~VLC() {
		Release();
	}

	FORCEINLINE void SetMedia(VLCMedia::TStruct *const &media) {
		if (auto m = GetStructure())
			libvlc_media_player_set_media(m, media);
	}
	FORCEINLINE void SetMedia(VLCMedia &media) {
		SetMedia(media.GetStructure());
	}
	FORCEINLINE VLCMedia::TStruct *const GetMediaStructure() const {
		if (auto m = GetStructure())
			return libvlc_media_player_get_media(m);
		return nullptr;
	}
	FORCEINLINE VLCMedia GetMedia() const { return VLCMedia(GetMediaStructure()); }

	FORCEINLINE void Play() { libvlc_media_player_play(GetStructure()); }
	FORCEINLINE void Pause() { libvlc_media_player_pause(GetStructure()); }
	FORCEINLINE void Stop() { libvlc_media_player_stop(GetStructure()); }
	FORCEINLINE void SetPause(bool pause) { libvlc_media_player_set_pause(GetStructure(), pause ? (int)1 : (int)0); }

	FORCEINLINE bool IsSeekable() const { return libvlc_media_player_is_seekable(GetStructure()) ? true : false; }
	FORCEINLINE bool CanPause() const { return libvlc_media_player_can_pause(GetStructure()) ? true : false; }
	FORCEINLINE bool IsPlaying()const { return libvlc_media_player_is_playing(GetStructure()) ? true : false; }
	FORCEINLINE bool GetFullscreen() const { return libvlc_get_fullscreen(GetStructure()) ? true : false; }
	FORCEINLINE void ToggleFullscreen() { libvlc_toggle_fullscreen(GetStructure()); }
	FORCEINLINE void SetFullscreen(const bool on) { libvlc_set_fullscreen(GetStructure(), on ? (int)1 : (int)0); }
	FORCEINLINE void SetKeyInput(const bool on) { libvlc_video_set_key_input(GetStructure(), on ? (int)1 : (int)0); }
	FORCEINLINE void SetMouseInput(const bool on) { libvlc_video_set_mouse_input(GetStructure(), on ? (int)1 : (int)0); }


	FORCEINLINE bool GetVideoSize(unsigned &width, unsigned &height, const unsigned num = 0)const { return libvlc_video_get_size(GetStructure(), num, &width, &height)?true:false; }
	FORCEINLINE bool GetCursor(int &x, int &y, const unsigned num = 0)const { return libvlc_video_get_cursor(GetStructure(), num, &x, &y) ? true : false; }
	FORCEINLINE float GetVideoScale()const { return libvlc_video_get_scale(GetStructure()); }
	FORCEINLINE void SetVideoScale(const float factor) { libvlc_video_set_scale(GetStructure(), factor); }
	FORCEINLINE VLCString GetAspectRatio()const { return libvlc_video_get_aspect_ratio(GetStructure()); }
	FORCEINLINE void SetAspectRatio(const char*const aspect) { libvlc_video_set_aspect_ratio(GetStructure(), aspect); }

	FORCEINLINE int GetSPU()const { return libvlc_video_get_spu(GetStructure()); }
	FORCEINLINE bool SetSPU(const int spu) { return libvlc_video_set_spu(GetStructure(), spu) ? false : true;/*<-- note that zero is returned on success*/ }
	FORCEINLINE int GetSPUCount() const { return libvlc_video_get_spu_count(GetStructure()); }
	FORCEINLINE bool SetSubtitleFile(const char *const subtitle) { return libvlc_video_set_subtitle_file(GetStructure(), subtitle) ? true : false; }
	FORCEINLINE int64_t GetSPUDelay()const { return libvlc_video_get_spu_delay(GetStructure()); }
	FORCEINLINE bool SetSPUDelay(const int64_t &delay) { return libvlc_video_set_spu_delay(GetStructure(), delay) ? false : true;/*<--*/ }
	FORCEINLINE VLCString GetCropGeometry()const { return libvlc_video_get_crop_geometry(GetStructure()); }
	FORCEINLINE void SetCropGeometry(const char *const geometry) { libvlc_video_set_crop_geometry(GetStructure(), geometry); }
	FORCEINLINE int GetVideoTrack()const { return libvlc_video_get_track(GetStructure()); }
	FORCEINLINE int GetTitle() const { return libvlc_media_player_get_title(GetStructure()); }
	FORCEINLINE int GetTitleCount() const { return libvlc_media_player_get_title_count(GetStructure()); }
	FORCEINLINE int GetChapterCountForTitle(const int title) const { return libvlc_media_player_get_chapter_count_for_title(GetStructure(), title); }
	FORCEINLINE int GetChapterCount() const { return libvlc_media_player_get_chapter_count(GetStructure()); }
	FORCEINLINE int GetChapter()const { return libvlc_media_player_get_chapter(GetStructure()); }
	FORCEINLINE void SetChapter(const int chapter) { libvlc_media_player_set_chapter(GetStructure(), chapter); }
	FORCEINLINE float GetPosition() const { return libvlc_media_player_get_position(GetStructure()); }
	FORCEINLINE void SetPosition(const float position)const { libvlc_media_player_set_position(GetStructure(), position); }
	FORCEINLINE libvlc_time_t GetTime() const { return libvlc_media_player_get_time(GetStructure()); }
	FORCEINLINE libvlc_time_t GetLength() const { return libvlc_media_player_get_length(GetStructure()); }
	FORCEINLINE void SetTime(const libvlc_time_t &time)const { libvlc_media_player_set_time(GetStructure(), time); }
	FORCEINLINE bool WillPlay()const { return libvlc_media_player_will_play(GetStructure()) ? true : false; }
	FORCEINLINE VLCAllocList<libvlc_track_description_t> GetTitleDescription() const { return libvlc_video_get_title_description(GetStructure()); }
	FORCEINLINE VLCAllocList<libvlc_track_description_t> GetVideoTrackDescription() const { return libvlc_video_get_track_description(GetStructure()); }
	FORCEINLINE VLCAllocList<libvlc_track_description_t> GetSPUDescription() const { return libvlc_video_get_spu_description(GetStructure()); }
	FORCEINLINE VLCAllocList<libvlc_track_description_t> GetChapterDescription(const int title) const { return libvlc_video_get_chapter_description(GetStructure(), title); }
	FORCEINLINE void SetTitleDisplay(libvlc_position_t position, unsigned int timeout) {
		libvlc_media_player_set_video_title_display(GetStructure(), position, timeout);
	}
	FORCEINLINE void Navigate(const VLCEnum<libvlc_navigate_mode_t> navigate) {
		libvlc_media_player_navigate(GetStructure(), navigate);
	}
	FORCEINLINE void NextFrame() {
		libvlc_media_player_next_frame(GetStructure());
	}
	FORCEINLINE void NextChapter() {
		libvlc_media_player_next_chapter(GetStructure());
	}
	FORCEINLINE void PreviousChapter() {
		libvlc_media_player_next_chapter(GetStructure());
	}
	FORCEINLINE unsigned HasVOut()const { return libvlc_media_player_has_vout(GetStructure()); }
	// same as HasVOut, which is somewhat confusing.
	FORCEINLINE unsigned GetEstablishedVideoOutputCount()const { return HasVOut(); }
	FORCEINLINE float GetFPS()const { return libvlc_media_player_get_fps(GetStructure()); }
	FORCEINLINE float GetRate()const { return libvlc_media_player_get_rate(GetStructure()); }
	FORCEINLINE bool SetRate(const float rate) { return libvlc_media_player_set_rate(GetStructure(), rate) ? false : true;/*<--*/ }
	FORCEINLINE int GetVideoTrackCount()const { return libvlc_video_get_track_count(GetStructure()); }
	FORCEINLINE bool SetVideoTrack(const int track) { return libvlc_video_set_track(GetStructure(), track) ? false : true; /*<--*/}
	FORCEINLINE int GetTeletext()const { return libvlc_video_get_teletext(GetStructure()); }
	FORCEINLINE void SetTeletext(const int page) { libvlc_video_set_teletext(GetStructure(), page); }
	FORCEINLINE void ToggleTeletext() { libvlc_toggle_teletext(GetStructure()); }
	FORCEINLINE int GetLogoInt(const VLCEnum<libvlc_video_logo_option_t> option)const {
		return libvlc_video_get_logo_int(GetStructure(), option);
	}
	FORCEINLINE void SetLogoInt(const VLCEnum<libvlc_video_logo_option_t>  option, const int value) {
		libvlc_video_set_logo_int(GetStructure(), option, value);
	}
	FORCEINLINE void SetLogoString(const VLCEnum<libvlc_video_logo_option_t>  option, const char *const value) {
		libvlc_video_set_logo_string(GetStructure(), option, value);
	}
	FORCEINLINE void SetLogo(const VLCEnum<libvlc_video_logo_option_t> option, const int value) { SetLogoInt(option, value); }
	FORCEINLINE void SetLogo(const VLCEnum<libvlc_video_logo_option_t> option, const char *const value) { SetLogoString(option, value); }
	FORCEINLINE void GetLogo(const VLCEnum<libvlc_video_logo_option_t> option, int&value)const { value = GetLogoInt(option); }

	FORCEINLINE int GetVideoAdjustmentInt(const VLCEnum<libvlc_video_adjust_option_t> option)const {
		return libvlc_video_get_adjust_int(GetStructure(), option);
	}
	FORCEINLINE float GetVideoAdjustmentFloat(const VLCEnum<libvlc_video_adjust_option_t> option)const {
		return libvlc_video_get_adjust_float(GetStructure(), option);
	}
	FORCEINLINE void SetVideoAdjustmentInt(const VLCEnum<libvlc_video_adjust_option_t> option, const int value) {
		libvlc_video_set_adjust_int(GetStructure(), option, value);
	}
	FORCEINLINE void SetVideoAdjustmentFloat(const VLCEnum<libvlc_video_adjust_option_t> option, const float value) {
		libvlc_video_set_adjust_float(GetStructure(), option, value);
	}
	FORCEINLINE void GetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, int&value)const { value = GetVideoAdjustmentInt(option); }
	FORCEINLINE void GetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, float&value)const { value = GetVideoAdjustmentFloat(option); }
	FORCEINLINE void SetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, const int value) { SetVideoAdjustmentInt(option, value); }
	FORCEINLINE void SetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, const float value) { SetVideoAdjustmentFloat(option, value); }
	
	FORCEINLINE int GetMarqueeInt(const VLCEnum<libvlc_video_marquee_option_t> option)const {
		return libvlc_video_get_marquee_int(GetStructure(), option);
	}
	FORCEINLINE VLCString GetMarqueeString(const VLCEnum<libvlc_video_marquee_option_t> option)const {
		return libvlc_video_get_marquee_string(GetStructure(), option);
	}
	FORCEINLINE void SetMarqueeInt(const VLCEnum<libvlc_video_marquee_option_t> option, const int value) {
		libvlc_video_set_marquee_int(GetStructure(), option, value);
	}
	FORCEINLINE void SetMarqueeString(const VLCEnum<libvlc_video_marquee_option_t> option, const char *const value) {
		libvlc_video_set_marquee_string(GetStructure(), option, value);
	}
	FORCEINLINE void GetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, int&value)const { value = GetMarqueeInt(option); }
	FORCEINLINE void SetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, const int value) { SetMarqueeInt(option, value); }
	FORCEINLINE void SetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, const char*const value) { SetMarqueeString(option, value); }
	
	FORCEINLINE void SetVideoFormat(const char *const chroma,
		unsigned width, unsigned height,
		unsigned pitch) {
		libvlc_video_set_format(GetStructure(), chroma, width, height, pitch);
	}
	FORCEINLINE void SetVideoCallbacks(
		libvlc_video_lock_cb lock,
		libvlc_video_unlock_cb unlock,
		libvlc_video_display_cb display,
		void *opaque) {
		libvlc_video_set_callbacks(GetStructure(), lock, unlock, display, opaque);
	}
	FORCEINLINE void SetVideoFormatCallbacks(
		libvlc_video_format_cb setup,
		libvlc_video_cleanup_cb cleanup
	) {
		libvlc_video_set_format_callbacks(GetStructure(), setup, cleanup);
	}

	template<VLCDrawableKind Drawable>
	FORCEINLINE auto SetDrawable(typename VLCDrawableKindInfo<Drawable>::THandle const handle) {
		return VLCDrawableKindInfo<Drawable>::Set(GetStructure(), handle);
	}
	template<VLCDrawableKind Drawable>
	FORCEINLINE auto GetDrawable()const {
		return VLCDrawableKindInfo<Drawable>::Get(GetStructure());
	}
#define Macro_VLCDrawableKindGetSet(Kind)\
	FORCEINLINE auto Set##Kind (VLCDrawableKindInfo<VLCDrawableKind::Draw_##Kind>::THandle const handle){ return SetDrawable<VLCDrawableKind::Draw_##Kind>(handle); } \
	FORCEINLINE auto Get##Kind () const {return GetDrawable<VLCDrawableKind::Draw_##Kind>();}
	Macro_VLCDrawableKindGetSet(AGL)
	Macro_VLCDrawableKindGetSet(XWindow)
	Macro_VLCDrawableKindGetSet(HWND)
	Macro_VLCDrawableKindGetSet(NSObject)
#undef Macro_VLCDrawableKindGetSet

	FORCEINLINE void SetDeinterlace(const char *const mode) {
		libvlc_video_set_deinterlace(GetStructure(), mode);
	}

	FORCEINLINE bool TakeSnapshot(const unsigned num,
		const char *const filepath, const unsigned int width,
		const unsigned int height) const {
		return libvlc_video_take_snapshot(GetStructure(), num, filepath, width, height) ? false : true;/*<--*/
	}



	FORCEINLINE bool SetAudioOutput(const char*const name) {
		return libvlc_audio_output_set(GetStructure(), name) ? false : true/*<--*/;
	}
	FORCEINLINE void SetAudioOutputDevice(const char*const output, const char*const device) {
		return libvlc_audio_output_device_set(GetStructure(), output, device);
	}
	FORCEINLINE void ToggleMute() { libvlc_audio_toggle_mute(GetStructure()); }
	FORCEINLINE bool GetMute() const { return libvlc_audio_get_mute(GetStructure()); }
	FORCEINLINE void SetMute(const bool mute) { libvlc_audio_set_mute(GetStructure(), mute); }

	//0...100
	FORCEINLINE int GetVolume()const { return libvlc_audio_get_volume(GetStructure()); }
	FORCEINLINE bool SetVolume(const int value) const { return libvlc_audio_set_volume(GetStructure(), value) ? false : true;/*<--*/ }
	FORCEINLINE int GetAudioTrackCount()const { return libvlc_audio_get_track_count(GetStructure()); }
	FORCEINLINE VLCAllocList<libvlc_track_description_t> GetAudioTrackDescription() const { return libvlc_audio_get_track_description(GetStructure()); }
	FORCEINLINE int GetAudioTrack()const { return libvlc_audio_get_track(GetStructure()); }
	FORCEINLINE bool SetAudioTrack(const int track) { return libvlc_audio_set_track(GetStructure(), track)?false:true;/*<--*/ }
	FORCEINLINE VLCEnumI<libvlc_audio_output_channel_t> GetAudioChannel()const { return libvlc_audio_get_channel(GetStructure()); }
	FORCEINLINE bool SetAudioChannel(const VLCEnumI<libvlc_audio_output_channel_t> channel) { return libvlc_audio_set_channel(GetStructure(), channel) ? false : true;/*<--*/ }

	FORCEINLINE int64_t GetAudioDelay()const { return libvlc_audio_get_delay(GetStructure()); }
	FORCEINLINE bool SetAudioDelay(const int64_t &value) { return libvlc_audio_set_delay(GetStructure(), value)?false:true;/*<--*/ }

	FORCEINLINE void SetAudioFormat(const char *const format,
		unsigned rate, unsigned channels) {
		libvlc_audio_set_format(GetStructure(), format, rate, channels);
	}

	FORCEINLINE void SetAudioCallbacks(
		libvlc_audio_play_cb play,
		libvlc_audio_pause_cb pause,
		libvlc_audio_resume_cb resume,
		libvlc_audio_flush_cb flush,
		libvlc_audio_drain_cb drain,
		void *opaque) {
		libvlc_audio_set_callbacks(GetStructure(), play, pause, resume, flush, drain, opaque);
	}
	FORCEINLINE void SetAudioFormatCallbacks(
		libvlc_audio_setup_cb setup,
		libvlc_audio_cleanup_cb cleanup) {
		libvlc_audio_set_format_callbacks(GetStructure(), setup, cleanup);
	}
	FORCEINLINE void SetVolumeCallback(
		libvlc_audio_set_volume_cb set_volume
	) {
		libvlc_audio_set_volume_callback(GetStructure(), set_volume);
	}


	Macro_LIBVLCX_ExposeEventManager(libvlc_media_player_event_manager);
};
using VLCMediaPlayer = VLC<VLCKind::MediaPlayer>;

#undef Macro_LIBVLCX_ExposeEventManager

LIBVLCX_NAMESPACE(END)

#endif