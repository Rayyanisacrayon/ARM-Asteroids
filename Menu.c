//Menu.c 
//Select the language of the main menu screen and game. 

#include "Menu.h"
#include "../inc/tm4c123gh6pm.h"
#include "Images.h"
#include "ST7735.h"
#include "EdgeTrigger.h"
#include "game.h"
#include "random.h"


//define language variables
Language_t myLanguage=English;
const char Welcome_English[] ="Welcome";
const char Welcome_Spanish[] ="Bienvenido"; 
const char WelcomeB_English[] ="to";
const char WelcomeB_Spanish[] ="a";
const char Onep_English[]="Single-Player";
const char Onep_Spanish[]="Individual";
const char Twop_English[]="Two-Player";
const char Twop_Spanish[]="Acompo\xA4" "ado";
const char Language_English[]="Language";
const char Language_Spanish[]="Idioma";

menuoption_t mainmenu = multi;
Language_t langoption = English;
uint8_t confirm = 0;

const char *Phrases[5][2]={
  {Welcome_English,Welcome_Spanish},
	{WelcomeB_English, WelcomeB_Spanish},
  {Onep_English, Onep_Spanish},
	{Twop_English, Twop_Spanish},
  {Language_English,Language_Spanish}
};


//Displays the Logo, "Welcome To:", and menu options
void MenuInit(void){
	ST7735_FillScreen(0x0000);            // set screen to black

	uint8_t x=8;	uint8_t len=0;
	for(len = 0; Phrases[WELCOME][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 10,Phrases[WELCOME][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}	
	x=45;
	for(len = 0; Phrases[WELCOMEB][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 30,Phrases[WELCOMEB][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}

	ST7735_DrawBitmap(20, 70, logo, 100, 24);  //outputs logo
			
			
	//display menu options
	ST7735_SetTextColor(ST7735_WHITE);
	ST7735_SetCursor(7, 8);
	ST7735_OutString((char *)Phrases[ONEP][myLanguage]);
  ST7735_SetCursor(7, 10);
//	ST7735_OutString((char *)Phrases[TWOP][myLanguage]);
  ST7735_SetCursor(7, 12);
	ST7735_OutString((char *)Phrases[LANG][myLanguage]);
		
	//setup interrupts
	EdgeTrigger_Init();
	btnBehavior = inMenu;
		
	MenuLoop();
}


//Main menu loop
void MenuLoop (void){
	while(1){
		//depending on value of main menu, move the ship
		if(mainmenu == single){
			ST7735_DrawBitmap(12, 92, p1ship0d, 16, 16);  
			ST7735_FillRect(12, 112-16, 16, 17, ST7735_BLACK); //hide other two options
			ST7735_FillRect(12, 132-16, 16, 17, ST7735_BLACK);
			
			//Send to game
			if(confirm == 1){ 
				confirm = 0;
				GameInit();
			}
		}
		if(mainmenu == multi){
			ST7735_DrawBitmap(12, 112, p1ship0d, 16, 16);  
			ST7735_FillRect(12, 92-16, 16, 17, ST7735_BLACK); //hide other two options
			ST7735_FillRect(12, 132-16, 16, 17, ST7735_BLACK);
			
			//send to multiplayer
			if(confirm == 1){ 
				confirm = 0;
			}
		}
		if(mainmenu == lang){
			ST7735_DrawBitmap(12, 132, p1ship0d, 16, 16);  
			ST7735_FillRect(12, 112-16, 16, 17, ST7735_BLACK); //hide other two options
			ST7735_FillRect(12, 92-16, 16, 17, ST7735_BLACK);
			// send to language loop
			if(confirm == 1){ 
				confirm = 0;
				btnBehavior = inLang;
				LanguageMenu();
			}
		}
	}
}

void LanguageMenu(void){
	ST7735_FillScreen(0x0000); 
	ST7735_SetCursor(7, 4);
	ST7735_OutString("English");
	ST7735_SetCursor(7, 7);
	ST7735_OutString("Espa\xA4ol");
	while(1){
		if(langoption == English){
			ST7735_DrawBitmap(12, 50, p1ship0d, 16, 16);  
			ST7735_FillRect(12, 80-16, 16, 17, ST7735_BLACK); //hide other option
			
			if(confirm == 1){ 
				confirm = 0;
				myLanguage = English;
				MenuInit();
			}
		}
		if(langoption == Spanish){
			ST7735_DrawBitmap(12, 80, p1ship0d, 16, 16);  
			ST7735_FillRect(12, 50-16, 16, 17, ST7735_BLACK); //hide other option
			
			if(confirm == 1){ 
				confirm = 0;
				myLanguage = Spanish;
				btnBehavior = inMenu;
				MenuInit();
			}
		}
	}
	
}
