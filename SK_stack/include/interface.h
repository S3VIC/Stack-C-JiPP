#pragma once

typedef enum Interface {
	EXIT,
	INIT_STACK,
	ADD_ITEM,
	POP_ITEM,
	DISPLAY_FIRST,
	DISPLAY_ITEMS,
	SEARCH_ITEM,
	SAVE_TO_FILE,
	READ_FROM_FILE,
	FREE_STACK,
	CLEAR_SCREEN,
	OPT_NUM
} Interface;

// Clears terminal and displays interface
int display_interface();

// Main app loop
void app_loop();
