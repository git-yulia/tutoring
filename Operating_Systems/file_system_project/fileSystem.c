/**
* Name: Julia Maliauka
* Project 1, Task 1 and 2
* File: fileSystem.c 
* (the main and file system operations)
* Started on 4/05/18
* Projected completion date: 4/08/19
**/

#include "bit_operations.h"
#include "file_system_operations.h"

// The following variable is global because when we step up 
// 		or down through directories, we need to know where we are
index_t active_directory;

/* For a full list of commands this system accepts, please try
the command 'help' while the system is operational. */ 
int main()
{
	initialize_file_system(); 

	char* command = malloc(sizeof(MAXIMUM_INPUT_SIZE));
	char* name = malloc(sizeof(MAXIMUM_INPUT_SIZE));
	char* file_content = malloc(sizeof(MAXIMUM_FILE_SIZE));
	char temp = '?'; 

	active_directory = 0; // start at root 

	while (printf("\n") && (scanf("%s", command)) && (strcmp(command, "quit")) != 0)
	{
		if (strcmp(command, "mkdir") == 0)
		{
			scanf("%s", name);  
			create_directory(name, 362, active_directory); 
		}
		else if (strcmp(command, "rmdir") == 0)
		{
			scanf("%s", name); 
			remove_directory(name); 
		}
		else if (strcmp(command, "write") == 0)
		{
			scanf("%s", name); 
			printf("> ");
			scanf("%c", &temp); // clear the buffer 
			scanf("%10000[^\n]", file_content);
			create_file(name, SYSTEM_OWNER, file_content, active_directory);
		}
		else if (strcmp(command, "read") == 0)
		{
			scanf("%s", name); 
			read_file(name);
		}
		else if (strcmp(command, "getInfo") == 0)
		{
			scanf("%s", name); 
			get_info_about(name);
		}
		else if (strcmp(command, "open") == 0)
		{
			scanf("%s", name); 
			// ...
		}
		else if (strcmp(command, "close") == 0)
		{
			scanf("%s", name); 
			// ...
		}
		else if (strcmp(command, "rm") == 0)
		{
			scanf("%s", name); 
			delete_file(name);
		}
		else if (strcmp(command, "ls") == 0)
		{
			// Set it up so we traverse from active directory downwards
			display_file_system(active_directory); 
		}
		else if (strcmp(command, "cd") == 0)
		{
			char direction[4]; 
			scanf("%s", direction); 

			if (strcmp("..", direction) == 0)
			{
				if (active_directory != 0)
				{
					active_directory = memory[active_directory].parent_directory; 
				}
				else 
				{
					error_message(6); // can't move up from root
				}
			}
			else 
			{
				index_t directory_index = move_into_folder(direction);
				if (directory_index != 0)
				{
					active_directory = directory_index; 
				}
				else 
				{
					error_message(5); // requested directory not found
				}
			}
		}
		else if (strcmp(command, "help") == 0)
		{
			print_list_of_commands(); 
		}
		else 
		{
			error_message(4); // unrecognized command
		}

		printf("\n");
	}

	free_file_system(); 
}

/* Enters directory if found, or returns 0 (superblock) otherwise. 
This is a very simple function but the main was cluttered enough
so I moved it out. */
index_t move_into_folder(char* directory_name)
{
	index_t index = 0; 
	index = find_file(directory_name);
	return index; 
}

/* Callocs space for the bitvector and create the superblock. */ 
void initialize_file_system()
{
	if ((bitvector = calloc(1, (sizeof(char) * (8192)))) == NULL)
	{
		error_message(3); // malloc failed
	}

	create_directory("/", 362, 0);
}

/* If index is 0, we are setting up the superblock. */ 
NODE create_directory(char* directory_name, unsigned short owner_ID, index_t parent_directory)
{
	time_t current_time;
	time(&current_time);

	NODE node; 
	FILE_SYSTEM_NODE file_system_node; 
	node.content.fd = file_system_node; 

	// Add this directory to a spot in the file system 
	index_t index = find_next_open_spot(); 

	if (index == 0)
	{
		node.content.fd.creation_time = 0; 
		node.content.fd.permissions = 400; 
	}
	else 
	{
		node.content.fd.creation_time = current_time; 
		node.content.fd.permissions = DEFAULT_PERMISSIONS;
	}

	// Mark this spot as taken in the bitvector 
	mark_spot_taken_at(index); 

	// Assign the attributes of this directory  
	node.type = DIR; 
	strcpy(node.content.fd.name, directory_name);
	node.content.fd.access_time   = current_time; 
	node.content.fd.modified_time = current_time; 
	node.content.fd.owner_ID  = owner_ID; 
	node.content.fd.node_size = 0; // because no files contained yet

	// Create index node for this directory 
	index_t next_index = find_next_open_spot(); 
	node.content.fd.block_ref = next_index; 
	mark_spot_taken_at(next_index);

	node.parent_directory = active_directory; 

	memory[index] = node; 
	memory[next_index] = create_index_node(); 

	// Add a link to this directory in the root index node 
	if (index != 0)
	{
		int last_index = (memory[memory[active_directory].content.fd.block_ref].content.fd.node_size); 
		memory[memory[active_directory].content.fd.block_ref].content.index[last_index] = index; 
		memory[memory[active_directory].content.fd.block_ref].content.fd.node_size++; 
	}

	return node; 
}

