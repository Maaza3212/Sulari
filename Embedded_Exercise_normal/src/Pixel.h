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
#include "stdlib.h"
#include "time.h"


// Define Pixel struct for storing x and y coordinates
struct PixelCoord {
    uint8_t x;
    uint8_t y;
};

// Alien structure
struct Alien {
    uint8_t posX;
    uint8_t posY;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct PixelCoord pixels[4]; 
    uint8_t exists;
};

struct Bullet {
    uint8_t posX;
    uint8_t posY;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t exists;
};
//Ship
struct Ship {
    uint8_t posX;
    uint8_t posY;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct PixelCoord pixels[4];
    uint8_t exists;
};

// Function prototypes
void initAlien(struct Alien* alien, uint8_t posX, uint8_t posY, uint8_t r, uint8_t g, uint8_t b);
void initShip(struct Ship* ship, uint8_t posX, uint8_t posY, uint8_t r, uint8_t g, uint8_t b);

void createBullet();
void setShipPixels(struct Ship* ship, uint8_t posX, uint8_t posY);
void drawShip();
void drawAlien();
void drawBullet();
void moveShipLeft();
void moveShipRight();
void setAlienPixels(struct Alien* alien, uint8_t posX, uint8_t posY);
void removeAlienPixels();
void removeShipPixels();
void removeShip();
void resetGame();
void removeBullet();
void destroyAlien();

uint8_t checkHit();


void GG();
void initBullet(struct Ship* ship);
void handleAlien();
void handleShip();

void handleBullet();
void clearScreen();
void moveBullet(struct Bullet* bullet);
void removeBulletPixel();
void handleGameTick();
void moveAlien(struct Alien* alien);
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


void pause();
void Zero(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b);
void One(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b);
void Two(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b);
void Three(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b);

void closeChannel();
void RstOn();   // Turn on Rst (Bit 0)
void RstOff();  // Turn off Rst (Bit 0)
void open_line(uint8_t x);
void scoreBoard();

void resetAlien();

#endif /* PIXEL_H_ */
