#ifndef __BTREE_H__
#define __BTREE_H__

#define MAX_CHILDREN 3 
#define MAX_KEYS MAX_CHILDREN-1
#define MIN_KEYS (int)(ceil(M/2.0))-1 // �ּ� Ű���� ���ϴ� ��

struct KeyPair
{
    /* B-Tree ����� ���� ������ �Ǵ� �ε��� Ű */
    int key;
    /* B-Tree ����� Ű�� ����� ���ڵ� �ּ� */
    long offset;
};

struct BTNode
{
    /* ��尡 ���� Ű�� �� */
    int num_keys;
    /* ��尡 ���� �ڽ� ����� �� */
    int num_children;
    /* Ű �迭 */
    struct KeyPair keys[MAX_KEYS];
    /* �ڽ� ��� ������ �迭 */
    struct BTNode* children[MAX_CHILDREN];
    // leaf ��� ����
    bool leaf;
};

struct BTNode* splitNode(int pos, struct BTNode* node, struct BTNode* parent);

struct BTNode* insertNode(int parent_pos, int val, struct BTNode* node, struct BTNode* parent);



/*
 * B-Tree ��带 ����
 * �޸𸮸� �Ҵ��ϰ� ����� �ʱ�ȭ �� ��
 * �Ҵ�� �޸� �ּ� ��ȯ
 */
struct BTNode* BT_create_node(void);

/*
 * Ʈ���� ��Ʈ���� �����Ͽ� �־��� Ű�� ������ ��带 Ž��
 * Ž�� ���� �� �ش� ����� �ּҸ� ��ȯ
 * Ž�� ���� �� NULL ������ ��ȯ
 */
struct BTNode* BT_search_key(struct BTNode* root, struct KeyPair k);

/*
 * Ʈ���� ��Ʈ���� �����Ͽ� �־��� Ű-���ڵ��ּ� ���� ������ ��带 ����
 * ��Ʈ ��带 ����Ű�� ������ ������ �ּҸ� �Ķ���ͷ� ����
 * Ʈ���� �����ϰ��� �ϴ� Ű-���ڵ� ���� �Ķ���ͷ� ����
 * Ʈ���� �����ϰ��� �ϴ� Ű�� �������� ���� ��� ���ο� ��带 �Ҵ�ް� Ʈ���� ����
 * ���� �� ���뷱�� �ʿ�
 */
void BT_insert_key(struct BTNode** root, struct KeyPair k);

/*
 * Ʈ���� ��Ʈ���� �����Ͽ� �־��� Ű�� ������ ��带 ã�� ���� (�޸� ����)
 * ��� ���� �� ���뷱�� �ʿ�
 */
void BT_remove_key(struct BTNode** root, struct KeyPair k);


#endif #pragma once
