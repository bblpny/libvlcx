#pragma once
#ifndef HEADER_LIBVLCX
#	define HEADER_LIBVLCX 1

#	if !LIBVLCX_NO_VLC_H
#		include <vlc/vlc.h>
#	endif

#	include "vlcx_enum.h"
#	include "vlcx_alloc.h"
#	include "vlcx_object.h"
#	include "vlcx_media_string.h"
#	include "vlcx_drawable.h"

LIBVLCX_NAMESPACE(START)

Macro_LIBVLCX_BindKind(Instance,
	libvlc_instance_t,
	libvlc_retain,
	libvlc_release);

Macro_LIBVLCX_BindKind(Media,
	libvlc_media_t,
	libvlc_media_retain,
	libvlc_media_release);

Macro_LIBVLCX_BindKind(MediaPlayer,
	libvlc_media_player_t,
	libvlc_media_player_retain,
	libvlc_media_player_release);

// no longer need the macro as there are finite things to choose from.
#	undef Macro_LIBVLCX_BindKind

Macro_LIBVLCX_BindAlloc(
	libvlc_audio_output_t,
	libvlc_audio_output_list_release,
	VLCAudioOutputList);

Macro_LIBVLCX_BindAlloc(
	libvlc_track_description_t,
	libvlc_track_description_list_release,
	VLCTrackDescriptionList);

Macro_LIBVLCX_BindAlloc(
	libvlc_audio_output_device_t,
	libvlc_audio_output_device_list_release,
	VLCAudioOutputDeviceList);

Macro_LIBVLCX_BindAlloc(
	libvlc_module_description_t,
	libvlc_module_description_list_release,
	VLCModuleDescriptionList);

Macro_LIBVLCX_BindAlloc(
	char,
	libvlc_free,
	VLCString);


Macro_LIBVLCX_BindMediaString(Path, libvlc_media_new_path(instance, sz))
Macro_LIBVLCX_BindMediaString(Location, libvlc_media_new_location(instance, sz))
Macro_LIBVLCX_BindMediaString(NodeName, libvlc_media_new_as_node(instance, sz))
// no longer need the macro as there are finite things to choose from.
#	undef Macro_LIBVLCX_BindMediaString

Macro_LIBVLCX_BindDrawable(NSObject, void*, libvlc_media_player_get_nsobject, libvlc_media_player_set_nsobject);
Macro_LIBVLCX_BindDrawable(HWND, void*, libvlc_media_player_get_hwnd, libvlc_media_player_set_hwnd);
Macro_LIBVLCX_BindDrawable(XWindow, uint32_t, libvlc_media_player_get_xwindow, libvlc_media_player_set_xwindow);
Macro_LIBVLCX_BindDrawable(AGL, uint32_t, libvlc_media_player_get_agl, libvlc_media_player_set_agl);
// no longer need the macro as there are finite things to choose from.
#	undef Macro_LIBVLCX_BindDrawable

