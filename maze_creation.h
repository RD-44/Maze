#define MAZESIZE 27 //Maze has dimensions MAZESIZE X MAZESIZE Precondition: MAZESIZE is an ODD number.
#define WINSIDE 648 //Window's side length (is a square). Precondition: WINSIDE is an integer multiple of MAZESIZE.
void maze_creation();
extern int grid[MAZESIZE][MAZESIZE];

enum directions{forwards, down, backwards, up}; //Each index in the options array corresponds to a direction (e.g up means go to above neighbour).
enum items{path, wall, end, visited};  //Used to represent element of the maze grid array.
int moveFinder(int moveNumber, int* opt); //Used to give a valid move to do from the current position in the maze, used by both the solver and generator.

