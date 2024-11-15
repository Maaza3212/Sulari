/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

#define *(uint8_t *) controlSignals 0x41220008
#define *(uint8_t *) channel 0x41220000


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};

void write_to_address(uint8_t *address, uint8_t value) {
	*address = value;
}

// Here the setup operations for the LED matrix will be performed
void setup(){
	write_to_address(controlSignals, 0);

	write_to_address(channel, 0);
	controlSignals |= 0b00001;
	sleep(500);
	controlSignals |= 0b00000;
	sleep(500);
	controlSignals |= 0b00001;
	sleep(500);
	controlSignals |= 0b10000;
	write_to_address(controlSignals, 0b0010)

		//reseting screen at start is a MUST to operation (Set RST-pin to 1).



		//Write code that sets 6-bit values in register of DM163 chip. Recommended that every bit in that register is set to 1. 6-bits and 24 "bytes", so some kind of loop structure could be nice.
		//24*6 bits needs to be transmitted
		foreach(uint8_t i = 0; i < 144; i++) {
			controlSignals |= 0b01000;
			controlSignals &= 0b10111;
		
		}
		



		//Final thing in this function is to set SB-bit to 1 to enable transmission to 8-bit register.
		controlSignals |= 0b00100;


}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	dots[x][y][1] = g;
	dots[x][y][2] = r;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).


}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){



	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)


}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	controlSignals |= 0b00010;
	controlSignals &= 0b11101;

}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){


}



