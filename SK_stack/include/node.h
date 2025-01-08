#pragma once
// StackNode representation containing pointer to data and to previous element
typedef struct StackNode {
	void* curr;
	struct StackNode* prev;
} StackNode;
