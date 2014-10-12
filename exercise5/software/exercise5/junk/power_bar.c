///*
// * power_bar.c
// *
// *  Created on: 2014-10-01
// *      Author: Adam
// */
//#include <stdio.h>
//#include <stdlib.h>
//#include "system.h"
//#include "io.h"
//#include "altera_up_avalon_video_pixel_buffer_dma.h"
//#include "altera_up_avalon_video_character_buffer_with_dma.h"
//#define drawer_base (volatile int *) 0x6000 //must be base address of pixel_drawer avalon_slave
//#define BACKGROUND 0x00000;
//
//void draw_power_bar(alt_up_pixel_buffer_dma_dev* pixel_buffer);
//void update_power_bar(alt_up_pixel_buffer_dma_dev* pixel_buffer, double velocity);
//void draw_box(int x_1, int y_1, int x_2, int y_2, unsigned int colour);
//
//void draw_power_bar(alt_up_pixel_buffer_dma_dev* pixel_buffer){
//	int x_1, y_1, bar_width, bar_height;
//	//draw player1 power bar
//	IOWR_32DIRECT(drawer_base,0,x_1);
//	IOWR_32DIRECT(drawer_base,4,y_1);
//	IOWR_32DIRECT(drawer_base,8,x_1+bar_width);
//	IOWR_32DIRECT(drawer_base,12,y_1-bar_height);
//	IOWR_32DIRECT(drawer_base,16,0xFFFFF);
//	IOWR_32DIRECT(drawer_base,20,1);
//	while(IORD_32DIRECT(drawer_base,20)==0);
//	IOWR_32DIRECT(drawer_base,0,x_1+2);
//	IOWR_32DIRECT(drawer_base,4,y_1+2);
//	IOWR_32DIRECT(drawer_base,8,x_1+bar_width-2);
//	IOWR_32DIRECT(drawer_base,12,y_1-bar_height-2);
//	IOWR_32DIRECT(drawer_base,16,0x00000);
//	IOWR_32DIRECT(drawer_base,20,1);
//	while(IORD_32DIRECT(drawer_base,20)==0);
//	//draw player2 power bar
//	IOWR_32DIRECT(drawer_base,0,320-x_1);
//	IOWR_32DIRECT(drawer_base,4,y_1);
//	IOWR_32DIRECT(drawer_base,8,320-x_1-bar_width);
//	IOWR_32DIRECT(drawer_base,12,y_1-bar_height);
//	IOWR_32DIRECT(drawer_base,16,0xFFFFF);
//	IOWR_32DIRECT(drawer_base,20,1);
//	while(IORD_32DIRECT(drawer_base,20)==0);
//	IOWR_32DIRECT(drawer_base,0,320-x_1-2);
//	IOWR_32DIRECT(drawer_base,4,y_1+2);
//	IOWR_32DIRECT(drawer_base,8,320-x_1-bar_width+2);
//	IOWR_32DIRECT(drawer_base,12,y_1-bar_height-2);
//	IOWR_32DIRECT(drawer_base,16,0x00000);
//	IOWR_32DIRECT(drawer_base,20,1);
//	while(IORD_32DIRECT(drawer_base,20)==0);
//}