template<> struct VLC<VLCKind::Instance> 
	: public VLCWrapper<libvlc_instance_t> 
{
	// construct a new vlc lib instance with command line arguments.
	VLCX_INLINE VLC(const int argc, const char *const *const &argv)
		: Wrapper(ConstructorRetain, libvlc_new(argc, 0==argc?nullptr:argv)) {}
	template<size_t argc>
	VLCX_INLINE VLC(const char *const (&argv)[argc]) : VLC((int)argc, 0== argc?nullptr:&argv[0]) {};
	VLCX_INLINE VLC(TStruct *const& retain) : Wrapper(retain) {}
	VLCX_INLINE VLC(const VLC& copy) : Wrapper(copy.GetStructure()) {}
	VLCX_INLINE VLC() : VLC(0, nullptr) {}
	VLCX_INLINE ~VLC() { Release(); }

	VLCX_INLINE VLCAudioOutputList && GetAudioOutputList() const {
		return libvlc_audio_output_list_get(GetStructure());
	}

	VLCX_INLINE VLCAudioOutputDeviceList && GetAudioOutputDeviceList(const char*const output) const {
		return libvlc_audio_output_device_list_get(GetStructure(), output);
	}

	VLCX_INLINE VLCModuleDescriptionList && GetAudioFilterList()const {
		return libvlc_audio_filter_list_get(GetStructure());
	}

	VLCX_INLINE VLCModuleDescriptionList && GetVideoFilterList()const {
		return libvlc_video_filter_list_get(GetStructure());
	}

	VLCX_INLINE void SetLogCallback(const libvlc_log_cb callback, void*const data) {
		libvlc_log_set(GetStructure(), callback, data);
	}

	VLCX_INLINE void SetLogFile(FILE*const stream) {
		libvlc_log_set_file(GetStructure(), stream);
	}
	VLCX_INLINE void UnsetLogCallback() {
		libvlc_log_unset(GetStructure());
	}
	VLCX_INLINE void SetUserAgent(
		const char *const name, const char *const http) {
		libvlc_set_user_agent(GetStructure(), name, http);
	}
	VLCX_INLINE void SetAppId(const char *const id,
		const char *const version, const char *const icon) {
		libvlc_set_app_id(GetStructure(),id, version, icon);
	}
	VLCX_INLINE void SetExitHandler(void(*cb) (void *), void *opaque) {
		libvlc_set_exit_handler(GetStructure(), cb, opaque);
	}
	VLCX_INLINE void Wait()const {
		libvlc_wait(GetStructure());
	}
	VLCX_INLINE VLCResult AddInterface(const char*const name) {
		return VLCRes(libvlc_add_intf(GetStructure(), name));/*<--*/
	}

	/*VLM*/
	VLCX_INLINE VLCResult AddMediaBroadcast(
		const char *const name, const char *const input,
		const char *const output, const int options,
		const char * const*const  ppsz_options,
		const bool enabled, const bool loop) {
		return VLCRes(libvlc_vlm_add_broadcast(GetStructure(),
			name, input,
			output, options,
			ppsz_options,
			enabled ? (int)1 : (int)0,
			loop ? (int)1 : (int)0));/*<--*/
	}
	VLCX_INLINE VLCResult AddMediaVOD(
		const char *const name, const char *const input,
		const int options, const char * const* const ppsz_options,
		const bool enabled, const char *const mux) {
		return VLCRes(libvlc_vlm_add_vod(GetStructure(),
			name, input, options, ppsz_options, enabled ? (int)1 : (int)0, mux));/*<--*/
	}
	VLCX_INLINE VLCResult AddMediaInput(
		const char *const name, const char *const input) {
		return VLCRes(libvlc_vlm_add_input(GetStructure(), name, input));/*<--*/
	}
	VLCX_INLINE VLCResult DeleteMedia(const char *const name) {
		return VLCRes(libvlc_vlm_del_media(GetStructure(), name));/*<--*/
	}
	VLCX_INLINE VLCResult SetMediaEnabled(const char *const name, const bool enabled) {
		return VLCRes(libvlc_vlm_set_enabled(GetStructure(), name, enabled ? (int)1 : (int)0));/*<--*/
	}
	VLCX_INLINE VLCResult SetMediaOutput(const char *const name, const char *const output) {
		return VLCRes(libvlc_vlm_set_output(GetStructure(), name, output));/*<--*/
	}
	VLCX_INLINE VLCResult SetMediaInput(const char *const name, const char *const input) {
		return VLCRes(libvlc_vlm_set_input(GetStructure(), name, input));/*<--*/
	}
	VLCX_INLINE VLCResult SetMediaMux(const char *const name, const char *const mux) {
		return VLCRes(libvlc_vlm_set_mux(GetStructure(), name, mux));/*<--*/
	}
	VLCX_INLINE VLCResult SetMediaLoop(
		const char *const name, const bool enabled
	) {
		return VLCRes(libvlc_vlm_set_loop(GetStructure(), name, enabled ? (int)1 : (int)0));/*<--*/
	}
	VLCX_INLINE VLCResult ChangeMedia(
		const char *const name, const char *const input,
		const char *const output, const int options,
		const char * const*const  ppsz_options,
		const bool enabled, const bool loop) {
		return VLCRes(libvlc_vlm_change_media(GetStructure(),
			name, input,
			output, options,
			ppsz_options,
			enabled ? (int)1 : (int)0,
			loop ? (int)1 : (int)0));/*<--*/
	}
	VLCX_INLINE VLCResult PlayMedia(const char *const name) {
		return VLCRes(libvlc_vlm_play_media(GetStructure(), name));/*<--*/
	}
	VLCX_INLINE VLCResult StopMedia(const char *const name) {
		return VLCRes(libvlc_vlm_stop_media(GetStructure(), name));/*<--*/
	}
	VLCX_INLINE VLCResult PauseMedia(const char *const name) {
		return VLCRes(libvlc_vlm_pause_media(GetStructure(), name));/*<--*/
	}
	VLCX_INLINE VLCResult SeekMedia(const char *const name, const float percentage) {
		return VLCRes(libvlc_vlm_seek_media(GetStructure(), name, percentage));/*<--*/
	}
	VLCX_INLINE float GetMediaPosition(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_position(GetStructure(), name, instance_id);
	}
	VLCX_INLINE int GetMediaTime(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_time(GetStructure(), name, instance_id);
	}
	VLCX_INLINE int GetMediaLength(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_length(GetStructure(), name, instance_id);
	}
	VLCX_INLINE int GetMediaRate(const char *const name, const int instance_id) const {
		return libvlc_vlm_get_media_instance_rate(GetStructure(), name, instance_id);
	}
	VLCX_INLINE const char * ShowMedia(const char *const name) const {
		return libvlc_vlm_show_media(GetStructure(), name);/*<--*/
	}

	Macro_LIBVLCX_VLCBody(libvlc_vlm_get_event_manager);
};

