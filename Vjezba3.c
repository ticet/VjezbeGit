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

struct _student;
typedef struct _student _STUDENT;
typedef struct _student* Position;

struct _student
{
	char ime[128];
	char prezime[128];
	int godina_rodenja;
	Position Next;
};

int PrintMenu();
int CreateNew(Position *);
int InsertUserData(Position);
int Insert(Position);
int PrintList(Position);
Position FindLast(Position);
int InsertEnd(Position);
Position FindPrez(Position);
int DeletePrez(Position);
int InsertAfter(Position);
Position FindPrevPrez(Position);
int InsertBefore(Position);
int ReadFromFile(Position);
int PrintToFile(Position);
int SortList(Position);
int GarbageCollector(Position);

int main(int argc, char *argv[])
{
	_STUDENT head;
	Position studentF = NULL;
	char *izbor = NULL;

	head.Next = NULL;
	izbor = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (izbor == NULL) return ERROR;
	memset(izbor, '\0', BUFFER_LENGTH);

	while (_stricmp(izbor, "EXIT") != 0)
	{
		memset(izbor, 0, BUFFER_LENGTH);

		PrintMenu();

		scanf(" %s", izbor);

		if (strcmp(izbor, "1") == 0)
		{
			Insert(&head);
		}
		else if (strcmp(izbor, "2") == 0)
		{
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "3") == 0)
		{
			InsertEnd(&head);
		}
		else if (strcmp(izbor, "4") == 0)
		{
			studentF = FindPrez(head.Next);
			if (studentF != NULL)
				printf("\r\nTrazeni student je :\r\n%s %s %d\r\n", studentF->ime, studentF->prezime, studentF->godina_rodenja);
			else
				printf("\r\nStudent s trazenim prezimenom nije pronaden\r\n");
		}
		else if (strcmp(izbor, "5") == 0)
		{
			DeletePrez(&head);
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "6") == 0)
		{
			InsertAfter(&head);
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "7") == 0)
		{
			InsertBefore(&head);
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "8") == 0)
		{
			SortList(&head);
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "9") == 0)
		{
			ReadFromFile(&head);
			PrintList(head.Next);
		}
		else if (strcmp(izbor, "A") == 0)
		{
			PrintToFile(head.Next);
		}
		else if (strcmp(izbor, "exit") == 0)
		{//Do Nothing
			break;
		}
		else
		{
			printf("\r\nPogresan izbor <%s>.\r\nPokusajte ponovno.\r\n", izbor);
		}
	}

	free(izbor);
	GarbageCollector(head.Next);

	return SUCCESS;
}

int PrintMenu()
{
	printf("\t*********************************************\r\n");
	printf("\t***   VJEZBA BR. 02 / 03  -  Vezane liste ***\r\n");
	printf("\t*********************************************\r\n");
	printf("\r\n\r\n");
	printf("\t\t1. Unos\r\n");
	printf("\t\t2. Ispis\r\n");
	printf("\t\t3. Unos na kraj\r\n");
	printf("\t\t4. Pronadji po prezimenu\r\n");
	printf("\t\t5. Izbrisi po prezimenu\r\n");
	printf("\t\t6. Unos nakon studenta\r\n");
	printf("\t\t7. Unos prije studenta\r\n");
	printf("\t\t8. Sortiraj\r\n");
	printf("\t\t9. Ucitaj listu iz datoteke\r\n");
	printf("\t\tA. Ispisi listu u datoteku\r\n");
	printf("\t\tExit. Izlaz iz programa\r\n");
	printf("\r\n\tIzbor : ");

	return SUCCESS;
}

int CreateNew(Position *head)
{
	Position q = NULL;

	*head = (Position)malloc(sizeof(_STUDENT));
	if (*head == NULL) return ERROR;

	q = *head;
	q->Next = NULL;

	return SUCCESS;
}

int InsertUserData(Position P)
{
	if (P == NULL) return ERROR;

	printf("\r\n\tUnesite ime, prezime i godinu rodenja studenta :\r\n\t");
	scanf(" %s %s %d", P->ime, P->prezime, &P->godina_rodenja);

	return SUCCESS;
}

int Insert(Position P)
{
	int err = ERROR;
	Position q = NULL;

	err = CreateNew(&q);
	if (err)
	{
		printf("\r\nGRESKA!\r\nAlokacija memorije neuspjela.\r\n");
		return err;
	}

	err = InsertUserData(q);
	if (err)
	{
		printf("\r\nGRESKA!\r\nUnos korisnickih podataka.\r\n");
		return err;
	}

	q->Next = P->Next;
	P->Next = q;

	return SUCCESS;
}

int PrintList(Position P)
{
	if (P == NULL)
		printf("\r\n\tLista je prazna!\r\n");
	else
	{
		printf("\r\n\r\n\tU listi se nalaze : \r\n");
		while (P != NULL)
		{
			printf("\t\t%s %s %d\r\n", P->ime, P->prezime, P->godina_rodenja);
			P = P->Next;
		}
	}
	printf("\r\n\r\n");

	return SUCCESS;
}

