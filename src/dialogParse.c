#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct dialogLine {
    int identifier;
    char* dialog;
};

// Get the ID from format <XXXX>
int getID(const char* line) {
    char id_str[5] = {0};  // Only take 4 digits
    // Skip the '<' and copy until '>'
    int i = 1;  // Start after '<'
    while (line[i] != '>' && i < 5) {
        id_str[i-1] = line[i];
        i++;
    }
    return atoi(id_str);
}

// Get the dialog text after the ID
char* getDialog(const char* line) {
    const char* text_start = strchr(line, '>');
    if (text_start == NULL) return NULL;
    
    text_start++;  // Move past the '>'
    size_t len = strlen(text_start);
    char* dialog = malloc(len + 1);
    if (dialog == NULL) return NULL;
    
    strcpy(dialog, text_start);
    // Remove trailing newline if present
    if (len > 0 && dialog[len-1] == '\n') {
        dialog[len-1] = '\0';
    }
    
    return dialog;
}

int main() {
    struct dialogLine* allText = NULL;
    int numLines = 0;
    
    FILE* fptr = fopen("../ass/dialog.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    char line[1029];
    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] != '<') continue;  // Skip invalid lines
        
        numLines++;
        struct dialogLine* temp = realloc(allText, numLines * sizeof(struct dialogLine));
        if (temp == NULL) {
            printf("Memory allocation failed\n");
            break;
        }
        allText = temp;
        
        //Parse the line
        allText[numLines - 1].identifier = getID(line);
        allText[numLines - 1].dialog = getDialog(line);
    }
    
    return 0;
}