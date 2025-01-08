#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"
#include "../include/logger.h"
free_item stack_pop_item;
display_item stack_display_item;
search_item stack_search_item;
save_item stack_save_item;
read_item stack_read_item;

Stack* stack_init(
	free_item pop_item,
	display_item print_item,
	search_item search_item,
	save_item save_item,
	read_item read_item
) {
	if (pop_item == NULL || print_item == NULL || search_item == NULL || save_item == NULL || read_item == NULL) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack initialisation");
	}
	stack_pop_item = pop_item;
	stack_display_item = print_item;
	stack_search_item = search_item;
	stack_save_item = save_item;
	stack_read_item = read_item;

	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if (!stack) {
		log_message(ERROR, ERROR_MEM_ALLOC, "stack allocation");
		return NULL;
	}
	stack->item = NULL;
	stack->count = 0;
	return stack;
}

int8_t stack_add(Stack* stack, void* data) {
	if (stack == NULL || data == NULL) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack_add");
		return 1;
	}
	StackNode* node = (StackNode*)malloc(sizeof(StackNode));
	if (!node) {
		log_message(ERROR, ERROR_MEM_ALLOC, "stack node allocation");
		return 1;
	}
	node->curr = data;
	//Assign proper previous node reference based on initial stack count
	node->prev = (stack->count == 0 && stack->item == NULL) ? NULL : stack->item;
	stack->item = node;
	stack->count++;
	return 0;
}

int8_t stack_pop(Stack* stack) {
	if (!stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack_pop");
		return 1;
	}
	if (stack->count == 0 && stack->item == NULL) {
		log_message(WARNING, WARNING_STACK_EMPTY, "stack_pop");
		return 0;
	}

	(*stack_pop_item)(&stack->item->curr);
	StackNode* tmpNode = stack->item->prev != NULL ? stack->item->prev : NULL;
	free(stack->item);
	stack->item = tmpNode;
	stack->count--;
	return 0;
}

int8_t stack_free(Stack** stack) {
	if (!stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack");
		return 1;
	}
	if (!*stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "*stack");
		return 1;
	}
	if ((*stack)->count == 0 && (*stack)->item == NULL) {
		free(*stack);
		return 0;
	}

	while ((*stack)->item != NULL)
		stack_pop(*stack);
		
	free(*stack);
	*stack = NULL;
	return 0;
}

int8_t stack_display_first(Stack* stack) {
	if (!stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack (stack_display_first)");
		return 1;
	}
	StackNode* item = stack->item;
	while (item->prev != NULL) {
		item = item->prev;
	}
	(*stack_display_item)(item->curr);
	return 0;
}

int8_t stack_display(Stack* stack) {
	if (!stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack (stack_display_first)");
		return 1;
	}
	StackNode* item = stack->item;
	while (item != NULL) {
		(*stack_display_item)(item->curr);
		item = item->prev;
	}
	return 0;
}

void* stack_search(Stack* stack) {
	char str[25];
	printf("Provide lastname to search: ");
	scanf_s("%s", str, sizeof(str));
	size_t len = strlen(str) + 1;
	char* searchStr = (char*)malloc(sizeof(char) * len);
	if (!searchStr) {
		log_message(ERROR, ERROR_MEM_ALLOC, "searchStr");
		return NULL;
	}
	strncpy_s(searchStr, len, str, len);
	int8_t found = 0;
	StackNode* item = stack->item;
	while (item != NULL && !found) {
		found = (*stack_search_item)(item->curr, searchStr);
		if (found)
			break;
		item = item->prev;
	}
	return found ? item->curr : NULL;
}

int8_t stack_save_bin(Stack* stack) {
	if (!stack) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "stack");
		return 1;
	}
	if (stack->count == 0) {
		log_message(WARNING, WARNING_STACK_EMPTY, "aborting saving to file");
		return 1;
	}
	unsigned int itemsNum = stack->count;
	char fileName[30];
	int64_t filePos = 0;
	int64_t *fileDesc = (int64_t*)malloc((itemsNum + 1) * sizeof(int64_t));
	if (fileDesc == NULL) {
		log_message(ERROR, ERROR_MEM_ALLOC, "fileDesc");
		return 1;
	}
	printf("Provide fileName: ");
	scanf_s("%s", fileName, 30);
	FILE* file = fopen(fileName, "wb");
	if (!file) {
		log_message(ERROR, ERROR_FILE_OPEN, "(stack_save_bin)");
		free(fileDesc);
		return 1;
	}
	if (fwrite(&itemsNum, sizeof(unsigned int), 1, file) != 1) {
		log_message(ERROR, ERROR_FILE_WRITE, "number of items");
		free(fileDesc);
		fclose(file);
		return 1;
	}

	_fseeki64(file, (itemsNum + 1) * sizeof(int64_t), SEEK_CUR);
	size_t iter;
	StackNode* item = stack->item;
	int8_t result = 0;
	for (iter = 0; iter < itemsNum; iter++) {
		fileDesc[iter] = _ftelli64(file);
		result = (*stack_save_item)(item->curr, file);
		if (result != 0) {
			log_message(ERROR, ERROR_FILE_WRITE, "writing data to binary file");
			free(fileDesc);
			fclose(file);
			return 1;
		}
		item = item->prev;
	}

	fileDesc[iter] = _ftelli64(file);
	_fseeki64(file, sizeof(unsigned int), SEEK_SET);
	if (fwrite(fileDesc, sizeof(int64_t), itemsNum + 1, file) != itemsNum + 1) {
		log_message(ERROR, ERROR_FILE_WRITE, "writing fileDesc to binary file");
		free(fileDesc);
		fclose(file);
		return 1;
	}
	if (file)
		fclose(file);
	file = NULL;
	if (fileDesc)
		free(fileDesc);
	fileDesc = NULL;
	return 0;
}

int8_t stack_read_bin(const char* fileName, Stack* stack) {
	FILE* file = fopen(fileName, "rb");
	if (!file) {
		log_message(ERROR, ERROR_FILE_OPEN, "opening file for reading");
		return 1;
	}
	unsigned int itemsNum = 0;
	if (fread(&itemsNum, sizeof(unsigned int), 1, file) != 1) {
		log_message(ERROR, ERROR_FILE_WRITE, "writing fileDesc to binary file");
		fclose(file);
		return 1;
	}
	int64_t* fileDesc = (int64_t*)malloc(sizeof(int64_t) * (itemsNum + 1));
	if (!fileDesc) {
		log_message(ERROR, ERROR_MEM_ALLOC, "fileDesc (stack_read_bin)");
		return 1;
	}
		
	
	if (fread(fileDesc, sizeof(fileDesc[0]), itemsNum + 1, file) != itemsNum + 1) {
		log_message(ERROR, ERROR_FILE_READ, "fileDesc (stack_read_bin)");
		return 1;
	}
	void* data;
	for (int iter = itemsNum - 1; iter >= 0; iter--) {
		_fseeki64(file, fileDesc[iter], SEEK_SET);
		data = (*stack_read_item)(file);
		if (!data) {
			log_message(ERROR, ERROR_FILE_READ, "reading item data");
			free(fileDesc);
			stack_free(stack);
			return 1;
		}
		stack_add(stack, data);
	}

	if (file)
		fclose(file);
	file = NULL;
	if (fileDesc)
		free(fileDesc);
	fileDesc = NULL;
	return 0;
}
