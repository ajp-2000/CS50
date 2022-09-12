#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define MAX_BLOCKS 2048
typedef uint8_t BYTE;

// Check for JPEGs based on the first four bytes of a block
bool isjpg(BYTE a, BYTE b, BYTE c, BYTE d)
{
    if (a == 0xff)
    {
        if (b == 0xd8)
        {
            if (c == 0xff)
            {
                // The fourth byte should be 0xe*
                if ((d >> 4) == 0xe)
                {
                    return true;
                }
            }
        }
    }


    return false;
}

int main(int argc, char *argv[])
{
    // Command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open image
    FILE *image = fopen(argv[1], "r");
    if (image == NULL)
    {
        printf("Unable to open file \"%s\".\n", argv[1]);
        return 2;
    }

    // Go one block at a time
    BYTE *block = malloc(sizeof(BYTE) * BLOCK_SIZE);
    int photo = 0;
    bool writing = false;
    int b = 0;

    char *filename;
    FILE *output;
    while (fread(block, 1, BLOCK_SIZE, image) == BLOCK_SIZE)
    {
        if (isjpg(block[0], block[1], block[2], block[3]))
        {
            // Write until we reach a new JPEG header
            if (!writing)
            {
                writing = true;
                filename = malloc(sizeof(char) * 8);
                sprintf(filename, "%03d.jpg", photo);
                output = fopen(filename, "w");
            }
            else
            {
                // Free the resources for the finished JPEG
                fclose(output);
                free(filename);
                photo++;

                // And set them up for the next, since we've now found a new header
                filename = malloc(sizeof(char) * 8);
                sprintf(filename, "%03d.jpg", photo);
                output = fopen(filename, "w");
            }
        }

        if (writing)
        {
            if (fwrite(block, 1, BLOCK_SIZE, output) != BLOCK_SIZE)
            {
                printf("Error writing to filename %s.\n", filename);
                return 3;
            }
        }

        b++;
    }
    fclose(output);
    free(filename);

    fclose(image);
    free(block);
}