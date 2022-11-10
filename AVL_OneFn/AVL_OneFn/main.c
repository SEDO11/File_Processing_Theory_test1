#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

#define MAX_TEST_COUNT 9
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
    if (*root == NULL) {
        printf("cnt: %d", cnt);
        *root = AVL_create_node(new_node->key);
        cnt++;
    }

    if (new_node->key < (*root)->key)
        (*root)->left = new_node;
    else if ((*root)->key > new_node->key)
        (*root)->right = new_node;
    else
        return -1;

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));

    int balance = getBalance(*root);
    if (balance > 1 && new_node->key < (*root)->left->key)
        rightRotate(*root);

    if (balance < -1 && new_node->key >(*root)->right->key)
        leftRotate(*root);

    if (balance > 1 && new_node->key > (*root)->left->key) {
        (*root)->left = leftRotate((*root)->left);
        rightRotate(*root);
    }

    if (balance < -1 && new_node->key < (*root)->right->key) {
        (*root)->right = rightRotate((*root)->right);
        leftRotate(*root);
    }

    return 0;
}

void AVL_print_keys_preorder(struct AVL_Node* root) { // ���� ��ȸ ���
    if (root != NULL) {
        printf("%d\t", root->key); // Ű�� ���
        AVL_print_keys_preorder(root->left, root->key, root->right); // ���� ����Ʈ���� �̵�
        // AVL_print_keys_Preorder(root->right); // ������ ����Ʈ���� �̵�
    }
}

void mytest(void)
{
	struct AVL_Node* mytree = NULL;

	int rand_key = 0;
	int i;

	// ���� �õ� �ʱ�ȭ 
	// srand((unsigned int)time(NULL));
	for (i = 1; i < MAX_TEST_COUNT; i++) {
		struct AVL_Node* new_node = NULL;

		// ���� Ű ���� 
		// rand_key = rand() % MAX_KEY_RANGE;

		// ��� ���� 
		new_node = AVL_create_node(i);
        // printf("%d ", i);

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