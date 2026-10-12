#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

static Node* create_node(int value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node* insert(Node *root, int value) {
    if (root == NULL) return create_node(value);

    Node *cur = root;
    while (1) {
        if (value >= cur->value) {
            if (cur->left == NULL) {
                cur->left = create_node(value);
                break;
            }
            cur = cur->left;
        } else {
            if (cur->right == NULL) {
                cur->right = create_node(value);
                break;
            }
            cur = cur->right;
        }
    }
    return root;
}

static int replace_rec(Node *root) {
    if (root == NULL) return 0;

    int sumL = replace_rec(root->left);
    int sumR = replace_rec(root->right);

    int old = root->value;
    root->value = sumL + sumR;

    return old + root->value;
}

void replace(Node *root) {
    replace_rec(root);
}

static void inorder_to_file(Node *root, FILE *fp) {
    if (!root) return;
    inorder_to_file(root->left, fp);
    fprintf(fp, "%d ", root->value);
    inorder_to_file(root->right, fp);
}

static void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int n = 10;
    Node *root = NULL;

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        int x = 10 + rand() % 80;
        root = insert(root, x);
    }

    FILE *fp = fopen("inorder.txt", "w");
    if (!fp) {
        free_tree(root);
        return 1;
    }

    inorder_to_file(root, fp);
    fprintf(fp, "\n");

    replace(root);

    inorder_to_file(root, fp);
    fprintf(fp, "\n");

    fclose(fp);
    free_tree(root);

    return 0;
}