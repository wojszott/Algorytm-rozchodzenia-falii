import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Read data from the file
try:
    with open("Frames.txt", "r") as file:
        lines = file.readlines()
except FileNotFoundError:
    print("File not found. Please make sure the file exists.")
    exit()

# Parse the size of the array
if lines:
    try:
        size = int(lines[0])
    except ValueError:
        print("Error parsing the size of the array. Please check the format of the file.")
        exit()
else:
    print("No data found in the file. Please check the file contents.")
    exit()

# Parse the data
frames = []
for line in lines[1:]:
    if line.strip():
        row = [float(val) for val in line.split()]
        frames.append(row)

# Convert data to numpy array and reshape
frames = np.array(frames).reshape(-1, size, size)

# Check the number of frames and the size of the array
print("Number of frames:", len(frames))
print("Size of array:", frames.shape[1:])

if len(frames) == 0:
    print("No frames found. Please check the data file.")
else:
    # Create a figure and axis
    fig, ax = plt.subplots()

    # Function to update the plot for each frame
    def update(frame):
        ax.clear()
        ax.imshow(frame, cmap='viridis', interpolation='nearest')  # Change the colormap here
        ax.set_title('Frame {}'.format(update.frame_number))
        update.frame_number += 1

    update.frame_number = 1

    # Create the animation with repeat=True
    ani = animation.FuncAnimation(fig, update, frames=frames, repeat=True, interval=200)

    plt.show()
