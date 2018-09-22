/**
* Name: Julia Maliauka
* Lab/task: Lab 4 Task 2
* Date: 02/17/18
**/

#define MAX_SIZE 1024

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h> 

int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];
int m, k, n = 0;

typedef struct {
	int row; 
	int col; 
}
Vertex; 

void *matrixThread(void *cellIndex);
int  loadMatrices(char *fileName);
void loadMatrix(FILE *file, int m[][MAX_SIZE], int rows, int cols);
void multiply(int a[][MAX_SIZE], int b[][MAX_SIZE], int c[][MAX_SIZE], int m, int k, int n);
void displayMatrix(int m[][MAX_SIZE], int rows, int cols);
void errorMessage(); 

int main(int argumentCount, char *argumentValues[]) {
	// Load the matrices from a user-specified file. 
	if (loadMatrices(argumentValues[1]) != 0) {
		errorMessage(); 
	}

	// Show the user the matrices that we will multiply. 
	displayMatrix(a, m, n);
	displayMatrix(b, n, k);

	// Perform the calculations to multiply a and b using threads. 
	multiply(a, b, c, m, k, n);

	// Show the user the final result. 
	displayMatrix(c, m, k);

	return 0; 
}

void *matrixThread(void *cellIndex) {
	Vertex *vertex = cellIndex;
	int product = 0; 

	// Multiply and add the products of previous indices.
	for (int currentIndex = 0; currentIndex < n; currentIndex++) {
		product += (a[vertex->row][currentIndex] * b[currentIndex][vertex->col]); 
	}

	// Place the new number in our product matrix c. 
	c[vertex->row][vertex->col] = product;
}

/* Load the matrices to multiply from an external file */ 
int loadMatrices(char *fileName) {
	FILE *filePointer; 
	int nextNumber; 
	char buffer[1]; 

	// Check to make sure file exits, or return -1. 
	if (!(filePointer = fopen(fileName, "r"))) {
		return(-1);
	}

	// Before we can load the matrices, we need to know their sizes. 
	if ((buffer[0] = fgetc(filePointer)) != EOF) { 
		m = atoi(buffer);
		fgetc(filePointer); 
	}

	if ((buffer[0] = fgetc(filePointer)) != EOF) { 
		n = atoi(buffer);
		fgetc(filePointer); 
	}

	if ((buffer[0] = fgetc(filePointer)) != EOF) { 
		k = atoi(buffer);
		fgetc(filePointer); 
	}

	// Move on to the next line so we can look at the matrices. 
	fgetc(filePointer); 

	// Using the acquired constraints, we create both matrices. 
	loadMatrix(filePointer, a, m, n);
	loadMatrix(filePointer, b, n, k);

	// Close the file and return to main with a success signal. 
	fclose(filePointer);
	return(0); 
}

/* Scan the open file to build our matrices */ 
void loadMatrix(FILE *file, int m[][MAX_SIZE], int rows, int cols) {
	char buffer[1]; 
	int nextNumber; 

	for (int currentRow = 0; currentRow < rows; currentRow++) {
		for (int currentCol = 0; currentCol < cols; currentCol++) {
			buffer[0] = fgetc(file);
			nextNumber = atoi(buffer); 

			// Assign the contents of this spot in our matrix 
			m[currentRow][currentCol] = nextNumber; 

			// skip the space in between digits in the file 
			fgetc(file);
		}
		// skip the newline 
		fgetc(file); 
	}
}

/* Multiply the two matrices using threads */ 
void multiply(int a[][MAX_SIZE], int b[][MAX_SIZE], int c[][MAX_SIZE], int m, int k, int n) {
	for (int row = 0; row < m; row++) {
		for (int col = 0; col < k; col++) {
			Vertex *vertex = malloc(sizeof(Vertex));

			if (vertex == NULL) {
				printf("Malloc failed.\n");
				exit(-1);  
			}

			// Assign the rows and columns we will work with to the vertex struct.
			vertex->row = row;
			vertex->col = col;

			pthread_t tid;
			// Now we can create our thread to calculate the product. 
			if (pthread_create(&tid, NULL, matrixThread, vertex)) {
				printf("Thread creation failed.\n");
				exit(-1);
			}

			// Run the calculation by calling the threaded function.
			matrixThread(vertex);

			// Free up the space used to malloc the vertex. 
			free(vertex);
		}
	}
}

/* Prints the size and contents of the matrix */ 
void displayMatrix(int m[][MAX_SIZE], int rows, int cols) {
	printf("This matrix has %d rows and %d columns.\n", rows, cols);

	for (int currentRow = 0; currentRow < rows; currentRow++) {
		for (int currentCol = 0; currentCol < cols; currentCol++) {
			printf(" %d ", m[currentRow][currentCol]);
		}
		printf("\n");
	}
}

/* Inform the user of the program of the crash. */
void errorMessage() {
	printf("Incorrect file or format. Please rerun.\n");
	exit(-1); 
}