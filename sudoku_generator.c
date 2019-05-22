#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Sudoku {
  int grid[9][9];
  int solnGrid[9][9];
  int guessNum[9];
  int gridPos[81];
};
typedef struct Sudoku Sudoku;


int FindUnassignedLocation(int grid[9][9], int *row, int *col) {

  int i = 0, j = 0;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
    *row = i;
    *col = j;
    return 0;
}



int UsedInRow(int grid[9][9], int row, int num) {

    int col = 0;

    for (col = 0; col < 9; col++) {
        if (grid[row][col] == num)
            return 1;
    }

    return 0;
}

int UsedInCol(int grid[9][9], int col, int num) {

    int row = 0;

    for (row = 0; row < 9; row++) {
        if (grid[row][col] == num)
            return 1;
    }

    return 0;
}

int UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num) {

    int row, col;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (grid[row+boxStartRow][col+boxStartCol] == num) {
                return 1;
            }
        }
    }

    return 0;
}


int isSafe(int grid[9][9], int row, int col, int num) {

    if (UsedInRow(grid, row, num)==0 && UsedInCol(grid, col, num)==0 && UsedInBox(grid, row - row%3 , col - col%3, num)==0) {
      return 1;
    } else {
      return 0;
    }
}


int solveGrid(Sudoku *puzzle) {

    int row = 0, col = 0, num = 0;
    if (FindUnassignedLocation(puzzle->grid, &row, &col) == 0) {
       return 1;
    }
    
    for (num = 0; num < 9; num++) {
        
        if (isSafe(puzzle->grid, row, col, puzzle->guessNum[num]) == 1) {
            
            puzzle->grid[row][col] = puzzle->guessNum[num];

            if (solveGrid(puzzle)) {
                return 1;
            } else {
              puzzle->grid[row][col] = 0;
            }

        }
    }

    return 0;

}

void createSeed(Sudoku *puzzle) {

  int i, j;

  solveGrid(puzzle);
  
  for(i=0;i<9;i++) {
    for(j=0;j<9;j++) {
      puzzle->solnGrid[i][j] = puzzle->grid[i][j];
    }
  }
}

void countSoln(Sudoku *puzzle, int *number) {
  int row = 0, col = 0, i = 0;

  if(FindUnassignedLocation(puzzle->grid, &row, &col) == 0) {
    *number++;
    return ;
  }

  for(i=0;i<9 && *number<2;i++) {
      if( isSafe(puzzle->grid, row, col, puzzle->guessNum[i]) ) {
        puzzle->grid[row][col] = puzzle->guessNum[i];
        countSoln(puzzle, number);
      }

      puzzle->grid[row][col] = 0;
  }

}

void genPuzzle(Sudoku *puzzle) {
  int i, x, y, temp, check;

  for(i=0;i<81;i++) {
    x = (puzzle->gridPos[i])/9;
    y = (puzzle->gridPos[i])%9;
    temp = puzzle->grid[x][y];
    puzzle->grid[x][y] = 0;

    check=0;
    countSoln(puzzle, &check);
    if(check!=1) {
      puzzle->grid[x][y] = temp;
    }
  }
}

void swap(int *list, int i, int j) {
  int temp = list[i];
  list[i] = list[j];
  list[j] = temp;
}

void shuffle(int *list, int n) {
  int i = 0, j = 0;
  for ( i = n - 1; i >= 1; i--) {
    j = rand() % (i + 1);

    swap(list, i, j);
  }
}

void initializePuzzle(Sudoku *puzzle) {
  int i, j;

  for(i=0;i<81;i++) {
    puzzle->gridPos[i] = i;
  }

  shuffle(puzzle->gridPos, 81);

  for(i=0;i<9;i++) {
    puzzle->guessNum[i]=i+1;
  }

  shuffle(puzzle->guessNum, 9);

  for (i=0; i<9; i++) {
    for (j=0; j<9; j++) {
      puzzle->grid[i][j] = 0;
    }
  }

}

int randomPos() {
	return rand() % 9 + 0;
}

void printGame(int game[9][9]) {
	int i, j, lCount, cCount;

	lCount = 0;
	for (i=0; i<9; i++) {
		if (lCount > 2) {
			printf(" ---------------------------\n");
			lCount = 0;
		}
		cCount = 0;
   		for (j=0; j<9; j++) {
   			if (cCount > 2) {
				printf("|");
				cCount = 0;
			}
   			if(game[i][j] == 0) {
   				printf(" . ");
   			}else {
   				printf(" %d ", game[i][j]);
   			}
   			cCount++;
   		}
   		printf("\n");
   		lCount++;
   	}
   	printf("\n");
}

void persistGame(int game[9][9], char *fileName) {
   	int i, j, lCount, cCount;

	FILE *fp = fopen(fileName,"w+");
	
	lCount = 0;
	for (i=0; i<9; i++) {
		if (lCount > 2) {
			fprintf(fp, " ---------------------------\n");
			lCount = 0;
		}
		cCount = 0;
   		for (j=0; j<9; j++) {
   			if (cCount > 2) {
				fprintf(fp, "|");
				cCount = 0;
			}
   			if(game[i][j] == 0) {
   				fprintf(fp, " . ");
   			}else {
   				fprintf(fp, " %d ", game[i][j]);
   			}
   			cCount++;
   		}
   		fprintf(fp, "\n");
   		lCount++;
   	}

   	fclose (fp);
}

void generateBaseGame(Sudoku *puzzle) {

	initializePuzzle(puzzle);
  	createSeed(puzzle);
  	genPuzzle(puzzle);
  	printGame(puzzle->solnGrid);
}

void generate(int originGame[9][9], int off, char *fileName) {
	int i, j, line, col, game[9][9];

	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {
			game[i][j] = originGame[i][j];
		}
	}

	for (i=0; i<off;) {
		line = randomPos();
		col = randomPos();
		if (game[line][col] != 0) {
			game[line][col] = 0;
			i++;
		}
	}

	persistGame(game, fileName);
}

void generateEasyGame(int originGame[9][9], char *fileName) {
	int off = 37;
	generate(originGame, off, fileName);
}

void generateMediumGame(int originGame[9][9], char *fileName) {
	int off = 48;
	generate(originGame, off, fileName);
}

void generateHardGame(int originGame[9][9], char *fileName) {
	int off = 56;
	generate(originGame, off, fileName);
}

int main()
{
	Sudoku puzzle;

   	srand(time(NULL));

   	generateBaseGame(&puzzle);

   	generateEasyGame(puzzle.solnGrid, "easyGame_1.txt");
   	generateEasyGame(puzzle.solnGrid, "easyGame_2.txt");
   	generateEasyGame(puzzle.solnGrid, "easyGame_3.txt");
   	generateMediumGame(puzzle.solnGrid, "mediumGame_1.txt");
   	generateMediumGame(puzzle.solnGrid, "mediumGame_2.txt");
   	generateMediumGame(puzzle.solnGrid, "mediumGame_3.txt");
   	generateHardGame(puzzle.solnGrid, "hardGame_1.txt");
   	generateHardGame(puzzle.solnGrid, "hardGame_2.txt");
   	generateHardGame(puzzle.solnGrid, "hardGame_3.txt");


   	return 0;
}