/* Index node contains sequential references to indices, 0 otherwise. Cannot link to superblock at this current time. */ 
NODE create_index_node()
{	
	time_t current_time;
	time(&current_time);

	NODE index_node; 
	index_node.type = INDEX; 
	index_node.content.fd.permissions   = DEFAULT_PERMISSIONS; 
	index_node.content.fd.creation_time = current_time; 
	index_node.content.fd.access_time   = current_time; 
	index_node.content.fd.modified_time = current_time; 
	index_node.content.fd.owner_ID      = SYSTEM_OWNER; 
	index_node.content.fd.node_size = 0; 

	return index_node; 
}

/* File node either contains chunk of data (if it's short) 
or a reference to one or more data nodes. */ 
void create_file(char* filename, unsigned short owner_ID, char* data, index_t current_directory)
{
	time_t current_time;
	time(&current_time);

	NODE node; 

	// Set node attributes 
	node.type = DOC; 
	node.content.fd.node_size     = strlen(data); 
	node.content.fd.access_time   = current_time; 
	node.content.fd.creation_time = current_time; 
	node.content.fd.modified_time = current_time; 
	node.content.fd.owner_ID      = owner_ID; 
	node.content.fd.permissions   = DEFAULT_PERMISSIONS; 

	index_t index = find_next_open_spot(); 
	mark_spot_taken_at(index);

	// find a spot for the index of this file node 
	index_t next_index = find_next_open_spot(); 
	node.content.fd.block_ref = next_index; 
	mark_spot_taken_at(next_index);
	memory[next_index] = create_index_node();

	// We may need to split up the data among several data nodes
	int number_of_data_nodes = (strlen(data) / DATA_SIZE);
	if (number_of_data_nodes == 0) 
	{
		// If we have less than 254 bytes of data, create a single node.
		number_of_data_nodes = 1; 
	} 

	for (int node_count = 0; node_count < number_of_data_nodes; node_count++)
	{
		index_t placement_index = find_next_open_spot(); 
		memory[node.content.fd.block_ref].content.index[node_count] = placement_index; 
		mark_spot_taken_at(placement_index);

		// keeps track of how many data nodes we point to 
		memory[node.content.fd.block_ref].content.fd.node_size++; 

		// lob off a chunk of the data to add to data node 
		char memory_copy[DATA_SIZE]; 
		strncpy(memory_copy, data + (node_count * DATA_SIZE), DATA_SIZE);
		memory[placement_index] = create_data_node(memory_copy); 
	}

	strcpy(node.content.fd.name, filename);

	// Add a link to this file in the active directory index node 
	int last_index = (memory[memory[active_directory].content.fd.block_ref].content.fd.node_size); 
	memory[memory[active_directory].content.fd.block_ref].content.index[last_index] = index; 
	memory[memory[active_directory].content.fd.block_ref].content.fd.node_size++; 
	memory[active_directory].content.fd.node_size++; 

	memory[index] = node; 
}

/* Data nodes simply store raw data. Accessible via file node. */ 
NODE create_data_node(char* data)
{
	NODE node; 
	node.type = DATA; 
	node.content.fd.node_size = strlen(data); 
	strcpy(node.content.data, data);
	return node; 
}

/* Prints permissions, preview of content if file node, or number
of files enclosed (if directory node) */
void get_info_about(char* filename)
{
	index_t file_location = find_file(filename);

	// If the file was found, print information about it
	if (file_location != 0)
	{
		printf("\nInformation about the file called '%s':\n", memory[file_location].content.fd.name);
		printf("    Date created:  %s", ctime(&memory[file_location].content.fd.creation_time));
		printf("    Date modified: %s", ctime(&memory[file_location].content.fd.modified_time));
		printf("    Permissions:   %d\n", memory[file_location].content.fd.permissions);
	}
	else 
	{
		error_message(5); // file not found
	}
}

