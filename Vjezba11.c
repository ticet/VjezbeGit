#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

typedef struct City {
    char name[64];
    int population;
    struct City *left;
    struct City *right;
} City;

typedef struct Country {
    char name[64];
    char file[128];
    City *cities;
    struct Country *next;
} Country;

static int hash_key(const char *s) {
    int sum = 0;
    for (int i = 0; i < 5 && s[i] != '\0'; i++) sum += (unsigned char)s[i];
    return sum % TABLE_SIZE;
}

static int city_cmp(int p1, const char *n1, int p2, const char *n2) {
    if (p1 < p2) return -1;
    if (p1 > p2) return 1;
    return strcmp(n1, n2);
}

static City* create_city(const char *name, int pop) {
    City *c = (City*)malloc(sizeof(City));
    if (!c) return NULL;
    strcpy(c->name, name);
    c->population = pop;
    c->left = NULL;
    c->right = NULL;
    return c;
}

static City* insert_city(City *root, const char *name, int pop) {
    if (!root) return create_city(name, pop);

    int cmp = city_cmp(pop, name, root->population, root->name);
    if (cmp < 0) root->left = insert_city(root->left, name, pop);
    else root->right = insert_city(root->right, name, pop);

    return root;
}

static Country* create_country(const char *name, const char *file) {
    Country *c = (Country*)malloc(sizeof(Country));
    if (!c) return NULL;
    strcpy(c->name, name);
    strcpy(c->file, file);
    c->cities = NULL;
    c->next = NULL;
    return c;
}

static void load_cities(Country *c) {
    FILE *fp = fopen(c->file, "r");
    if (!fp) return;

    char city[64];
    int pop;

    while (fscanf(fp, "%63s %d", city, &pop) == 2)
        c->cities = insert_city(c->cities, city, pop);

    fclose(fp);
}

static void insert_country_sorted(Country **head, const char *name, const char *file) {
    Country *n = create_country(name, file);
    if (!n) return;

    if (!(*head) || strcmp(name, (*head)->name) < 0) {
        n->next = *head;
        *head = n;
    } else {
        Country *cur = *head;
        while (cur->next && strcmp(name, cur->next->name) > 0)
            cur = cur->next;
        n->next = cur->next;
        cur->next = n;
    }

    load_cities(n);
}

static void print_cities_inorder(City *root) {
    if (!root) return;
    print_cities_inorder(root->left);
    printf("    %s %d\n", root->name, root->population);
    print_cities_inorder(root->right);
}

static void print_table(Country *table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("[%d]\n", i);
        Country *cur = table[i];
        while (cur) {
            printf("%s\n", cur->name);
            print_cities_inorder(cur->cities);
            cur = cur->next;
        }
    }
}

static void print_cities_greater(City *root, int min) {
    if (!root) return;

    if (root->population <= min) {
        print_cities_greater(root->right, min);
    } else {
        print_cities_greater(root->left, min);
        printf("%s %d\n", root->name, root->population);
        print_cities_greater(root->right, min);
    }
}

static Country* find_country(Country *table[], const char *name) {
    int idx = hash_key(name);
    Country *cur = table[idx];
    while (cur) {
        int c = strcmp(cur->name, name);
        if (c == 0) return cur;
        if (c > 0) break;
        cur = cur->next;
    }
    return NULL;
}

static void free_cities(City *root) {
    if (!root) return;
    free_cities(root->left);
    free_cities(root->right);
    free(root);
}

static void free_table(Country *table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Country *cur = table[i];
        while (cur) {
            Country *tmp = cur->next;
            free_cities(cur->cities);
            free(cur);
            cur = tmp;
        }
        table[i] = NULL;
    }
}

int main(void) {
    Country *table[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL;

    FILE *fp = fopen("drzave.txt", "r");
    if (!fp) return 1;

    char country[64];
    char file[128];

    while (fscanf(fp, "%63s %127s", country, file) == 2) {
        int idx = hash_key(country);
        insert_country_sorted(&table[idx], country, file);
    }

    fclose(fp);

    print_table(table);

    char query[64];
    int min;

    printf("Drzava: ");
    if (scanf("%63s", query) != 1) {
        free_table(table);
        return 0;
    }

    printf("Minimalan broj stanovnika: ");
    if (scanf("%d", &min) != 1) {
        free_table(table);
        return 0;
    }

    Country *c = find_country(table, query);
    if (c) print_cities_greater(c->cities, min);

    free_table(table);
    return 0;
}