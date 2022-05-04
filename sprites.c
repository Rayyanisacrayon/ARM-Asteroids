//sprites.c

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "game.h"



typedef enum status {health3, health2, health1, destroyed} status_t;
status_t playerhealth = health3;


struct sprite {
	int16_t x, fx, dx, fdx, oldx; //integer x position, fraction x position, change in integer x for next timestep, change in fraction x for next timestep, old x position
	int16_t y, fy, dy, fdy, oldy; //integer y position, fraction y position, change in integer y for next timestep, change in fraction y for next timestep, old y position
	int16_t rotation; //sprite direction in relation to base image
	int16_t velocity; //velocity in direction of rotation, in tenths of a pixel
	
	const uint16_t **images; //set of sprite images
	uint8_t imagesLen;
	uint16_t *imageCurr; //current image of sprite
	uint8_t h, w; //height and width of image
	status_t condition; //status of sprite
};
typedef struct sprite spr_s;

spr_s ship1 = {50,0,0,0,0,50,0,0,0,0,270,50,(const uint16_t **) p1shipRotSet,7,(uint16_t *) p1shipRotSet[0],8,8,health3};
spr_s ship2 = {50,0,0,0,0,50,0,0,0,0,270,50,(const uint16_t **) p1shipRotSet,7,(uint16_t *) p1shipRotSet[0],8,8,health3};

struct laser {
	spr_s properties;
	uint8_t isactive;	
};
typedef struct laser laser_s;

laser_s lasers[5] = {
	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, (const uint16_t **) lazer, 1,(uint16_t *) lazer[0], 11, 11, health1}, 0},
	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, (const uint16_t **) lazer, 1,(uint16_t *) lazer[0], 11, 11, health1}, 0},
	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, (const uint16_t **) lazer, 1,(uint16_t *) lazer[0], 11, 11, health1}, 0},
	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, (const uint16_t **) lazer, 1,(uint16_t *) lazer[0], 11, 11, health1}, 0},
	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, (const uint16_t **) lazer, 1,(uint16_t *) lazer[0], 11, 11, health1}, 0}
};

struct asteroid{
	spr_s properties;
	uint8_t isactive;
};
typedef struct asteroid ast_s;

void vbmove(spr_s *prop);
void vbmove_laser(spr_s *prop, laser_s *laser);

#define astMax 10
static ast_s asteroids[astMax] ={
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0},
	{{100,0,0,0,0,100,0,0,0,0,270,50,(const uint16_t **) smallAsteroidRotSet,7,(uint16_t *) smallAsteroidRotSet[0],8,8,health3},0}
};

void vbmove_asteroid(spr_s *prop);
void moveAsteroids(void){
    uint8_t i;
    for(i = 0; i < 10; i++){
        if(asteroids[i].isactive == 1)vbmove_asteroid(&asteroids[i].properties);
    }
}

void moveLasers(void){
	for(uint8_t i=0; i<5; i++){
		if(lasers[i].isactive == 1)
			{vbmove_laser(&lasers[i].properties, &lasers[i]);}
	}
}	



