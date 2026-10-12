#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

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

enum _TREE_PRINT { INORDER = 1, PREORDER, POSTORDER };

struct _Node;
typedef struct _Node _NODE;
typedef struct _Node* Pos;
struct _Node
{
	int El;
	Pos Right;
	Pos Left;
};

int PrintError(char*);
int PrintMenu();
int RandomeGenerateInteger(int, int, int *);
int InsertInteger(int *);
Pos Insert(Pos, int);
int Print(int, Pos);
Pos FindMin(Pos);
Pos FindMax(Pos);
Pos Delete(Pos, int);
Pos Find(Pos, int);

int main(int argc, char *argv[])
{
	int result = SUCCESS;
	Pos root = NULL;
	Pos found = NULL;
	char *izbor = NULL;
	int n = 0;

	srand((unsigned)time(NULL));

	izbor = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	memset(izbor, 0, BUFFER_LENGTH);

	printf("\r\n");
	printf("\r\n");
	while (_strnicmp(izbor, "EXIT", 4) != 0)
	{
		PrintMenu();

		scanf(" %s", izbor);

		system(_CLRSCR);

		if (_stricmp(izbor, "1") == 0)
		{
			result = RandomeGenerateInteger(0, 10000, &n);
			if (result == SUCCESS)
			{
				printf("\r\n\tNumber to insert : %d\r\n", n);
				root = Insert(root, n);
			}
			else
			{
				PrintError("Random generated integer error.");
			}
		}
		else if (_stricmp(izbor, "2") == 0)
		{
			printf("\r\n\t");
			if (root == NULL)
				PrintError("Tree is empty");
			else
				Print(INORDER, root);
			printf("\r\n");
		}
		else if (_stricmp(izbor, "3") == 0)
		{
			printf("\r\n\t");
			if (root == NULL)
				PrintError("Tree is empty");
			else
				Print(PREORDER, root);
			printf("\r\n");
		}
		else if (_stricmp(izbor, "4") == 0)
		{
			printf("\r\n\t");
			if (root == NULL)
				PrintError("Tree is empty");
			else
				Print(POSTORDER, root);
			printf("\r\n");
		}
		else if (_stricmp(izbor, "5") == 0)
		{
			if (root == NULL)
				PrintError("Tree is empty");
			else
			{
				printf("\r\n\tDelete");
				result = InsertInteger(&n);
				if (result == SUCCESS)
				{
					root = Delete(root, n);
				}
			}
		}
		else if (_stricmp(izbor, "6") == 0)
		{
			if (root == NULL)
				PrintError("Tree is empty");
			else
			{
				printf("\r\n\tFind");
				result = InsertInteger(&n);
				if (result == SUCCESS)
				{
					found = Find(root, n);
					if (found != NULL)
						printf("\r\n\tElement: %d found at address: 0x%xh.", found->El, (unsigned int)found);
					else
						printf("\r\n\tElement: %d not found.", n);
				}
			}
		}
		else if (_stricmp(izbor, "EXIT") == 0)
		{
			printf("\r\n\tHave a nice day.\r\n");
		}
		else
		{
			printf("\r\n\t<%s> -> Unsuported selection!\r\n", izbor);
		}
	}

	free(izbor);
	return result;
}

int PrintMenu()
{
	printf("\r\n");
	printf("\t***************************************\r\n");
	printf("\t***   VJEZBA BR. 09  -  Binary tree ***\r\n");
	printf("\t***************************************\r\n");
	printf("\r\n");
	printf("\t\t1.\tInsert element\r\n");
	printf("\t\t2.\tPrint tree - infix\r\n");
	printf("\t\t3.\tPrint tree - prefix\r\n");
	printf("\t\t4.\tPrint tree - posfix\r\n");
	printf("\t\t5.\tDelete element\r\n");
	printf("\t\t6.\tFind element\r\n");
	printf("\t\texit.\tExit program\r\n");
	printf("\tIzbor : ");

	return SUCCESS;
}

int RandomeGenerateInteger(int min, int max, int *number)
{
	int range = max - min;

	if (range == 0) return ERROR;

	*number = rand() % range + min;

	return SUCCESS;
}

int InsertInteger(int *number)
{
	int result = -1;

	while (result < 0)
	{
		printf("\r\n\tInsert integer: ");
		result = scanf(" %d", number);
	}

	return SUCCESS;
}

Pos Insert(Pos P, int n)
{
	if (P == NULL)
	{
		P = (Pos)malloc(sizeof(struct _Node));
		if (P == NULL)
		{
			PrintError("Memory allocation failed.");
			return P;
		}

		P->El = n;
		P->Left = NULL;
		P->Right = NULL;
	}
	else if (P->El < n)
		P->Right = Insert(P->Right, n);
	else if (P->El > n)
		P->Left = Insert(P->Left, n);
	else
		PrintError("Element already exists.");
	return P;
}

int Print(int type, Pos P)
{
	switch (type)
	{
		case INORDER:
		{
			if (P != NULL)
			{
				Print(type, P->Left);
				printf(" %d", P->El);
				Print(type, P->Right);
			}
		}
		break;
		case PREORDER:
		{
			if (P != NULL)
			{
				printf(" %d", P->El);
				Print(type, P->Left);
				Print(type, P->Right);
			}
		}
		break;
		case POSTORDER:
		{
			if (P != NULL)
			{
				Print(type, P->Left);
				Print(type, P->Right);
				printf(" %d", P->El);
			}
		}
		break;
	}

	return SUCCESS;
}

Pos FindMin(Pos P)
{
	if (P != NULL && P->Left != NULL)
	{
		return FindMin(P->Left);
	}

	return P;
}

Pos FindMax(Pos P)
{
	if (P == NULL) return P;

	while (P->Right != NULL)
		P = P->Right;
	return P;
}

Pos Find(Pos P, int n)
{
	if (P == NULL)
		return P;

	if (P->El > n)
		return Find(P->Left, n);
	else if (P->El < n)
		return Find(P->Right, n);
	else
		return P;
}

Pos Delete(Pos P, int n)
{
	Pos tmp;
	if (P == NULL)
	{
		PrintError("Element not found.");
	}
	else if (n < P->El)
		P->Left = Delete(P->Left, n);
	else if (n > P->El)
		P->Right = Delete(P->Right, n);
	else
	{
		if (P->Left != NULL && P->Right != NULL)
		{
			// Min from Right subtree
			tmp = FindMin(P->Right);
			P->El = tmp->El;
			P->Right = Delete(P->Right, tmp->El);
			/* or Max from Left subtree
			tmp = FindMax(P->Left);
			P->El = tmp->El;
			P->Left = Delete(P->Left, tmp->El);
			*/
		}
		else
		{
			tmp = P;
			if (P->Left == NULL)
				P = P->Right;
			else
				P = P->Left;
			free(tmp);
		}
	}

	return P;
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
