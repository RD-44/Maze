#include "graphics.h"
#include "maze_creation.h"
#include "stack.h"
#include <math.h>
#include <stdlib.h>
#include <time.h> 

static int x[3], y[3]; //Arrays for robot triangle's coordinates.
static int dir = 0; //Current direction of robot, as an integer multiple of pi/2 clockwise from the forward (right) direction.
static int cx = 0, cy = 0; //Coordinates of robot centre, to rotate robot about its centre.
static int brain[MAZESIZE][MAZESIZE] = {0}; //Robot's image of the maze.
static int squareSize = WINSIDE/MAZESIZE;

//Below are exact trig functions (cos Exact and sin Exact) designed to return exact values for integer multiples of pi/2, to overcome floating point accuracy errors when drawing.
static int cosE(int x){ //x is an integer multiple of pi/2.
    switch(abs(x) % 4){ //cos(x) is an even function so we can take the absolute value to deal with negative arguments. Mod 4 as cos has a period of 2pi = 4(pi/2).
        case 0: //cos(0) = 1
            return 1; break;
        case 1: //cos(pi/2) = 0
            return 0; break;
        case 2: //cos(2pi/2) = -1
            return -1; break; 
        case 3: //cos(3pi/2) = 0
            return 0;  break;
    }
}
static int sinE(int x){
    return cosE(1-x); //As sin(x) = cos(pi/2 - x) we can use the cosE function with a simple phase shift.
}

static void drawRobot(int sleepTime){ 
    foreground();
    setColour(green);
    clear();
    fillPolygon(3, x, y); 
    sleep(sleepTime);
}

static void spawn(){ //Used to initially display robot in starting position.
    x[0] = squareSize; x[1] = 0; x[2] = 0;
    y[0] = 1.5*squareSize; y[1] = squareSize; y[2] = 2*squareSize;
    cx = squareSize/2; cy = 1.5*squareSize; // Centre of the square enclosing the triangle.
    drawRobot(1);
}

static void rotate(int n){ //Rotates direction clockwise by n multiples of 90 degrees.
    if(n%4 == 0){return;} //A multiple of 4 * pi/2 corresponds to full rotations which do not need to be animated.
    if(n%2 == 0){n=2;} //An even number of rotations can be simplified to a single 180 degree rotation so n = 2.
    if(n%2 != 0){n = sinE(n);} //When n > 2 or n < -2 this ensures -2 <= n <= 2 to avoid rotatng more than 360 degrees at a time.
    dir = (dir + n) % 4; //Updates current direction (note that this is periodic every 4 90 degree rotations hence the mod 4).
    int a[3], b[3];
    for(int u = 0; u < 3; u++){ a[u] = x[u]; b[u] = y[u];} //Copy arrays x and y into a and b.
    for(int j = 0; j < abs(90*n); j+=2){ //Absolute value to ensure loop works for negative n. Loop goes one degree at a time from 0 to 90 to animate the rotation of triangle about its square's centre.
        for (int i = 0; i < 3; i++){
            //Coordinates of triangle are rotated about centre using matrix transformation.
            x[i] = (a[i]-cx)*cos(j*M_PI/180 * n/abs(n)) + (cy - b[i])*sin(j*M_PI/180 * n/abs(n)) + cx; //Inputs to sin and cos are signed depending on the sign of n to ensure rotation in correct direction.
            y[i] = (a[i]-cx)*sin(j*M_PI/180 * n/abs(n)) + (b[i] - cy)*cos(j*M_PI/180 * n/abs(n)) + cy;
        }
        drawRobot(1);
    }
    for (int i = 0; i < 3; i++){
        x[i] = (a[i]-cx)*cosE(n) + (cy - b[i])*sinE(n) + cx; //Final state of robot after transition. 
        y[i] = (a[i]-cx)*sinE(n) + (b[i] - cy)*cosE(n) + cy;
    }
    drawRobot(1);
}

static void move(int way){ //Move robot one unit in the direction given by way (way is an integer multiple of pi/2 radians, angle measured clockwise from forward direction).
    foreground();
    rotate(way-dir); //Rotates robot from its current direction (dir) to face the required direction (way).
    for (int j = 0; j < squareSize; j++){ 
        for(int i = 0; i < 3; i++){
            x[i] += cosE(way); y[i] += sinE(way);
        } 
        drawRobot(2);
    } 
    cx+=squareSize*cosE(way); cy+=squareSize*sinE(way); //Update centre coordinates of robot.
}

static int moves(int x, int y, int* opt){ //Stores possible neighbours to visit in each direction from the current position in the array given by opt pointer.
    int numMoves = 0; 
    if((grid[x+1][y] == path || grid[x+1][y] == end) && x+1 <= MAZESIZE-1 && brain[x+1][y] != visited){
        *(opt + down) = 1; numMoves++;
    }
    if ((grid[x-1][y] == path || grid[x-1][y] == end) && x-1 >= 0 && brain[x-1][y] != visited){ 
        *(opt + up) = 1; numMoves++;
    }
    if ((grid[x][y+1] == path || grid[x][y+1] == end) && y+1 <= MAZESIZE-1 && brain[x][y+1] != visited){
        *(opt + forwards) = 1; numMoves++;
    }
    if ((grid[x][y-1] == path || grid[x][y-1] == end) && y-1 >= 0 && brain[x][y-1] != visited){ 
        *(opt + backwards) = 1; numMoves++;
    } 
    return numMoves; //Returns the number of possible moves.
}

static void solve(int x, int y, int way){ //Recursive depth first searcher, check random routes until dead end then backtrack. 
    brain[x][y] = visited; //Marks current position in robot's image of the maze as visited.
    int options[4] = {0}; //Array to store options for which neighbours to visit.
    int numMoves = moves(x, y, options); //Used to store number of possible moves, and also options array will now contain possible moves.
    if(numMoves == 0){
        if(grid[x][y] == end){brain[x][y] = end;} //Recursion terminates here once robot sees the end.
        else{ 
            move(way+2); //Adding pi to the direction causes the robot to move in the opposite direction it came in, allowing the backtrack to be animated.
            point next = pop();
            solve(next.x, next.y, next.prev); //Stack pop then recurse to previous visited position.
        }
    }else{
        push(x, y, way); 
        switch(moveFinder(rand() % numMoves, options)){ //Randomly chooses a nseighbour to visit and adjusts x, y to get this neighbour's position. 
            case backwards: y--; way = backwards; move(way); break;
            case forwards:  y++; way = forwards;  move(way); break;
            case up:        x--; way = up;        move(way); break;
            case down:      x++; way = down;      move(way); break;
        }
        solve(x, y, way); //Recurses to chosen neighbour after setting current position to a path.
    }   
}

void robot(){
    createStack(MAZESIZE*MAZESIZE);
    srand(time(NULL)); //Seed for randomness
    spawn();
    solve(1, 0, forwards); //Start robot from starting square in maze at (1, 0) with initial direction forwards.
    clearStack();
}