template<>
struct VLC<VLCKind::Media> : public VLCWrapper<libvlc_media_t> {
	
	enum EDuplicate { Duplicate, };

#	define Macro_LIBVLCX_MediaUseCase(Name) Type == VLCMediaUse::Name ? VLCMedia##Name##String::Open(instance, Sz)

	VLCX_INLINE VLC(VLCStruct<VLCKind::Instance> *const &instance, const char *const& Sz, const VLCMediaUse Type)
		: Wrapper(ConstructorRetain,
			Macro_LIBVLCX_Enumerate_VLCMediaUse(Macro_LIBVLCX_MediaUseCase, : ) : nullptr) {
#	undef Macro_LIBVLCX_MediaUseCase
	}
	template<VLCMediaUse Type>
	VLCX_INLINE VLC(VLCStruct<VLCKind::Instance> *const &instance, const VLCMediaString<Type> & Sz)
		: Wrapper(ConstructorRetain, Sz.Open(instance)) {}

	template<VLCMediaUse Type>
	VLCX_INLINE VLC(const VLCInstance &instance, const VLCMediaString<Type> & Sz)
		: Wrapper(ConstructorRetain, Sz.Open(instance.GetStructure())) {}

	VLCX_INLINE VLC(const VLCInstance &instance, const char *const& Sz, const VLCMediaUse Type)
		: VLC(instance.GetStructure(), Sz, Type) {}

	VLCX_INLINE VLC(VLCStruct<VLCKind::Instance> *const &instance, const int Fd)
		: Wrapper(ConstructorRetain,
			nullptr != instance ? libvlc_media_new_fd(instance, Fd) : nullptr) {}

	VLCX_INLINE VLC(const VLCInstance &instance, const int Fd) : VLC(instance.GetStructure(), Fd) {}

	explicit VLCX_INLINE VLC(TStruct *const &ptr) : Wrapper(ptr) {}

	VLCX_INLINE VLC(EDuplicate, TStruct*const&copy) : Wrapper(ConstructorRetain, libvlc_media_duplicate(copy)){}
	VLCX_INLINE VLC(EDuplicate, const VLC&copy) : Wrapper(ConstructorRetain, libvlc_media_duplicate(copy.GetStructure())){}
	VLCX_INLINE VLC(const VLC&copy) : Wrapper(copy) { }
	VLCX_INLINE ~VLC() { Release(); }

	Macro_LIBVLCX_VLCBody(libvlc_media_event_manager);

