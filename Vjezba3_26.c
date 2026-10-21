#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person* Position;
typedef struct _Person {
    char name[50];
    char lastname[50];
    int year;
    Position next;
} Person;

Position createPerson(char* ime, char* prezime, int god) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greska pri alokaciji\n");
        return NULL;
    }
    strcpy(newPerson->name, ime);
    strcpy(newPerson->lastname, prezime);
    newPerson->year = god;
    newPerson->next = NULL;
    return newPerson;
}

int insertAfter(Position p, Position newP) {
    newP->next = p->next;
    p->next = newP;
    return 0;
}

// dodavanje na pocetak 
int insertBegin(Position head, char* ime, char* prezime, int god) {
    Position newPerson = createPerson(ime, prezime, god);
    if (!newPerson) return -1;

    insertAfter(head, newPerson);
    return 0;
}

// ispis liste 
int printList(Position p) {
    p = p->next;
    if (!p) {
        printf("Lista je prazna\n");
        return 0;
    }
    while (p != NULL) {
        printf("%s %s, %d\n", p->name, p->lastname, p->year);
        p = p->next;
    }
    return 0;
}

// dodavanje na kraj 
int insertEnd(Position head, char* ime, char* prezime, int god) {
    Position newPerson = createPerson(ime, prezime, god);
    Position p = head;
    if (!newPerson) return -1;

    while (p->next != NULL) {
        p = p->next;
    }
    insertAfter(p, newPerson);
    return 0;
}

// trazenje po prezimenu 
Position findLastN(Position p, char* prezime) {
    for (p = p->next; p != NULL; p = p->next) {
        if (strcmp(p->lastname, prezime) == 0)
            return p;
    }
    return NULL;
}

// brisanje po prezimenu 
int deletePerson(Position p, char* prezime) {
    Position prev = p;
    Position curr = findLastN(p, prezime);

    if (!curr) return -1;

    while (prev->next != curr) {
        prev = prev->next;
    }
    prev->next = curr->next;
    free(curr);
    return 0;
}

int freeList(Position head) {
    Position temp;
    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return 0;
}

int main() {
    Person Head = { .name = {0}, .lastname = "", .year = 0, .next = NULL };
    Position found = NULL;
    char ime[50], prezime[50];
    int god, izbor;

    do {
        printf("\n1 - dodaj na pocetak\n");
        printf("2 - ispisi listu\n");
        printf("3 - dodaj na kraj\n");
        printf("4 - pronadi po prezimenu\n");
        printf("5 - obrisi po prezimenu\n");
        printf("0 - izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
        case 3:
            printf("Ime: ");
            scanf("%s", ime);
            printf("Prezime: ");
            scanf("%s", prezime);
            printf("Godina rodenja: ");
            scanf("%d", &god);
            if (izbor == 1)
                insertBegin(&Head, ime, prezime, god);
            else
                insertEnd(&Head, ime, prezime, god);
            break;
        case 2:
            printList(&Head);
            break;
        case 4:
            printf("Prezime: ");
            scanf("%s", prezime);
            found = findLastN(&Head, prezime);
            if (found)
                printf("Pronadeno: %s %s, %d\n", found->name, found->lastname, found->year);
            else
                printf("Osoba nije pronadena\n");
            break;
        case 5:
            printf("Prezime: ");
            scanf("%s", prezime);
            if (deletePerson(&Head, prezime) == 0)
                printf("Osoba obrisana\n");
            else
                printf("Osoba nije pronadena\n");
            break;
        case 0:
            break;
        default:
            printf("Krivi unos\n");
        }
    } while (izbor != 0);

    freeList(&Head);
    return 0;
}