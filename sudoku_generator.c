#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generateSudoku(int n, int m, int mat[n][m]) {
	int i, j, k, r, lineFlag, count, aux, linha, coluna;

	for (i=0; i<n; i++) {
   		for (j=0; j<m; j++) {
	   		r = rand() % 9 + 1;
			mat[i][j] = r;
   		}
   	}

// todo
   	for (i=0; i<n; i++) {
   		for (j=0; j<m; j++) {
   			lineFlag = 0;
   			while (lineFlag==0){
   				aux = 0;
	   			r = rand() % 9 + 1;
	   			for (k=0; k<9; k++) {
	   				if (r == mat[i][k]) {
	   					aux = aux + 1;
	   				}
	   			}
	   			if (aux < 1) {
	   				mat[i][j] = r;
	   				lineFlag = 1;
	   			}
	   			
	   		}
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
	printGame(game);
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
   	int list[9], i, j, r, exemplo[9];

   	int mat[9][9] = { 	{5,3,4,6,7,8,9,1,2},{6,7,2,1,9,5,3,4,8},{1,9,8,3,4,2,5,6,7},
						{8,5,9,7,6,1,4,2,3},{4,2,6,8,5,3,7,9,1},{7,1,3,9,2,4,8,5,6},
						{9,6,1,5,3,7,2,8,4},{2,8,7,4,1,9,6,3,5},{3,4,5,2,8,6,1,7,9}};

   	srand(time(NULL));

   	generateEasyGame(mat, "easyGame_1.txt");
   	generateEasyGame(mat, "easyGame_2.txt");
   	generateEasyGame(mat, "easyGame_3.txt");
   	generateMediumGame(mat, "mediumGame_1.txt");
   	generateMediumGame(mat, "mediumGame_2.txt");
   	generateMediumGame(mat, "mediumGame_3.txt");
   	generateHardGame(mat, "hardGame_1.txt");
   	generateHardGame(mat, "hardGame_2.txt");
   	generateHardGame(mat, "hardGame_3.txt");
   	
   	// generateRandomGame(9,9, mat);

   	return 0;
}