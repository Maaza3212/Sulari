/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

#define controlSignals *(volatile uint8_t *)0x41220008
#define channel *(volatile uint8_t *)0x41220000
#define OpenChannel *(volatile uint8_t *)0x1234


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};

void write_to_address(uint8_t *address, uint8_t value) {
	*address = value;
}

// Turn on SDA (Bit 4)
void SDAon() {
    controlSignals |= 0b10000;
}

// Turn off SDA (Bit 4)
void SDAoff() {
    controlSignals &= 0b01111;
}

// Turn on SCK (Bit 3)
void SCKon() {
    controlSignals |= 0b01000;
}

// Turn off SCK (Bit 3)
void SCKoff() {
    controlSignals &= 0b10111;
}

// Turn on SB (Bit 2)
void SBon() {
    controlSignals |= 0b00100;
}

// Turn off SB (Bit 2)
void SBoff() {
    controlSignals &= 0b11011;
}

// Turn on Lat (Bit 1)
void latOn() {
    controlSignals |= 0b00010;
}

// Turn off Lat (Bit 1)
void latOff() {
    controlSignals &= 0b11101;
}

// Turn on Rst (Bit 0)
void RstOn() {
    controlSignals |= 0b00001;
}

// Turn off Rst (Bit 0)
void RstOff() {
    controlSignals &= 0b11110;
}

// Initialize Alien
void initAlien(struct Alien* alien, uint8_t posX, uint8_t posY, uint8_t r, uint8_t g, uint8_t b) {
    alien->posX = posX;
    alien->posY = posY;
    alien->r = r;
    alien->g = g;
    alien->b = b;
    setAlienPixels(alien, posX, posY);
}

void setAlienPixels(struct Alien* alien, uint8_t posX, uint8_t posY) {
    // Reset PixelCoord array
    for (int i = 0; i < 4; i++) {  // Adjusted to 4 elements
        alien->pixels[i].x = 0;
        alien->pixels[i].y = 0;
    }

    // Set the 4 coordinates to form a square starting at (posX, posY)
    alien->pixels[0].x = posX;       // Top-left
    alien->pixels[0].y = posY;

    alien->pixels[1].x = posX + 1;   // Top-right
    alien->pixels[1].y = posY;

    alien->pixels[2].x = posX;       // Bottom-left
    alien->pixels[2].y = posY + 1;

    alien->pixels[3].x = posX + 1;   // Bottom-right
    alien->pixels[3].y = posY + 1;
}

// Global variable for current alien
struct Alien currentAlien;


// Here the setup operations for the LED matrix will be performed
void setup(){
	controlSignals=0;
    OpenChannel=0;
	channel=0;
    struct Alien first;
    initAlien(&first, 4,4,0,255,0);
    currentAlien = first;
    drawAlien();
	RstOn();
	usleep(500);
	RstOff();
	usleep(500);
    RstOn();
    usleep(500);
	SDAon();


		//Write code that sets 6-bit values in register of DM163 chip. Recommended that every bit in that register is set to 1. 6-bits and 24 "bytes", so some kind of loop structure could be nice.
		//24*6 bits needs to be transmitted
		for(uint8_t i = 0; i < 144; i++) {
			SCKon();
			SCKoff();
		};
		



		//Final thing in this function is to set SB-bit to 1 to enable transmission to 8-bit register.
		SBon();


}
void drawAlien(){
    SetPixel(currentAlien.posX, currentAlien.posX, currentAlien.r, currentAlien.g, currentAlien.b);
    /*for(uint8_t i = 0; i < 4; i++){
        uint8_t x = currentAlien.pixels[i].x;
        uint8_t y = currentAlien.pixels[i].y;
        SetPixel(x, y, currentAlien.r, currentAlien.g, currentAlien.b);
    }*/
    
}
//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	dots[x][y][1] = g;
	dots[x][y][2] = r;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).


}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	//toggle lat
	latOn();
	latOff();

}
//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x) {
    latOff();

    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t c = 0; c < 3; c++) {
            uint8_t byte = dots[x][y][c];

            for (uint8_t b = 0; b < 8; b++) {

                // Set SDA based on the current bit
                if (byte & 0x80) {
                    SDAon(); 
                } else {
                    SDAoff();
                }
                SCKoff();
                SCKon();
				byte <<=1;
                SCKoff();
            }
        }
    }

    latch();  // Toggle the latch to store the data in the LED driver
}



void closeChannel(){
    channel = 0;
}
//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){
	switch (x) {
        case 7:
            channel |= 0b10000000; // Turn on Bit 7 (C7)
            break;
        case 6:
            channel |= 0b01000000; // Turn on Bit 6 (C6)
            break;
        case 5:
            channel |= 0b00100000; // Turn on Bit 5 (C5)
            break;
        case 4:
            channel |= 0b00010000; // Turn on Bit 4 (C4)
            break;
        case 3:
            channel |= 0b00001000; // Turn on Bit 3 (C3)
            break;
        case 2:
            channel |= 0b00000100; // Turn on Bit 2 (C2)
            break;
        case 1:
            channel |= 0b00000010; // Turn on Bit 1 (C1)
            break;
        case 0:
            channel |= 0b00000001; // Turn on Bit 0 (C0)
            break;
        default:
            channel = 0;
            break;
    }

}