/* Print full contents of the file, if it exists */ 
void read_file(char* filename)
{
	index_t file_location = find_file(filename); 

	if (file_location == 0)
	{
		// It returned 0 - but it can't be the superblock, so: 
		error_message(5); // file not found
	}
	else 
	{
		display_file_system(memory[file_location].content.fd.block_ref); // this will print only the data, nifty 
	}
}

/* Search the active directory for the requested file */ 
index_t find_file(char* filename)
{
	index_t index = 0; 
	int found = -1; 

	index_t index_node = memory[active_directory].content.fd.block_ref;

	int index_pointer = 0; 
	while ((index_pointer < memory[index_node].content.fd.node_size) && (found == -1))
	{
		if ((strcmp(memory[memory[index_node].content.index[index_pointer]].content.fd.name, filename) == 0))
		{
			index = memory[index_node].content.index[index_pointer]; 
			found = 0; 
		}

		index_pointer++; 
	}

	return index; 
}

/* Removes reference to file from bitvector and memory - but node 
still exists somewhere, so the FBI can have their fun */ 
void delete_file(char* name)
{
	printf("You want to get rid of %s?\n", name);


} 

/* Removes reference to directory in bitvector and main memory */
void remove_directory(char* name)
{
	index_t index = find_file(name);

	if (index != 0)
	{
		if (memory[index].content.fd.node_size == 0)
		{
			printf("deleting...\n");
		}
		else 
		{
			error_message(7); // cannot delete non-empty directory  
		}
	}
	else 
	{
		error_message(5); // file not found in directory 
	}
} 	

void free_file_system()
{
	free(bitvector);
} 

/* Recursively prints information about each node, top-down through
the system. */ 
void display_file_system(index_t node_index)
{
	NODE node = memory[node_index]; 

	printf("[%d]", node_index);

	if (node.type == INDEX)
	{
		printf("     ");
		printf("\e[7mINDEX\e[27m\n"); 
		index_t pointer_index = 0; 
		while (pointer_index < node.content.fd.node_size)
		{
			display_file_system(node.content.index[pointer_index]);
			pointer_index++; 
		}
	}

	else if (node.type == DIR)
	{
		printf("     %s %s\n", "DIR", node.content.fd.name);

		// go to the index node pointed to by the directory 
		display_file_system(node.content.fd.block_ref);
	}

	else if (node.type == DOC)
	{
		printf("     %s '%s'\n", "FILE", node.content.fd.name);

		// If I wanted to print file content too: 
		// display_file_system(node.content.fd.block_ref);
	}
	// if NODE_TYPE == DATA: 
	else 
	{
		printf("     %s\n", node.content.data);
	} 
}

void error_message(int error_type)
{
	switch(error_type)
	{
		case 1:
			printf("NONFATAL ERROR: File could not be opened. Try again.\n");
			break; 
		case 2: 
			printf("FATAL ERROR: 0 spaces allocated in page table.\n");
			exit(-1);
		case 3: 
			printf("FATAL ERROR: Malloc failed.\n");
			exit(-1);
		case 4:
			printf("NONFATAL ERROR: Unrecognized command.\n");
			print_list_of_commands(); 
			break; 
		case 5: 
			printf("NONFATAL ERROR: Requested location was not found in this directory.\n(Current active directory: %s)\n", memory[active_directory].content.fd.name);
			break; 
		case 6: 
			printf("NONFATAL ERROR: Cannot move up from root folder.\n");
			break; 
		case 7:
			printf("NONFATAL ERROR: Cannot delete non-empty directory.\n");
			break; 
		default: 
			printf("NONFATAL ERROR: Unknown error type. Please consult user manual.\n");
	}
}

void print_list_of_commands()
{
	printf("\n_____________ USER MANUAL, section 1: _____________\n");
	printf("Commands this file system accepts: \n");
	printf("(Please note that capitalized items are variables)\n\n");
	printf("    mkdir    DIRECTORY_NAME\n");
	printf("    rmdir    DIRECTORY_NAME\n");
	printf("    cd       DIRECTORY_NAME\n");
	printf("    getInfo  FILE_NAME\n");
	printf("    write    FILE_NAME\n");
	printf("    read     FILE_NAME\n");
	printf("    open     FILE_NAME\n");
	printf("    close    FILE_NAME\n");
	printf("    rm       FILE_NAME\n");
	printf("    cd ..\n");
	printf("    ls\n");
	printf("    help\n"); // how meta, this just prints the manual
	printf("    quit\n");
	printf("___________________________________________________\n");
}


