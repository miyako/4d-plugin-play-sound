/* --------------------------------------------------------------------------------
 #
 #	4DPlugin-Sound.h
 #	source generated by 4D Plugin Wizard
 #	Project : Sound
 #	author : miyako
 #	2021/01/08
 #  
 # --------------------------------------------------------------------------------*/

#ifndef PLUGIN_SOUND_H
#define PLUGIN_SOUND_H

#include "4DPluginAPI.h"

#pragma mark -

void process_sound_file(PA_PluginParameters params);

#include "C_TEXT.h"

typedef enum {
    
    sound_action_open = 0,
    sound_action_pause = 1,
    sound_action_play = 2,
    sound_action_stop = 3,
    sound_action_close = 4,
    sound_action_resume = 5,
    sound_duration = 6,
    sound_is_playing = 7
    
}sound_action_t;

#endif /* PLUGIN_SOUND_H */
