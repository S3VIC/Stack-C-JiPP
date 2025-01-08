#pragma once

void clear_stdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		//printf("Discarded code: %d", c);
		//Discard chars
	}
}
