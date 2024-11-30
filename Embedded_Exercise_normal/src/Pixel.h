/*
 * Pixel.h
 *
 *  Created on: ------
 *      Author: ------
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xgpiops.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xparameters.h"


// Define Pixel struct for storing x and y coordinates
struct PixelCoord {
    uint8_t x;
    uint8_t y;
};

// Now define the Alien struct which references PixelCoord
struct Alien {
    uint8_t posX;
    uint8_t posY;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct PixelCoord pixels[4];  // Array of PixelCoord structs (to hold the 4 pixels)
};

// Function prototypes
void initAlien(struct Alien* alien, uint8_t posX, uint8_t posY, uint8_t r, uint8_t g, uint8_t b);
void drawAlien();
void setAlienPixels(struct Alien* alien, uint8_t posX, uint8_t posY);
//void setAlienPixels(Alien* alien, uint8_t posX, uint8_t posY);

void setup();
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void run(uint8_t x);
// Function prototypes for controlling each signal
void SDAon();   // Turn on SDA (Bit 4)
void SDAoff();  // Turn off SDA (Bit 4)

void SCKon();   // Turn on SCK (Bit 3)
void SCKoff();  // Turn off SCK (Bit 3)

void SBon();    // Turn on SB (Bit 2)
void SBoff();   // Turn off SB (Bit 2)
void latch();
void latOn();   // Turn on Lat (Bit 1)
void latOff();  // Turn off Lat (Bit 1)

void closeChannel();
void RstOn();   // Turn on Rst (Bit 0)
void RstOff();  // Turn off Rst (Bit 0)
void open_line(uint8_t x);

#endif /* PIXEL_H_ */
