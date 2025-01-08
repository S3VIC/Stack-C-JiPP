#pragma once

// This file contains combined groups of different types of messages
// INFO, WARNINGS, ERRORS, alongside with their string representations and enum mappings
typedef enum {
	INFO,
	WARNING,
	ERROR
} LOG_TYPE;

static const char* logType[] = {
	"[INFO]",
	"[WARNING]",
	"[ERROR]"
};

typedef enum {
	ERROR_MEM_ALLOC,
	ERROR_FILE_OPEN,
	ERROR_FILE_WRITE,
	ERROR_FILE_READ,
	ERROR_NULL_ARGUMENTS,
	ERROR_OPERAION,
	ERROR_MSG_NUM
} ERROR_TYPE;

static const char* errorMessages[] = {
	"Error during memory allocation",
	"Error opening file",
	"Error writing to file",
	"Error reading from a file",
	"Null arguments passed",
	"Operation error"
};

typedef enum {
	WARNING_STACK_NOT_INITIALISED,
	WARNING_STACK_ALREADY_INITIALISED,
	WARNING_STACK_EMPTY,
	WARNING_STACK_NOT_EMPTY,
	WARNING_INVALID_INPUT,
	WARNING_ITEM_NOT_FOUND,
	WARNING_MSG_NUM
} WARNING_TYPE;

static const char* warningMessages[] = {
	"Stack not initialised",
	"Stack already initialised",
	"Stack empty",
	"Stack not empty",
	"Invalid input",
	"Item not found"
};


typedef enum {
	INFO_STDIN_CLEANUP,
	INFO_STACK_INITIALISED,
	INFO_ITEM_FOUND,
	INFO_RESOURCE_DEALLOCATED,
	INFO_MSG_NUM
} INFO_TYPE;

static const char* infoMessages[] = {
	"Clearing stdin",
	"Stack initialised",
	"Item found",
	"Resource deallocated"
};

void log_message(LOG_TYPE type, int message, const char* context);
