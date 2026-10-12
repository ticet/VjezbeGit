#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct City {
    char name[64];
    int population;
    struct City *left;
    struct City *right;
} City;

typedef struct Country {
    char name[64];
    City *cities;
    struct Country *next;
} Country;

static City* create_city(char *name, int population) {
    City *c = malloc(sizeof(City));
    strcpy(c->name, name);
    c->population = population;
    c->left = NULL;
    c->right = NULL;
    return c;
}

City* insert_city(City *root, char *name, int population) {
    if (!root) return create_city(name, population);

    if (population < root->population ||
       (population == root->population && strcmp(name, root->name) < 0))
        root->left = insert_city(root->left, name, population);
    else
        root->right = insert_city(root->right, name, population);

    return root;
}

static Country* create_country(char *name) {
    Country *c = malloc(sizeof(Country));
    strcpy(c->name, name);
    c->cities = NULL;
    c->next = NULL;
    return c;
}

Country* insert_country(Country *head, char *name) {
    if (!head || strcmp(name, head->name) < 0) {
        Country *c = create_country(name);
        c->next = head;
        return c;
    }

    Country *cur = head;
    while (cur->next && strcmp(name, cur->next->name) > 0)
        cur = cur->next;

    Country *c = create_country(name);
    c->next = cur->next;
    cur->next = c;

    return head;
}

void load_cities(Country *c) {
    char filename[128];
    sprintf(filename, "%s.txt", c->name);

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char city[64];
    int pop;

    while (fscanf(fp, "%s %d", city, &pop) == 2)
        c->cities = insert_city(c->cities, city, pop);

    fclose(fp);
}

void print_cities(City *root) {
    if (!root) return;
    print_cities(root->left);
    printf("    %s %d\n", root->name, root->population);
    print_cities(root->right);
}

void print_countries(Country *head) {
    while (head) {
        printf("%s\n", head->name);
        print_cities(head->cities);
        head = head->next;
    }
}

void search_cities(City *root, int min) {
    if (!root) return;
    search_cities(root->left, min);
    if (root->population > min)
        printf("%s %d\n", root->name, root->population);
    search_cities(root->right, min);
}

int main(void) {
    FILE *fp = fopen("drzave.txt", "r");
    if (!fp) return 1;

    Country *head = NULL;
    char name[64];

    while (fscanf(fp, "%s", name) == 1)
        head = insert_country(head, name);

    fclose(fp);

    Country *cur = head;
    while (cur) {
        load_cities(cur);
        cur = cur->next;
    }

    print_countries(head);

    char search[64];
    int min;

    printf("Drzava: ");
    scanf("%s", search);
    printf("Minimalan broj stanovnika: ");
    scanf("%d", &min);

    cur = head;
    while (cur && strcmp(cur->name, search) != 0)
        cur = cur->next;

    if (cur)
        search_cities(cur->cities, min);

    return 0;
}