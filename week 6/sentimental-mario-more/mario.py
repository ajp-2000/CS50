from cs50 import get_int

# Get height
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

# Print layers
for i in range(0, height):
    # Space to the left
    for j in range(1, height-i):
        print(" ", end="")

    # Bricks on the left
    for j in range(height-i, height+1):
        print("#", end="")

    # Gap of two
    print("  ", end="")

    # Bricks on the right
    for j in range(height-i, height+1):
        print("#", end="")

    print("")