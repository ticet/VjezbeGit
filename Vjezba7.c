#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#ifdef _WIN64
#ifndef _CLRSCR
#define _CLRSCR "cls"
#endif
#elif _WIN32
#ifndef _CLRSCR
#define _CLRSCR "cls"
#endif
#else
#ifndef _CLRSCR
#define _CLRSCR "clear"
#endif
#endif

#ifndef ERROR
#define ERROR -1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 1024
#endif

struct _Node;
typedef struct _Node _NODE;
typedef struct _Node* Position;
struct _Node
{
	char *name;
	int isRoot;
	Position Next;
	Position Child;
};

struct _Stack;
typedef struct _Stack _STACK;
typedef struct _Stack* Pos;
struct _Stack
{
	Position Node;
	Pos Next;
};

int PrintError(char*);
int CreateNewDirectoryNode(Position *);
int PrintDirectory(Position, char *);
Position UpDirectory(Position, char *, Pos);
Position InDirectory(Position, char *, char *, Pos);
int MakeDirectory(Position, char *);
Position InsertDirectory(Position, Position);
int CreateNewStackNode(Pos *);
int Push(Pos, Position);
int Pop(Pos, Position *);
int AllocateBuffers(char **, char**, char **, char **, char **);
int DeAllocateBuffers(char *, char*, char *, char *, char *);
int ReleaseStack(Pos);
int ReleaseTree(Position);
int GarbageCollector(char *, char*, char *, char *, char *, Pos, Position);
int PrintPath(char *);
int PrintHelp(char *);

int main(int argc, char*argv[])
{
	int nParams = 0;
	int result = SUCCESS;
	char *path = NULL;
	char *command = NULL;
	char *directoryName = NULL;
	char *instruction = NULL;
	char *buff = NULL;
	char *i = NULL;
	char *j = NULL;
	Position root = NULL;
	Pos stack = NULL;
	Position upDir = NULL;

	result = AllocateBuffers(&path, &command, &directoryName, &instruction, &buff);
	if (result)
	{
		GarbageCollector(path, command, directoryName, instruction, buff, stack, root);
		return result;
	}

	result = CreateNewDirectoryNode(&root);
	if (result)
	{
		PrintError("Root not initialized");
		GarbageCollector(path, command, directoryName, instruction, buff, stack, root);
		return result;
	}

	while (1)
	{
		printf("Insert root directory name <max %d characters>: ", BUFFER_LENGTH);
		fgets(buff, BUFFER_LENGTH, stdin);

		if (strlen(buff) != 0)
			break;
	}

	root->name = (char*)malloc(sizeof(char) * (strlen(buff) + 1));
	memset(root->name, '\0', sizeof(root->name));
	sscanf(buff, " %s", root->name);
	root->isRoot = 1;
	root->Child = NULL;
	root->Next = NULL;
	upDir = root;

	result = CreateNewStackNode(&stack);
	if (result)
	{
		PrintError("Stack not initialized");
		GarbageCollector(path, command, directoryName, instruction, buff, stack, root);
		return result;
	}
	stack->Next = NULL;

	system(_CLRSCR);

	strcat(path, root->name);
	strcat(path, ":\\");

	while (_stricmp(instruction, "exit") != 0)
	{
		PrintPath(path);

		memset(command, 0, BUFFER_LENGTH);
		memset(directoryName, 0, BUFFER_LENGTH);
		memset(instruction, 0, BUFFER_LENGTH);

		fgets(command, BUFFER_LENGTH, stdin);

		nParams = sscanf(command, " %s %s", instruction, directoryName);
		if (nParams == EOF || nParams <= 0) continue;

		if (strcmp(instruction, "dir") == 0 || strcmp(instruction, "DIR") == 0)
		{
			if (nParams != 1)
			{
				PrintError("The syntax of the command is incorrect.\n");
			}
			else
			{
				PrintDirectory(upDir, path);
			}
		}
		else if (strcmp(instruction, "cd..") == 0 || strcmp(instruction, "CD..") == 0)
		{
			if(nParams != 1)
				PrintError("The syntax of the command is incorrect.\n");
			else
				upDir = UpDirectory(upDir, path, stack);
		}
		else if (strcmp(instruction, "cd") == 0 || strcmp(instruction, "CD") == 0)
		{
			if (nParams != 2)
				PrintError("The syntax of the command is incorrect.\n");
			else
				upDir = InDirectory(upDir, directoryName, path, stack);
			if (upDir == NULL)
			{
				PrintError(NULL);
				break;
			}
		}
		else if (strcmp(instruction, "md") == 0 || strcmp(instruction, "MD") == 0)
		{
			if (upDir == NULL)
			{
				PrintError(NULL);
				break;
			}
			if (nParams != 2)
				PrintError("The syntax of the command is incorrect.\n");
			else
				MakeDirectory(upDir, directoryName);
		}
		else if (strcmp(instruction, "exit") == 0 || strcmp(instruction, "EXIT") == 0)
		{
			if (nParams != 1)
			{
				PrintError("The syntax of the command is incorrect.\n");
				strcat(instruction, ".");
			}
		}
		else if (strcmp(instruction, "help") == 0 || strcmp(instruction, "HELP") == 0)
		{
			if (nParams == 2)
			{
				PrintHelp(directoryName);
			}
			else if (nParams == 1)
			{
				PrintHelp(NULL);
			}
			else
			{
				PrintError("The syntax of the command is incorrect.\n");
			}
		}
		else
		{
			printf("\r\n'%s' is not recognized as an internal or external command,\r\noperable program or batch file.\r\n\r\n", instruction);
		}
	}

	GarbageCollector(path, command, directoryName, instruction, buff, stack, root);
	return result;
}

