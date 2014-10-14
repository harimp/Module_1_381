/*
 * global_include.h
 *
 *  Created on: 2014. 9. 30.
 *      Author: Harry
 */

#include "images.h"

#ifndef GLOBAL_INCLUDE_H_
#define GLOBAL_INCLUDE_H_

// definition of null
#ifndef NULL
#define NULL		((void*)0)
#endif

#define NO_PS2 		0 /* for cases where PS/2 keyboard unavailable */
#define TEST_MODE 	1
#define TRUE 		1
#define FALSE		0

/* Necessary Addresses */
#define drawer_base	PIXEL_DRAWER_0_BASE

/* PS/2 Input Map */
#define	UP			(char)0x75
#define DOWN		(char)0x72
#define ENTER		(char)0x5A

/* Calculation Constants */
#define HARDWARE_EN	0
#define	ACCEL_G		-9.81
#define DAMAGE		50

/* Trajectory Constants */
#define INIT_BALL_XPOS_TO_RIGHT (int)20
#define INIT_BALL_YPOS_TO_RIGHT	(int)210
#define INIT_BALL_XPOS_TO_LEFT	(int)298
#define INIT_BALL_YPOS_TO_LEFT	(int)210
#define TIME_SCALE	0.03
#define GRAVITY_SCALE	(double)(3/2) /* recommended to set it in (1,2) */

/* Animation Constants */
#define BACKGROUND_HEX 	0xf7f8e0 // white
#define BACKGROUND		0xCFFF 	//sky blue
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define PLAYER_WIDTH	20
#define PLAYER_HEIGHT	30

/* Image Position Constants */
#define CANNON_1_X	PLAYER_1_WIDTH
#define CANNON_1_Y	SCREEN_HEIGHT-GROUND_HEIGHT-CANNON_1_HEIGHT
#define CHAR_1_X	0
#define CHAR_1_Y	SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_1_HEIGHT

#define CANNON_2_X	SCREEN_WIDTH-PLAYER_2_WIDTH-CANNON_2_WIDTH
#define CANNON_2_Y	SCREEN_HEIGHT-GROUND_HEIGHT-CANNON_2_HEIGHT
#define CHAR_2_X	SCREEN_WIDTH-PLAYER_2_WIDTH
#define CHAR_2_Y	SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_2_HEIGHT

#define P1GUI_X		5
#define P1GUI_Y		SCREEN_HEIGHT-P1BOX_HEIGHT-5

#define P2GUI_X		SCREEN_WIDTH-P1GUI_X-P2BOX_WIDTH
#define P2GUI_Y		SCREEN_HEIGHT-P2BOX_HEIGHT-5

#define DIALOGUE_X	115
#define DIALOGUE_Y	90

#define PLAYER_ONE_X1	0
#define PLAYER_ONE_Y1	SCREEN_HEIGHT
#define PLAYER_ONE_X2	PLAYER_WIDTH
#define PLAYER_ONE_Y2	SCREEN_HEIGHT-PLAYER_HEIGHT

#define PLAYER_TWO_X1	SCREEN_WIDTH-PLAYER_WIDTH
#define PLAYER_TWO_Y1	SCREEN_HEIGHT
#define PLAYER_TWO_X2	SCREEN_WIDTH
#define PLAYER_TWO_Y2	SCREEN_HEIGHT-PLAYER_HEIGHT

#define INIT_VELOCITY	0
#define INIT_ANGLE		0

#define BALL_HEX		0x000000 // black
#define BALL_SIZE		1

#endif /* GLOBAL_INCLUDE_H_ */
