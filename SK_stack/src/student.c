#include<stdlib.h>
#include <string.h>
#include "../include/student.h"
#include "../include/helpers.h"
#include "../include/logger.h"

#define MAX_LASTNAME_LEN 25

MY_STUDENT* MY_STUDENT_create() {
	MY_STUDENT* student = (MY_STUDENT*)malloc(sizeof(MY_STUDENT));
	if (!student) {
		log_message(ERROR, ERROR_MEM_ALLOC, "student (MY_STUDENT_create)");
		return NULL;
	}
	char lastnameBuff[MAX_LASTNAME_LEN];
	int result = 0;
	size_t len;
	do {
		printf("Lastname:\n");
		scanf_s("%s", lastnameBuff, (unsigned)MAX_LASTNAME_LEN);
		len = strlen(lastnameBuff);
		//validate input for Lastname, if wrong - clear stdin and repeat input
		if (len == 0) {
			log_message(WARNING, WARNING_INVALID_INPUT, "Wrong or too long input provided");
			log_message(INFO, INFO_STDIN_CLEANUP, "");
			clear_stdin();
		}
	} while (len < 1);
	student->lastname = (char*)malloc(sizeof(char) * (len + 1));
	if (student->lastname == NULL) {
		log_message(ERROR, ERROR_MEM_ALLOC, "student lastname (MY_STUDENT_create)");
		return NULL;
	}
	strncpy_s(student->lastname, (rsize_t)(len + 1), lastnameBuff, MAX_LASTNAME_LEN );
	printf("Year:\n");
	result = scanf_s("%d", &student->year);
	int field = 0;
	do {
		printf("Field num:\n");
		result = scanf_s("%d", &field);
	} while (field >= FIELD_NUM && result > 0);
	
	student->studyField = field;
	return student;
}

void MY_STUDENT_free(void** data) {
	if (!data) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "data (MY_STUDENT_free)");
		return;
	}
	if (!*data) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "*data (MY_STUDENT_free)");
		return;
	}
		MY_STUDENT **student = (MY_STUDENT**)data;
	free((*student)->lastname);
	(*student)->lastname = NULL;
	free(*student);
	*student = NULL;
}

char* MY_STUDENT_get_study_field(MY_STUDENT* student) {
	if (!student) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "student (MY_STUDENT_get_study_field)");
		return NULL;
	}
	//Check index low/high boundaries
	if (student->studyField >= FIELD_NUM || student->studyField < 0) {
		log_message(WARNING, WARNING_INVALID_INPUT, "index out of boundaries");
		return NULL;
	}
	return fieldStrs[student->studyField];
}

int8_t MY_STUDENT_save_bin(void* data, FILE *file) {
	if (data == NULL || file == NULL) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "(MY_STUDENT_save_bin)");
		return NULL;
	}
	MY_STUDENT* student = (MY_STUDENT*)data;
	size_t result = 0;
	if(fwrite(&student->year, sizeof student->year, 1, file) != 1)
		return 1;
	if(fwrite(&student->studyField, sizeof student->studyField, 1, file) != 1)
		return 1;
	size_t len = strlen(student->lastname) + 1;
	if(fwrite(&len, sizeof len, 1, file) != 1)
		return 1;
	if (fwrite(student->lastname, sizeof student->lastname[0], len, file) != len)
		return 1;

	return 0;
}

void* MY_STUDENT_read_bin(FILE* file) {
	MY_STUDENT* student = (MY_STUDENT*)malloc(sizeof(MY_STUDENT));
	if (!student) {
		log_message(ERROR, ERROR_MEM_ALLOC, "student (MY_STUDENT_read_bin)");
		return NULL;
	}
	if (fread(&student->year, sizeof(int), 1, file) != 1) {
		free(student);
		return NULL;
	}
	if (fread(&student->studyField, sizeof(int), 1, file) != 1) {
		free(student);
		return NULL;
	}
	size_t len;
	if (fread(&len, sizeof(size_t), 1, file) != 1) {
		free(student);
		return NULL;
	}
	student->lastname = (char*)malloc(sizeof(char) * len);
	if (!student->lastname) {
		free(student);
		return NULL;
	}
	if (fread(student->lastname, sizeof(char), len, file) != len) {
		free(student->lastname);
		free(student);
		return NULL;
	}
	return (void *)student;
}

int8_t MY_STUDENT_display(void* data) {
	if (!data) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "data (MY_STUDENT_display)");
		return 1;
	}
	MY_STUDENT* student = (MY_STUDENT*)data;
	printf("Name: %s\nYear: %d\nStudy field: %s\n",
		student->lastname,
		student->year,
		MY_STUDENT_get_study_field(student)
	);
	return 0;
}

int8_t MY_STUDENT_search(void* data, const char* context) {
	if (data == NULL || strlen(context) == 0) {
		log_message(ERROR, ERROR_NULL_ARGUMENTS, "Null data or empty context (MY_STUDENT_search)");
		// Because of specifics of strcmp return values, here 0 return means error
		return 0; 
	}
	MY_STUDENT* student = (MY_STUDENT*)data;
	return strcmp(student->lastname, context) == 0;
}
