#pragma once
#include <stdint.h>
#include <stdio.h>
#include "studyFiled.h"

// Student representation as a struct
typedef struct MY_STUDENT {
	int year;
	StudyField studyField;
	const char* lastname;
} MY_STUDENT;


// Creates an instance of student
MY_STUDENT* MY_STUDENT_create();

// Releases resources taken by student instance
// ARGS: student - pointer to the pointer to the student
void MY_STUDENT_free(void** student);

// Retrieves str form of study field
// ARGS: student - pointer to the student to be processed 
char* MY_STUDENT_get_study_field(MY_STUDENT* student);

// Compares passed data with context
// ARGS:
// data - general student data representation
// context - str by which a student will be searched
int8_t MY_STUDENT_search(void* data, const char* context);

// displays particular student with some formatting
// ARGS: data - general student data representation
int8_t MY_STUDENT_display(void* data);

// Saves student data to binary file format
// ARGS:
// data - general student data representation
// file - file handler
int8_t MY_STUDENT_save_bin(void* data, FILE *file);

// Reads student data from a binary file format
// ARGS: file - file handler 
void* MY_STUDENT_read_bin(FILE* file);