//velocity-based move command
//takes in sprite properties attribute, i.e. a sprite object
//updates dx,dy based on velocity (pixels/timestep) and direction (degrees)
//then stores x in old and y in oldy before updates x and y based on dx, dy
//Checks for border collision for edge looping
void vbmove(spr_s *prop) {
	//declarations of variables
	static int16_t cos[12] = {1000,866,500,0,-500,-866,-1000,-866,-500,0,500,866};
	static int16_t sin[12] = {0,-500,-866,-1000,-866,-500,0,500,866,1000,866,500};
 	int16_t posOffsetX;
	int16_t wOffsetX;
	int16_t posOffsetY;
	int16_t hOffsetY;
	uint16_t ind;
	uint16_t imInd;
	uint8_t imFlip;
	
	//updating of space properties (dx & dy based on velocity and rotation, x & y based on dx & dy respectively)
	ind = ((prop->rotation+15)%360)/30;
	prop->dx = ((prop->velocity/10)*cos[ind])/1000;
	prop->dy = ((prop->velocity/10)*sin[ind])/1000;
	
	prop->fdx += ((prop->velocity%10)*cos[ind])/1000;
	prop->fdy += ((prop->velocity%10)*sin[ind])/1000;
	
	if(prop->velocity == 0){
        prop->dx = 0;
        prop->dy = 0;
        prop->fdx = 0;
        prop->fdy = 0;
    }
	//updating image based on properties
	if(ind == 0 || ind == 6) imInd = 0;
	else if (ind == 1 || ind == 7) imInd = 1;
	else if (ind == 2 || ind == 8) imInd = 2;
	else if (ind == 3) imInd = 3;
	else if (ind == 9) imInd = 4;
	else if (ind == 4 || ind == 10) imInd = 5;
	else if (ind == 5 || ind == 11) imInd = 6;
	prop->imageCurr = (uint16_t *) (prop->images + imInd*prop->h*prop->w/2);
	
	if(ind >= 4 && ind < 9) imFlip = 1;
	else imFlip = 0;
	
	//update x and y position, including fractions and old (old only takes integer positions)
	prop->oldx = prop->x;
	prop->oldy = prop->y;
	
	prop->x += prop->dx;
	prop->y += prop->dy;
	prop->fx += prop->fdx;
	prop->fy += prop->fdy;
	
	if(prop->fx >= 10){
		prop->x++;
		prop->fx %= 10;
	}
	if(prop->fy >= 10){
		prop->y++;
		prop->fy %= 10;
	}
	
	//checks if image is fully off screen, loops to opposite edge of screen if true
	if(prop->x < -prop->w) prop->x = 129 + prop->x + prop->w;
	else if(prop->x > 129) prop->x = -prop->w + 1 + prop->x - 129;
	
	if(prop->y < 0) prop->y = 159 + prop->h + prop->y;
	else if(prop->y > 159 + prop->h) prop->y = 0 + prop->y - (159 + prop->h);
	
	//Adjusts old position erasure box for when image is partially off screen
	if(prop->oldx < 0) {
		posOffsetX = -prop->oldx;
		wOffsetX = 0;
	}
	else if(prop->oldx + prop->w > 129) {
		wOffsetX = 129 - (prop->oldx + prop->w);
		posOffsetX = 0;
	}
	else {
		posOffsetX = 0;
		wOffsetX = 0;
	}
	
	if(prop->oldy < prop->h) {
		posOffsetY = -(prop->oldy-prop->h)-1;
		hOffsetY = 0;
	}
	else if(prop->oldy > 159) {
		hOffsetY = 159 -(prop->oldy-159);
		posOffsetY = 0;
	}
	else {
		posOffsetY = 0;
		hOffsetY = 0;
	}
	
	ST7735_FillRect(prop->oldx + posOffsetX, prop->oldy - prop->h + 1 + posOffsetY, prop->w + wOffsetX, prop->h + hOffsetY, ST7735_BLACK);
	
	ST7735_DrawBitmapWithRotation(prop->x, prop->y, prop->imageCurr, prop->w, prop->h, imFlip);
}

int32_t Convert(uint32_t x){
	return (1741*x)/4096+146 - 1000;
}

