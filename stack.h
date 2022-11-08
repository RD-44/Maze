typedef struct point{ //point datatype to store (x,y) coordinates in the stack.
    int x;
    int y;
    int prev; //This keeps track of the robots direction when it previously moved to this point, allowing the robot to move in the opposite direction when backtracking. This attribute is unused for maze gen.
}point;

void createStack(int size);
void clearStack();
point pop();
void push(int x, int y, int prev);