int AllocateBuffers(char **ppath, char **pcommand, char **pdirectoryName, char **pinstruction, char **pbuff)
{
	char *path = NULL;
	char *command = NULL;
	char *directoryName = NULL;
	char *instruction = NULL;
	char *buff = NULL;

	path = (char*)malloc(sizeof(char) * (BUFFER_LENGTH + 1));
	if (path == NULL)
	{
		PrintError("Path buffer not allocated!");
		return ERROR;
	}
	memset(path, '\0', BUFFER_LENGTH + 1);
	*ppath = path;

	command = (char*)malloc(sizeof(char) * (BUFFER_LENGTH + 1));
	if (command == NULL)
	{
		PrintError("Command buffer not allocated!");
		return ERROR;
	}
	memset(command, '\0', BUFFER_LENGTH + 1);
	*pcommand = command;

	directoryName = (char*)malloc(sizeof(char) * (BUFFER_LENGTH + 1));
	if (directoryName == NULL)
	{
		PrintError("Directory name buffer not allocated!");
		return ERROR;
	}
	memset(directoryName, '\0', BUFFER_LENGTH + 1);
	*pdirectoryName = directoryName;

	instruction = (char*)malloc(sizeof(char) * (BUFFER_LENGTH + 1));
	if (instruction == NULL)
	{
		PrintError("Instruction buffer not allocated!");
		return ERROR;
	}
	memset(instruction, '\0', BUFFER_LENGTH + 1);
	*pinstruction = instruction;

	buff = (char*)malloc(sizeof(char) * (BUFFER_LENGTH + 1));
	if (buff == NULL)
	{
		PrintError("Buffer not allocated!");
		return ERROR;
	}
	memset(buff, '\0', BUFFER_LENGTH + 1);
	*pbuff = buff;

	return SUCCESS;
}

int DeAllocateBuffers(char *path, char *command, char *directoryName, char *instruction, char *buff)
{
	if (path != NULL) free(path);
	if (command != NULL) free(command);
	if (directoryName != NULL) free(directoryName);
	if (instruction != NULL) free(instruction);
	if (buff != NULL) free(buff);

	return SUCCESS;
}

int ReleaseStack(Pos S)
{
	if (S == NULL) return SUCCESS;

	ReleaseStack(S->Next);
	free(S);
	return SUCCESS;
}

int ReleaseTree(Position T)
{
	if (T == NULL) return SUCCESS;
	ReleaseTree(T->Child);
	ReleaseTree(T->Next);
	free(T);
	return SUCCESS;
}

int GarbageCollector(char *path, char *command, char *directoryName, char *instruction, char *buff, Pos stack, Position root)
{
	DeAllocateBuffers(path, command, directoryName, instruction, buff);
	ReleaseStack(stack);
	ReleaseTree(root);

	return SUCCESS;
}

int PrintError(char *message)
{
	if (message == NULL || strlen(message) == 0)
	{
		printf("\r\n\tUnknown error!");
	}
	else
	{
		printf("\r\n\t%s", message);
	}
	printf("\r\n");
	return SUCCESS;
}

int PrintHelp(char *command)
{
	if (command == NULL)
	{
		printf("\r\n\tDIR\t\tDisplays a list of files and subdirectories in a directory.");
		printf("\r\n\tCD\t\tDisplays the name of or changes the current directory.");
		printf("\r\n\tMD\t\tCreates a directory.");
		printf("\r\n\tEXIT\t\tQuits the CMD.EXE program (command interpreter).");
	}
	else if (strcmp(command, "dir") == 0 || strcmp(command, "DIR") == 0)
	{
		printf("\r\n\tDIR\t\tDisplays a list of files and subdirectories in a directory.");
	}
	else if (strcmp(command, "cd") == 0 || strcmp(command, "CD") == 0)
	{
		printf("\r\n\tCD\t\tDisplays the name of or changes the current directory.");
	}
	else if (strcmp(command, "md") == 0 || strcmp(command, "MD") == 0)
	{
		printf("\r\n\tMD\t\tCreates a directory.");
	}
	else if (strcmp(command, "EXIT") == 0 || strcmp(command, "exit") == 0)
	{
		printf("\r\n\tEXIT\t\tQuits the CMD.EXE program (command interpreter).");
	}
	else
	{
		printf("\r\n'%s' is not recognized as an internal or external command,\r\noperable program or batch file.\r\n\r\n", command);
	}
	printf("\r\n\r\n");

	return SUCCESS;
}

