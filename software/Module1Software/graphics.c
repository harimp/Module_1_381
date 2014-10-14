/*
 * graphics.c
 *
 */

#include "graphics.h"
#include "images.h"
#include "global_include.h"

int formatColor(int hexCode) {
	int red = (hexCode >> 16) & 0xff;
	int green = (hexCode >> 8) & 0xff;
	int blue = hexCode & 0xff;

	unsigned short b = (blue >> 3) & 0x001f;
	unsigned short g = ((green >> 2) & 0x003f) << 6;
	unsigned short r = ((red >> 3) & 0x001f) << 11;

	return (int) (r | g | b);
}

void clearScreen(system_t* system) {
	alt_up_pixel_buffer_dma_draw_box(system->pixel_buffer, 0, 0, 320, 240,
			formatColor(BACKGROUND_HEX), 0);
}

void drawBox(system_t* system, int x1, int y1, int x2, int y2, int color) {
	if (HARDWARE_EN) {
		IOWR_32DIRECT(drawer_base, 0, x1);
		IOWR_32DIRECT(drawer_base, 4, y1);
		IOWR_32DIRECT(drawer_base, 8, x2);
		IOWR_32DIRECT(drawer_base, 12, y2);
		IOWR_32DIRECT(drawer_base, 16, formatColor(color));
		IOWR_32DIRECT(drawer_base, 20, 1);
		while (IORD_32DIRECT(drawer_base, 20) == 0)
			;
	} else {
		alt_up_pixel_buffer_dma_draw_box(system->pixel_buffer, x1, y1, x2, y2,
				color, 0);
	}
}

void drawLine(system_t* system, int x1, int y1, int x2, int y2, int color) {
	alt_up_pixel_buffer_dma_draw_line(system->pixel_buffer, x1, y1, x2, y2,
			color, 0);
}

void drawPlayers(system_t* system, player_t* player) {
	// draw player 1 UI
	drawBox(system, PLAYER_ONE_X1, PLAYER_ONE_Y1, PLAYER_ONE_X2, PLAYER_ONE_Y2,
			formatColor(0xff0000));

	// draw player 2 UI
	drawBox(system, PLAYER_TWO_X1, PLAYER_TWO_Y1, PLAYER_TWO_X2, PLAYER_TWO_Y2,
			formatColor(0x0000ff));
}

