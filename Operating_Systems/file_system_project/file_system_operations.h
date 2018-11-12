/**
* Name: Julia Maliauka
* Project 1, Task 1 and 2
* File: file_system_operations.h
* Started on 4/05/18
**/

#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h> 
#include <sys/stat.h> 

NODE create_directory(char* directory_name, unsigned short owner_ID, index_t parent_directory); 
NODE create_data_node(char* data); 
NODE create_index_node(); 

index_t move_into_folder(char* directory_name);
index_t find_file(char* filename); 

void create_file(char* filename, unsigned short owner_ID, char* data, index_t current_directory);
void read_file(char* filename);
void get_info_about(char* filename);
void display_file_system(index_t node_index); 
void error_message(int error_type); 
void remove_directory(char* name); 
void initialize_file_system(); 
void print_list_of_commands(); 
void free_file_system(); 
void delete_file(char* filename); 