	VLCX_INLINE libvlc_time_t GetDuration() const { return libvlc_media_get_duration(GetStructure()); }
	VLCX_INLINE VLCString && GetMRL() const { return libvlc_media_get_mrl(GetStructure()); }
	VLCX_INLINE void Parse() { libvlc_media_parse(GetStructure()); }
	VLCX_INLINE void BeginParse() { libvlc_media_parse_async(GetStructure()); }
	VLCX_INLINE void Parse(bool asyncronous) { if (asyncronous)BeginParse();else Parse(); }
	VLCX_INLINE VLCBoolean IsParsed() const { return VLCBool(libvlc_media_is_parsed(GetStructure())); }
	VLCX_INLINE libvlc_state_t GetState() const { return libvlc_media_get_state(GetStructure()); }
	VLCX_INLINE VLCString && GetMeta(const libvlc_meta_t Entry)const { return libvlc_media_get_meta(GetStructure(), Entry); }
	VLCX_INLINE void SetMeta(const libvlc_meta_t Entry, const char *const& Value) {
		libvlc_media_set_meta(GetStructure(), Entry, Value);
	}
	template<libvlc_meta_t Entry>
	VLCX_INLINE VLCString &&GetMeta() const { return GetMeta(Entry); }

	template<libvlc_meta_t Entry>
	VLCX_INLINE void SetMeta(const char *const& Value) { SetMeta(Entry, Value); }
	VLCX_INLINE VLCBoolean SaveMeta() { return VLCBool(libvlc_media_save_meta(GetStructure())); }

	VLCX_INLINE void AddOptions(const char *const& Options) { libvlc_media_add_option(GetStructure(), Options); }
	VLCX_INLINE void AddOptionFlag(const char *const& Options, unsigned int Flags) { libvlc_media_add_option_flag(GetStructure(), Options, Flags); }
	VLCX_INLINE VLCBoolean GetStats(libvlc_media_stats_t &stats) const { return VLCBool(libvlc_media_get_stats(GetStructure(), &stats)); }
	VLCX_INLINE libvlc_media_stats_t GetStats(bool &got) const {
		libvlc_media_stats_t stats{0};
		got = GetStats(stats);
		return stats;
	}
	VLCX_INLINE libvlc_media_stats_t GetStats() const {
		libvlc_media_stats_t stats{ 0 };
		GetStats(stats);
		return stats;
	}

	VLCX_INLINE void *GetUserData()const { return libvlc_media_get_user_data(GetStructure()); }
	VLCX_INLINE void SetUserData(void *const userData) { libvlc_media_set_user_data(GetStructure(),userData); }

};

template<>
struct VLC<VLCKind::MediaPlayer> : public VLCWrapper<libvlc_media_player_t> {
	explicit VLCX_INLINE VLC(VLCStruct<VLCKind::Instance> *const &instance)
		: Wrapper(ConstructorRetain,
			nullptr != instance ?
			libvlc_media_player_new(instance) : nullptr) {
	}
	explicit VLCX_INLINE VLC(VLCMedia::TStruct*const &media)
		: Wrapper(ConstructorRetain,
			nullptr != media ? libvlc_media_player_new_from_media(media) : nullptr) {
	}
	explicit VLCX_INLINE VLC(const VLCInstance &instance) : VLC(instance.GetStructure()) {}
	explicit VLCX_INLINE VLC(const VLCMedia &media) : VLC(media.GetStructure()) {}
	explicit VLCX_INLINE VLC(TStruct *const &ptr) : Wrapper(ptr) {}

	VLCX_INLINE VLC(const VLC&copy) : Wrapper(copy) {}
	VLCX_INLINE ~VLC() { Release(); }

	VLCX_INLINE void SetMedia(VLCMedia::TStruct *const &media) {
		if (auto m = GetStructure())
			libvlc_media_player_set_media(m, media);
	}
	VLCX_INLINE void SetMedia(VLCMedia &media) {
		SetMedia(media.GetStructure());
	}
	VLCX_INLINE VLCMedia::TStruct *const GetMediaStructure() const {
		if (auto m = GetStructure())
			return libvlc_media_player_get_media(m);
		return nullptr;
	}
	VLCX_INLINE VLCMedia GetMedia() const { return VLCMedia(GetMediaStructure()); }

	VLCX_INLINE VLCResult Play() { return VLCRes(libvlc_media_player_play(GetStructure())); }
	VLCX_INLINE void Pause() { libvlc_media_player_pause(GetStructure()); }
	VLCX_INLINE void Stop() { libvlc_media_player_stop(GetStructure()); }
	VLCX_INLINE void SetPause(bool pause) { libvlc_media_player_set_pause(GetStructure(), pause ? (int)1 : (int)0); }

