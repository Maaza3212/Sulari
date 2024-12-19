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
#define buttons *(volatile uint8_t *)0xE000A068

// Global variable for items
struct Alien currentAlien;
struct Ship currentShip;
struct Bullet currentBullet;
uint8_t score;
uint8_t missedScore;
uint8_t gameRunning;
uint8_t rageCount;
//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};

void createBullet(){
    if(currentBullet.exists == FALSE){
        initBullet(&currentShip);
    }
}

void initBullet(struct Ship* ship){
    struct Bullet newBullet;
    newBullet.posX = ship->posX;
    newBullet.posY = ship->posY;
    newBullet.r = 255;
    newBullet.g = 0;
    newBullet.b = 0;
    newBullet.exists = TRUE;

    currentBullet = newBullet;
    
}
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
    alien->exists = TRUE;
}

void initShip(struct Ship* ship, uint8_t posX, uint8_t posY, uint8_t r, uint8_t g, uint8_t b) {
    ship->posX = posX;
    ship->posY = posY;
    ship->r = r;
    ship->g = g;
    ship->b = b;
    ship->exists = TRUE;
    setShipPixels(ship, posX, posY);
}

void moveShipLeft(){
    if(currentShip.posY > 1){
        currentShip.posY = currentShip.posY-1;
    }
}

void moveShipRight(){
    if(currentShip.posY < 6){
        currentShip.posY = currentShip.posY+1;
    }
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

//Set ship pixels.
void setShipPixels(struct Ship* ship, uint8_t posX, uint8_t posY){
    // Reset PixelCoord array
    for (int i = 0; i < 4; i++) {  // Adjusted to 4 elements
        ship->pixels[i].x = 0;
        ship->pixels[i].y = 0;
    }
    // Set the 4 coordinates to form a square starting at (posX, posY)
    ship->pixels[0].x = posX;       // Cannon
    ship->pixels[0].y = posY;

    ship->pixels[1].x = posX-1;
    ship->pixels[1].y = posY;

    ship->pixels[2].x = posX-1;
    ship->pixels[2].y = posY +1;

    ship->pixels[3].x = posX-1;
    ship->pixels[3].y = posY-1;
    

}

void pause() {
    
    if(gameRunning!=4){
        gameRunning=4;
        clearScreen();
        uint8_t greens[8][3] = {
        {204, 255, 204}, // Lighter to darker shades of green
        {153, 255, 153},
        {102, 255, 102},
        {51, 255, 51},
        {0, 255, 0},
        {0, 204, 0},
        {0, 153, 0},
        {0, 102, 0}
        };
        for(uint8_t x = 0; x<8;x++){
            
            for(uint8_t y=0;y<8;y++){
                uint8_t rando = rand()%8;
                uint8_t r = greens[rando][0];
                uint8_t g = greens[rando][1];
                uint8_t b = greens[rando][2];
                SetPixel(x,y,r,g,b);
            }
        }
    }else{
        clearScreen();
        gameRunning =1;
    }
    
}

void resetAlien(){
    uint8_t new_r = rand() % 255;
    uint8_t new_g = rand() % 255;
    uint8_t new_b = rand() % 255;

    struct Alien newAlien;

    initAlien(&newAlien, 4, 4, new_r, new_g, new_b);
    currentAlien = newAlien;
}
void removeShip(){
    struct Ship newFakeShip;
    newFakeShip.exists = FALSE;
    currentShip = newFakeShip;
}
void resetGame(){
    clearScreen();
    destroyAlien();
    removeBullet();
    removeShip();
    setup();  
}



// Here the setup operations for the LED matrix will be performed
void setup(){
	controlSignals=0;
    OpenChannel=0;
	channel=0;
    struct Alien newAlien;
    struct Bullet newBullet;
    struct Ship newShip;
    rageCount =0;
    score= 0;
    missedScore=0;
    gameRunning=1;
    newBullet.exists = FALSE;
    currentBullet = newBullet;
    initShip(&newShip, 1, 4, 0,0,255);
    initAlien(&newAlien, 4,4,0,255,0);
    currentShip = newShip;
    currentAlien = newAlien;
    drawAlien();
    drawShip();
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
    for(uint8_t i = 0; i < 4; i++){
        uint8_t x = currentAlien.pixels[i].x;
        uint8_t y = currentAlien.pixels[i].y;
        SetPixel(x, y, currentAlien.r, currentAlien.g, currentAlien.b);
    }
    
}

void drawShip(){
    for(uint8_t i = 0; i < 4; i++){
        uint8_t x = currentShip.pixels[i].x;
        uint8_t y = currentShip.pixels[i].y;
        SetPixel(x, y, currentShip.r, currentShip.g, currentShip.b);
    }
}
//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	dots[x][y][1] = g;
	dots[x][y][2] = r;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).


}

void clearScreen(){
    for(uint8_t x = 0; x<8; x++){
        for(uint8_t y = 0; y<8; y++){
            SetPixel(x,y,0,0,0);
        }
    }
}