int drawCannonBall(system_t* system, player_t* player, dir_t dir, double time,
		int color, int WIND_VEL) {
	double theta = player->angle * M_PI / 180;
	int initXPos, initYPos;

	if (player->id == 1) {
		initXPos = INIT_BALL_XPOS_TO_RIGHT;
		initYPos = INIT_BALL_YPOS_TO_RIGHT;
	} else if (player->id == 2) {
		initXPos = INIT_BALL_XPOS_TO_LEFT;
		initYPos = INIT_BALL_YPOS_TO_LEFT;
	}
	int ballXPos = 0;
	int ballYPos = 0;

	if (time == 0.0) {
		if (dir == RIGHT) {
			ballXPos = INIT_BALL_XPOS_TO_RIGHT;
			ballYPos = INIT_BALL_YPOS_TO_RIGHT;
		} else if (dir == LEFT) {
			ballXPos = INIT_BALL_XPOS_TO_LEFT;
			ballYPos = INIT_BALL_YPOS_TO_LEFT;
		}
	} else {
		if (dir == RIGHT) {
			//x(t) = initial_position_x + velocity*cos(theta)*t
			ballXPos = initXPos + (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
		} else if (dir == LEFT) {
			ballXPos = initXPos - (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
		}

		//y(t) = initial_position_y - (velocity*sin(theta)*t + (0.5)*ACCEL_G*t*t)
		ballYPos = initYPos - (0.5) * ACCEL_G * GRAVITY_SCALE * time * time
				- player->velocity * time * sin(theta);
	}

	// Cannonball out-of-bound check
	// does not include out of bound to top
	if (ballXPos >= 320 || ballYPos >= 240 || ballXPos <= 0)
		return 1;

	// Cannonball hit player check
	if (dir == RIGHT) {
		if ((ballXPos >= PLAYER_TWO_X1 - BALL_SIZE)
				&& (ballYPos >= PLAYER_TWO_Y2 - BALL_SIZE)) {
			return 2;
		} else if ((ballXPos <= PLAYER_ONE_X2 + BALL_SIZE)
				&& (ballYPos >= PLAYER_ONE_Y2 + BALL_SIZE)) {
			return 3;
		}
	} else if (dir == LEFT) {
		if ((ballXPos <= PLAYER_ONE_X2 + BALL_SIZE)
				&& (ballYPos >= PLAYER_ONE_Y2 + BALL_SIZE)) {
			return 2;
		} else if ((ballXPos >= PLAYER_TWO_X1 - BALL_SIZE)
				&& (ballYPos >= PLAYER_TWO_Y2 - BALL_SIZE)) {
			return 3;
		}
	}

	drawBox(system, ballXPos, ballYPos, ballXPos + BALL_SIZE,
			ballYPos - BALL_SIZE, formatColor(color));
	return 0;
}

int animateShooting(system_t* system, player_t* player, int wind_vel) {
	dir_t dir;
	int result = 0;
	double time = 0.0;

	if (player->id == 1) {
		dir = RIGHT;
	} else if (player->id == 2) {
		dir = LEFT;
	}

	while (result == 0) {
		if (time != 0.0) {
			time -= TIME_SCALE;
			drawCannonBall(system, player, dir, time, BACKGROUND_HEX, wind_vel);
			time += TIME_SCALE;
		}
		result = drawCannonBall(system, player, dir, time, BALL_HEX, wind_vel);
		time += TIME_SCALE;
		usleep(12000);
	}
	drawPlayers(system, player);
	if (result == 2) {
		return 1; //Hit
	} else if (result == 3) {
		return 2; // self-hit
	} else
		return 0; //Miss
}

void show_angle(player_t *Player, system_t *system, shape_t *last_line,
		int intAngle) {
	int player = Player->id;
	double angle = (double) intAngle;
	alt_up_pixel_buffer_dma_dev* pixel_buffer = system->pixel_buffer;
	alt_up_char_buffer_dev * char_buffer = system->char_buffer;
	double theta = angle * M_PI / 180;
	//TODO: convert input angle number into string...
	int x_1, x_2;
	int y_1 = 210;
	int y_2 = y_1 - 15 * sin(theta);
	int line_length = 25;

	if (player == 1) {
		x_1 = 20;
		x_2 = x_1 + line_length * cos(theta);
		clear_last_line(last_line, system);
	} else {
		x_1 = 298;
		x_2 = x_1 - line_length * cos(theta);
		clear_last_line(last_line, system);
	}

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x_1, y_1, x_2, y_2, 0x0000,
			0); //draw line
	//store endpoints of line for later
	last_line->x_1 = x_1;
	last_line->x_2 = x_2;
	last_line->y_1 = y_1;
	last_line->y_2 = y_2;

	return;
}

void clear_last_line(shape_t *last_line, system_t *system) {
	alt_up_pixel_buffer_dma_dev* pixel_buffer = system->pixel_buffer;
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, last_line->x_1,
			last_line->y_1, last_line->x_2, last_line->y_2, 0xf7f8e0,
			0);
}

void draw_power_bar(player_t *Player, system_t *system) {
	int player = Player->id;
	int x_1, y_1, bar_width, bar_height;
	x_1 = 30;
	y_1 = 230;
	bar_width = 52;
	bar_height = 5;
	if (player == 1) {
		drawBox(system, x_1, y_1, x_1 + bar_width, y_1 + bar_height, 0xFFFFF);
		drawBox(system, x_1 + 1, y_1 + 1, x_1 + bar_width - 1, y_1 + bar_height - 1,
				0x00000);
	} else { //player2 power bar
		drawBox(system, 320 - x_1, y_1, 320 - (x_1 + bar_width), y_1 + bar_height,
				0xFFFFF);
		drawBox(system, 320 - (x_1 + 1), y_1 + 1, 320 - (x_1 + bar_width - 1),
				y_1 + bar_height - 1, 0x00000);
	}

}

//assumes velocity range is from 0-100
void update_power(player_t *Player, system_t *system) {
	int player = Player->id;
	int velocity = Player->velocity;
	draw_power_bar(Player, system); //clear previous level
	if (player == 1) {
		drawBox(system, 31, 231, 31 + (velocity / 2), 234, 0xFF000);
	} else {
		drawBox(system, 320 - 31, 231, 320 - (31 + (velocity / 2)), 234, 0x0000ff);
	}
	return;
}


void animate();

