//Vjezba 1

#include <stdio.h>
#include <stdlib.h>
typedef struct_student{
    chat ime[50];
    chat prezime[50];
    int bodovi;
}stud;

int main(){
    int i=0;
    stud*studenti=NULL;
    studenti=(stud*)malloc(brojac*sizeof(stud));
    if(!dat){
        free(studenti);
    }
    FILE* fp;
    fp=fopen("popis.txt","r");
    if(!fp){
        printf("greška");
        return-1;
    }
    char bufer[50]={0};
    int brojac =0;
    while(!feof){
        fscanf(fp,"%s,%s,%d",studenti[i].ime,studenti[i].prezime,&studenti[i].bodovi);
        
    fgets(buffer,velicijna(50),fp);
    brojac++;

    }
}