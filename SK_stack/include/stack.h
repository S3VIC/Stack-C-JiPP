#pragma once
#include "node.h"
#include<stdint.h>
#include <stdio.h>

// Stack representation as structure containing pointer to the node and count
typedef struct Stack {
	StackNode* item;
	unsigned int count;
} Stack;

// Prototype of the function for releasing resources of a single item
// ARGS: stack - pointer to the stack to be processed
typedef int (*free_item)(Stack* stack);

// Prototype of the function for displaying single item
// ARGS: data - data to be displayed 
typedef int (*display_item)(void* data);

// Prototype of the function for searching for an item in stack
// ARGS:
// data - data to be validated with context
// context - context used for comparison
typedef int8_t (*search_item)(void* data, const char* context); //TODO

// Prototype of the function for saving an item to file
// ARGS: 
// file - pointer to FILE stream
// data - pointer to data to be saved
typedef int8_t (*save_item)(void* data, FILE* file);

// Prototype of the function for reading an item from file
// ARGS: file - pointer to FILE stream
typedef void* (*read_item)(FILE* file);

// Adds an element to the stack
// ARGS: 
// stack - pointer to the stack to be processed
// data - pointer to the data to be attached to stack
int8_t stack_add(Stack* stack, void* data);

// Removes last element of the stack
// ARGS: stack - pointer to the stack to be processed
int8_t stack_pop(Stack* stack);

// Releases resources taken by the stack
// ARGS: stack pointer to the pointer to stack - making available setting NULL to the pointer to stack within function
int8_t stack_free(Stack** stack);

// Displays first item of the stack
// ARGS: stack - pointer to stack to be processed
int8_t stack_display_first(Stack* stack);

// Displays all stack items
// NOTE: order of displaying is from top of the stack to bottom (from last item to first)
// ARGS: stack - pointer to stack to be displayed
int8_t stack_display(Stack* stack);

// Saves stack data to binary file format
// ARGS: stack - pointer to the stack to be processed
int8_t stack_save_bin(Stack* stack);

// Reads stack data from binary file format
// ARGS:
// stack - pointer to the stack to be processed
// fileName - name of the file to be opened
int8_t stack_read_bin(const char* fileName, Stack* stack);

// Searches stack for particular item
// ARGS: stack - pointer to the stack to be processed
void* stack_search(Stack* stack);

// Initialises stack with all necessary function pointers
// ARGS:
// pop_item - pointer to the function for releasing item
// print_item - pointer to the function for displaying item
// search_item - pointer to the function for searching item
// save_item - pointer to the function for saving item
// read_item - pointer to the function for reading item
Stack* stack_init(free_item pop_item, display_item print_item, search_item search_item, save_item save_item, read_item read_item); //TODO