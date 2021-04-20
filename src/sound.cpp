/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include <stdlib.h>
#include <stdio.h>

extern "C" {

#include "sound.h"
#include "memory.h"

#ifdef EMSCRIPTEN
EMSCRIPTEN_KEEPALIVE
int * getSoundRegisters()
{
	return (int *)&memory[0xFF10];
}

EMSCRIPTEN_KEEPALIVE
int * getMemoryPtr() {
	return (int *)&memory[0];
}
#endif

}

#ifndef EMSCRIPTEN
#include <Basic_Gb_Apu.h>
#include <Multi_Buffer.h>
#include <Sound_Queue.h>

static Basic_Gb_Apu apu;
static Sound_Queue sound;

static void handle_error( const char* str )
{
	if ( str )
	{
		fprintf( stderr, "Error: %s\n", str );
		exit( EXIT_FAILURE );
	}
}

void init_sound()
{
	long const sample_rate = 44100;
	handle_error( apu.set_sample_rate( sample_rate ) );
	
	// Generate a few seconds of sound and play using SDL
	handle_error( sound.start( sample_rate, 2 ) );
}


void sync_sound(u_int16 address, u_int8 byte)
{
	apu.write_register( address, byte);
}

void update_sound(int counter)
{
	apu.end_frame();
	int const buf_size = 2048;
	static blip_sample_t buf [buf_size];
		
	// Play whatever samples are available
	long count = apu.read_samples( buf, buf_size );
	sound.write( buf, count );
}
#endif

#ifdef EMSCRIPTEN
EM_JS(void, call_write_memory, (int address, int byte), {
	write_memory(address, byte);
});

EM_JS(void, call_update_sound, (int cycles), {
	update_sound(cycles);
})

void sync_sound(u_int16 address, u_int8 byte)
{
	// call_write_memory((int) address, (int) byte);
}

void update_sound(int cycles)
{
	call_update_sound(cycles);
}
#endif