//velocity based move that does not rotate sprite. Objects do not loop off screen. 
//accepts entire struct as a parameter, allowing isActive to be updated when off screen
void vbmove_laser(spr_s *prop, laser_s *laser) {
	//declarations of variables
	const static int16_t cos[12] = {1000,866,500,0,-500,-866,-1000,-866,-500,0,500,866};
	const static int16_t sin[12] = {0,-500,-866,-1000,-866,-500,0,500,866,1000,866,500};
 	int16_t posOffsetX;
	int16_t wOffsetX;
	int16_t posOffsetY;
	int16_t hOffsetY;
	uint16_t ind;
	
	//updating of space properties (dx & dy based on velocity and rotation, x & y based on dx & dy respectively)
	ind = ((prop->rotation+15)%360)/30;
	prop->dx = ((prop->velocity/10)*cos[ind])/1000;
	prop->dy = ((prop->velocity/10)*sin[ind])/1000;
	
	prop->fdx += ((prop->velocity%10)*cos[ind])/1000;
	prop->fdy += ((prop->velocity%10)*sin[ind])/1000;

	if(prop->velocity == 0){
					prop->dx = 0;
					prop->dy = 0;
					prop->fdx = 0;
					prop->fdy = 0;
			}	
	//update x and y position, including fractions and old (old only takes integer positions)
	prop->oldx = prop->x;
	prop->oldy = prop->y;
	
	prop->x += prop->dx;
	prop->y += prop->dy;
	prop->fx += prop->fdx;
	prop->fy += prop->fdy;
	
	if(prop->fx >= 10){
		prop->x++;
		prop->fx %= 10;
	}
	if(prop->fy >= 10){
		prop->y++;
		prop->fy %= 10;
	}
	
	//checks if image is fully off screen, deactivates if true
	if(prop->x < -prop->w) laser->isactive = 0;
	else if(prop->x > 129) laser->isactive = 0;
	
	if(prop->y < 0) laser->isactive = 0;
	else if(prop->y > 159 + prop->h) laser->isactive = 0;
	
	//Adjusts old position erasure box for when image is partially off screen
	if(prop->oldx < 0) {
		posOffsetX = -prop->oldx;
		wOffsetX = 0;
	}
	else if(prop->oldx + prop->w > 129) {
		wOffsetX = 129 - (prop->oldx + prop->w);
		posOffsetX = 0;
	}
	else {
		posOffsetX = 0;
		wOffsetX = 0;
	}
	
	if(prop->oldy < prop->h) {
		posOffsetY = -(prop->oldy-prop->h)-1;
		hOffsetY = 0;
	}
	else if(prop->oldy > 159) {
		hOffsetY = 159 -(prop->oldy-159);
		posOffsetY = 0;
	}
	else {
		posOffsetY = 0;
		hOffsetY = 0;
	}
	
	ST7735_FillRect(prop->oldx + posOffsetX, prop->oldy - prop->h + 1 + posOffsetY, prop->w + wOffsetX, prop->h + hOffsetY, ST7735_BLACK);
	
	ST7735_DrawBitmap(prop->x, prop->y, prop->imageCurr, prop->w, prop->h);
}


void shipRotationUpdate(spr_s *ship){
	uint32_t temp1;
	int32_t temp2;
	int16_t rotAngle;
	const uint8_t maxAcc = 70;
	
	temp1 = ADC_In();
	temp2 = Convert(temp1);
	
	rotAngle = -(temp2 * maxAcc) / 1000;
	if(rotAngle > 0) rotAngle = (rotAngle-15 < 0)? (rotAngle = 0):(rotAngle-15);
  if(rotAngle < 0) rotAngle = (rotAngle+15 > 0)? (rotAngle = 0): (rotAngle+15);
	ship->rotation += rotAngle;
	ship->rotation = (ship->rotation < 0)? (ship->rotation + 360) : ship->rotation;
	ship->rotation = (ship->rotation > 360)? (ship->rotation%360) : ship->rotation;
}

