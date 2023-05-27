#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void loadbinary(int* memory, char* path) {

    FILE* binary = fopen(path, "r")

    // If file pointer is null then return with error.
    if (binary == NULL) {
        printf("Error in opening file.\n")
        return;
    }

    // Read data from binary until end of file. Only 1 piece of data read each time.
    int i = 0, read = 1;
    while (read) {
        read = fread(&memory[i], sizeof(int), 1, binary)
        i++;
    }

    fclose(binary);
}