Position FindLast(Position P)
{
	if (P == NULL) return NULL;

	while (P->Next != NULL)
		P = P->Next;
	return P;
}

int InsertEnd(Position P)
{
	P = FindLast(P);
	return Insert(P);
}

Position FindPrez(Position P)
{
	char *prez = NULL;

	prez = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (prez == NULL) return NULL;
	memset(prez, '\0', BUFFER_LENGTH);

	printf("\r\n\tUnesite prezime koje trazite <max %d znakova>: ", BUFFER_LENGTH - 1);
	scanf(" %s", prez);
	while (P != NULL && strcmp(P->prezime, prez) != 0)
		P = P->Next;	

	free(prez);

	return P;
}

Position FindPrevPrez(Position P)
{
	char *prez = NULL;

	prez = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (prez == NULL) return NULL;
	memset(prez, '\0', BUFFER_LENGTH);

	printf("\r\n\tUnesite prezime koje trazite <max %d znakova>: ", BUFFER_LENGTH - 1);
	scanf(" %s", prez);
	while (P->Next != NULL && strcmp(P->Next->prezime, prez) != 0)
		P = P->Next;

	if (P->Next == NULL)
		P = NULL;

	free(prez);

	return P;
}

int DeletePrez(Position P)
{
	Position tmp = NULL;

	printf("\r\n\tBrisanje!");
	P = FindPrevPrez(P);
	if (P == NULL)
	{
		printf("\r\n\tTrazeno prezime se ne nalazi u listi!");
		return ERROR;
	}

	tmp = P->Next;
	if (tmp != NULL)
	{
		P->Next = tmp->Next;
		printf("\r\nIz liste se brise : %s %s %d", tmp->ime, tmp->prezime, tmp->godina_rodenja);
		free(tmp);
	}

	return SUCCESS;
}

int InsertAfter(Position P)
{
	P = FindPrez(P);
	if (P == NULL) return ERROR;
	
	return Insert(P);
}

int InsertBefore(Position P)
{
	P = FindPrevPrez(P);
	if (P == NULL) return ERROR;

	return Insert(P);
}

int ReadFromFile(Position P)
{
	int err = SUCCESS;
	char *fileName = NULL;
	FILE *fp = NULL;
	Position q = NULL;

	fileName = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (fileName == NULL) return ERROR;
	memset(fileName, '\0', BUFFER_LENGTH);

	printf("\r\n\tUnesite datoteku za citanje : ");
	scanf(" %s", fileName);

	if (strchr(fileName, '.') == NULL)
		strcat(fileName, ".txt");

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("\r\n\tDatoteka \"%s\" nije otvorena.", fileName);
		return ERROR;
	}

	while (!feof(fp))
	{
		err = CreateNew(&q);
		if (err) break;
			
		fscanf(fp, " %s %s %d", q->ime, q->prezime, &q->godina_rodenja);
		q->Next = P->Next;
		P->Next = q;
	}
	fclose(fp);

	return err;
}

int PrintToFile(Position P)
{
	FILE *fp = NULL;
	char *fileName = NULL;

	fileName = (char*)malloc(sizeof(char) * BUFFER_LENGTH);
	if (fileName == NULL) return ERROR;
	memset(fileName, '\0', BUFFER_LENGTH);

	printf("\r\n\tUnesite datoteku za upisivanje : ");
	scanf(" %s", fileName);

	if (strchr(fileName, '.') == NULL)
		strcat(fileName, ".txt");

	fp = fopen(fileName, "w+");
	if (fp == NULL)
	{
		printf("\r\n\tDatoteka \"%s\" nije otvorena.", fileName);
		return ERROR;
	}

	while (P != NULL)
	{
		fprintf(fp, "%s %s %d", P->ime, P->prezime, P->godina_rodenja);
		P = P->Next;

		if (P != NULL)
		{
			fprintf(fp, "\n");
		}
	}

	fclose(fp);

	return SUCCESS;
}

int SortList(Position P)
{
	Position i = P;
	Position j = NULL;
	Position prev_j = NULL;
	Position end = NULL;

	while (i->Next != end)
	{
		prev_j = i;
		j = prev_j->Next;

		while (j->Next != end)	
		{
			if (strcmp(j->prezime, j->Next->prezime) > 0)
			{
				prev_j->Next = j->Next;
				j->Next = j->Next->Next;
				prev_j->Next->Next = j;
				j = prev_j->Next;
			}
			prev_j = j;
			j = j->Next;
		}
		end = j;
	}	

	return SUCCESS;
}

int GarbageCollector(Position P)
{
	if (P->Next != NULL)
		GarbageCollector(P->Next);

	free(P);

	return SUCCESS;
}