	VLCX_INLINE VLCBoolean IsSeekable() const { return VLCBool(libvlc_media_player_is_seekable(GetStructure())); }
	VLCX_INLINE VLCBoolean CanPause() const { return VLCBool(libvlc_media_player_can_pause(GetStructure())); }
	VLCX_INLINE VLCBoolean IsPlaying()const { return VLCBool(libvlc_media_player_is_playing(GetStructure())); }
	VLCX_INLINE VLCBoolean GetFullscreen() const { return VLCBool(libvlc_get_fullscreen(GetStructure())); }
	VLCX_INLINE void ToggleFullscreen() { libvlc_toggle_fullscreen(GetStructure()); }
	VLCX_INLINE void SetFullscreen(const bool on) { libvlc_set_fullscreen(GetStructure(), on ? (int)1 : (int)0); }
	VLCX_INLINE void SetKeyInput(const bool on) { libvlc_video_set_key_input(GetStructure(), on ? (int)1 : (int)0); }
	VLCX_INLINE void SetMouseInput(const bool on) { libvlc_video_set_mouse_input(GetStructure(), on ? (int)1 : (int)0); }


	VLCX_INLINE VLCResult GetVideoSize(unsigned &width, unsigned &height, const unsigned num = 0)const { return VLCRes(libvlc_video_get_size(GetStructure(), num, &width, &height));/*<--*/ }
	VLCX_INLINE VLCResult GetCursor(int &x, int &y, const unsigned num = 0)const { return VLCRes(libvlc_video_get_cursor(GetStructure(), num, &x, &y));/*<--*/ }
	VLCX_INLINE float GetVideoScale()const { return libvlc_video_get_scale(GetStructure()); }
	VLCX_INLINE void SetVideoScale(const float factor) { libvlc_video_set_scale(GetStructure(), factor); }
	VLCX_INLINE VLCString && GetAspectRatio()const { return libvlc_video_get_aspect_ratio(GetStructure()); }
	VLCX_INLINE void SetAspectRatio(const char*const aspect) { libvlc_video_set_aspect_ratio(GetStructure(), aspect); }

