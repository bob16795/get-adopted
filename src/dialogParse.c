#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct dialogLine {
	int identifier;
	int* dialog;
};


char getDialog(int pos, int len, char string[]) {

    char substring[1024];

    int i = 0;
    while (i < len) {
        substring[i] = string[pos + i - 1];
        i++;
    }

    substring[i] = '\0';
    return substring;
}

char getID(int pos, int len, char string[]) {

	char substring[4];

	int i = 0;
	while (i < len) {
		substring[i] = string[pos + i - 1];
		i++;
	}

	substring[i] = '\0'; 
	return substring;
}


int main() {
	//Initalize array of dialog
	struct dialogLine* allText = calloc(0, sizeof(struct dialogLine));
	int numLines = 0;

	//Read file
	FILE *fptr;
	fptr = fopen("../ass/dialog.txt", "r");

	//Make string to assign lines to
	char line[1029];
	
	while(fgets(line, 1029, fptr)) {
	//Start reading a line
	numLines++;
	allText = realloc(allText, numLines * sizeof(struct dialogLine));
	
	char identifier = substring(1, 5, line);
	char dialog = substring(5, 1029, line);

	printf("%s. %s", identifier, dialog);
	
	}
	return 0;
}

