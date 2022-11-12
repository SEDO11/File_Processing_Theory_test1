#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "AVL.h"


// AVL ����

// ���� ���
int height(struct AVL_Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// �뷱�� ���� ���
int getBalance(struct AVL_Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// ������ ȸ��
struct AVL_Node* rightRotate(struct AVL_Node* b) {
    struct AVL_Node* a = b->left;
    struct AVL_Node* t = a->right;

    a->right = b;
    b->left = t;

    b->height = max(height(b->left), height(b->right)) + 1;
    a->height = max(height(a->left), height(a->right)) + 1;

    return a;
}

// ���� ȸ��
struct AVL_Node* leftRotate(struct AVL_Node* a) {
    struct AVL_Node* b = a->right;
    struct AVL_Node* t = b->left;

    b->left = a;
    a->right = t;

    a->height = max(height(a->left), height(a->right)) + 1;
    b->height = max(height(b->left), height(b->right)) + 1;

    return b;
}

/*
 * AVL_Node�� �Ҵ�
 * ����ü ��� �ʱ�ȭ
 * Ű���� key�� ����
 * ������ AVL_Node ����ü�� �Ҵ�� �ּ� ��ȯ
 * ���н� NULL ��ȯ
 */
struct AVL_Node* AVL_create_node(int key) {
    struct AVL_Node* node = (struct AVL_Node*)malloc(sizeof(struct AVL_Node));
    if (node != NULL)
    {
        node->key = key;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}

/*
 * �Ķ���ͷ� �־��� AVL_Node�� AVL Ʈ���� root ��忡 ����
 * ������ 0 ��ȯ
 * ���н� (Ű�� �ִ� ���) �̿��� �� ��ȯ
 */
int AVL_insert_node(struct AVL_Node** root, struct AVL_Node* new_node) {
    if (*root == NULL) { // ó�� ���� ����
        *root = new_node;
    }
    else { // ������ ���� ����
        if (new_node->key < (*root)->key) { // �θ� ���� Ű���� ���� ���
            AVL_insert_node(&(*root)->left, new_node); // �θ��� ���ʿ� ����
        }
        else if ((new_node->key > (*root)->key)) { // �θ� ���� Ű���� ū ���
            AVL_insert_node(&(*root)->right, new_node); // �θ��� �����ʿ� ����
        }
        else { // ���� ���, ���� ���ϰ� ����
            return -1;
        }

        (*root)->height = 1 + max(height((*root)->left), height((*root)->right)); // ���� ���

        int balance = getBalance(*root); // �뷱�� ���� �� ����
        if (balance > 1 && new_node->key < (*root)->left->key) { // LL����
            *root = rightRotate((*root));
        }

        if (balance < -1 && new_node->key >(*root)->right->key) { // RR����
            *root = leftRotate((*root));
        }

        if (balance > 1 && new_node->key > (*root)->left->key) { // LR����
            (*root)->left = leftRotate((*root)->left);
            *root = rightRotate((*root));
        }

        if (balance < -1 && new_node->key < (*root)->right->key) { // RL����
            (*root)->right = rightRotate((*root)->right);
            *root = leftRotate((*root));
        }
    }

    return 0;
}

/*
 * Ʈ���� ������ȸ �ϸ� ����� Ű ���� ������� ���
 * Ű ���� ����ϰ� �� ���� (\t) �� �����Ͽ� ���� Ű�� ����
 */
void AVL_print_keys_preorder(struct AVL_Node* root) {
	if (root != NULL) {
		printf("%d\t", root->key);
		AVL_print_keys_preorder(root->left);
		AVL_print_keys_preorder(root->right);
	}
}
