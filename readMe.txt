Description of program:

My program is able to generate random mazes and solve them. Both the algorithms to generates and solve the maze are based on a 
depth first search, so I created a stack file with pop, push operations. This stack uses dynamically allocated memory 
(which I made sure to free after use!). The stack also stores a struct called point which makes adding and removing (x,y) 
coordinates from the stack simple. I also have a file for maze generation and one for maze solving. The maze solving file is linked 
to the maze generation as some functions in maze generation can be reused for solving due to their similarity. I have utilised 
enum (in the maze_creation header file) to refer to directions and maze items in a meaningful way rather than simply using integers 
to represent different things. To make my solution unique I have animated the maze generation process which has its own loading bar
at the bottom of the window. The maze generation also displays random shades of blue for the paths and random shades of red for the walls. 
Also the robot's animations are smooth as I used matrix transformations to animate the rotations of the robot's sprite. 
Backtracking animations are also shown so the robot never just jumps to a previous stage. I have minimised the use of global variables
using pointers to pass data between functions. I only really needed a global variable for the grid array as this needed to be easily 
accessible across the robot and maze_creation files. I also made global variables for the maze size and window side as I only need 
to change these values for their value across the entire program to be changed, allowing me to easily generate mazes of different sizes
by changing a single value.

Picture of maze generation: https://drive.google.com/file/d/1mh_voYGYIyv1G59ufu4q7psnhWBnOikD/view?usp=sharing
Picture of maze solving: https://drive.google.com/file/d/1k8DRdVyvGVXyL4CWNzBNbPz9zM8ft4HM/view?usp=share_link

Commands to run program and requires libraries: 

gcc -o maze main.c robot.c maze_creation.c graphics.c stack.c
.\maze | java -jar drawapp-2.0.jar

Requirements to run program:

- gcc compiler
- java SDK
- drawapp-2.0.jar, main.c, graphics.c, graphics.h, maze_creation.c, maze_creation.h, robot.c, robot.h, stack.c, stack.h       (obviously)