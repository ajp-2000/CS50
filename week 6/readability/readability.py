import re
from cs50 import get_string

# Get text
text = get_string("Text: ")

# Count letters
letters = 0

for ch in text:
    if ch.isalpha():
        letters += 1

# Count words
words = len(text.split())

# Count sentences
sentences = 0

for i in range(0, len(text)):
    if text[i] == "!" or text[i] == "?":
        sentences += 1
    elif text[i] == ".":
        # Make sure we aren't inside an ellipsis
        if i > 0:
            if text[i - 1] != ".":
                sentences += 1

# Calculate
l = letters / (words / 100)
s = sentences / (words / 100)
index = (0.0588 * l) - (0.296 * s) - 15.8
grade = round(index)

# Print
if grade < 1:
    print("Before Grade 1")
elif grade > 16:
    print("Grade 16+")
else:
    print("Grade " + str(grade))