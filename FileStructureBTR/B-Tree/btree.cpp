#include "btree.h"

#include <stdlib.h>
#include <string.h> 


// �����ε� ���Ѽ� ¥����?
struct BTNode* BT_create_node(void)
{
    struct BTNode* new_node = (struct BTNode*)malloc(sizeof(struct BTNode));
    if (new_node == NULL) {
        // alloc failed 
        return NULL;
    }
    new_node->num_keys = 0;
    new_node->num_children = 0;
    memset(new_node->keys, 0x00, sizeof(new_node->keys));
    memset(new_node->children, 0x00, sizeof(struct BTNode*) * MAX_CHILDREN);
    return new_node;
}

void BT_insert_key(struct BTNode** root, struct KeyPair k)
{
    if (!*root) { // root�� ������
        *root = BT_create_node(); // root�� ������.
        (*root)->num_keys = 1;
        (*root)->keys[0] = k;
    }
    else { // ��Ʈ�� ������
        (*root)->num_keys += 1;
        if ((*root)->num_keys <= MAX_KEYS) {
            
        }
    }
}

void BT_remove_key(struct BTNode** root, struct KeyPair k)
{
    //// ��� ���� ����
    //if (!*root) { // ���� ���� ��忡 �ƿ� ���� ������
    //    printf("Empty tree!!\n");
    //    return;
    //}
    //int flag = deleteValFromNode(k.key, *root); // ���� ��� ������ ���� ����� �Լ� ȣ��. ����� ���̶� ���� ��带 ���ڷ� ����. ������ flag�� ����.
    //if (!flag) { // flag�� 0�̸� ����.
    //    printf("%d does not exist in this tree. \n", k.key); // �Լ� ������ ��ã���� �÷��װ� 0�ΰŴϱ� ���� �޼��� ���
    //    return;
    //}
    //if ((*root)->keys == 0) {  // deleteVal�� �ϰ� ���� node�� Ű������ 0�� �� = ���� ��忡 �ƹ��͵� ��� ��ȭ�� �ʿ���.
    //    *root = (*root)->children[0]; // ���� ��带 ���� ���� �ڽ� ���� ����.
    //}
    //root = &*root;
}

struct BTNode* BT_search_key(struct BTNode* root, struct KeyPair k)
{
    // ��� Ž�� ���� 
    if (!root) {
        return NULL;
    }
    while (true) {
        int pos;
        for (pos = 0; pos < root->num_keys; pos++) {
            if (k.key == root->keys[pos].key) {
                return root;
            }
            else if (k.key < root->keys[pos].key) {
                break;
            }
        }
        if (root->num_children) break;
        root = root->children[pos];
    }

    return NULL;
}