void fireLaser(){
	for(uint8_t i = 0; i<5; i++){
		if(lasers[i].isactive == 0){
			lasers[i].properties.x = ship1.x; 
			lasers[i].properties.y = ship1.y;
			lasers[i].properties.rotation = ship1.rotation; 
			lasers[i].properties.rotation = ship1.rotation;
			lasers[i].properties.velocity = 120;
			lasers[i].properties.condition = health1;
			lasers[i].isactive = 1;
			
			return;
		}
	}
	return;
	
}
uint8_t shipAcc = 0;
#define deccel 2
#define accel 5
#define vmax 100
#define vmin 0
void shipAccelerate(spr_s *ship){
    ship->velocity = (ship->velocity + accel);
    if(ship->velocity > 100) ship->velocity = vmax;
    if(!(GPIO_PORTE_DATA_R&0x01)) shipAcc = 0;
}
void shipDeccelerate(spr_s *ship){
    ship->velocity = ship->velocity - deccel;
    if(ship->velocity < vmin) ship->velocity = vmin;
}
void checkCollision(){
	for(uint8_t i = 0; i < astMax; i++){ //iterate through asteroids
		
		//first, check if all asteroids are touching any laser. 

		if(asteroids[i].isactive == 0) continue; //check if active
		for(uint8_t j = 0; j < 5; j++){ //iterate through lasers
			if(lasers[j].isactive == 0) continue; //check if active
			
			
			//calculate manhattan distance
			uint32_t xone, yone, xtwo, ytwo, md;
			int32_t xchange, ychange;
			xone = asteroids[i].properties.x - (asteroids[i].properties.w/2);
			yone = asteroids[i].properties.y - (asteroids[i].properties.h/2);
			xtwo = lasers[j].properties.x - (lasers[j].properties.w/2);
			ytwo = lasers[j].properties.y - (lasers[j].properties.h/2);
			
			xchange = xone - xtwo;
			xchange = (xchange<0)? (-xchange) : xchange;
			ychange = yone - ytwo;
			ychange = (ychange<0)? (-ychange) : ychange;
			
			md = xchange + ychange;
			//check for collision
			
			if (md < (asteroids[i].properties.w + lasers[j].properties.w)){
				asteroids[i].properties.condition++; 
				lasers[j].properties.condition++; 
				playsound(Boom); 

				
				//check if asteroid/laser destroyed and set inactivity. 
				if(asteroids[i].properties.condition == destroyed) {
					asteroids[i].isactive = 0;
					if((asteroids[i].properties.x>=0) && ((asteroids[i].properties.y - asteroids[i].properties.h + 1) >= 0)){
						ST7735_FillRect(asteroids[i].properties.x, asteroids[i].properties.y - asteroids[i].properties.h + 1, 
						asteroids[i].properties.w, asteroids[i].properties.h, ST7735_BLACK);
					}
				}
				if(lasers[j].properties.condition == destroyed){
					lasers[j].isactive = 0;
					if((lasers[j].properties.x>=0) && ((lasers[j].properties.y - lasers[j].properties.h)>=0)){
						ST7735_FillRect(lasers[j].properties.x, lasers[j].properties.y - lasers[j].properties.h, 
						lasers[j].properties.w, lasers[j].properties.h, ST7735_BLACK);
					}
				}
			}
		
		}
		
		
		
		//now check if asteroids are touching a ship
		if(1) { //block to limit variable scope			
			
			//calculate manhattan distance
			uint32_t xone, yone, xtwo, ytwo, md;
			int32_t xchange, ychange;
			xone = asteroids[i].properties.x - (asteroids[i].properties.w/2);
			yone = asteroids[i].properties.y - (asteroids[i].properties.h/2);
			xtwo = ship1.x - (ship1.w/2);
			ytwo = ship1.y - (ship1.h/2);
			
			xchange = xone - xtwo;
			xchange = (xchange<0)? (-xchange) : xchange;
			ychange = yone - ytwo;
			ychange = (ychange<0)? (-ychange) : ychange;
			
			md = xchange + ychange;
			//check for collision
			
			if (md < (asteroids[i].properties.w + ship1.w)){
				asteroids[i].properties.condition = destroyed; 
				playerhealth++; 
				playsound(Boom); 

				
				//check if asteroid destroyed and set inactivity. 
				if(asteroids[i].properties.condition == destroyed) {
					asteroids[i].isactive = 0;
					if((asteroids[i].properties.x>=0) && ((asteroids[i].properties.y - asteroids[i].properties.h + 1) >= 0)){
						ST7735_FillRect(asteroids[i].properties.x, asteroids[i].properties.y - asteroids[i].properties.h + 1, 
						asteroids[i].properties.w, asteroids[i].properties.h, ST7735_BLACK);
					}
				}
				switch(playerhealth){
					case destroyed :	ST7735_FillRect(110, 3, 16, 16, ST7735_BLACK); gameover(); break;
					case health1 :		ST7735_FillRect(92, 3, 16, 16, ST7735_BLACK); break;
					case health2 :		ST7735_FillRect(76, 3, 16, 16, ST7735_BLACK); break;
					case health3 :		break;
				}
			}
		
		}
		
		
		
	}
		
}		
void spawnNextAsteroid(void){
	const static int16_t spawnPointsTop[7][2] = {
		{20,0},{40,0},{60,0},{80,0},{100,0},{120, 0}
	};
	const static int16_t spawnPointsRight[7][2] = {
		{129,20},{129,40},{129,60},{129,80},{128,100},{129,120},{129,140}
	};
	const static int16_t spawnPointsBottom[7][2] = {
		{120,159},{100,159},{80,159},{60,159},{40,159},{20,159},{0,159}
	};
	const static int16_t spawnPointsLeft[7][2] = {
		{0,140},{0,120},{0,100},{0,80},{0,60},{0,40},{0,20} 
	};
	static uint8_t spawnPointIndex = 0;
	static int16_t x, y;
	int16_t randNum;
	uint8_t i;
	uint8_t speedFactor;
	
	static uint8_t count = 0;
	
	count++;
	if (count<=5) return;
	else count = 0;
	
	for(i = 0; i < astMax; i++){
		if(asteroids[i].isactive == 0) break;
	}
	if(i == astMax) return;
	
	randNum = ((uint16_t) Random())%4; // a number from 0 to 3
	if(randNum == 0){
		randNum = ((uint16_t) Random())%20 + 1; //a number from 1 to 20
		spawnPointIndex = (spawnPointIndex + randNum)%7;
		x = spawnPointsTop[spawnPointIndex][0];
		y = spawnPointsTop[spawnPointIndex][1];
		
		asteroids[i].properties.x = x;
		asteroids[i].properties.y = y;
		
		randNum = ((uint16_t) Random())%90 + 225; // a number from 225 to 314
		asteroids[i].properties.rotation = randNum;
	}
	else if(randNum == 1){
		randNum = ((uint16_t) Random())%20 + 1; //a number from 1 to 20
		spawnPointIndex = (spawnPointIndex + randNum)%7;
		x = spawnPointsRight[spawnPointIndex][0];
		y = spawnPointsRight[spawnPointIndex][1];
		
		asteroids[i].properties.x = x;
		asteroids[i].properties.y = y;
		
		randNum = ((uint16_t) Random())%90 + 135; // a number from 135 to 224
		asteroids[i].properties.rotation = randNum;
	}
	else if(randNum == 2){
		randNum = (Random())%20 + 1; //a number from 1 to 20
		spawnPointIndex = (spawnPointIndex + randNum)%7;
		x = spawnPointsBottom[spawnPointIndex][0];
		y = spawnPointsBottom[spawnPointIndex][1];
		
		asteroids[i].properties.x = x;
		asteroids[i].properties.y = y;
		
		randNum = ((uint16_t) Random())%90 + 45; // a number from 45 to 134
		asteroids[i].properties.rotation = randNum;
	}
	else {
		randNum = ((uint16_t) Random())%20 + 1; //a number from 1 to 20
		spawnPointIndex = (spawnPointIndex + randNum)%7;
		x = spawnPointsLeft[spawnPointIndex][0];
		y = spawnPointsLeft[spawnPointIndex][1];
		
		asteroids[i].properties.x = x;
		asteroids[i].properties.y = y;
		
		randNum = (Random())%90 + 315; // a number from 315 to 404
		asteroids[i].properties.rotation = randNum%360;
	}
	
	randNum = ((uint16_t) Random())%60; // a number from 0 to 59
	
	if(randNum < 35){
		asteroids[i].properties.images = (const uint16_t **) smallAsteroidRotSet;
		asteroids[i].properties.imageCurr = (uint16_t *) smallAsteroidRotSet[0];
		asteroids[i].properties.h = 8;
		asteroids[i].properties.w = 8;
		asteroids[i].properties.condition = health1;
		speedFactor = 1;
	}
	else if(randNum < 50){
		asteroids[i].properties.images = (const uint16_t **) mediumAsteroidRotSet;
		asteroids[i].properties.imageCurr = (uint16_t *) mediumAsteroidRotSet[0];
		asteroids[i].properties.h = 12;
		asteroids[i].properties.w = 12;
		asteroids[i].properties.condition = health1;
		speedFactor = 2;
	}
	else{
		asteroids[i].properties.images = (const uint16_t **) largeAsteroidRotSet;
		asteroids[i].properties.imageCurr = (uint16_t *) largeAsteroidRotSet[0];
		asteroids[i].properties.h = 16;
		asteroids[i].properties.w = 16;
		asteroids[i].properties.condition = health1;
		speedFactor = 3;
	}
	
	randNum = ((uint16_t) Random())%60 + 21; // a number from 21 to 80
	asteroids[i].properties.velocity = randNum/speedFactor;
	asteroids[i].isactive = 1;
	return;
}