void Zero(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b) {
    // Top row
    SetPixel(baseX, baseY, r, g, b);         // Top-left
    SetPixel(baseX + 1, baseY, r, g, b);     // Top-middle
    SetPixel(baseX + 2, baseY, r, g, b);     // Top-right

    // Left column
    SetPixel(baseX, baseY + 1, r, g, b);     // Left-top
    SetPixel(baseX, baseY + 2, r, g, b);     // Left-middle
    SetPixel(baseX, baseY + 3, r, g, b);     // Left-bottom

    // Right column
    SetPixel(baseX + 2, baseY + 1, r, g, b); // Right-top
    SetPixel(baseX + 2, baseY + 2, r, g, b); // Right-middle
    SetPixel(baseX + 2, baseY + 3, r, g, b); // Right-bottom

    // Bottom row
    SetPixel(baseX, baseY + 4, r, g, b);     // Bottom-left
    SetPixel(baseX + 1, baseY + 4, r, g, b); // Bottom-middle
    SetPixel(baseX + 2, baseY + 4, r, g, b); // Bottom-right
}

void One(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b) {
    // Column for 1
    SetPixel(baseX + 1, baseY, r, g, b);     // Top
    SetPixel(baseX + 1, baseY + 1, r, g, b); // Top-middle
    SetPixel(baseX + 1, baseY + 2, r, g, b); // Middle
    SetPixel(baseX + 1, baseY + 3, r, g, b); // Bottom-middle
    SetPixel(baseX + 1, baseY + 4, r, g, b); // Bottom
}

void Two(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b) {
    // Top row
    SetPixel(baseX, baseY, r, g, b);
    SetPixel(baseX + 1, baseY, r, g, b);
    SetPixel(baseX + 2, baseY, r, g, b);

    // Middle-right
    SetPixel(baseX + 2, baseY + 1, r, g, b);
    SetPixel(baseX + 2, baseY + 2, r, g, b);

    // Middle row
    SetPixel(baseX, baseY + 3, r, g, b);
    SetPixel(baseX + 1, baseY + 3, r, g, b);
    SetPixel(baseX + 2, baseY + 3, r, g, b);

    // Bottom-left
    SetPixel(baseX, baseY + 4, r, g, b);
    SetPixel(baseX, baseY + 5, r, g, b);

    // Bottom row
    SetPixel(baseX, baseY + 6, r, g, b);
    SetPixel(baseX + 1, baseY + 6, r, g, b);
    SetPixel(baseX + 2, baseY + 6, r, g, b);
}

void Three(uint8_t baseX, uint8_t baseY, uint8_t r, uint8_t g, uint8_t b) {
    // Top row
    SetPixel(baseX, baseY, r, g, b);
    SetPixel(baseX + 1, baseY, r, g, b);
    SetPixel(baseX + 2, baseY, r, g, b);

    // Top-right vertical
    SetPixel(baseX + 2, baseY + 1, r, g, b);
    SetPixel(baseX + 2, baseY + 2, r, g, b);

    // Middle row
    SetPixel(baseX, baseY + 3, r, g, b);
    SetPixel(baseX + 1, baseY + 3, r, g, b);
    SetPixel(baseX + 2, baseY + 3, r, g, b);

    // Bottom-right vertical
    SetPixel(baseX + 2, baseY + 4, r, g, b);
    SetPixel(baseX + 2, baseY + 5, r, g, b);

    // Bottom row
    SetPixel(baseX, baseY + 6, r, g, b);
    SetPixel(baseX + 1, baseY + 6, r, g, b);
    SetPixel(baseX + 2, baseY + 6, r, g, b);
}

  

void scoreBoard(){
    
    if(gameRunning == 1){
        clearScreen();
        gameRunning = 0;
        uint8_t x = 2;
        uint8_t y = 1;
        switch (score)
        {
        case 0:
            Zero(x,y,255,0,0);
            break;
        case 1:
            One(x,y,255,0,0);
            break;
        case 2:
            Two(x,y,255,0,0);
            break;
        case 3:
            Three(x,y,255,0,0);
            break;
        default:
            break;
        }
        
    }else if (gameRunning==0){
        clearScreen();
        gameRunning = 1;
    }
    
}

