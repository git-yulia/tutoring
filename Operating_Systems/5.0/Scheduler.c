/**
* Name:      Julia Maliauka
* Lab/task:  Lab 5 
* Task Name: CPU Scheduler
**/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAX_NUMBER_OF_PROCESSES 15 

int FCFS(int *jobQueue, int *currentProcess, int *lastProcess); 
int RR(int *jobQueue, int *currentProcess, int *lastProcess);
int SJF(int *jobQueue, int *currentProcess, int *lastProcess); 
int SRTF(int *jobQueue, int *currentProcess, int *lastProcess);
void errorMessage(int errorType); 
	
int main (int argumentCount, char *argumentValue[]) 
{
	// Variables we will use to keep track of time and attributes
	int currentTimeTick = 0; 
	int nextArrivalTime = 0; 
	int burstTime = 0; 
	int currentProcess = 0; 
	int timeQuantum = 0; 
	char *algorithmChoice = (char*) malloc(5 * sizeof(char));

	// These are the queues we will need to keep track of as we schedule. 
	int jobQueue[MAX_NUMBER_OF_PROCESSES];    
	int waitingTimes[MAX_NUMBER_OF_PROCESSES];  

	// Variables for reading from the input file 
	FILE *inputFile; 
	char *nextWord = (char*) malloc(3 * sizeof(char));
	inputFile = fopen(argumentValue[1], "r");

	// Assign the scheduling algorithm to be used
	if (fscanf(inputFile, "%s", algorithmChoice) != EOF) 
	{
		printf("You chose the %s method.\n", algorithmChoice);
		// If the user chose RR, they must also provide a time quantum. 
		if (strcmp(algorithmChoice, "RR") == 0) 
		{
			fscanf(inputFile, "%d", &timeQuantum);
			printf("Time quantum is %d.\n", timeQuantum);
		}
	} 
	else 
	{
		errorMessage(0); // not enough arguments
	}

	// Initialize the job queue with zeroes 
	for (int index = 0; index < MAX_NUMBER_OF_PROCESSES; index++)
	{
		jobQueue[index] = 0; 
	}

	// Initialize the job queue with zeroes 
	for (int index = 0; index < MAX_NUMBER_OF_PROCESSES; index++)
	{
		waitingTimes[index] = 0; 
	}

	printf("\n_________________%s SCHEDULING_______________________\n", algorithmChoice);

	// Per time tick, schedule processes and report any changes 
	if (strcmp(algorithmChoice, "FCFS") == 0) 
	{
		fscanf(inputFile, "%s", nextWord); // discard "P1"
		fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival time 
		int lastProcess = 0; 
		int processCount = 0; 
		int end = 1; // our end flag
		int countDown = timeQuantum; // count down from quantum 

		while (end == 1) // Our end signal, jobQueue[0] != 100
		{
			printf("\nT%d:\n", currentTimeTick);

			if (nextArrivalTime == currentTimeTick) 
			{
				// Add the next process to the queue 
				fscanf(inputFile, "%d", &burstTime);
				jobQueue[lastProcess] = burstTime;
				waitingTimes[lastProcess] = 1; // for wait times
				lastProcess++; 
				processCount++; 

				// Get the next arrival 
				fscanf(inputFile, "%s", nextWord); // discard "P1"
				fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival 
			}

			printf("CPU: P%d(%d)\n", (currentProcess + 1), jobQueue[currentProcess]);
			printf("QUEUE: ");
			int span = 1; 
			while (span <= lastProcess)
			{
				printf("P%d(%d) ", span, jobQueue[span - 1]);
				span++; 
			}

			int result = FCFS(jobQueue, &currentProcess, &lastProcess); 
			if (result == 10)
			{
				currentProcess++; 
			}
			else if (result == 100)
			{
				end = 0;
			}

			// Increment waiting time for all elements of the queue
			int counter = 0; 
			while (counter < MAX_NUMBER_OF_PROCESSES)
			{
				if (waitingTimes[counter] > 0)
				{
					waitingTimes[counter]++; 
				}
				counter++; 
			}

			printf("\n____________________________________________\n");
			currentTimeTick++; 
		}

		// compute and report average waiting time
		int counter = 0; 
		int sum = 0; 
		int averageWaitingTime = 0; 
		while (counter < MAX_NUMBER_OF_PROCESSES)
		{
			if (waitingTimes[counter] > 0)
			{
				sum += (waitingTimes[counter] - 1); 
			}
			counter++; 
		}

		averageWaitingTime = sum / processCount; 
		printf("Average waiting time was %d\n", averageWaitingTime);

	}
	else if (strcmp(algorithmChoice, "RR") == 0)
	{
		fscanf(inputFile, "%s", nextWord); // discard "P1"
		fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival time 
		int lastProcess = 0; 
		int processCount = 0; 
		int countDown = timeQuantum; // count down from quantum 

		while (currentTimeTick < 15) // Our end signal, jobQueue[0] != 100
		{
			printf("\nT%d:\n", currentTimeTick);

			if (nextArrivalTime == currentTimeTick) 
			{
				// Add the next process to the queue 
				fscanf(inputFile, "%d", &burstTime);
				jobQueue[lastProcess] = burstTime;
				waitingTimes[lastProcess] = 1; // for wait times
				lastProcess++; 
				processCount++; 

				// Get the next arrival 
				fscanf(inputFile, "%s", nextWord); // discard "P1"
				fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival 
			}

			printf("CPU: P%d(%d)\n", (currentProcess + 1), jobQueue[currentProcess]);
			printf("QUEUE: ");
			int span = 1; 
			while (span <= lastProcess)
			{
				printf("P%d(%d) ", span, jobQueue[span - 1]);
				span++; 
			}

			if (countDown > 1)
			{
				RR(jobQueue, &currentProcess, &lastProcess);
				countDown--; 
			}
			else 
			{
				RR(jobQueue, &currentProcess, &lastProcess);
				countDown = timeQuantum; 

				if (currentProcess == lastProcess)
				{
					currentProcess = 0; 
				}
				else 
				{
					currentProcess++; 
				}
			}

			// Increment waiting time for all elements of the queue
			int counter = 0; 
			while (counter < MAX_NUMBER_OF_PROCESSES)
			{
				if (waitingTimes[counter] > 0)
				{
					waitingTimes[counter]++; 
				}
				counter++; 
			}

			printf("\n____________________________________________\n");
			currentTimeTick++; 

			
			}

			// compute and report average waiting time
			int counter = 0; 
			int sum = 0; 
			int averageWaitingTime = 0; 
			while (counter < MAX_NUMBER_OF_PROCESSES)
			{
				if (waitingTimes[counter] > 0)
				{
					sum += (waitingTimes[counter] - 1); 
				}
				counter++; 
			}

			averageWaitingTime = sum / processCount; 
			printf("Average waiting time was %d\n", averageWaitingTime);
	}
	else if (strcmp(algorithmChoice, "SJF") == 0)
	{
		fscanf(inputFile, "%s", nextWord); // discard "P1"
		fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival time 
		int lastProcess = 0; 
		int end = 1; // our end flag
		int countDown = timeQuantum; // count down from quantum 

		while (end == 1) 
		{
			printf("\nT%d:\n", currentTimeTick);

			if (nextArrivalTime == currentTimeTick) 
			{
				// Add the next process to the queue 
				fscanf(inputFile, "%d", &burstTime);
				jobQueue[lastProcess] = burstTime;
				waitingTimes[lastProcess] = 1; // for wait times
				lastProcess++; 

				// Get the next arrival 
				fscanf(inputFile, "%s", nextWord); // discard "P1"
				fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival 
			}

			printf("CPU: P%d(%d)\n", (currentProcess + 1), jobQueue[currentProcess]);
			printf("QUEUE: ");
			int span = 1; 
			while (span <= lastProcess)
			{
				printf("P%d(%d) ", span, jobQueue[span - 1]);
				span++; 
			}

			int result = SJF(jobQueue, &currentProcess, &lastProcess); 
			if (result == 10)
			{
				currentProcess++; 
			}
			else if (result == 100)
			{
				end = 0;
			}

			printf("\n____________________________________________\n");
			currentTimeTick++; 
		}
	}
	else if (strcmp(algorithmChoice, "SRTF") == 0)
	{
		fscanf(inputFile, "%s", nextWord); // discard "P1"
		fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival time 
		int lastProcess = 0; 
		int end = 1; // our end flag
		int countDown = timeQuantum; // count down from quantum 

		while (end == 1) 
		{
			printf("\nT%d:\n", currentTimeTick);

			if (nextArrivalTime == currentTimeTick) 
			{
				// Add the next process to the queue 
				fscanf(inputFile, "%d", &burstTime);
				jobQueue[lastProcess] = burstTime;
				waitingTimes[lastProcess] = 1; // for wait times
				lastProcess++; 

				// Get the next arrival 
				fscanf(inputFile, "%s", nextWord); // discard "P1"
				fscanf(inputFile, "%d", &nextArrivalTime); // Grab the arrival 
			}

			printf("CPU: P%d(%d)\n", (currentProcess + 1), jobQueue[currentProcess]);
			printf("QUEUE: ");
			int span = 1; 
			while (span <= lastProcess)
			{
				printf("P%d(%d) ", span, jobQueue[span - 1]);
				span++; 
			}

			int result = SRTF(jobQueue, &currentProcess, &lastProcess); 
			if (result == 10)
			{
				currentProcess++; 
			}
			else if (result == 100)
			{
				end = 0;
			}

			printf("\n____________________________________________\n");
			currentTimeTick++; 
		}
	}
	else 
	{
		errorMessage(1); 
	}

	return 0; 
}

