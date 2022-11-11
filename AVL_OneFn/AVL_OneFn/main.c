#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

#define MAX_TEST_COUNT 5
#define MAX_KEY_RANGE 100

int cnt = 1;

struct AVL_Node
{
    int key;
    struct AVL_Node* left;
    struct AVL_Node* right;
    int height;
    // �ٸ� ��� �߰� ���� 
};

// ���� ���
int height(struct AVL_Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// AVL ����
struct AVL_Node* AVL_create_node(int key) { // ��� ����
    struct AVL_Node* node = (struct AVL_Node*)malloc(sizeof(struct AVL_Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

// ������ ȸ��
struct AVL_Node* rightRotate(struct AVL_Node* y) {
    struct AVL_Node* x = y->left;
    struct AVL_Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// ���� ȸ��
struct AVL_Node* leftRotate(struct AVL_Node* x) {
    struct AVL_Node* y = x->right;
    struct AVL_Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// �뷱�� ���� (-1, 0, 1) �̸� ȸ�� x
int getBalance(struct AVL_Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


int AVL_insert_node(struct AVL_Node** root, struct AVL_Node* new_node) { // ��� ����
    if (!*root) {
        *root = calloc(sizeof * *root, 1);
        (*root)->key = new_node->key;
        (*root)->height = 1;
    }
    else if (new_node->key < (*root)->key) {
        AVL_insert_node(&(*root)->left, new_node);
        (*root)->height = (*root)->left->height + 1;
        getBalance(*root);
    }
    else if (new_node->key > (*root)->key) {
        AVL_insert_node(&(*root)->right, new_node);
        (*root)->height = (*root)->right->height + 1;
        getBalance(*root);
    }

    return 0;
}

void AVL_print_keys_preorder(struct AVL_Node* root) { // ���� ��ȸ ���
    if (root != NULL) {
        printf("%d\t", root->key); // Ű�� ���
        AVL_print_keys_preorder(root->left); // ���� ����Ʈ���� �̵�
        AVL_print_keys_Preorder(root->right); // ������ ����Ʈ���� �̵�
    }
}

void mytest(void)
{
	struct AVL_Node* mytree = NULL;

	int rand_key = 0;
	int i;

	// ���� �õ� �ʱ�ȭ 
	srand((unsigned int)time(NULL));
	for (i = 0; i < MAX_TEST_COUNT; i++) {
		struct AVL_Node* new_node = NULL;

		// ���� Ű ���� 
		rand_key = rand() % MAX_KEY_RANGE;

		// ��� ���� 
		new_node = AVL_create_node(rand_key);
        printf("%d %d\n", rand_key, i);

		// ��� ����
		if (AVL_insert_node(&mytree, new_node) != 0) {
			// ���� ���н� ������ ��� ���� 
			free(new_node);
		}
	}

	if (mytree) {
		AVL_print_keys_preorder(mytree);
	}
}

int main(void)
{
	mytest();

	return 0;
}