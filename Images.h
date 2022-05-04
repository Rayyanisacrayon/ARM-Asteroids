// images.h
// contains image buffers for simple space invaders
// Jonathan Valvano, March 5, 2018
// Capture image dimensions from BMP files

#ifndef __images_h
#define __images_h
#include <stdint.h>


// *************************** Images ***************************
// medium sized asteroid. 16x16
extern const unsigned short asteroid1[];

//set of images for small asteroid from 0-90 and 270-330 degrees by increments of 30, each image is 8x8
extern const uint16_t smallAsteroidRotSet[7][64];

//set of images for blue ship from 0-90 and 270-330 degrees by increments of 30, each image is 8x8
extern const uint16_t p1shipRotSet[7][64];

//red ship facing upwards. 16x16
extern const unsigned short p2ship90d[];

//blue ship facing towards the right 16x16
extern const unsigned short p1ship0d[];

//asteroids logo, 100x25
extern const unsigned short logo[];


//lazer bullet image
//11x11
extern const unsigned short lazer[1][121];

extern const unsigned short heart[];

//set of images for large asteroid, just 0 degrees, each image is 16x16
extern const unsigned short largeAsteroidRotSet[1][256];

//set of images for small asteroid, just 0 degrees, each image is 12x12
extern const unsigned short mediumAsteroidRotSet[1][144];

#endif /* __images_h */
