#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get the height
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (!(height >= 1 && height <= 8));

    // Loop through rows
    for (int row = 0; row < height; row++)
    {
        // Print the left half of the pyramid
        for (int col = 0; col < height; col++)
        {
            if (col >= height - row - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }

        // Print the gap
        printf("  ");

        // And the right half of the pyramid
        for (int col = 0; col <= row; col++)
        {
            printf("#");
        }

        // Then newline
        printf("\n");
    }
}