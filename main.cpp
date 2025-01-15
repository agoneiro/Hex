#include "Hex.h"
#include <iostream>

int main() {
	Hex* hex = nullptr;

	int buffer = 0;
	while ((buffer = getchar()) != EOF) {
		if (buffer == ' ') {
			if (hex) delete hex; 
			hex = new Hex();
		}
		else if (buffer == 'B' || buffer == 'P' || buffer == 'I' || buffer == 'C') {
			ungetc((char)buffer, stdin);
			hex->queryService();
		}
	}

	delete hex;
	return 0;
}