	VLCX_INLINE int GetSPU()const { return libvlc_video_get_spu(GetStructure()); }
	VLCX_INLINE VLCResult SetSPU(const int spu) { return VLCRes(libvlc_video_set_spu(GetStructure(), spu));/*<-- note that zero is returned on success*/ }
	VLCX_INLINE int GetSPUCount() const { return libvlc_video_get_spu_count(GetStructure()); }
	// note that docs say this returns the success status (boolean)
	// this really isn't clear on if true or false is returned for success.
	//
	// So until someone wants to test this, returning int.
	VLCX_INLINE int SetSubtitleFile(const char *const subtitle) { return libvlc_video_set_subtitle_file(GetStructure(), subtitle); }
	VLCX_INLINE int64_t GetSPUDelay()const { return libvlc_video_get_spu_delay(GetStructure()); }
	VLCX_INLINE VLCResult SetSPUDelay(const int64_t &delay) { return VLCRes(libvlc_video_set_spu_delay(GetStructure(), delay));/*<--*/ }
	VLCX_INLINE VLCString && GetCropGeometry()const { return libvlc_video_get_crop_geometry(GetStructure()); }
	VLCX_INLINE void SetCropGeometry(const char *const geometry) { libvlc_video_set_crop_geometry(GetStructure(), geometry); }
	VLCX_INLINE int GetVideoTrack()const { return libvlc_video_get_track(GetStructure()); }
	VLCX_INLINE int GetTitle() const { return libvlc_media_player_get_title(GetStructure()); }
	VLCX_INLINE int GetTitleCount() const { return libvlc_media_player_get_title_count(GetStructure()); }
	VLCX_INLINE int GetChapterCountForTitle(const int title) const { return libvlc_media_player_get_chapter_count_for_title(GetStructure(), title); }
	VLCX_INLINE int GetChapterCount() const { return libvlc_media_player_get_chapter_count(GetStructure()); }
	VLCX_INLINE int GetChapter()const { return libvlc_media_player_get_chapter(GetStructure()); }
	VLCX_INLINE void SetChapter(const int chapter) { libvlc_media_player_set_chapter(GetStructure(), chapter); }
	VLCX_INLINE float GetPosition() const { return libvlc_media_player_get_position(GetStructure()); }
	VLCX_INLINE void SetPosition(const float position)const { libvlc_media_player_set_position(GetStructure(), position); }
	VLCX_INLINE libvlc_time_t GetTime() const { return libvlc_media_player_get_time(GetStructure()); }
	VLCX_INLINE libvlc_time_t GetLength() const { return libvlc_media_player_get_length(GetStructure()); }
	VLCX_INLINE void SetTime(const libvlc_time_t &time)const { libvlc_media_player_set_time(GetStructure(), time); }
	VLCX_INLINE VLCBoolean WillPlay()const { return VLCBool(libvlc_media_player_will_play(GetStructure())); }
	VLCX_INLINE VLCTrackDescriptionList && GetTitleDescription() const { return libvlc_video_get_title_description(GetStructure()); }
	VLCX_INLINE VLCTrackDescriptionList && GetVideoTrackDescription() const { return libvlc_video_get_track_description(GetStructure()); }
	VLCX_INLINE VLCTrackDescriptionList && GetSPUDescription() const { return libvlc_video_get_spu_description(GetStructure()); }
	VLCX_INLINE VLCTrackDescriptionList && GetChapterDescription(const int title) const { return libvlc_video_get_chapter_description(GetStructure(), title); }
	VLCX_INLINE void SetTitleDisplay(libvlc_position_t position, unsigned int timeout) {
		libvlc_media_player_set_video_title_display(GetStructure(), position, timeout);
	}
	VLCX_INLINE void Navigate(const VLCEnum<libvlc_navigate_mode_t> navigate) {
		libvlc_media_player_navigate(GetStructure(), navigate);
	}
	VLCX_INLINE void NextFrame() {
		libvlc_media_player_next_frame(GetStructure());
	}
	VLCX_INLINE void NextChapter() {
		libvlc_media_player_next_chapter(GetStructure());
	}
	VLCX_INLINE void PreviousChapter() {
		libvlc_media_player_next_chapter(GetStructure());
	}
	VLCX_INLINE unsigned HasVOut()const { return libvlc_media_player_has_vout(GetStructure()); }
	// same as HasVOut, which is somewhat confusing.
	VLCX_INLINE unsigned GetEstablishedVideoOutputCount()const { return HasVOut(); }
	VLCX_INLINE float GetFPS()const { return libvlc_media_player_get_fps(GetStructure()); }
	VLCX_INLINE float GetRate()const { return libvlc_media_player_get_rate(GetStructure()); }
	VLCX_INLINE VLCResult SetRate(const float rate) { return VLCRes(libvlc_media_player_set_rate(GetStructure(), rate));/*<--*/ }
	VLCX_INLINE int GetVideoTrackCount()const { return libvlc_video_get_track_count(GetStructure()); }
	VLCX_INLINE VLCResult SetVideoTrack(const int track) { return VLCRes(libvlc_video_set_track(GetStructure(), track)); /*<--*/}
	VLCX_INLINE int GetTeletext()const { return libvlc_video_get_teletext(GetStructure()); }
	VLCX_INLINE void SetTeletext(const int page) { libvlc_video_set_teletext(GetStructure(), page); }
	VLCX_INLINE void ToggleTeletext() { libvlc_toggle_teletext(GetStructure()); }
	VLCX_INLINE int GetLogoInt(const VLCEnum<libvlc_video_logo_option_t> option)const {
		return libvlc_video_get_logo_int(GetStructure(), option);
	}
	VLCX_INLINE void SetLogoInt(const VLCEnum<libvlc_video_logo_option_t>  option, const int value) {
		libvlc_video_set_logo_int(GetStructure(), option, value);
	}
	VLCX_INLINE void SetLogoString(const VLCEnum<libvlc_video_logo_option_t>  option, const char *const value) {
		libvlc_video_set_logo_string(GetStructure(), option, value);
	}
	VLCX_INLINE void SetLogo(const VLCEnum<libvlc_video_logo_option_t> option, const int value) { SetLogoInt(option, value); }
	VLCX_INLINE void SetLogo(const VLCEnum<libvlc_video_logo_option_t> option, const char *const value) { SetLogoString(option, value); }
	VLCX_INLINE void GetLogo(const VLCEnum<libvlc_video_logo_option_t> option, int&value)const { value = GetLogoInt(option); }

