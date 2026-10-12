#include <stdio.h>
#include <stdlib.h>

typedef struct _student {
    char ime[50];
    char prezime[50];
    int bodovi;
} stud;

int main() {
    int i, brojac = 0;
    int max_bodova;
    stud *studenti = NULL;
    FILE *fp;
    char bufer[200];

    fp = fopen("popis.txt", "r");
    if (!fp) {
        printf("Greska pri otvaranju datoteke\n");
        return -1;
    }

    while (fgets(bufer, sizeof(bufer), fp) != NULL) {
        brojac++;
    }

    //alokacija niza struktura //
    studenti = (stud *)malloc(brojac * sizeof(stud));
    if (!studenti) {
        printf("Greska pri alokaciji\n");
        fclose(fp);
        return -1;
    }

    // vracamo se na pocetak datoteke i ucitavamo zapise //
    rewind(fp);
    for (i = 0; i < brojac; i++) {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }
    fclose(fp);

    printf("Unesite maksimalan broj bodova: ");
    scanf("%d", &max_bodova);

    printf("\n%-15s %-15s %-10s %-10s\n", "Ime", "Prezime", "Bodovi", "Relativno");
    for (i = 0; i < brojac; i++) {
        double relativno = (double)studenti[i].bodovi / max_bodova * 100;
        printf("%-15s %-15s %-10d %.2f%%\n",
               studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativno);
    }

    free(studenti);
    return 0;
}