#include <stdio.h>
#include <stdlib.h>

struct Student {
    char ime[40];
    char prezime[40];
    int bodovi;
};

int main()
{
    FILE *fp;
    int br=0, i;
    struct Student *s;
    int max=0;
    char tmpIme[40], tmpPrez[40];
    int tmpBod;

    fp = fopen("studenti.txt","r");
    if(fp==NULL){
        printf("Ne mogu otvorit datoteku!\n");
        return -1;
    }

    // broji koliko ima redaka (studenata)
    while(fscanf(fp,"%s %s %d", tmpIme, tmpPrez, &tmpBod)==3){
        br++;
    }

    // printf("Broj studenata: %d\n", br); // provjera

    rewind(fp); // vrati pokazivac na pocetak

    s = (struct Student*)malloc(br * sizeof(struct Student));
    if(s==NULL){
        printf("Greska u alokaciji memorije!\n");
        fclose(fp);
        return -1;
    }

    // ucitavanje u strukturu
    for(i=0; i<br; i++){
        fscanf(fp,"%s %s %d", s[i].ime, s[i].prezime, &s[i].bodovi);
        if(s[i].bodovi > max)
            max = s[i].bodovi;
    }

    // printf("Max bodovi = %d\n", max); // kontrola

    printf("\nRezultati:\n");
    printf("-----------------------------\n");
    for(i=0; i<br; i++){
        double rel = (double)s[i].bodovi / max * 100;
        printf("%d) %s %s  ->  %d bodova  (%.2f%%)\n", i+1, s[i].ime, s[i].prezime, s[i].bodovi, rel);
    }

    free(s);
    fclose(fp);

    return 0;
}