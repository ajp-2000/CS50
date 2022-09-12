// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    if (fread(&header, 1, 44, input) < 0)
    {
        printf("Error reading header.");
        return 2;
    }

    if (fwrite(&header, 1, 44, output) != 44)
    {
        printf("Error writing header.");
        return 2;
    }

    // Read samples from input file and write updated data to output file
    int16_t buffer;
    while (fread(&buffer, 2, 1, input) == 1)
    {
        buffer *= factor;
        if (fwrite(&buffer, 2, 1, output) != 1)
        {
            printf("Error writing audio.");
            return 3;
        }
    }

    printf("Write successful.\n");

    // Close files
    fclose(input);
    fclose(output);
}