void GG() {
    clearScreen();
    gameRunning = 2;
    uint8_t baseX = 0;
    uint8_t baseY = 0;

    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    // First "G"
    // Top row
    SetPixel(baseX, baseY, r, g, b);
    SetPixel(baseX + 1, baseY, r, g, b);
    SetPixel(baseX + 2, baseY, r, g, b);

    // Left vertical
    SetPixel(baseX, baseY + 1, r, g, b);
    SetPixel(baseX, baseY + 2, r, g, b);
    SetPixel(baseX, baseY + 3, r, g, b);
    SetPixel(baseX, baseY + 4, r, g, b);

    // Bottom row
    SetPixel(baseX, baseY + 5, r, g, b);
    SetPixel(baseX + 1, baseY + 5, r, g, b);
    SetPixel(baseX + 2, baseY + 5, r, g, b);

    // Right vertical (bottom half)
    SetPixel(baseX + 2, baseY + 4, r, g, b);
    SetPixel(baseX + 2, baseY + 3, r, g, b);

    // Middle row
    SetPixel(baseX + 1, baseY + 3, r, g, b);

    // Second "G" (adjust spacing as needed, here assumed 4 pixels between)
    baseX += 4; // Adjust horizontal spacing for the second "G"

    // Top row
    SetPixel(baseX, baseY, r, g, b);
    SetPixel(baseX + 1, baseY, r, g, b);
    SetPixel(baseX + 2, baseY, r, g, b);

    // Left vertical
    SetPixel(baseX, baseY + 1, r, g, b);
    SetPixel(baseX, baseY + 2, r, g, b);
    SetPixel(baseX, baseY + 3, r, g, b);
    SetPixel(baseX, baseY + 4, r, g, b);

    // Bottom row
    SetPixel(baseX, baseY + 5, r, g, b);
    SetPixel(baseX + 1, baseY + 5, r, g, b);
    SetPixel(baseX + 2, baseY + 5, r, g, b);

    // Right vertical (bottom half)
    SetPixel(baseX + 2, baseY + 4, r, g, b);
    SetPixel(baseX + 2, baseY + 3, r, g, b);

    // Middle row
    SetPixel(baseX + 1, baseY + 3, r, g, b);
}

void handleGameTick(){
    if(gameRunning != 1){
        return;
    }
    handleAlien();
    handleShip();
    handleBullet();
    uint8_t alienHit =  checkHit();
    if(alienHit == TRUE){
        removeBulletPixel();
        removeAlienPixels();
        destroyAlien();
        removeBullet();
        drawShip();
        resetAlien();
        score = score +1;
        if(score > 3){
            GG();
        }
    }else{
        if(currentAlien.exists == TRUE){
            drawAlien();
        }
        drawShip();
        if(currentBullet.exists == TRUE){
            drawBullet();
        }
        missedScore = missedScore + 1;
        if(missedScore > 50){

        }
    }
}
void handleAlien(){
    if(currentAlien.exists == TRUE){
        removeAlienPixels();
        moveAlien(&currentAlien);
        setAlienPixels(&currentAlien, currentAlien.posX, currentAlien.posY);
    }  
}

void destroyAlien(){
    struct Alien newFakeAlien;
    newFakeAlien.exists = FALSE;
    currentAlien = newFakeAlien;
}

uint8_t checkHit(){
    if(currentAlien.exists == TRUE){
        uint8_t bulletX = currentBullet.posX;
        uint8_t bulletY = currentBullet.posY;
        uint8_t hit = FALSE;
        for(uint8_t i = 0; i <4; i++){
            if(bulletX == currentAlien.pixels[i].x &&
                bulletY == currentAlien.pixels[i].y){
                    hit = TRUE;
                    break;
                }
        }
    return hit;
    }
    return FALSE;
    


}
void handleShip(){
    removeShipPixels();
    setShipPixels(&currentShip, currentShip.posX, currentShip.posY);
    
}
void drawBullet(){
    SetPixel(currentBullet.posX, currentBullet.posY, currentBullet.r, currentBullet.g, currentBullet.b);
}


void removeBulletPixel(){
    SetPixel(currentBullet.posX, currentBullet.posY, 0, 0, 0);
}

void handleBullet(){
    if(currentBullet.exists==TRUE){
        removeBulletPixel();
        moveBullet(&currentBullet);
        
    }
    
}

void moveBullet(struct Bullet* bullet){
    if(bullet->posX < 7){
        bullet->posX = bullet->posX+1;
    }else{
        removeBullet();
    }
}

void removeBullet(){
    struct Bullet newFalseBullet;
        newFalseBullet.exists=FALSE;
        newFalseBullet.r = 0;
        newFalseBullet.g = 0;
        newFalseBullet.b = 0;
        currentBullet = newFalseBullet;
}

//Remove current aliens position
void removeAlienPixels(){
    for(uint8_t i = 0; i<4; i++){
        uint8_t x = currentAlien.pixels[i].x;
        uint8_t y = currentAlien.pixels[i].y;
        SetPixel(x, y, 0, 0, 0);
    }
}

void removeShipPixels(){
    for(uint8_t i = 0; i<4; i++){
        uint8_t x = currentShip.pixels[i].x;
        uint8_t y = currentShip.pixels[i].y;
        SetPixel(x, y, 0, 0, 0);
    }
}


// Function to move the alien randomly within bounds
void moveAlien(struct Alien* alien) {


    // Randomly choose a direction to move in: up, down, left, right
    int direction = rand() % 4;  // Random number between 0 and 3

    // Move the alien within bounds (0 to 7 for x and y)
    switch (direction) {
        case 0:  // Move up
            if (alien->posY > 0) {
                alien->posY--;
            }
            break;
        case 1:  // Move down
            if (alien->posY < 6) {
                alien->posY++;
            }
            break;
        case 2:  // Move left
            if (alien->posX > 4) {
                alien->posX--;
            }
            break;
        case 3:  // Move right
            if (alien->posX < 6) {
                alien->posX++;
            }
            break;
    }
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



