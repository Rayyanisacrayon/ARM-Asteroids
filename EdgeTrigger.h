//EdgeTrigger.h

#ifndef EdgeTrigger_H
#define EdgeTrigger_H
#include <stdint.h>

//sets up interrupts on port E, pins 0-3
void EdgeTrigger_Init(void);

typedef enum {inMenu, inLang, inGame, inPause} btnLoc_t;
extern btnLoc_t btnBehavior;
extern uint8_t fire1; 

void togglePause(void); 

#endif //EdgeTrigger_H
