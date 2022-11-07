#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "AVL.h"

// AVL ����
struct AVL_Node* AVL_create_node(int key) { // ��� ����
    struct AVL_Node* node = (struct AVL_Node*)malloc(sizeof(struct AVL_Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

int AVL_insert_node(struct AVL_Node** root, struct AVL_Node* new_node) { // ��� ����
    if (root == NULL)
        return (new_node->key);

    if (new_node->key < root)
        node->left = insertNode(node->left, key);
    else if (root > new_node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left),
        height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVL_print_keys_preorder(struct AVL_Node* root) { // ���� ��ȸ ���
    if (root != NULL) {
        printf("%d\t", root->key); // Ű�� ���
        AVL_print_keys_Preorder(root->left); // ���� ����Ʈ���� �̵�
        AVL_print_keys_Preorder(root->right); // ������ ����Ʈ���� �̵�
    }
}