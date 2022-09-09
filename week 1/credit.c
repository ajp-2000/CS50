#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Return the nth-to-last digit of num, starting with 0 as the last digit
int get_digit(long num, int n)
{
    // Find the number of digits of num
    int digits = 0;
    long curr_size = 1;
    do
    {
        curr_size *= 10;
        digits++;
    }
    while (curr_size < num);

    // Cycle through digits last to first
    if (n > digits)
    {
        return 0;
    }

    long unit = 10;
    long stub = 0;
    for (int d = 0; d < n - 1; d++)
    {
        unit *= 10;
    }
    stub = num % unit;

    // Make use of truncation
    long surplus = 1;
    for (int i = 0; i < n - 1; i++)
    {
        surplus *= 10;
    }


    return (int)(stub / surplus);
}

// Return a if one digit, else the sum of a's two digits
int pair_digits(int a)
{
    if (a < 10)
    {
        return a;
    }
    else
    {
        int sum = a % 10;
        sum += 1;
        return sum;
    }
}

int main(void)
{
    // Get the number
    int digits = 0;
    long num;
    string card = "";

    while (digits == 0)
    {
        num = get_long("Number: ");

        if (num >= 1000000000000 && num <= 9999999999999)
        {
            // 13 digits, hence Visa
            card = "VISA";
            digits = 13;
        }
        else if (num >= 100000000000000 && num <= 999999999999999)
        {
            // 15 digits, AmEx
            int first_two = (num - (num % 10000000000000)) / 10000000000000;
            if (first_two == 34 || first_two == 37)
            {
                card = "AMEX";
                digits = 15;
            }
        }
        else if (num >= 1000000000000000 && num <= 9999999999999999)
        {
            // Either MasterCard or Visa
            int first_two = (num - (num % 100000000000000)) / 100000000000000;
            if (first_two > 50 && first_two < 56)
            {
                card = "MASTERCARD";
                digits = 16;
            }
            else if (get_digit(num, 16) == 4)
            {
                card = "VISA";
                digits = 16;
            }
        }

        if (digits == 0)
        {
            printf("INVALID\n");
            exit(0);
        }
    }

    // Calculate
    int every_second = 0;                       // Starting with the second-to-right digit
    int every_other = 0;                        // Starting with the rightmost digit
    for (int d = 0; d <= digits; d++)
    {
        // Digits start at 1, not zero, to make the get_digit function work
        if ((d % 2) == 1)
        {
            every_other += get_digit(num, d);
        }
        else if (d != 0)
        {
            every_second += pair_digits(get_digit(num, d) * 2);
        }
    }

    if ((every_other + every_second) % 10 == 0)
    {
        printf("%s\n", card);
    }
    else
    {
        printf("INVALID\n");
    }
}