	VLCX_INLINE int GetVideoAdjustmentInt(const VLCEnum<libvlc_video_adjust_option_t> option)const {
		return libvlc_video_get_adjust_int(GetStructure(), option);
	}
	VLCX_INLINE float GetVideoAdjustmentFloat(const VLCEnum<libvlc_video_adjust_option_t> option)const {
		return libvlc_video_get_adjust_float(GetStructure(), option);
	}
	VLCX_INLINE void SetVideoAdjustmentInt(const VLCEnum<libvlc_video_adjust_option_t> option, const int value) {
		libvlc_video_set_adjust_int(GetStructure(), option, value);
	}
	VLCX_INLINE void SetVideoAdjustmentFloat(const VLCEnum<libvlc_video_adjust_option_t> option, const float value) {
		libvlc_video_set_adjust_float(GetStructure(), option, value);
	}
	VLCX_INLINE void GetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, int&value)const { value = GetVideoAdjustmentInt(option); }
	VLCX_INLINE void GetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, float&value)const { value = GetVideoAdjustmentFloat(option); }
	VLCX_INLINE void SetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, const int value) { SetVideoAdjustmentInt(option, value); }
	VLCX_INLINE void SetVideoAdjustment(const VLCEnum<libvlc_video_adjust_option_t> option, const float value) { SetVideoAdjustmentFloat(option, value); }
	
	VLCX_INLINE int GetMarqueeInt(const VLCEnum<libvlc_video_marquee_option_t> option)const {
		return libvlc_video_get_marquee_int(GetStructure(), option);
	}
	VLCX_INLINE VLCString && GetMarqueeString(const VLCEnum<libvlc_video_marquee_option_t> option)const {
		return libvlc_video_get_marquee_string(GetStructure(), option);
	}
	VLCX_INLINE void SetMarqueeInt(const VLCEnum<libvlc_video_marquee_option_t> option, const int value) {
		libvlc_video_set_marquee_int(GetStructure(), option, value);
	}
	VLCX_INLINE void SetMarqueeString(const VLCEnum<libvlc_video_marquee_option_t> option, const char *const value) {
		libvlc_video_set_marquee_string(GetStructure(), option, value);
	}
	VLCX_INLINE void GetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, int&value)const { value = GetMarqueeInt(option); }
	VLCX_INLINE void SetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, const int value) { SetMarqueeInt(option, value); }
	VLCX_INLINE void SetMarquee(const VLCEnum<libvlc_video_marquee_option_t> option, const char*const value) { SetMarqueeString(option, value); }
	
	VLCX_INLINE void SetVideoFormat(const char *const chroma,
		unsigned width, unsigned height,
		unsigned pitch) {
		libvlc_video_set_format(GetStructure(), chroma, width, height, pitch);
	}
	VLCX_INLINE void SetVideoCallbacks(
		libvlc_video_lock_cb lock,
		libvlc_video_unlock_cb unlock,
		libvlc_video_display_cb display,
		void *opaque) {
		libvlc_video_set_callbacks(GetStructure(), lock, unlock, display, opaque);
	}
	VLCX_INLINE void SetVideoFormatCallbacks(
		libvlc_video_format_cb setup,
		libvlc_video_cleanup_cb cleanup
	) {
		libvlc_video_set_format_callbacks(GetStructure(), setup, cleanup);
	}

	template<VLCDrawableKind Drawable>
	VLCX_INLINE auto SetDrawable(typename VLCDrawableKindInfo<Drawable>::THandle const handle) {
		return VLCDrawableKindInfo<Drawable>::Set(GetStructure(), handle);
	}
	template<VLCDrawableKind Drawable>
	VLCX_INLINE auto GetDrawable()const {
		return VLCDrawableKindInfo<Drawable>::Get(GetStructure());
	}

#	define Macro_LIBVLCX_DrawableKindGetSet(Kind)\
	VLCX_INLINE auto Set##Kind (VLCDrawableKindInfo<VLCDrawableKind::Draw_##Kind>::THandle const handle){ return SetDrawable<VLCDrawableKind::Draw_##Kind>(handle); } \
	VLCX_INLINE auto Get##Kind () const {return GetDrawable<VLCDrawableKind::Draw_##Kind>();}
	
	Macro_LIBVLCX_Enumerate_VLCDrawableKind(Macro_LIBVLCX_DrawableKindGetSet)

