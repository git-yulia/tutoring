/**
* Name: Julia Maliauka
* Project 1, Task 1 and 2
* File: bit_operations.h
* (bit operations involving the bitvector and main_memory array)
* Started on 4/05/18
**/

#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h> 
#include <sys/stat.h> 

#define DATA_SIZE           254 
#define BLOCK_SIZE          256
#define INDEX_SIZE          127
#define SYSTEM_OWNER        362   
#define MAXIMUM_INPUT_SIZE  10   
#define MAX_NAME_LENGTH     128              
#define DEFAULT_PERMISSIONS 0600
#define MAXIMUM_FILE_SIZE   10000
#define MEMORY_SIZE         65536

typedef char data_t;
typedef unsigned short index_t; 

typedef enum
{
   DOC, 
   DIR,         
   INDEX,
   DATA
} NODE_TYPE;

typedef struct file_system_node
{
   char name[MAX_NAME_LENGTH];
   time_t creation_time;       
   time_t access_time;        
   time_t modified_time;      
   mode_t permissions;         
   unsigned short owner_ID;    
   unsigned short node_size;   // size of node in bytes 
   index_t block_ref;          // reference to the data or index block
} FILE_SYSTEM_NODE;

typedef struct node
{
   NODE_TYPE type;

   union
   {
      FILE_SYSTEM_NODE fd;     
      data_t data[DATA_SIZE];
      index_t index[INDEX_SIZE];
   } content;

   index_t parent_directory; 

} NODE;

NODE memory[MEMORY_SIZE];
unsigned char* bitvector;

index_t find_next_open_spot()
{
	index_t index = 0; 

	// Find the first character that isn't completely full (255)
	while (bitvector[index] == 255)
	{
		index++; 
	}

	// Go through bitvector[index] to find the first open spot 
	int bitmask = 128; 
	int bit_offset = 0; 
	while ((bitvector[index] & bitmask) != 0)
	{
		bitmask = bitmask >> 1; 
		bit_offset++; 
	}

	// Convert to the filesystem memory scope (0 to 65,535)
	index = (index * 8) + bit_offset; 

	return index; 
}

void mark_spot_taken_at(index_t directory_index)
{
	int index_to_edit = directory_index / 8; 
	int offset = directory_index % 8; 
	int bitmask = 128; 

	for (int index = 0; index < offset; index++)
	{
		bitmask = bitmask >> 1; 
	}

	bitvector[index_to_edit] = (bitvector[index_to_edit] | bitmask); 
}