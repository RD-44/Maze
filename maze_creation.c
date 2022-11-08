#include "graphics.h"
#include "maze_creation.h"
#include "stack.h"
#include <time.h> 
#include <stdlib.h>

int grid[MAZESIZE][MAZESIZE] = {path}; //Maze grid initially set to all zeroes (paths). Maze is a square matrix.
static int squareSize = WINSIDE/MAZESIZE;

static void initMaze(){ //Setup pregeneration maze with cells bordered by walls.
    for(int i = 0; i < MAZESIZE; i++){ 
        grid[0][i] = wall;
        grid[i][0] = wall;
        grid[MAZESIZE-1][i] = wall;
        grid[i][MAZESIZE-1] = wall;
    }
    for(int i = 1; i < MAZESIZE-1; i++){
        for(int j = 2; j < MAZESIZE-1; j+=2){
            grid[i][j] = wall;
            grid[j][i] = wall;
        }
    }
}

static int moves(int x, int y, int* opt){ 
    int numMoves = 0; //Stores possible neighbours to visit in each direction from the current position.
    if(grid[x+1][y] == wall && grid[x+2][y] == path && x+2 <= MAZESIZE-2){
        *(opt + down) = 1; numMoves++;
    }
    if(grid[x-1][y] == wall && grid[x-2][y] == path && x-2 >= 1){ 
        *(opt + up) = 1; numMoves++;
    }
    if(grid[x][y+1] == wall && grid[x][y+2] == path && y+2 <= MAZESIZE-2){
        *(opt + forwards) = 1; numMoves++;
    }
    if(grid[x][y-1] == wall && grid[x][y-2] == path && y-2 >= 1){ 
        *(opt + backwards) = 1; numMoves++;
    } 
    return numMoves; //Returns the number of possible moves and stores these possible moves in the options array.
}

int moveFinder(int moveNumber, int* opt){ //Takes in the randomly generated number and the options array and gives a valid move to visit the next neighbour.
    int numMovesChecked = 0; 
    for (int move = 0; move < 4; move++){ //Finds the move corresponding to the given number
        if(*opt){
            if(numMovesChecked==moveNumber){
                return move;
            }
            numMovesChecked++;
        }
        opt++;
    }
}

static void colChooser(int squareType){ //Sets colour corresponding to number.
    switch(squareType){ //Colours are random, with path colours as shades of blue and wall colours as shades of red.
        case path:
            setRGBColour(rand() % 10, rand() % 10, 120 + rand() % 135); 
            break;
        case visited: //These are identical to paths, just marked as visited by the maze gen algorithm.
            setRGBColour(rand() % 10, rand() % 10, 120 + rand() % 135); 
            break;
        case wall:
            setRGBColour(120 + rand() % 135, rand() % 20, rand() % 10); 
            break;
        case end:
            setColour(gray); //Ending square is filled gray.
            break;
    }
}

static void drawMaze(int done){ // If maze is drawn during maze generation the visited squares are left unchanged. If drawn after generation these visited squares are reset.
    if(done){setWindowSize(WINSIDE, WINSIDE);} 
    else{setWindowSize(WINSIDE, WINSIDE+squareSize);} //If not done maze gen a space is made below the maze to show a loading bar.
    background();
    int numVisited=0;
    for(int y = 0; y < MAZESIZE; y++){ //Iterate through array and draw up corresponding maze.
        for(int x = 0; x < MAZESIZE; x++){
            if(grid[x][y] == visited){
                if(done){grid[x][y] = path;} //Sets the visited squares back to normal path squares if finished maze gen.
                numVisited++;
            } 
            colChooser(grid[x][y]);
            fillRect( y*squareSize, x*squareSize, squareSize, squareSize);
        }
    }
    setColour(green);
    fillRect(0, WINSIDE, (4*WINSIDE*numVisited)/((MAZESIZE-1)*(MAZESIZE-1)), squareSize); //Draws a loading bar at the bottom of screen.
    sleep(20); 
}

static void mazeGen(int x, int y){
    grid[x][y] = visited; //Mark current position as visited
    drawMaze(0); //Draws maze at each stage of generation along with a loading bar. Comment this out to disable this.
    int options[4] = {0}; //Array to store options for which neighbours to visit.
    int numMoves = moves(x, y, options); //Used to store number of possible moves, and also options array will now contain possible moves.
    if (numMoves == 0){ //Check if no neighbours to visit
        point next = pop();
        if(next.x == -1){ grid[1][0] = path; grid[MAZESIZE-2][MAZESIZE-1] = end; } //Place start and end square if no more places to backtrack to.
        else{ mazeGen(next.x, next.y); } //Recurses back to the previous unvisited spot in the stack. 
    }else{
        push(x, y, 0); //Direction is irrelevant for maze gen so the third argument is always zero.
        int newX, newY;
        switch(moveFinder(rand()%numMoves, options)){ //Randomly chooses a neighbour to visit and adjusts x, y to get this neighbour's position. Position must be only odd coordinates.
            case backwards: y--; newX = x;   newY = y-1; break;
            case forwards:  y++; newX = x;   newY = y+1; break;
            case up:        x--; newX = x-1; newY = y;   break;
            case down:      x++; newX = x+1; newY = y;   break;
        }
        grid[x][y] = path; mazeGen(newX, newY); //Recurses to chosen neighbour after setting current position to a path.
    }
}

void maze_creation(){
    srand(time(NULL)); //Seed for randomness
    createStack(MAZESIZE*MAZESIZE);
    initMaze(); //Puts maze in a state ready for maze generation.
    mazeGen(1,1); //Maze generation, starting from top left square.
    drawMaze(1); //Draws final maze post generation.
    clearStack();
}