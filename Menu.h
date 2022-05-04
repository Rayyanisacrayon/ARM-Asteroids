//Menu.h

#ifndef Menu_H
#define Menu_H
#include <stdint.h>

typedef enum {English, Spanish} Language_t;
extern Language_t myLanguage;
typedef enum {WELCOME, WELCOMEB, ONEP, TWOP, LANG} phrase_t;
extern const char Welcome_English[];
extern const char Welcome_Spanish[];
extern const char WelcomeB_English[];
extern const char WelcomeB_Spanish[];
extern const char Onep_English[];
extern const char Onep_Spanish[];
extern const char Twop_English[];
extern const char Twop_Spanish[];
extern const char Language_English[];
extern const char Language_Spanish[];

typedef enum {single, multi, lang} menuoption_t;
extern menuoption_t mainmenu;
extern Language_t langoption;
extern const char *Phrases[5][2];

extern uint8_t confirm;

void MenuInit(void);

void MenuLoop(void);

void LanguageMenu(void);




#endif //Menu_H
