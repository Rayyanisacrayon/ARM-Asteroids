//Game.c

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "game.h"
#include "st7735.h"
#include "Timer0.h"
#include "EdgeTrigger.h" 
#include "menu.h"
#include "Timer1.h"
#include "Timer3.h"
#include "sprites.h"

uint32_t nsecs = 0;
uint8_t refresh = 0;

const char win_English[] ="You Win!";
const char win_Spanish[] ="\xADT\xA3 ganas!";
const char score_English[] ="Your Score:";
const char score_Spanish[] ="Tu puntaje";
const char restart_English[] ="Hit restart to play";
const char restart_Spanish[] ="Presiona reiniciar";
const char restartb_English[] ="again";
const char restartb_Spanish[] ="para volver a jugar.";
const char ready_English[] ="Ready?";
const char ready_Spanish[] ="\xA8Listo?";
const char start_English[] ="Press PE1 to start.";
const char start_Spanish[] ="Presiona PE1 para";
const char startb_Spanish[] ="comenzar";
const char gameover_English[] = "Game Over";
const char gameover_Spanish[] = "Fin del";
const char gameoverB_Spanish[] = "Juego";




const char *gPhrases[9][2]={
  {win_English,win_Spanish},
	{score_English, score_Spanish},
  {restart_English, restart_Spanish},
	{restartb_English, restartb_Spanish},
	{ready_English, ready_Spanish},
	{start_English, start_Spanish},
	{"", startb_Spanish},
	{gameover_English, gameover_Spanish},
	{"", gameoverB_Spanish},


};

void DisableInterrupts(void); 
void EnableInterrupts(void);
void victory(void);

void GameInit(void){
	DisableInterrupts();
	ST7735_FillScreen(0x0000);            // set screen to black
	EnableInterrupts();
	
	
	//display start screen  
	uint8_t x=10;	uint8_t len=0;

	for(len = 0; gPhrases[4][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 10,gPhrases[4][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}
	ST7735_SetCursor(0, 7);
	ST7735_OutString((char *)gPhrases[5][myLanguage]);
	if(myLanguage == Spanish){
	ST7735_SetCursor(0, 8);
	ST7735_OutString((char *)gPhrases[6][myLanguage]);		
	}	
	while(confirm == 0){}
	confirm = 0;
	//end start screen	
	ST7735_FillScreen(0x0000);            // set screen to black

		
		
		
	btnBehavior = inGame;
	Timer0_Init(80000000, 1);
  Timer1_Init(8000000, 2);
	Timer3_Init(spawnNextAsteroid, 84000000);


	GameLoop();
}

void GameLoop(void){
	while(1){
		//display timer
		if(refresh == 1){
			ST7735_SetCursor(0, 0);
			ST7735_OutUDec(nsecs);
			refresh = 0;
		}
		
		//check win condition
		if(nsecs >= 120){
			victory();
		}
		
		
		
	};
}


void Timer0A_Handler (void) {
	TIMER0_ICR_R = 0x01;
	nsecs++;
	refresh = 1;

}






void victory(){
	DisableInterrupts();
	TIMER0_CTL_R = 0x0; //stop incrementing nsecs
	ST7735_FillScreen(0x0000);            // set screen to black
	
	uint8_t x=10;	uint8_t len=0;

	for(len = 0; gPhrases[0][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 10,gPhrases[0][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}
	
	x=10;
	for(len = 0; gPhrases[1][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 50,gPhrases[1][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}
	ST7735_SetCursor(10, 10);
	ST7735_OutUDec(nsecs);
		
	
	ST7735_SetCursor(0, 14);
	ST7735_OutString((char *)gPhrases[2][myLanguage]);
		
	ST7735_SetCursor(0, 15);
	ST7735_OutString((char *)gPhrases[3][myLanguage]);
	while(1);
	
}

void gameover(){
	DisableInterrupts();
	TIMER0_CTL_R = 0x0; //stop incrementing nsecs
	ST7735_FillScreen(0x0000);            // set screen to black
	
	uint8_t x=10;	uint8_t len=0;

	for(len = 0; gPhrases[7][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 10,gPhrases[7][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}
	x=10;
	if(myLanguage == Spanish){
		for(len = 0; gPhrases[8][myLanguage][len] != '\0'; len++);
		for(uint8_t i = 0; i < len; i++){
			ST7735_DrawChar(x, 30,gPhrases[8][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
			x+=11;}
	}
	x=10;
	for(len = 0; gPhrases[1][myLanguage][len] != '\0'; len++);
	for(uint8_t i = 0; i < len; i++){
		ST7735_DrawChar(x, 60,gPhrases[1][myLanguage][i], ST7735_WHITE, ST7735_BLACK, 2);
		x+=11;}
	ST7735_SetCursor(10, 10);
	ST7735_OutUDec(nsecs);
		
	
	ST7735_SetCursor(0, 14);
	ST7735_OutString((char *)gPhrases[2][myLanguage]);
		
	ST7735_SetCursor(0, 15);
	ST7735_OutString((char *)gPhrases[3][myLanguage]);
	while(1);
	
}
