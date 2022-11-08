#include "graphics.h"
#include "maze_creation.h" 
#include "robot.h"         

void main(void)
{
    maze_creation(); //Generates and draws up maze.
    robot();         //Solves maze and animates a robot going through the maze.
}