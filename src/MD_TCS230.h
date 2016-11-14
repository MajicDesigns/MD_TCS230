/**
  MD_TCS230.h - Arduino library for TCS230 Colour Sensor.
  Copyright (C) 2013 Marco Colli
  All rights reserved.
	
  The TCS230 programmable color light-to-frequency converter
  combines configurable silicon photodiodes and a current-to-
  frequency converter on single CMOS integrated circuit. 
  The output is a square wave(50% duty cycle) with frequency 
  directly proportional to light intensity (irradiance). 
  The full-scale output frequency can be scaled by one of three 
  preset values via two control input pins. 
  Output enable (OE) places the output in the high-impedance 
  state for multiple-unit sharing of a microcontroller input line.
	
  This library has a dependency on the FreqCount library for 
  frequency counting. FreqCount library is available at
  http://www.pjrc.com/teensy/td_libs_FreqCount.html
	
  ** IMPORTANT NOTE** 
  FreqCount imposes a limitation that the frequency can only be 
  counted on specific pins and limits the use of other pins as 
  follows:
  Board       Input Pin Pins Unusable with analogWrite()
  ----------- --------- --------------------------------  
  Arduino Uno     5      3, 9, 10, 11
  Arduino 2009    5      3, 9, 10, 11
  Arduino Mega   47      9, 10, 44, 45, 46
  Sanguino        1      12, 13, 14, 15

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _MD_TCS230_H
#define _MD_TCS230_H

#include <Arduino.h>
#include <FreqCount.h>

#define	ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))

// Frequency setting defines
#define	TCS230_FREQ_HI	0	// 100% prescaler
#define	TCS230_FREQ_MID	1	// 20%
#define	TCS230_FREQ_LO	2	// 2%
#define	TCS230_FREQ_OFF	3	// off

// Indices for any RGB data/ filter selection
#define	TCS230_RGB_R	0
#define	TCS230_RGB_G	1
#define	TCS230_RGB_B	2
#define	TCS230_RGB_X	3	// 'Clear' filter selection

#define	RGB_SIZE	3	// array index counter limit

#define	NO_PIN	0xff	// value set when no pin has been defined

typedef struct
{
	int32_t value[RGB_SIZE];	// Raw data from the sensor
} sensorData;

typedef struct
{
	uint8_t	value[RGB_SIZE]; // the evaluated colour data (RGB value 0-255 or other)
} colorData;

/**
  Main Class for the sensor. This is the only class implemented
	to abstract and manage the sensor.
*/
class MD_TCS230 
{
  public:
	  /**
		  Constructor. Various combinations of the sensor pins can be used to 
			control the device and obtain data.
		*/
    MD_TCS230(uint8_t s2, uint8_t s3);
    MD_TCS230(uint8_t s2, uint8_t s3, uint8_t oe);
    MD_TCS230(uint8_t s2, uint8_t s3, uint8_t s0, uint8_t s1);
    MD_TCS230(uint8_t s2, uint8_t s3, uint8_t s0, uint8_t s1, uint8_t oe);
    ~MD_TCS230(void);

	//	Data handling
	void	begin();				// used to initialise hardware
	void	read();					// start the process of asynchronously reading the value
	bool	available();			// is a reading available yet?
	void	getRGB(colorData *rgb);	// return RGB color data for the last reading
	void	getRaw(sensorData *d);	// return the raw data from the last reading
	uint32_t readSingle();			// sydnchronously (blocking) read a value

	void	setFilter(uint8_t f);	// set the photodiode filter
	void	setFrequency(uint8_t f);// set frequency prescaler - default 100%
	void	setEnable(bool t);		// enable the device (using OE or frequency)
	void	setSampling(uint8_t t);	// set the divisor of 1000ms for a reading - default 10
	void	setDarkCal(sensorData *d);	// set the dark calibration data
	void	setWhiteCal(sensorData *d);	// set the white balance calibration data
											
  protected:
	uint8_t		_OE;		// output enable pin
	uint8_t		_S0, _S1;	// frequency scaler
	uint8_t		_S2, _S3;	// photodiode filter selection
	uint8_t		_readDiv;	// fraction of 1 second read
	uint8_t		_freqSet;	// current FREQ_* setting 

	sensorData	_Fd;		// Dark calibration data
	sensorData	_Fw;		// White balance calibration data

	sensorData	_Fo;		// current raw sensor reading
	colorData	_rgb;		// colour based data for current reading
	uint8_t		_readState;	// FSM state

	void	initialise(void);			// initialise variables
	uint8_t	readFSM(uint8_t s);			// reading values fsm
	void	RGBTransformation(void);	// convert raw data to RGB
	void	setFrequency2(uint8_t f);	// internal function for frequency prescaler

};

#endif
