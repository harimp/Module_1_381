/*
 * graphics.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "global_include.h"
#include "hardware_init.h"
#include "player.h"
#include "game.h"
#include "io.h"

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef enum dir_t {
	RIGHT = 1, LEFT = 2
} dir_t;

typedef struct shape_t {
	int x_1, x_2, y_1, y_2;
} shape_t;

int formatColor(int);
void clearScreen(system_t*);
void waitBuffer(system_t*);
void drawBox(system_t*, int, int, int, int, int);
void drawLine(system_t*, int, int, int, int, int);
void drawPlayers(system_t*, player_t*);
int drawCannonBall(system_t*, player_t*, dir_t, double, int, int);
int animateShooting(system_t*, player_t*, int);
void animate();
//draws a line at the input angle from the horizontal
void show_angle(player_t*, system_t*, shape_t*, int);
//clears the last line drawn
void clear_last_line(shape_t*, system_t*);
//draws empty power bar for specified player on the screen
void draw_power_bar(player_t*, system_t*);
//update power bar based on input velocity
void update_power(player_t*, system_t*);

#endif /* GRAPHICS_H_ */
