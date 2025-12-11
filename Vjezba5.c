#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#ifndef ERROR
#define ERROR -1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 1024
#endif

struct _Cvor;
typedef struct _Cvor _CVOR;
typedef struct _Cvor* Position;
struct _Cvor
{
	float Element;
	Position Next;
};


int CreateNew(Position *);
int PrintMenu();
int Push(Position, float);
int Pop(Position, float*);
int Calculator(float, float, char *, float *);
int IsOperand(char*, float *);
int IsValidStack(Position);
int CalculatePosfix(char *, Position);
int GarbageCollector(Position);

int main(int argc, char *argv[])
{
	int error = SUCCESS;
	char *fileName = NULL;
	_CVOR _Stack;
	_Stack.Next = NULL;

	PrintMenu();

	fileName = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (fileName == NULL) return ERROR;
	memset(fileName, '\0', BUFFER_LENGTH);

	while (strlen(fileName) == 0)
	{
		printf("\r\n\tUnesite naziv datoteke s posfix izrazome <max %d znakova>: ", BUFFER_LENGTH);
		scanf(" %s", fileName);
	}

	if (strchr(fileName, '.') == NULL)
		strcat(fileName, ".txt");

	error = CalculatePosfix(fileName, &_Stack);
	if (error) return error;

	error = IsValidStack(&_Stack);
	if (error)
		printf("\r\nPOSFIX calculation error!");
	else
	{
		printf("\r\n\tRezultat: %.2f", _Stack.Next->Element);
	}

	GarbageCollector(_Stack.Next);
	free(fileName);

	printf("\r\n");
	return error;
}

int PrintMenu()
{
	printf("\t****************************************\r\n");
	printf("\t***   VJEZBA BR. 07  - Posfix izraz ***\r\n");
	printf("\t****************************************\r\n");
	printf("\r\n");

	return SUCCESS;
}

int CreateNew(Position *head)
{
	Position q = NULL;

	*head = (Position)malloc(sizeof(_CVOR));
	if (*head == NULL) return ERROR;

	q = *head;
	q->Next = NULL;

	return SUCCESS;
}

int Push(Position P, float x)
{
	int err = SUCCESS;
	Position q = NULL;

	err = CreateNew(&q);
	if (err) return err;

	q->Element = x;

	q->Next = P->Next;
	P->Next = q;

	return err;
}

int Pop(Position P, float *x)
{
	Position tmp = NULL;

	tmp = P->Next;
	if (tmp == NULL) return ERROR;

	P->Next = tmp->Next;
	*x = tmp->Element;
	free(tmp);

	return SUCCESS;
}

int Calculator(float x, float y, char *operation, float *result)
{
	int error = SUCCESS;
	if (operation == NULL || strlen(operation) == 0)
		return ERROR;

	if (strcmp(operation, "+") == 0)
	{
		*result = x + y;
	}
	else if (strcmp(operation, "-") == 0)
	{
		*result = x - y;
	}
	else if (strcmp(operation, "*") == 0)
	{
		*result = x * y;
	}
	else if (strcmp(operation, "/") == 0)
	{
		if (y < 0)
		{
			*result = 0;
			printf("\r\n\t\tDivide with zero not allowed!");
			error = ERROR;
		}
		else
		{
			*result = x / y;
		}
	}
	else
	{
		*result = 0;
		printf("\r\n\t\tOperation not recognized!");
		error = ERROR;
	}

	return error;
}

int IsOperand(char *buff, float *number)
{
	int result = SUCCESS;

	result = sscanf(buff, " %f", number);
	if (result == EOF || result == 0 || result != 1) return ERROR;

	return SUCCESS;
}

int CalculatePosfix(char *fileName, Position S)
{
	int error = SUCCESS;
	float number = 0;
	float operand1 = 0;
	float operand2 = 0;
	char *buff = NULL;
	FILE *fp = NULL;

	buff = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (buff == NULL) return ERROR;
	memset(buff, '\0', BUFFER_LENGTH);

	fp = fopen(fileName, "r");
	if (fp == NULL) return ERROR;

	while (!feof(fp))
	{
		memset(buff, 0, BUFFER_LENGTH);
		fscanf(fp, " %s", buff);
		if (strlen(buff) == 0) continue;

		if (SUCCESS != IsOperand(buff, &number))
		{
			error = Pop(S, &operand2);
			if (error) break;

			error = Pop(S, &operand1);
			if (error) break;

			error = Calculator(operand1, operand2, buff, &number);
			if (error) break;

		}
		error = Push(S, number);
		if (error) break;
	}

	fclose(fp);
	
	if (error)
		printf("\r\nPOSFIX error!");

	return error;
}

int IsValidStack(Position S)
{
	if (S == NULL) return ERROR;

	if (S->Next == NULL) return ERROR;

	if (S->Next->Next != NULL) return ERROR;

	return SUCCESS;
}

int GarbageCollector(Position S)
{
	if (S == NULL) return SUCCESS;

	GarbageCollector(S->Next);
	free(S);
	return SUCCESS;
}