#	undef Macro_LIBVLCX_DrawableKindGetSet

	VLCX_INLINE void SetDeinterlace(const char *const mode) {
		libvlc_video_set_deinterlace(GetStructure(), mode);
	}

	VLCX_INLINE VLCResult TakeSnapshot(const unsigned num,
		const char *const filepath, const unsigned int width,
		const unsigned int height) const {
		return VLCRes(libvlc_video_take_snapshot(GetStructure(), num, filepath, width, height));/*<--*/
	}



	VLCX_INLINE VLCResult SetAudioOutput(const char*const name) {
		return VLCRes(libvlc_audio_output_set(GetStructure(), name))/*<--*/;
	}
	VLCX_INLINE void SetAudioOutputDevice(const char*const output, const char*const device) {
		return libvlc_audio_output_device_set(GetStructure(), output, device);
	}
	VLCX_INLINE void ToggleMute() { libvlc_audio_toggle_mute(GetStructure()); }
	// note that this returns negative if muting is irrelevant (no-audio).
	// this is why it is not wrapped to VLCBoolean 
	VLCX_INLINE int GetMute() const { return libvlc_audio_get_mute(GetStructure()); }
	VLCX_INLINE void SetMute(const bool mute) { libvlc_audio_set_mute(GetStructure(), mute); }

	//0...100
	VLCX_INLINE int GetVolume()const { return libvlc_audio_get_volume(GetStructure()); }
	VLCX_INLINE VLCResult SetVolume(const int value) { return VLCRes(libvlc_audio_set_volume(GetStructure(), value));/*<--*/ }
	VLCX_INLINE int GetAudioTrackCount()const { return libvlc_audio_get_track_count(GetStructure()); }
	VLCX_INLINE VLCTrackDescriptionList && GetAudioTrackDescription() const { return libvlc_audio_get_track_description(GetStructure()); }
	VLCX_INLINE int GetAudioTrack()const { return libvlc_audio_get_track(GetStructure()); }
	VLCX_INLINE VLCResult SetAudioTrack(const int track) { return VLCRes(libvlc_audio_set_track(GetStructure(), track));/*<--*/ }
	VLCX_INLINE VLCEnumI<libvlc_audio_output_channel_t> GetAudioChannel()const { return libvlc_audio_get_channel(GetStructure()); }
	VLCX_INLINE VLCResult SetAudioChannel(const VLCEnumI<libvlc_audio_output_channel_t> channel) { return VLCRes(libvlc_audio_set_channel(GetStructure(), channel));/*<--*/ }

	VLCX_INLINE int64_t GetAudioDelay()const { return libvlc_audio_get_delay(GetStructure()); }
	VLCX_INLINE VLCResult SetAudioDelay(const int64_t &value) { return VLCRes(libvlc_audio_set_delay(GetStructure(), value)); }

	VLCX_INLINE void SetAudioFormat(const char *const format,
		unsigned rate, unsigned channels) {
		libvlc_audio_set_format(GetStructure(), format, rate, channels);
	}

	VLCX_INLINE void SetAudioCallbacks(
		libvlc_audio_play_cb play,
		libvlc_audio_pause_cb pause,
		libvlc_audio_resume_cb resume,
		libvlc_audio_flush_cb flush,
		libvlc_audio_drain_cb drain,
		void *opaque) {
		libvlc_audio_set_callbacks(GetStructure(), play, pause, resume, flush, drain, opaque);
	}
	VLCX_INLINE void SetAudioFormatCallbacks(
		libvlc_audio_setup_cb setup,
		libvlc_audio_cleanup_cb cleanup) {
		libvlc_audio_set_format_callbacks(GetStructure(), setup, cleanup);
	}
	VLCX_INLINE void SetVolumeCallback(
		libvlc_audio_set_volume_cb set_volume
	) {
		libvlc_audio_set_volume_callback(GetStructure(), set_volume);
	}
	Macro_LIBVLCX_VLCBody(libvlc_media_player_event_manager);
};

// no longer needed.
#	undef Macro_LIBVLCX_VLCBody

LIBVLCX_NAMESPACE(END)

#endif