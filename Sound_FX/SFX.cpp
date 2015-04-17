// include relevant SDL dependecies and C dependencies
#include "../deps/include/SDL/SDL.h"
#include "../deps/include/SDL/SDL_mixer.h"
#include <stdio.h>
#include <string>

// include class definition file
#include "SFX.h"

int main() {
    // this should actually be placed in beginning of init game (in whatever file Rhenn puts that in)
    // then, can call sound effects by Mix_PlayChannel method
    SFX sound_fx;

    return 0;
}

// constructor, which inits all sound effects
void SFX::SFX() {
    /* declaring pointers for each sound effect */

    // leaderboard SFX: applause and booing
    Mix_Chunk *m_applause = NULL;
    Mix_Chunk *m_booing = NULL;

    // possible SFX for obstacles or danger
    Mix_Chunk *m_arc = NULL;
    Mix_Chunk *m_bubble = NULL;
    Mix_Chunk *m_burning = NULL;
    Mix_Chunk *m_earcing = NULL;
    Mix_Chunk *m_electric_shock = NULL;
    Mix_Chunk *m_explosion = NULL;
    Mix_Chunk *m_fire = NULL;
    Mix_Chunk *m_missile = NULL;

    // menu SFX for changing settings and for moving around the menu
    Mix_Chunk *m_change_setting = NULL;
    Mix_Chunk *m_menu_navigate = NULL;

    // player model SFX (punch is a possibility)
    Mix_Chunk *m_heartbeat = NULL;
    Mix_Chunk *m_jump = NULL;
    Mix_Chunk *m_death_scream = NULL;
    Mix_Chunk *m_punch = NULL;

    // environmental SFX
    Mix_Chunk *m_wind = NULL;
    Mix_Chunk *m_thunderstorm = NULL;
    Mix_Chunk *m_rain = NULL;


    // init audio interface, using open audio because all files are WAV format, so no need for Mix_Init
    // SHOULD THIS BE DELETED IF AUDIO IS OPENED ELSEWHERE???
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "Mixer error: %s\n", Mix_GetError());
    }

    /* load in all wav files */

    // for each sound effect, load into mixer and print error if not loaded correctly

    m_applause = Mix_LoadWAV("applause.wav");
    if (m_applause == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_booing = Mix_LoadWAV("booing.wav");
    if (m_booing == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_arc = Mix_LoadWAV("arc.wav");
    if (m_arc == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_bubble = Mix_LoadWAV("bubble.wav");
    if (m_bubble == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_burning = Mix_LoadWAV("burning.wav");
    if (m_burning == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_earcing = Mix_LoadWAV("earcing.wav");
    if (m_earcing == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_electric_shock = Mix_LoadWAV("electricshock.wav");
    if (m_electric_shock == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_explosion = Mix_LoadWAV("explosion.wav");
    if (m_explosion == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_fire = Mix_LoadWAV("fire.wav");
    if (m_fire == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_missile = Mix_LoadWAV("missile.wav");
    if (m_missile == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_change_setting = Mix_LoadWAV("change_setting.wav");
    if (m_change_setting == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_menu_navigate = Mix_LoadWAV("menu_navigate.wav");
    if (m_menu_navigate == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_heartbeat = Mix_LoadWAV("heartbeat.wav");
    if (m_heartbeat == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_jump = Mix_LoadWAV("jump.wav");
    if (m_jump == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_death_scream = Mix_LoadWAV("death_scream.wav");
    if (m_death_scream == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_punch = Mix_LoadWAV("punch.wav");
    if (m_punch == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_wind = Mix_LoadWAV("wind.wav");
    if (m_wind == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_thunderstorm = Mix_LoadWAV("thunderstorm.wav");
    if (m_thunderstorm == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }

    m_rain = Mix_LoadWAV("rain.wav");
    if (m_rain == NULL) {
        printf("Mixer error: %s\n", Mix_GetError());
    }
}

// empty destructor method, as closeFX function handles cleanup
void SFX::~SFX() {

}

// clean up function for sound effects, to be called by the cleanup function of the main game
void SFX::closeFX() {
    // free up all chunks of the sfx mix
    Mix_FreeChunk();

    // reset all pointers to null
    m_applause = NULL;
    m_booing = NULL;

    m_arc = NULL;
    m_bubble = NULL;
    m_burning = NULL;
    m_earcing = NULL;
    m_electric_shock = NULL;
    m_explosion = NULL;
    m_fire = NULL;
    m_missile = NULL;

    m_change_setting = NULL;
    m_menu_navigate = NULL;

    m_heartbeat = NULL;
    m_jump = NULL;
    m_death_scream = NULL;
    m_punch = NULL;

    m_wind = NULL;
    m_thunderstorm = NULL;
    m_rain = NULL;

    // quit the mixer (again, calling CloseAudio because all sfx are WAV files, so no need for Mix_Quit)
    Mix_CloseAudio();
}