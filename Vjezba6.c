#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Artikl {
    char naziv[128];
    int kolicina;
    double cijena;
    struct Artikl *next;
} Artikl;

typedef struct Racun {
    char datum[11];      /* YYYY-MM-DD */
    Artikl *artikli;
    struct Racun *next;
} Racun;

void trim(char *s) {
    int i, start = 0, end = (int)strlen(s) - 1;
    while (end >= 0 && (s[end] == '\n' || s[end] == '\r' || isspace((unsigned char)s[end])))
        end--;
    s[end + 1] = '\0';
    while (s[start] != '\0' && isspace((unsigned char)s[start]))
        start++;
    if (start > 0)
        memmove(s, s + start, strlen(s + start) + 1);
}

Artikl *stvoriArtikl(const char *naziv, int kolicina, double cijena) {
    Artikl *a = (Artikl *)malloc(sizeof(Artikl));
    if (!a) {
        perror("malloc");
        exit(1);
    }
    strncpy(a->naziv, naziv, sizeof(a->naziv) - 1);
    a->naziv[sizeof(a->naziv) - 1] = '\0';
    a->kolicina = kolicina;
    a->cijena = cijena;
    a->next = NULL;
    return a;
}

Racun *stvoriRacun(const char *datum) {
    Racun *r = (Racun *)malloc(sizeof(Racun));
    if (!r) {
        perror("malloc");
        exit(1);
    }
    strncpy(r->datum, datum, sizeof(r->datum) - 1);
    r->datum[sizeof(r->datum) - 1] = '\0';
    r->artikli = NULL;
    r->next = NULL;
    return r;
}

void dodajArtiklSortirano(Artikl **head, Artikl *novi) {
    Artikl *p = *head, *prev = NULL;

    while (p && strcmp(p->naziv, novi->naziv) < 0) {
        prev = p;
        p = p->next;
    }

    if (prev == NULL) {
        novi->next = *head;
        *head = novi;
    } else {
        novi->next = p;
        prev->next = novi;
    }
}

void dodajRacunSortirano(Racun **head, Racun *novi) {
    Racun *p = *head, *prev = NULL;

    while (p && strcmp(p->datum, novi->datum) < 0) {
        prev = p;
        p = p->next;
    }

    if (prev == NULL) {
        novi->next = *head;
        *head = novi;
    } else {
        novi->next = p;
        prev->next = novi;
    }
}

void ucitajRacune(const char *imePopisa, Racun **head) {
    FILE *f = fopen(imePopisa, "r");
    if (!f) {
        printf("Ne mogu otvoriti %s\n", imePopisa);
        exit(1);
    }

    char linija[256];
    while (fgets(linija, sizeof(linija), f)) {
        trim(linija);
        if (linija[0] == '\0')
            continue;

        FILE *fr = fopen(linija, "r");
        if (!fr) {
            printf("Ne mogu otvoriti racun %s\n", linija);
            continue;
        }

        char datum[64];
        if (!fgets(datum, sizeof(datum), fr)) {
            fclose(fr);
            continue;
        }
        trim(datum);

        Racun *r = stvoriRacun(datum);

        char redak[256];
        while (fgets(redak, sizeof(redak), fr)) {
            trim(redak);
            if (redak[0] == '\0')
                continue;

            char *tok;
            char naziv[128];
            int kolicina;
            double cijena;

            tok = strtok(redak, ",");
            if (!tok) continue;
            strncpy(naziv, tok, sizeof(naziv) - 1);
            naziv[sizeof(naziv) - 1] = '\0';
            trim(naziv);

            tok = strtok(NULL, ",");
            if (!tok) continue;
            trim(tok);
            kolicina = atoi(tok);

            tok = strtok(NULL, ",");
            if (!tok) continue;
            trim(tok);
            cijena = atof(tok);

            Artikl *a = stvoriArtikl(naziv, kolicina, cijena);
            dodajArtiklSortirano(&r->artikli, a);
        }

        fclose(fr);
        dodajRacunSortirano(head, r);
    }

    fclose(f);
}

void upit(Racun *head) {
    char trazeni[128];
    char odDatum[16], doDatum[16];

    printf("Unesi naziv artikla: ");
    if (!fgets(trazeni, sizeof(trazeni), stdin)) return;
    trim(trazeni);

    printf("Unesi pocetni datum (YYYY-MM-DD): ");
    if (!fgets(odDatum, sizeof(odDatum), stdin)) return;
    trim(odDatum);

    printf("Unesi krajnji datum (YYYY-MM-DD): ");
    if (!fgets(doDatum, sizeof(doDatum), stdin)) return;
    trim(doDatum);

    long ukupnaKolicina = 0;
    double ukupnoNovca = 0.0;

    Racun *r = head;
    while (r) {
        if (strcmp(r->datum, odDatum) >= 0 && strcmp(r->datum, doDatum) <= 0) {
            Artikl *a = r->artikli;
            while (a) {
                if (strcmp(a->naziv, trazeni) == 0) {
                    ukupnaKolicina += a->kolicina;
                    ukupnoNovca += a->kolicina * a->cijena;
                }
                a = a->next;
            }
        }
        r = r->next;
    }

    printf("Artikl: %s\n", trazeni);
    printf("Ukupno kupljena kolicina: %ld\n", ukupnaKolicina);
    printf("Ukupno potrosen novac: %.2f\n", ukupnoNovca);
}

void obrisiSve(Racun *head) {
    while (head) {
        Racun *r = head;
        head = head->next;
        Artikl *a = r->artikli;
        while (a) {
            Artikl *t = a;
            a = a->next;
            free(t);
        }
        free(r);
    }
}

int main(void) {
    Racun *lista = NULL;

    ucitajRacune("racuni.txt", &lista);
    upit(lista);
    obrisiSve(lista);

    return 0;
}