int PrintPath(char *path)
{
	printf("%s>", path);
	return SUCCESS;
}

int PrintDirectory(Position node, char *path)
{
	int i = 0;
	printf("\r\nDirectory of %s", path);

	node = node->Child;
	while (node != NULL)
	{
		printf("\r\n\t<DIR> %s", node->name);
		node = node->Next;
		i++;
	}
	printf("\r\n");
	printf("\r\n\t\t %d Dir(s)", i);

	printf("\r\n\r\n");

	return SUCCESS;
}

int CreateNewDirectoryNode(Position *q)
{
	Position tmp = NULL;

	tmp = (Position)malloc(sizeof(_NODE));
	if (tmp == NULL) return ERROR;

	tmp->isRoot = 0;
	tmp->Child = NULL;
	tmp->Next = NULL;

	*q = tmp;
	return SUCCESS;
}

Position UpDirectory(Position node, char *path, Pos S)
{
	int result = SUCCESS;
	Position temp = NULL;
	char *a = NULL;

	result = Pop(S, &temp);
	if (result)
	{
		return node;
	}

	a = strrchr(path, '\\');
	if (temp->isRoot)
	{
		a++;
	}
	*a = '\0';

	return temp;
}

Position InDirectory(Position node, char *name, char *path, Pos S)
{
	Position current = NULL;
	char *a = NULL;

	if (node == NULL)
	{
		PrintError(NULL);
		return NULL;
	}

	current = node->Child;

	while (current != NULL && _stricmp(current->name, name) != 0)
		current = current->Next;

	if (NULL == current)
	{
		PrintError("The system cannot find the path specified.");
		return node;
	}

	Push(S, node);
	if(!node->isRoot)
		strcat(path, "\\");
	strcat(path, current->name);

	return current;
}

Position InsertDirectory(Position q, Position child)
{
	Position prev = NULL;

	if (child == NULL) return q;

	if (strcmp(q->name, child->name) < 0)
	{
		q->Next = child;
		return q;
	}
	else if (strcmp(q->name, child->name) == 0)
	{
		PrintError("Directory already exists");
		return child;
	}

	prev = child;

	while (prev->Next != NULL && strcmp(q->name, prev->Next->name) > 0)
	{
		prev = prev->Next;
	}
	if (prev->Next != NULL && strcmp(q->name, prev->Next->name) == 0)
	{
		PrintError("Directory already exists");
		return child;
	}

	q->Next = prev->Next;
	prev->Next = q;

	return child;
}

int MakeDirectory(Position node, char *name)
{
	int result = SUCCESS;
	Position q = NULL;

	if (node == NULL)
	{
		PrintError(NULL);
		return ERROR;
	}

	if (name == NULL || strlen(name) == 0)
	{
		PrintError("Directory name not defined!");
		return ERROR;
	}

	result = CreateNewDirectoryNode(&q);
	if (result)
	{
		PrintError("Memory allocation failed!");
		return result;
	}

	q->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	if(q->name == NULL)
	{
		PrintError("Memory allocation failed!");
		free(q);
		return ERROR;
	}
	memset(q->name, '\0', (strlen(name) + 1));

	strncpy(q->name, name, strlen(name));

	node->Child = InsertDirectory(q, node->Child);
	
	return SUCCESS;
}

int CreateNewStackNode(Pos *q)
{
	Pos tmp = NULL;

	tmp = (Pos)malloc(sizeof(_STACK));
	if (tmp == NULL) return ERROR;

	tmp->Next = NULL;
	*q = tmp;

	return SUCCESS;
}

int Push(Pos S, Position T)
{
	int result = SUCCESS;
	Pos q = NULL;

	result = CreateNewStackNode(&q);
	if (result)
	{
		PrintError("Memory allocation failed!");
		return result;
	}
	q->Node = T;
	q->Next = S->Next;
	S->Next = q;

	return SUCCESS;
}

int Pop(Pos S, Position *node)
{
	Pos tmp = NULL;

	if (NULL == S->Next)
		return ERROR;

	tmp = S->Next;
	if (tmp == NULL) //viska?
		return ERROR;

	S->Next = tmp->Next;
	if (tmp->Node == NULL)
		return ERROR;

	*node = tmp->Node;
	free(tmp);

	return SUCCESS;
}
