#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/interface.h"
#include "../include/stack.h"
#include "../include/student.h"
#include "../include/logger.h"

int display_interface() {
	system("cls");
	printf("Stack implementation by SK\n");
	printf("1. Initialise\n");
	printf("2. Add item\n");
	printf("3. Pop item\n");
	printf("4. Display first\n");
	printf("5. Display items\n");
	printf("6. Search item\n");
	printf("7. Save to file\n");
	printf("8. Read from file\n");
	printf("9. Free stack\n");
	printf("10. Clear screen\n");
	printf("0. Exit\n");
}

void app_loop() {
	int option = 0;
	int8_t initialised = 0, result = 0;
	Stack* stack = NULL;
	MY_STUDENT* student = NULL;
	display_interface();
	do {
		printf("Option: ");
		scanf_s("%2d", &option);
		switch (option) {
			case INIT_STACK:
				if (!initialised) {
					stack = stack_init(
						MY_STUDENT_free,
						MY_STUDENT_display,
						MY_STUDENT_search,
						MY_STUDENT_save_bin,
						MY_STUDENT_read_bin
					);
					if (!stack) {
						log_message(ERROR, ERROR_MEM_ALLOC, "");
						continue;
					}
					initialised = !initialised;
					log_message(INFO, INFO_STACK_INITIALISED, "");
					break;
				}
				log_message(WARNING, WARNING_STACK_ALREADY_INITIALISED, "");
				break;
			case ADD_ITEM:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "");
					break;
				}
				student = MY_STUDENT_create();
				if (!student) {
					log_message(ERROR, ERROR_MEM_ALLOC, "student creation");
					break;
				}
				result = stack_add(stack, (void*)student);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "adding data to stack");
				break;
			case POP_ITEM:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				result = stack_pop(stack);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "adding data to stack");
				break;
			case DISPLAY_FIRST:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				result = stack_display_first(stack);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "displaying stack items");
				break;
			case DISPLAY_ITEMS:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				result = stack_display(stack);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "displaying stack items");
				break;
			case SEARCH_ITEM:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				student = stack_search(stack);
				if (student == NULL) {
					log_message(WARNING, WARNING_ITEM_NOT_FOUND, "No item matching given requirement");
					break;
				}
				log_message(INFO, INFO_ITEM_FOUND, "");
				MY_STUDENT_display(student);
				break;
			case SAVE_TO_FILE:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				result = stack_save_bin(stack);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "saving data to file");
				break;
			case READ_FROM_FILE:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				if (stack->count != 0) {
					log_message(WARNING, WARNING_STACK_NOT_EMPTY, "Please empty current stack before proceeding with reading from file.");
					continue;
				}
				char fileName[30];
				scanf_s("%s", fileName, sizeof(fileName));
				result = stack_read_bin(fileName, stack);
				if (result != 0)
					log_message(ERROR, ERROR_OPERAION, "reading data from file");
				break;
			case FREE_STACK:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Proceed with option 1 first or read data from file");
					break;
				}
				stack_free(&stack);
				log_message(INFO, INFO_RESOURCE_DEALLOCATED, "stack");
				initialised = !initialised;
				break;
			case CLEAR_SCREEN:
				display_interface();
				break;
			case EXIT:
				if (!initialised) {
					log_message(WARNING, WARNING_STACK_NOT_INITIALISED, "Possible wrong input. Exiting...");
					exit(0);
				}
				stack_free(&stack);
				log_message(INFO, INFO_RESOURCE_DEALLOCATED, "stack");
				exit(0);
			default: 
				log_message(WARNING, WARNING_INVALID_INPUT, "invalid option chosen");
				continue;
		}
	} while (1);
}
