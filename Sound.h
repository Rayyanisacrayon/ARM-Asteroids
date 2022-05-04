// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// 11/15/2021 
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

void Sound_Init(void);
typedef enum {Shoot, Boom} soundeffect;
void playsound(soundeffect);

#endif
