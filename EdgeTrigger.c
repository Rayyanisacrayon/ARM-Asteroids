//EdgeTrigger.C
//Initialize edge triggered interrupts on PC0-3.

#include "EdgeTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "menu.h"
#include "Sound.h"
#include "sprites.h"
#include "random.h"

btnLoc_t btnBehavior = inMenu;
uint8_t fire1 = 0; 

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void EdgeTrigger_Init(void){       
  SYSCTL_RCGCGPIO_R |= 0x00000010; //  activate clock for port E
	for(uint8_t i = 2; i > 0; i++){} //wait for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x0F;    //     make PE0-3 input
  GPIO_PORTE_DEN_R |= 0x0F;     //     enable digital I/O on PE0-3
 // GPIO_PORTE_PDR_R |= 0x0F;     //     enable weak pull-down on PE0-3
  GPIO_PORTE_IS_R &= ~0x0F;     //     PE0-3 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x0F;    //     PE0-3 is not both edges
  GPIO_PORTE_IEV_R |= 0x0F;    //      PE0-3 rising edge event
  GPIO_PORTE_ICR_R = 0x0F;      //     clear flag0-3
  GPIO_PORTE_IM_R |= 0x0F;      //     arm interrupt on PE0-3
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x00000000; // priority 0
  NVIC_EN0_R = 0x00000010;      //     enable interrupt 4 in NVIC
  EnableInterrupts();           //     Enable global Interrupt flag 

}


//called when pe0 is pressed
void PE0Handler(){
	//check which button behavior to choose based on where in the program we are
	if(btnBehavior == inMenu){
		//appropriate behavior is to act as an "up" key
		switch(mainmenu){
			case single : mainmenu = lang; break;
			case multi : mainmenu = single; break;
			case lang : mainmenu = multi; break;
		}
	}
	
	if(btnBehavior == inLang){
		//appropriate behavior is to act as an "up" key
		switch(langoption){
			case English : langoption = Spanish; break;
			case Spanish : langoption = English; break;
		}
	}
	
	if(btnBehavior == inGame){
		//appropriate behavior is to act as a "thruster" key
		shipAcc = 1; 
	}
	if(btnBehavior == inPause){
	
	}
}



//called when pe1 is pressed
void PE1Handler(){
	
	if(btnBehavior == inMenu){
		//appropriate behavior is to act as a "confirm" button.
		confirm = 1;
		playsound(Shoot); 
	}
	
	if(btnBehavior == inLang){
		//appropriate behavior is to act as a "confirm" button.
		confirm = 1;		
		playsound(Shoot); 
	}
	
	if(btnBehavior == inGame){
		//fire a laser
		playsound(Shoot); 
		fireLaser();
		
	}
	if(btnBehavior == inPause){
		//do nothing
	}
}



//called when pe2 is pressed
void PE2Handler(){
	if(btnBehavior == inMenu){
		//appropriate behavior is to act as a "down" button.
		switch(mainmenu){
			case single : mainmenu = multi; break;
			case multi : mainmenu = lang; break;
			case lang : mainmenu = single; break;
		}
	}
	
	if(btnBehavior == inLang){
		//appropriate behavior is to act as a "down" button.
		switch(langoption){
			case English : langoption = Spanish; break;
			case Spanish : langoption = English; break;
		}
	}
	
	if(btnBehavior == inGame){
		togglePause();
		
	}	
	if(btnBehavior == inPause){
		
	}
}

//called when pe3 is pressed
void PE3Handler(){
	
	if(btnBehavior == inMenu){
	}
	
	if(btnBehavior == inLang){
	
	}
	
	if(btnBehavior == inGame){
	
	}
	if(btnBehavior == inPause){
	
	}
}

//called on interrupt
void GPIOPortE_Handler(void){
	//check which button is pressed, and call handler for that specific button
	if((GPIO_PORTE_DATA_R&0x01) == 0x01){PE0Handler(); GPIO_PORTE_ICR_R = 0x01;}     // acknowledge flag4}
	if((GPIO_PORTE_DATA_R&0x02) == 0x02){PE1Handler(); GPIO_PORTE_ICR_R = 0x02;}
	if((GPIO_PORTE_DATA_R&0x04) == 0x04){PE2Handler(); GPIO_PORTE_ICR_R = 0x04;}
	if((GPIO_PORTE_DATA_R&0x08) == 0x08){PE3Handler(); GPIO_PORTE_ICR_R = 0x08;}

	static uint8_t first = 1;
	if (first == 1) {Random_Init(NVIC_ST_CURRENT_R); NVIC_ST_CTRL_R &= ~1;}
	first = 0;
	
}

void togglePause() {
	btnBehavior = inPause; 
//	uint8_t pauseflag = 1; 
//	while(pauseflag){
//		for(int i=0; i<1000000; i++){}
//		if((GPIO_PORTE_DATA_R&0x04) == 0x04){
//			for(int i=0; i<100000; i++){}
//				if((GPIO_PORTE_DATA_R&0x04) == 0x04){
//					pauseflag = 0;
//					btnBehavior = inGame; 
//				}
//		}
//	for(int i=0; i<1000000; i++){}

//	}
	while((GPIO_PORTE_DATA_R&0x08) != 0x08){
	}
	btnBehavior = inGame; 
	
	

}


