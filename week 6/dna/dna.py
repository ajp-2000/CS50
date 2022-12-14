import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit()

    # Read database file into a variable
    database = load_database(sys.argv[1])

    # Read DNA sequence file into a variable
    f = open(sys.argv[2])
    seq = f.readline().strip()

    # Find longest match of each STR in DNA sequence
    longests = []
    for curr_str in database["strs"]:
        longests.append(longest_match(seq, curr_str))

    # Check database for matching profiles
    matched = False
    for person in database:
        # Compare this person's list to longests
        if person != "strs":
            i = 0
            for length in database[person]:
                #print(f"{length}, {longests[i]},  {i}")
                if int(length) != longests[i]:
                    break

                i += 1

            if i == len(longests):
                print(person)
                matched = True

    if not matched:
        print("No match")

    return


def load_database(filename):
    """Returns a dictionary loaded from the .csv at filename"""
    f = open(filename)
    reader = csv.reader(f)
    dict = {}

    for row in reader:
        if "name" in row:
            dict["strs"] = row[1:]
        else:
            dict[row[0]] = row[1:]

    return dict


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
