#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

/*
The InitialiseFloor function initialises the 2D array that represents the floor of the container yard.
It takes in the floor 2d array, an indication of which boundary to have the entry on, and the position of the entry.
From these input parameters, it updates the floor array with the corresponding integers representing the nature of each point on the floor.
*/
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
	int i, j;
	
	// Set all points on the boundary (edges) to the BOUNDARY value
	// Set all other points to the VACANT value

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			if (((i == 0) || (i == (NUM_ROWS - 1))) || ((j == 0) || (j == (NUM_COLS - 1)))) {
				floor[i][j] = BOUNDARY;
			} else {
				floor[i][j] = VACANT;
			}
		}
	}

	// Set the ENTRY and EXIT points based on the entryboundary and index parameters
	// The EXIT point is set directly opposite the ENTRY point

    if (entryboundary == 'T'){
        floor[0][index] = ENTRY;
        floor[NUM_ROWS-1][index] = EXIT;
    } 
    else if (entryboundary == 'R') {
        floor[index][NUM_COLS-1] = ENTRY;
        floor[index][0] = EXIT;
    } 
    else if (entryboundary == 'L') {
        floor[index][0] = ENTRY;
        floor[index][NUM_COLS-1] = EXIT;
    } 
    else if (entryboundary == 'B') {
        floor[NUM_ROWS - 1][index] = ENTRY;
        floor[0][index] = EXIT;
    }
}
/*
The PrintFloor function takes in the initialised floor 2D array, and prints the array in an easily-interpreted format,
with symbols representing each of the types of point on the floor (boundary, vacant, entry, exit).
*/
void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
    int i;
    int j;

	// Check the integer value of each element in the 2D array and use this to print the corresponding symbol
	for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLS; j++)
        {
            if (floor[i][j] == BOUNDARY)
            {
                printf("@");
            } else if (floor[i][j] == VACANT)
            {
                printf(" ");
            } else if (floor[i][j] == ENTRY)
            {
                printf("U");
            } else if (floor[i][j] == EXIT)
            {
                printf("X");
            } else 
			{
				printf("%c", floor[i][j]);
			}
        }
		// Print a new line at the end of each row of the floor array
        printf("\n");
    }
}
/*
The FloorAreaAvailable function takes in the floor 2D array, and values for the length and width of each grid cell.
It returns the value of the amount of free space available on the floor.
*/
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{
	double free = 0;
	int i;
	int j;

	// Calculate the area of each grid cell
	double cellArea = length * width;

	// Check each element in the 2D array (point on the floor) to see if it is vacant
	// If so, add the area of the cell to the total area available
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			if (floor[i][j] == VACANT)
			{
				free += cellArea;
			}
		}
	}
	return free;
}
/*
The AddContainer function is used to create containers and add them to the floor 2D array, if there is space.
Each container is represented by a letter (starting from A, then B, etc).
*/
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction)
{
	char contLet = 'A';
	int i;
	int j;

	int rowPos;
	int colPos;

	int k;

	// Loop over every element in the 2D floor array, to determine which letter to use for this container
	for (i = 1; i < NUM_ROWS - 1; i++)
	{
		for (j = 1; j < NUM_COLS - 1; j++)
		{
			if (floor[i][j] >= contLet)
			{
				contLet = (char)(floor[i][j] + 1);
			}
		}
	}

	// Set the row and column index positions based on the integer position
	rowPos = (position / NUM_COLS);
	colPos = (position % NUM_COLS);
	

	if (direction == 0) // Horizontal
	{
		for (k = 0; k < size; k++)
		{
			if (floor[rowPos][colPos + k] != 0) // If any of the spaces for the container are not vacant, return
			{
				return;
			}
		}
		
		for (k = 0; k < size; k++) // Set each position for the container to the container letter
		{
			floor[rowPos][colPos + k] = contLet;
		}

	} else if (direction == 1) // Vertical
	{
		for (k = 0; k < size; k++)
		{
			if (floor[rowPos + k][colPos] != 0) // If any of the spaces for the container are not vacant, return
			{
				return;
			}
		}

		for (k = 0; k < size; k++) // Set each position for the container to the container letter
		{
			floor[rowPos + k][colPos] = contLet;
		}
	}

	return;
}
/*
The LocateContainer function is used to get the position (row/column index) of a container in the floor, based on the letter of the container.
It also returns an integer based on whether the container is blocked, or if it is free to move in at least one direction.
*/
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd)
{
	int i, j;	
	int k = 0, l = 0;

	*rowStart = 99; // Arbitrary values, used to check if a position has been set yet for the start of the container
	*colStart = 99;
	
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			if ((floor[i][j] == move) && (*rowStart == 99))
			{
				*rowStart = i; // Set the position of the start of the container to the top left position with the container letter
				*colStart = j;
			}
		}
	}

	if (floor[*rowStart + 1][*colStart] == move) // Vertical
	{
		while (floor[*rowStart + k + 1][*colStart] == move) // While still container
		{
			k++; // Used to determine the end position
		}
		*rowEnd = *rowStart + k;
		*colEnd = *colStart;

		if ( (floor[*rowStart - 1][*colStart] == VACANT) || (floor[*rowEnd + 1][*colEnd] == VACANT) ) // If there is free space on either side
		{
			return 1;
		} else {
			return 0;
		}
		
	} 
	
	else // Horizontal
	{
		while (floor[*rowStart][*colStart + l + 1] == move) // While still container
		{
			l++; // Used to determine the end position
		}
		*rowEnd = *rowStart;
		*colEnd = *colStart + l;

		if ( (floor[*rowStart][*colStart - 1] == VACANT) || (floor[*rowEnd][*colEnd + 1] == VACANT) ) // If there is free space on either side
		{
			return 1;
		} else {
			return 0;
		}
	}
	
}
/*
The MoveContainer function is used to shift the containers around the floor (2D array).
Containers are preferrentially moved up or left is there is space, otherwise down or right.
Integers are returned from this function based on what point the container is blocked by after the move.
*/
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{
	int i = 1;

	if (r0 == r1) // Horizontal
	{
		if (isBlocked == 1) // Free to move in at least one direction
		{
			if (floor[r0][c0 - 1] == VACANT) // If free space is to the left
			{
				while (floor[r0][c0 - i] == VACANT) // While space to the left is free
				{
					floor[r0][c0-i] = floor[r0][c0-i+1]; // Set space to left of container to the container letter
					floor[r0][c1-i+1] = VACANT; // Set right end of the container to vacant
					i++;
				}

				if (floor[r0][c0 - i] == ENTRY) // If container reaches entry point
				{
					return 1;
				} else if (floor[r0][c0 - i] == EXIT) // If the container reaches exit point
				{
					return 2;
				} else 
				{
					return 0;
				}

			} else // Free space is to the right
			{
				while (floor[r0][c1 + i] == VACANT) // While space to the right is free
				{
					floor[r0][c1+i] = floor[r0][c1+i-1]; // Set space to right of container to the container letter
					floor[r0][c0+i-1] = VACANT; // Set left end of the container to vacant
					i++;
				}

				if (floor[r0][c1 + i] == ENTRY) // If container reaches entry point
				{
					return 1;
				} else if (floor[r0][c1 + i] == EXIT) // If the container reaches exit point
				{
					return 2;
				} else 
				{
					return 0;
				}
			}

		} else // Blocked
		{
			if (floor[r0][c0-1] == ENTRY || floor[r1][c1+1] == ENTRY) // Next to the entry
			{
				return 1;
			} else if (floor[r0][c0-1] == EXIT || floor[r1][c1+1] == EXIT) // Next to the exit
			{
				return 2;
			} else
			{
				return -1;
			}
		}
	} else // Vertical
	
	{
		if (isBlocked == 1) // Free to move in at least one direction
		{
			if (floor[r0 - 1][c0] == VACANT) // If free space is above
			{
				while (floor[r0 - i][c0] == VACANT) // While space above is free
				{
					floor[r0-i][c0] = floor[r0-i+1][c0]; // Set space above container to container letter
					floor[r1-i+1][c0] = VACANT; // Set bottom of the container to vacant
					i++;
				}

				if (floor[r0-i][c0] == ENTRY) // If container reaches entry point
				{
					return 1;
				} else if (floor[r0-i][c0] == EXIT) // If the container reaches exit point
				{
					return 2;
				} else 
				{
					return 0;
				}

			} else // Free space is below
			{
				while (floor[r1+i][c0] == VACANT) // While space below is free
				{
					floor[r1+i][c0] = floor[r0+i-1][c0]; // Set space below container to the container letter
					floor[r0+i-1][c0] = VACANT; // Set top end of the container to vacant
					i++;
				}

				if (floor[r1+i][c0] == ENTRY) // If container reaches entry point
				{
					return 1;
				} else if (floor[r1+i][c0] == EXIT) // If the container reaches exit point
				{
					return 2;
				} else 
				{
					return 0;
				}
			}
			
		} else // Blocked
		{
			if (floor[r0-1][c0] == ENTRY || floor[r1+1][c1] == ENTRY) // Next to the entry
			{
				return 1;
			} else if (floor[r0-1][c0] == EXIT || floor[r1+1][c1] == EXIT) // Next to the exit
			{
				return 2;
			} else
			{
				return -1;
			}
		}
	}

}


/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

/* The main Container Yard simulation */
int main(void)
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");
	
	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 28, 2, 0);
	AddContainer(floor, 11, 3, 1);
	AddContainer(floor, 41, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 34, 2, 0);
	AddContainer(floor, 36, 3, 1);
	AddContainer(floor, 37, 2, 1);
	AddContainer(floor, 53, 2, 0);
	AddContainer(floor, 30, 3, 1);

	/* Print status */
	printf("Container Yard!  The containers are rushing in!\n");
	printf("In fact, %.2f sq ft of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");

	return 0;
}
