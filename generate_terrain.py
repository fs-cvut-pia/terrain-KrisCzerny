import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import noise
import random

seed = random.randint(0,100)
plt.rcParams["figure.figsize"] = (20,20)

# Parameters for terrain generation
width = 256
height = 256
scale = 0.02  # Adjust this for different terrain scales
octaves = 6
persistence = 0.5

# Initialize an empty 3D array to store terrain data
terrain = np.zeros((height, width))

# Generate terrain using Perlin noise
for y in range(height):
    for x in range(width):
        value = noise.pnoise2(x * scale, y * scale, octaves=octaves, persistence=persistence,base=seed)# + noise.pnoise2(x * scale, y * scale, octaves=octaves, persistence=0.6,base=seed)
        terrain[y][x] = int(3500*(max(value,-0.01)+0.01)-300)

# Create a 3D visualization of the terrain
x_coords, y_coords = np.meshgrid(np.arange(width), np.arange(height))
fig = plt.figure()
ax = fig.add_subplot(111)
terrain_plot = ax.imshow(terrain, cmap='terrain')
ax.contour(terrain,[0])

ax.set_aspect('equal')
ax.set_xlabel('X')
ax.set_ylabel('Y')

plt.colorbar(terrain_plot)
plt.show()

f = open("terrain_new.dat",'w')
for i in range(height):
    for j in range(width):
        f.write(str(int(terrain[i][j])) + " ")
    f.write("\n")
f.close()
