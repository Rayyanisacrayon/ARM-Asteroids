#ifndef sprites_H
#define sprites_H
#include <stdint.h>
typedef enum status {health3, health2, health1, destroyed} status_t;

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
typedef struct laser laser_s;

extern spr_s ship1;

void moveAsteroids(void);
void moveLasers(void);

void sprite_Init(spr_s sprite, const uint16_t imageSet[3][256]);

void vbmove(spr_s *prop);
void vbmove_laser(spr_s *prop, laser_s *laser);

void shipRotationUpdate(spr_s *ship);

void fireLaser(void);

void shipAccelerate(spr_s *ship);
void shipDeccelerate(spr_s *ship);
extern uint8_t shipAcc;
void checkCollision(void);

void spawnNextAsteroid(void);
#endif
