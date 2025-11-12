
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
	int eksponent;
	Position Next;
};

int PrintMenu();
int CreateNew(Position *);
int ReadFromFile(Position);
int PrintList(Position);
int AddPoly(Position, Position, Position);
int MulPoly(Position, Position, Position);

int main(int argc, char *argv[])
{
	int err = SUCCESS;
	struct _Cvor P1, P2, S, M;

	P1.Next = NULL;
	P2.Next = NULL;
	S.Next = NULL;
	M.Next = NULL;

	err = PrintMenu();
	if (err) return err;

	printf("\r\n\t************************************");
	printf("\r\n\tPolinom 1.");
	err = ReadFromFile(&P1);
	if (err) return err;
	printf("\r\n\tP1(x) = ");
	PrintList(P1.Next);

	printf("\r\n\t************************************");
	printf("\r\n\tPolinom 2.");
	err = ReadFromFile(&P2);
	if (err) return err;
	printf("\r\n\tP2(x) = ");
	PrintList(P2.Next);

	printf("\r\n\t************************************");
	printf("\r\n\tSuma polinoma");
	printf("\r\n\tS(x) = ");
	AddPoly(P1.Next, P2.Next, &S);
	if (err) return err;
	PrintList(S.Next);

	printf("\r\n\t************************************");
	printf("\r\n\tUmnozak polinoma");
	printf("\r\n\tM(x) = ");
	MulPoly(P1.Next, P2.Next, &M);
	if (err) return err;
	PrintList(M.Next);

	printf("\r\n\t************************************");
	printf("\r\n\r\n");
	return err;
}

int PrintMenu()
{
	printf("\r\n\t************************************");
	printf("\r\n\t***   VJEZBA BR. 04  -  Polinomi ***");
	printf("\r\n\t************************************");
	printf("\r\n\r\n");

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
		printf("\r\n\tGRESKA!\r\n\t%s datoteka nije otvorena.", fileName);
		return ERROR;
	}

	while (!feof(fp))
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\n\tGRESKA!\r\n\tMemorija nije alocirana.");
			break;
		}

		fscanf(fp, " %d %d", &q->koeficijent, &q->eksponent);

		tmp = P;
		while (tmp->Next != NULL && tmp->Next->eksponent > q->eksponent)
			tmp = tmp->Next;

		if (tmp->Next != NULL && tmp->Next->eksponent == q->eksponent)
		{
			tmp->Next->koeficijent += q->koeficijent;
			free(q);
			if (tmp->Next->koeficijent == 0)
			{
				q = tmp->Next;
				tmp->Next = q->Next;

				free(q);
			}
		}
		else
		{
			q->Next = tmp->Next;
			tmp->Next = q;
		}

	}
	fclose(fp);

	return err;
}

int PrintList(Position P)
{
	int first = 1;
	while (P != NULL)
	{
		if (first)
		{
			printf("%dx^%d", P->koeficijent, P->eksponent);
			first = 0;
		}
		else
		{
			if (P->koeficijent > 0)
				printf(" +%dx^%d", P->koeficijent, P->eksponent);
			else
				printf(" %dx^%d", P->koeficijent, P->eksponent);
		}

		P = P->Next;
	}

	return SUCCESS;
}

int AddPoly(Position P1, Position P2, Position S)
{
	int err = SUCCESS;
	Position q = NULL;
	Position tmp = NULL;

	while (P1 != NULL && P2 != NULL)
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
			break;
		}

		if (P1->eksponent > P2->eksponent)
		{
			q->eksponent = P1->eksponent;
			q->koeficijent = P1->koeficijent;
			P1 = P1->Next;
		}
		else if (P1->eksponent < P2->eksponent)
		{
			q->eksponent = P2->eksponent;
			q->koeficijent = P2->koeficijent;
			P2 = P2->Next;
		}
		else
		{
			q->eksponent = P1->eksponent;
			q->koeficijent = P1->koeficijent + P2->koeficijent;
			P1 = P1->Next;
			P2 = P2->Next;
		}

		q->Next = S->Next;
		S->Next = q;

		S = q;
	}

	if (err) return err;

	if (P1 == NULL)
		tmp = P2;
	else
		tmp = P1;

	while (tmp != NULL)
	{
		err = CreateNew(&q);
		if (err)
		{
			printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
			break;
		}

		q->eksponent = tmp->eksponent;
		q->koeficijent = tmp->koeficijent;

		q->Next = S->Next;
		S->Next = q;

		S = q;

		tmp = tmp->Next;
	}

	return err;
}

int MulPoly(Position P1, Position P2, Position M)
{
	int err = SUCCESS;
	Position q = NULL; 
	Position  tmp = NULL;
	Position i = NULL;
	Position j = NULL;

	i = P1;

	while (i != NULL)
	{
		j = P2;
		while (j != NULL)
		{
			err = CreateNew(&q);
			if (err)
			{
				printf("\r\nGRESKA!\r\nMemorija nije alocirana.");
				break;
			}

			q->eksponent = i->eksponent + j->eksponent;
			q->koeficijent = i->koeficijent * j->koeficijent;

			tmp = M;

			while (tmp->Next != NULL && tmp->Next->eksponent > q->eksponent)
				tmp = tmp->Next;

			if (tmp->Next != NULL && tmp->Next->eksponent == q->eksponent)
			{
				tmp->Next->koeficijent += q->koeficijent;
				free(q);

				if (tmp->Next->koeficijent == 0)
				{
					q = tmp->Next;
					tmp->Next = q->Next;

					free(q);
				}
			}
			else
			{
				q->Next = tmp->Next;
				tmp->Next = q;
			}

			j = j->Next;
		}
		i = i->Next;
	}

	return err;
}