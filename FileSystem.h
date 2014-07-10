/*
 * FileSystem.h
 *
 *  Created on: Jul 8, 2014
 *      Author: davidhunt
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

typedef struct file
{
	char fileName[80];
	char isFolder;
	struct file * nextFile;
	struct file * insideFolder;
	struct file * parentDirectory;
}file;

typedef struct commands
{
	char ** commands;
	int capacity;
	int size;
	int commandSize;
	int end;
}commands;

file * createFile(file * top, char * name, char fold);

file * createRoot();

void printEntireSystem(file * root);

file * placeEnd(file * dir, file * nwFile);

file * placeFolderEnd(file * dir, file * nwFolder);

file * addFile(file * currentDirectory, char * name);

file * addFolder(file * currentDirectory, char * name);

void printFile(file * currentFile);

void clearBuffer(char * temp);

commands * createCommandBlock();

commands * clearCommands(commands * com);

commands * addString(commands * com, char * str);

file * prser(commands * com, file * currentDirectory);

file * findFolder(file * currentDirectory, char * filName);

file * listFiles(file * currentDirectory);

#endif /* FILESYSTEM_H_ */
