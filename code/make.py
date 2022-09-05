
# Author Bushui Zhang
# Time: 11/10/2020
import os

command = "gcc -DMACOSX -w -framework OpenGL -framework GLUT -framework CoreFoundation particle_system.c -o particle_system"
os.system(command)
