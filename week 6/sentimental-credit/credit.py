import sys
from cs50 import get_int

# Get number
number = get_int("Number: ")
number_s = str(number)
if number > 0:
    digits = len(number_s)
else:
    sys.exit("INVALID")

# Work out type of card
type = "INVALID"

if digits == 13:
    if number_s[0] == "4":
        type = "VISA"
elif digits == 15:
    if number_s[0] == "3" and (number_s[1] == "4" or number_s[1] == "7"):
        type = "AMEX"
elif digits == 16:
    if number_s[0] == "4":
        type = "VISA"
    elif number_s[0] == "5":
        if number_s[1] == "1" or number_s[1] == "2" or number_s[1] == "3" or number_s[1] == "4" or number_s[1] == "5":
            type = "MASTERCARD"

if type == "INVALID":
    sys.exit("INVALID")

# Checksum
d = digits - 2
sum = 0

while d >= 0:
    temp = int(number_s[d]) * 2
    if temp < 10:
        sum += temp
    else:
        sum += 1
        sum += (temp - 10)

    d -= 2

d = digits - 1
while d >= 0:
    sum += int(number_s[d])
    d -= 2

if sum % 10 == 0:
    print(type)
else:
    print("INVALID")