int FCFS(int *jobQueue, int *currentProcess, int *lastProcess)
{
	int result = 0; // Returns 0 for no change, 10 for updated queue

	if (jobQueue[*currentProcess] != 0)
	{
		jobQueue[*currentProcess] = jobQueue[*currentProcess] - 1; 
	}
	else 
	{
		result = 10; 

		if (*currentProcess == *lastProcess)
		{
			result = 100; // done with all processes
		}
	}

	return result; 
}

int RR(int *jobQueue, int *currentProcess, int *lastProcess)
{
	int result = 0; // Returns 0 for no change, 10 for updated queue

	if (jobQueue[*currentProcess] != 0)
	{
		jobQueue[*currentProcess] = jobQueue[*currentProcess] - 1; 
	}

	return result; 
}

int SJF(int *jobQueue, int *currentProcess, int *lastProcess)
{
	int result = 0; // Returns 0 for no change, 10 for updated queue
	int index = 0; 
	int smallestLocation = 0; 
	int smallestBurstTime = 55; // Arbitrarily chosen large number

	if (jobQueue[*currentProcess] == 0)
	{
		// Find the shortest job to work on 
		while (index < MAX_NUMBER_OF_PROCESSES) 
		{
			if ((jobQueue[index] < smallestBurstTime) && (jobQueue[index] != 0))
			{
				smallestBurstTime = jobQueue[index];
				smallestLocation = index;  
			}
			index++; 
		}

		*currentProcess = smallestLocation; 

		// Could not find a non-zero burst time left! 
		if (smallestBurstTime == 55)
		{
			result = 100; 
		}
	}
	else 
	{
		jobQueue[*currentProcess]--; // decrement the work left to be done
	}

	return result; 
}

int SRTF(int *jobQueue, int *currentProcess, int *lastProcess)
{
	int result = 0; // Returns 0 for no change, 10 for updated queue
	int index = 0; 
	int smallestLocation = 0; 
	int smallestBurstTime = 55; // Arbitrarily chosen large number

		// Find the shortest job to work on (in terms of remaining time)
		while (index < MAX_NUMBER_OF_PROCESSES) 
		{
			if ((jobQueue[index] < smallestBurstTime) && (jobQueue[index] != 0))
			{
				smallestBurstTime = jobQueue[index];
				smallestLocation = index;  
			}
			index++; 
		}

		*currentProcess = smallestLocation; 

		// Could not find a non-zero burst time left! 
		if (smallestBurstTime == 55)
		{
			result = 100; 
		}
		jobQueue[*currentProcess]--; // decrement the work left to be done
	

	return result; 
}

void errorMessage(int errorType)
{
	switch (errorType)
	{
		case 0: 
			printf("Not enough arguments in input. Exiting.\n"); 
			exit(-1);
			break; 
		case 1: 
			printf("Unrecognized algorithm specified. Exiting.\n");
			exit(-1); 
			break; 
		default: 
			printf("Unknown error. Exiting.\n"); 
			exit(-1); 
	}
}