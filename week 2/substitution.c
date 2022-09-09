#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int key[26];

char subst(char in)
{
    char lower = tolower(in);
    lower -= 97;
    return (in + key[(int)lower]);
}

int main(int argc, string argv[])
{
    // Check the args are correct
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Lower-case the whole key and set it
    bool found[26] = { false };
    for (int i = 0; i < 26; i++)
    {
        // Check the key is alphabetical
        if (!isalpha(argv[1][i]))
        {
            printf("Non-alphabeticalcharacter '%c' in key.\n", argv[1][i]);
            return 1;
        }

        // Find a number for the letter (de-cased) in the key, with 0 for a, then
        // find a number for the difference between the letter that triggers this part of the key and this number
        int charcode = tolower(argv[1][i]) - 97;
        key[i] = charcode - i;

        // Check for repeated chars
        if (found[charcode])
        {
            printf("Duplicate character '%c' in key.\n", argv[1][i]);
            return 1;
        }
        found[charcode] = true;


    }

    // Get input
    string plain = get_string("plaintext: ");
    int len = strlen(plain);
    char cipher[len];

    // Cipherise
    printf("ciphertext: ");
    for (int i = 0; i < len; i++)
    {
        if (isalpha(plain[i]))
        {
            cipher[i] = subst(plain[i]);
            printf("%c", cipher[i]);
        }
        else
        {
            cipher[i] = plain[i];
            printf("%c", cipher[i]);
        }
    }

    // Output
    printf("\n");
    return 0;
}