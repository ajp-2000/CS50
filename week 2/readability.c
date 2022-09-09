#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sents(string text);

int main(void)
{
    // Get text
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sents(text);

    // Calculate Coleman-Liau index
    float L = (float) letters / ((float) words / 100);
    float S = (float) sentences / ((float) words / 100);
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // Turn into required output
    int grade = round(index);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int i = 0;
    int total = 0;

    while (text[i] != '\0')
    {
        if (isalpha(text[i]))
        {
            total++;
        }

        i++;
    }


    return total;
}

int count_words(string text)
{
    int i = 0;                                      // If the first character is a space that doesn't add a word
    int total = 0;

    // Set words back to 1 if there's anything in the text at all, leaving the below to just deal with spaces
    while (text[i] != '\0')
    {
        if (isalpha(text[i]))
        {
            total = 1;
            break;
        }

        i++;
    }

    i = 1;
    while (text[i] != '\0')
    {
        if (text[i] == ' ')
        {
            // Only count this as a new word if we're at the beginning of the space, and the space doesn't end the text
            if (text[i - 1] != ' ' && text[i + 1] != '\0')
            {
                total++;
            }
        }

        i++;
    }


    return total;
}

int count_sents(string text)
{
    int i = 1;
    int total = 0;

    while (text[i] != '\0')
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            if (text[i - 1] != '.' && text[i - 1] != '!' && text[i - 1] != '?' && text[i - 1] != ' ')
            {
                total++;
            }
        }

        i++;
    }


    return total;
}