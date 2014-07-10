/*
 * FileSystem.c
 *
 *  Created on: Jul 8, 2014
 *      Author: davidhunt
 */
#include<stdio.h>
#include<stdlib.h>
#include"FileSystem.h"
#include<string.h>

#define GREEN "\e[32m"
#define WHITE "\e[97m"
#define RED   "\e[31m"
#define RESET "\e[0m"


int main(int args, char ** argv)
{
	printf(WHITE "Hello World\n");
	char input[100];
	char * par;
	commands * com = createCommandBlock();

	file * root = createRoot();
	file * currentFolder = root;
	printEntireSystem(root);
	strcpy(input, "myFile");
	root = addFile(root, input);
	printEntireSystem(root);

	clearBuffer(input);
	while(com->end != 1)
	{
		printf("Enter Command: ");
		scanf("%s", input);
		par = strtok(input, " ,.-");
		while((par != NULL) && (com->capacity < com->size))
		{
			com = addString(com, par);
			par = strtok(NULL, " ,.-");
		}
		currentFolder = prser(com, currentFolder);
		com = clearCommands(com);
		clearBuffer(input);
	}	

	printf("\nGoodbye\n" RESET);
	free(currentFolder);
	return 0;
}


//create a file
file * createFile(file * top, char * name, char fold)
{
	file * fl = (file *)malloc(sizeof(file) + 1);
	strcpy(fl->fileName, name);
	fl->isFolder = fold;
	fl->insideFolder = NULL;
	fl->nextFile = NULL;
	fl->parentDirectory = top;
	return fl;
}

//creates the root file
file * createRoot()
{
	file * home = (file*)malloc(sizeof(file) +1);
	strcpy(home->fileName, "Root");
	home->isFolder = 'Y';
	home->insideFolder = NULL;
	home->nextFile = NULL;
	return home;
}

//adds a file to the fileSystem
file * addFile(file * currentDirectory, char * name)
{
	char fn = 'N';
	file * nwFile = createFile(currentDirectory, name, fn);
	currentDirectory = placeEnd(currentDirectory, nwFile);
	return currentDirectory;
}

//adds a folder to the fileSystem
file * addFolder(file * currentDirectory, char * name)
{
	char fn = 'Y';
	file * nwFile = createFile(currentDirectory, name, fn);
	currentDirectory = placeFolderEnd(currentDirectory, nwFile);
	return currentDirectory;
}

//prints the entire file system
void printEntireSystem(file * root)
{
	if(root == NULL)
	{
		printf(RED "\nCould not print the content of the folders\n" RESET);
	}
	else
	{
		printFile(root);
		file * temp = root->nextFile;
		while(temp != NULL)
		{
			printFile(temp);
			temp = temp->nextFile;
		}
	}
}

//places the file to the end of the folder
file * placeEnd(file * dir, file * nwFile)
{
	file * temp = dir;
	if(temp->insideFolder == NULL)
	{
		temp->insideFolder = nwFile;
		return dir;
	}
	while(temp->nextFile != NULL)
	{
		temp  = temp->nextFile;
	}
	temp->nextFile = nwFile;
	return dir;
}

file * placeFolderEnd(file * dir, file * nwFolder)
{
	file * temp = dir;
	if(temp->insideFolder == NULL)
	{
		temp->insideFolder = nwFolder;
		return nwFolder;
	}
	temp = temp->insideFolder;
	while(temp->nextFile != NULL)
	{
		temp = temp->nextFile;
	}
	temp->nextFile = nwFolder;
	return temp->nextFile;
}

void printFile(file * currentFile)
{
	if(currentFile == NULL)
		return;
	else
	{
		if(currentFile->isFolder == 'Y')
			printf(GREEN "\n%s\n" RESET, currentFile->fileName);
		else
			printf(WHITE "\n%s in Folder %s\n" RESET, currentFile->fileName, currentFile->parentDirectory->fileName);
	}
}

void clearBuffer(char * temp)
{
	int i;
	for(i = 0; i <= 100; i++)
	{
		temp[i] = '\0'; 
	}
}

commands * createCommandBlock()
{
	commands * temp = (commands *)malloc(sizeof(commands) + 1);
	temp->capacity = 0;
	temp->size = 5;
	temp->commandSize = 20;
	temp->end = 0;
	int i;
	temp->commands = (char **)malloc(sizeof(char *) * 5  +1);
	for(i = 0; i < 5; i++)
	{
		temp->commands[i] = (char *)malloc(sizeof(char) * 20 + 1);
	}
	return temp;
}

commands * clearCommands(commands * com)
{
	int i, j;
	for(i = 0; i < com->size; i++)
	{
		for(j = 0; j < com->commandSize; j++)
		{
			com->commands[i][j] = '\0';
		}
	}
	com->capacity = 0;
	return com;
}

commands * addString(commands * com, char * str)
{
	if(com->capacity == (com->size - 1))
		return com;
	else
	{
		strcpy(com->commands[com->capacity], str);
		com->capacity = com->capacity + 1;
	}
	return com;
}

file * prser(commands * com, file * currentDirectory)
{
	char temp[80];
	strcpy(temp, com->commands[0]);
	if(strcmp(temp, "cd") == 0)
	{
		printf("changed directory\n");
		file * tempFile = findFolder(currentDirectory, com->commands[1]);
		if(tempFile != NULL)
			return tempFile;
		else
			return currentDirectory;
	}
	else if(strcmp(temp, "mkdir") == 0)
	{
		printf("make directory\n");
		file * tempFile = addFolder(currentDirectory, com->commands[1]);
		if(tempFile != NULL)
			return tempFile;
		else
			return currentDirectory;
	}
	else if(strcmp(temp, "mk") == 0)
	{
		printf("make file\n");
		return currentDirectory;
	}
	else if(strcmp(temp, "exit") == 0)
	{
		printf("exiting\n");
		com->end = 1;
		return currentDirectory;
	}
	else if(strcmp(temp, "ls") == 0)
	{
		printf("printing list");
		currentDirectory = listFiles(currentDirectory);
		return currentDirectory;
	}
	return NULL;
}

file * findFolder(file * currentDirectory, char * filName)
{
	if(strcmp(currentDirectory->fileName, filName) == 0)
	{
		printf(RED "Already in that folder\n" RESET);
		return NULL;
	}
	file * temp = currentDirectory->nextFile;
		while((temp != NULL) && (strcmp(temp->fileName, filName) != 0))
	{
		if(temp->nextFile == NULL)
			temp = NULL;
		else
			temp = temp->nextFile;
	}
	if((temp != NULL) && (strcmp(temp->fileName, filName) == 0))
		return temp;
	else
		return NULL;			
}

file * listFiles(file * currentDirectory)
{
	file * temp = currentDirectory;
	if(temp->insideFolder == NULL)
	{
		printf("Current Directory is empty\n");
	}
	else
	{
		temp = temp->insideFolder;
		while(temp != NULL)
		{
			printFile(temp);
			temp = temp->nextFile;
		}
	}
	return currentDirectory;
}
