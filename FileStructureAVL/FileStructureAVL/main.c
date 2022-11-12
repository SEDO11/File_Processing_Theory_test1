#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

#include "AVL.h"

#define MAX_TEST_COUNT 10
#define MAX_KEY_RANGE 100 

void mytest(void)
{
	struct AVL_Node* mytree = NULL;

	int rand_key = 0;
	int i; 
	printf("�Է� ���: ");
	// ���� �õ� �ʱ�ȭ 
	srand((unsigned int) time(NULL));
	for (i = 0; i < MAX_TEST_COUNT; i++) {
		struct AVL_Node* new_node = NULL;

		// ���� Ű ���� 
		rand_key = rand() % MAX_KEY_RANGE;

		// ��� ���� 
		new_node = AVL_create_node(rand_key);
		printf("%d ", rand_key);

		// ��� ����
		if (AVL_insert_node(&mytree, new_node) != 0) {
			// ���� ���н� ������ ��� ���� 
			free(new_node);
		}
	}

	if (mytree) {
		printf("\nAVL ������ȸ ���:\t");
		AVL_print_keys_preorder(mytree);
	}
}

int main(void)
{
	mytest();

	return 0;
}