/*
 * Function to draw a bitmap image to the VGA display.
 * Parameters:
 * 	*pixel buffer 				: pixel buffer
 * 	*pixel_data				 	: pointer to pixel data exported to C source from GIMP
 * 	image_width, image_height 	: width and height of image to draw
 * 	x, y						: starting location (top left pixel) of drawing
 * Optional: replace all draw_X() calls with this function instead.
 *
 */
void draw_bmp(system_t *system, unsigned short *pixel_data, int image_width, int image_height, int x, int y){
	int i, j;
	for(j=0; j <= image_height-1; j++){
		for(i=0; i <= image_width-1; i++){
			if(*pixel_data == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(system->pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(system->pixel_buffer, *pixel_data, x+i, y+j);
			}
			pixel_data++;
		}
	}
}

void draw_bomb(system_t *system, int x, int y){
	draw_bmp(system, &BOMB_pixel_data, BOMB_WIDTH, BOMB_HEIGHT, x, y);
}

void draw_ground(system_t *system){
	draw_bmp(system, &GROUND_pixel_data, GROUND_WIDTH, GROUND_HEIGHT, 0, SCREEN_HEIGHT-GROUND_HEIGHT);
}

void draw_player1(system_t *system, int x, int y ){
	draw_bmp(system, &PLAYER_1_pixel_data, PLAYER_1_WIDTH, PLAYER_1_HEIGHT, x, y);
}

void draw_player2(system_t *system, int x, int y ){
	draw_bmp(system, &PLAYER_2_pixel_data, PLAYER_2_WIDTH, PLAYER_2_HEIGHT, x, y);
}

void draw_cannon1(system_t *system, int x, int y){
	draw_bmp(system, &CANNON_1_pixel_data, CANNON_1_WIDTH, CANNON_1_HEIGHT, x, y);
}

void draw_cannon2(system_t *system, int x, int y){
	draw_bmp(system, &CANNON_2_pixel_data, CANNON_2_WIDTH, CANNON_2_HEIGHT, x, y);
}

void animate_cannon1(system_t *system){
	int t;
	for(t = 40; t >= 0; t--){
		if(t%5 == 0)
			draw_cannon1(system, CANNON_1_X, CANNON_1_Y);
		else
			draw_bmp(system, &CANNON_1_FRAME2_pixel_data, CANNON_1_WIDTH, CANNON_1_HEIGHT,CANNON_1_X, CANNON_1_Y);
	}
	draw_bmp(system, &CANNON_1_FRAME3_pixel_data, CANNON_1_WIDTH, CANNON_1_HEIGHT,CANNON_1_X, CANNON_1_Y);
	for(t = 55; t >=0; t--); //wait
	draw_cannon1(system, CANNON_1_X, CANNON_1_Y);
}

void animate_cannon2(system_t *system){
	int t;
	for(t = 40; t >= 0; t--){
		if(t%5 == 0)
			draw_cannon2(system, CANNON_2_X, CANNON_2_Y);
		else
			draw_bmp(system, &CANNON_2_FRAME2_pixel_data, CANNON_2_WIDTH, CANNON_2_HEIGHT,CANNON_2_X, CANNON_2_Y);
	}
	draw_bmp(system, &CANNON_2_FRAME3_pixel_data, CANNON_2_WIDTH, CANNON_2_HEIGHT,CANNON_2_X, CANNON_2_Y);
	for(t = 55; t >=0; t--); //wait
	draw_cannon2(system, CANNON_2_X, CANNON_2_Y);
}

void draw_P1WIN(system_t *system){
	draw_bmp(system, &P1WINS_pixel_data, P1WINS_WIDTH, P1WINS_HEIGHT, DIALOGUE_X, DIALOGUE_Y);
}

void draw_P2WIN(system_t *system){
	draw_bmp(system, &P2WINS_pixel_data, P2WINS_WIDTH, P2WINS_HEIGHT, DIALOGUE_X, DIALOGUE_Y);
}

void draw_player1GUI(system_t *system){
	draw_bmp(system, &P1BOX_pixel_data, P1BOX_WIDTH, P1BOX_HEIGHT, P1GUI_X, P1GUI_Y);
}

void draw_player2GUI(system_t *system){
	draw_bmp(system, &P2BOX_pixel_data, P2BOX_WIDTH, P2BOX_HEIGHT, P2GUI_X, P2GUI_Y);
}
