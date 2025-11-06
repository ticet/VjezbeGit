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
	int koeficijent;
	Position Next;
};

int PrintMenu();
int CreateNew(Position *);
int ReadFromFile(Position);
int PrintList(Position);
int Presijek(Position, Position, Position);
int Unija(Position, Position, Position);

int main(int argc, char *argv[])
{
	int err = SUCCESS;
	struct _Cvor L1, L2, P, U;

	L1.Next = NULL;
	L2.Next = NULL;
	P.Next = NULL;
	U.Next = NULL;

	err = PrintMenu();
	if (err) return err;

	printf("\r\n\t********************************************");
	printf("\r\n\tLista 1.");
	err = ReadFromFile(&L1);
	if (err)return err;
	printf("\r\n\t\tL1 : ");
	PrintList(L1.Next);

	printf("\r\n\t********************************************");
	printf("\r\n\tLista 2.");
	ReadFromFile(&L2);
	if (err)return err;
	printf("\r\n\t\tL2 : ");
	PrintList(L2.Next);

	printf("\r\n\t********************************************");
	err = Presijek(L1.Next, L2.Next, &P);
	if (err) return err;
	printf("\r\n\t\tPresijek : ");
	PrintList(P.Next);

	printf("\r\n\t********************************************");
	err = Unija(L1.Next, L2.Next, &U);
	if (err) return err;
	printf("\r\n\t\tUnija : ");
	PrintList(U.Next);

	printf("\r\n\t********************************************");
	printf("\r\n\r\n");

	return err;
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

int ReadFromFile(Position P)
{
	int err = SUCCESS;
	Position q = NULL;
	Position tmp = NULL;
	char *fileName = NULL;
	FILE *fp = NULL;

	fileName = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (fileName == NULL) return ERROR;
	memset(fileName, '\0', BUFFER_LENGTH);

	printf("\r\n\tUnesite ime dadoteke za citanje : ");
	scanf(" %s", fileName);

	if (strchr(fileName, '.') == NULL)
		strcat(fileName, ".txt");

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("\r\nGRESKA!\r\n%s datoteka nije otvorena.", fileName);
		return ERROR;
	}

	while (!feof(fp))
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
			break;
		}

		fscanf(fp, " %d", &q->koeficijent);

		tmp = P;
		while (tmp->Next != NULL && tmp->Next->koeficijent >= q->koeficijent)
			tmp = tmp->Next;

		q->Next = tmp->Next;
		tmp->Next = q;
	}
	fclose(fp);

	return err;
}

int PrintMenu()
{
	printf("\r\n\t********************************************");
	printf("\r\n\t***   VJEZBA BR. 05  -  Unija i presijek ***");
	printf("\r\n\t********************************************");
	printf("\r\n\r\n");

	return SUCCESS;
}

int PrintList(Position P)
{
	while (P != NULL)
	{
		printf(" %d", P->koeficijent);
		P = P->Next;
	}

	return SUCCESS;
}

int Presijek(Position L1, Position L2, Position P)
{
	int err = SUCCESS;
	Position q = NULL;

	while (L1 && L2)
	{
		if (L1->koeficijent > L2->koeficijent)
			L1 = L1->Next;
		else if (L1->koeficijent < L2->koeficijent)
			L2 = L2->Next;
		else
		{
			err = CreateNew(&q);
			if (err)
			{
				printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
				break;
			}

			q->koeficijent = L1->koeficijent;
			q->Next = P->Next;
			P->Next = q;
			P = q;

			L1 = L1->Next;
			L2 = L2->Next;
		}
	}

	return err;
}

int Unija(Position L1, Position L2, Position U)
{
	int err = SUCCESS;
	Position q, tmp;

	while (L1 && L2)
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
			break;
		}
		else
		{
			if (L1->koeficijent > L2->koeficijent)
			{
				q->koeficijent = L1->koeficijent;
				L1 = L1->Next;
			}
			else if (L1->koeficijent < L2->koeficijent)
			{
				q->koeficijent = L2->koeficijent;
				L2 = L2->Next;
			}
			else
			{
				q->koeficijent = L1->koeficijent;
				L1 = L1->Next;
				L2 = L2->Next;
			}

			if (U->koeficijent == q->koeficijent)
				free(q);
			else
			{
				q->Next = U->Next;
				U->Next = q;
				U = q;
			}
		}
	}

	if (err) return err;

	if (L1)
		tmp = L1;
	else
		tmp = L2;

	while (tmp)
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
			break;
		}

		q->koeficijent = tmp->koeficijent;

		if (U->koeficijent == q->koeficijent)
		{
			free(q);
		}
		else
		{
			q->Next = U->Next;
			U->Next = q;
			U = q;
		}

		tmp = tmp->Next;
	}

	return err;
}