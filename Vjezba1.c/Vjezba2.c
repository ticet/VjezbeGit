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
Position FindPrevPrez(Position);
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
                printf("\nTraženi student je:\n%s %s %d\n", studentF->ime, studentF->prezime, studentF->godina_rodenja);
            else
                printf("\nStudent s traženim prezimenom nije pronađen\n");
        }
        else if (strcmp(izbor, "5") == 0)
        {
            DeletePrez(&head);
            PrintList(head.Next);
        }
        else if (strcmp(izbor, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("\nPogrešan izbor <%s>. Pokušajte ponovno.\n", izbor);
        }
    }

    free(izbor);
    GarbageCollector(head.Next);

    return SUCCESS;
}
int PrintMenu()
{
    printf("\n=============================\n");
    printf("1 - Dodaj osobu na pocetak\n");
    printf("2 - Ispisi listu\n");
    printf("3 - Dodaj osobu na kraj\n");
    printf("4 - Pronadi po prezimenu\n");
    printf("5 - Izbrisi po prezimenu\n");
    printf("EXIT - Izlaz iz programa\n");
    printf("=============================\n");
    return SUCCESS;
}

int CreateNew(Position *NewElement)
{
    *NewElement = (Position)malloc(sizeof(_STUDENT));
    if (*NewElement == NULL)
    {
        printf("\nGreska pri alokaciji memorije!\n");
        return ERROR;
    }

    printf("Unesite ime: ");
    scanf(" %s", (*NewElement)->ime);
    printf("Unesite prezime: ");
    scanf(" %s", (*NewElement)->prezime);
    printf("Unesite godinu rodenja: ");
    scanf("%d", &(*NewElement)->godina_rodenja);

    (*NewElement)->Next = NULL;
    return SUCCESS;
}

int InsertUserData(Position q)
{
    printf("Unesite ime: ");
    scanf(" %s", q->ime);
    printf("Unesite prezime: ");
    scanf(" %s", q->prezime);
    printf("Unesite godinu rodenja: ");
    scanf("%d", &q->godina_rodenja);
    q->Next = NULL;
    return SUCCESS;
}

int Insert(Position P)
{
    Position NewElement = NULL;
    CreateNew(&NewElement);

    NewElement->Next = P->Next;
    P->Next = NewElement;

    return SUCCESS;
}

int PrintList(Position first)
{
    if (first == NULL)
    {
        printf("\nLista je prazna.\n");
        return SUCCESS;
    }

    printf("\nSADRZAJ LISTE:\n");
    while (first != NULL)
    {
        printf("%s %s %d\n", first->ime, first->prezime, first->godina_rodenja);
        first = first->Next;
    }

    return SUCCESS;
}
Position FindLast(Position head)
{
    Position temp = head;

    while (temp->Next != NULL)
        temp = temp->Next;

    return temp;
}

int InsertEnd(Position head)
{
    Position newElement = NULL;
    CreateNew(&newElement);

    Position last = FindLast(head);
    newElement->Next = last->Next;
    last->Next = newElement;

    return SUCCESS;
}

Position FindPrez(Position first)
{
    char prezime[128];
    printf("Unesite prezime koje trazite: ");
    scanf(" %s", prezime);

    while (first != NULL)
    {
        if (strcmp(first->prezime, prezime) == 0)
            return first;
        first = first->Next;
    }

    return NULL;
}

Position FindPrevPrez(Position head)
{
    Position prev = head;
    Position current = head->Next;
    char prezime[128];

    printf("Unesite prezime osobe koju zelite izbrisati: ");
    scanf(" %s", prezime);

    while (current != NULL && strcmp(current->prezime, prezime) != 0)
    {
        prev = current;
        current = current->Next;
    }

    if (current == NULL)
        return NULL;

    return prev;
}

int DeletePrez(Position head)
{
    Position prev = FindPrevPrez(head);
    Position temp = NULL;

    if (prev == NULL)
    {
        printf("Osoba s unesenim prezimenom nije pronadena.\n");
        return ERROR;
    }

    temp = prev->Next;
    prev->Next = temp->Next;
    free(temp);

    printf("Osoba je uspjesno izbrisana.\n");
    return SUCCESS;
}

int GarbageCollector(Position head)
{
    Position temp = NULL;

    while (head != NULL)
    {
        temp = head;
        head = head->Next;
        free(temp);
    }

    return SUCCESS;
}