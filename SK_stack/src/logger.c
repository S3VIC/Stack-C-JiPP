#include "../include/logger.h"
#include <stdio.h>
#include <string.h>

void log_message(LOG_TYPE type, int message, const char* context) {
	char* msgType = logType[type];
	char* messageStr;
	switch (type) {
	case INFO:
		messageStr = infoMessages[message];
		break;
	case WARNING:
		messageStr = warningMessages[message];
		break;
	case ERROR:
		messageStr = errorMessages[message];
		break;
	default:
		messageStr = "Wrong logging specification";
		break;
	}
	if (strlen(context) > 0) {
		printf("%s %s: %s\n", msgType, messageStr, context);
		return;
	}
		printf("%s %s\n", msgType, messageStr);
}
