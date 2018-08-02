#include "sound.h"

// only create header symbols
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

// create header + implementation
#define CUTE_SOUND_IMPLEMENTATION
#include "cute_sound.h"

cs_context_t* ctx;
cs_loaded_sound_t load_sound[MAX_NUM_SOUNDS];
cs_playing_sound_t* play_sound[MAX_NUM_PLAYING_SOUNDS];
int num_playing_sounds = 0;

int initAudio()
{
	ctx = cs_make_context(GetConsoleWindow(), SOUND_FREQUENCY, SOUND_LATENCY, SOUND_BUFFERED_SECONDS, MAX_NUM_PLAYING_SOUNDS);
	cs_spawn_mix_thread(ctx);
	
	if (ctx)
		return 0;
	else
		return -1;
}

void loadAudio(const char* name, unsigned int id)
{
	freeAudio(id);

	const char *dot = strrchr(name, '.');
	if (!dot || dot == name) return;

	if (!strcmp(dot, ".ogg"))
		load_sound[id] = cs_load_ogg(name);
	else if (!strcmp(dot, ".wav"))
		load_sound[id] = cs_load_wav(name);
}

void playAudio(int id, bool loop, float volume, float pitch, float pan, float delay)
{
	cs_play_sound_def_t def = cs_make_def(&load_sound[id]);
	def.looped = loop ? 1 : 0;
	def.volume_left = volume;
	def.volume_right = volume;
	def.pitch = pitch;
	def.pan = pan;
	def.delay = delay;
	cs_play_sound(ctx, def);
}

void updateAudio()
{
	
}

void stopAudio(int id)
{
	for (int i = 0; i < num_playing_sounds; i++)
	{
		if (play_sound[i]->loaded_sound == &load_sound[id])
			cs_stop_sound(play_sound[i]);
	}
}

void stopAllAudio()
{
	cs_stop_all_sounds(ctx);
}

void freeAudio(int id)
{
	cs_free_sound(&load_sound[id]);
	memset(&load_sound[id], 0, sizeof(load_sound[id]));
}

void shutdownAudio()
{
	cs_stop_all_sounds(ctx);
	cs_shutdown_context(ctx);

	for (int i = 0; i < MAX_NUM_SOUNDS; i++)
	{
		freeAudio(i);
	}
}

// create implementation
#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"