void vbmove_asteroid(spr_s *prop) {
	//declarations of variables
	const static int16_t cos[12] = {1000,866,500,0,-500,-866,-1000,-866,-500,0,500,866};
	const static int16_t sin[12] = {0,-500,-866,-1000,-866,-500,0,500,866,1000,866,500};
 	int16_t posOffsetX;
	int16_t wOffsetX;
	int16_t posOffsetY;
	int16_t hOffsetY;
	uint16_t ind;
	uint8_t imFlip;
	
	//updating of space properties (dx & dy based on velocity and rotation, x & y based on dx & dy respectively)
	ind = ((prop->rotation+15)%360)/30;
	prop->dx = ((prop->velocity/10)*cos[ind])/1000;
	prop->dy = ((prop->velocity/10)*sin[ind])/1000;
	
	prop->fdx += ((prop->velocity%10)*cos[ind])/1000;
	prop->fdy += ((prop->velocity%10)*sin[ind])/1000;
	
	if(prop->velocity == 0){
		prop->dx = 0;
		prop->dy = 0;
		prop->fdx = 0;
		prop->fdy = 0;
	}
	
	//update x and y position, including fractions and old (old only takes integer positions)
	prop->oldx = prop->x;
	prop->oldy = prop->y;
	
	prop->x += prop->dx;
	prop->y += prop->dy;
	prop->fx += prop->fdx;
	prop->fy += prop->fdy;
	
	if(prop->fx >= 10){
		prop->x++;
		prop->fx %= 10;
	}
	if(prop->fy >= 10){
		prop->y++;
		prop->fy %= 10;
	}
	
	//checks if image is fully off screen, loops to opposite edge of screen if true
	if(prop->x < -prop->w) prop->x = 129 + prop->x + prop->w;
	else if(prop->x > 129) prop->x = -prop->w + 1 + prop->x - 129;
	
	if(prop->y < 0) prop->y = 159 + prop->h + prop->y;
	else if(prop->y > 159 + prop->h) prop->y = 0 + prop->y - (159 + prop->h);
	
	//Adjusts old position erasure box for when image is partially off screen
	if(prop->oldx < 0) {
		posOffsetX = -prop->oldx;
		wOffsetX = 0;
	}
	else if(prop->oldx + prop->w > 129) {
		wOffsetX = 129 - (prop->oldx + prop->w);
		posOffsetX = 0;
	}
	else {
		posOffsetX = 0;
		wOffsetX = 0;
	}
	
	if(prop->oldy < prop->h) {
		posOffsetY = -(prop->oldy-prop->h)-1;
		hOffsetY = 0;
	}
	else if(prop->oldy > 159) {
		hOffsetY = 159 -(prop->oldy-159);
		posOffsetY = 0;
	}
	else {
		posOffsetY = 0;
		hOffsetY = 0;
	}
	
	ST7735_FillRect(prop->oldx + posOffsetX, prop->oldy - prop->h + 1 + posOffsetY, prop->w + wOffsetX, prop->h + hOffsetY, ST7735_BLACK);
	
	ST7735_DrawBitmapWithRotation(prop->x, prop->y, prop->imageCurr, prop->w, prop->h, imFlip);
}
