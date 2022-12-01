#include "btree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

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

// ����� ���� �и��ؼ� �ٸ� ��忡 �й��ϴ� �Լ�
struct BTNode* splitNode(int pos, struct BTNode* node, struct BTNode* parent) { // ���� ���, �θ� ���, �����忡�� ���� ���� ��ġ�� �˰��ִ� pos�� ���ڷ� �ޱ�
    int median; // �и��� ���� �߾Ӱ� �˾ƾ� ��.
    median = node->num_keys / 2; // �߾Ӱ��� ¦���̵� Ȧ���̵� 'Ű����/2' 
    struct BTNode* right_node; // �и� �� ���� ���� �־��� ������ ��� ����.(���� �ڽ��� ��)

    right_node = (struct BTNode*)malloc(sizeof(struct BTNode));
    right_node->leaf = node->leaf; // �и��� �ѰŴϱ� ���� �и��Ϸ��� ���� ���� ���ΰ� ����. �°� ���� ���� �ƴϿ����� �굵 ���� �ƴ�.
    right_node->num_keys = 0;
    right_node->num_children = 0;

    int num_iter = node->num_keys;
    for (int i = median + 1; i < num_iter; i++) { // �и��� ��忡 Ű ���(�����̵� �ƴϵ�)
        right_node->keys[i - (median + 1)] = node->keys[i];
        right_node->num_keys++;
        node->num_keys--;
    }

    if (!node->leaf) { // ���� ��尡 ������ �ƴϸ�, �ڽ� ���
        num_iter = node->num_children;
        for (int i = median + 1; i < num_iter; i++) { // ������ ��忡 ���� ��� �ڽ� ���� ���
            right_node->children[i - (median + 1)] = node->children[i];
            right_node->num_children++; // ���� ä���� ����� �ڽ� ������ ����, ���� ��忡���� ����
            node->num_children--;
        }
    }

    // �и��� �� ���� �ø� �θ� ��� ó��
    if (node == root) { // ��Ʈ�� �� �θ� ��� ������ ��. �� �κ��� ��� �ؾ� �� �� ���
        struct BTNode* new_parent_node;
        new_parent_node = BT_create_node(node->key[median]); // �߾Ӱ� ������ �� �θ� ��� �����
        node->num_keys--;
        new_parent_node->children[0] = node; // ���θ����� ���� �ڽ��� ���� ���
        new_parent_node->children[1] = right_node;

        new_parent_node->num_children = 2;
        return new_parent_node;
    }
    else { // ��Ʈ�� �ƴϸ�, ���� �ִ� �θ��� Ȱ��
        for (int i = parent->num_keys; i > pos; i--) { // �θ� ��忡 �־�ߵǴϱ� �ű��ִ� Ű ��ġ �ٽ��ϱ�
            parent->keys[i] = parent->keys[i - 1];
            parent->children[i + 1] = parent->children[i];
        }

        parent->keys[pos] = node->keys[median]; // �θ� ��忡 �־�ߵ� �ڸ��� �� �ֱ�
        parent->num_keys++; // �θ��忡 ���� �־����ϱ� Ű���� �߰�, ���� ���� Ű ���� ���̱�
        node->num_keys--;
        parent->children[pos + 1] = right_node; // ���� ���� ���� ����Ǿ������� �����ʸ� �θ��忡 ����.
        parent->num_children += 1;
    }
    return node; //���� ��� ����
}

// ��忡 ���� �����ϴ� �Լ� ����ü ����(split�� ���ؼ� �θ���(parent), ���� ���(node)�� ���� ����־�� ��.) �׸��� �θ��忡�� Ư�� Ű�� ��ġ�� ���� �־�� ��.**
struct BTNode* insertNode(int parent_pos, int val, struct BTNode* node, struct BTNode* parent) { // ������ ��,  
    int pos; // ���� ��忡�� Ű�� ��ġ�� ���� �־�� ��. �ֳĸ� �������� �ϴ� ���� ��ġ�� ã�ƾ� �ϱ� ����.
    for (pos = 0; pos < node->num_keys; pos++) {// pos ��ġ�� 0���� �ؼ�, ���� ����� Ű ������ŭ Ž��
        if (val == node->keys[pos].key) { // node�� pos��° Ű�� val�� ������
            printf("Duplicates are not permitted!\n");
            return node;
        }
        else if (val < node->keys[pos].key) { // val�� node�� pos��° Ű���� ������ �� pos���� �����.
            break;
        }
    }// ���� val�� �� node�� �ִ� ������ ũ�� �翬�� ������ pos�� ���� ����.
    if (!node->leaf) { // node leaf ���ΰ� false�̸�, leaf�� �ƴϸ�
        node->children[pos] = insertNode(pos, val, node->children[pos], node); // node�� pos��° �ڽ� ��忡 insertNode ���� ��´�. ��ͷ� �ڽ��� Ž���ϱ� ���� �� ��.
        if (node->num_keys == MAX_KEYS + 1) { // ���� ��� Ű ������ ��Ģ���� ����Ű�����
            node = splitNode(parent_pos, node, parent); // �� �������� �и��� �ؾ� ��.
        }
    }
    else { // leaf�� ���� ���� ����
        for (int i = node->num_keys; i > pos; i--) { // ���������� val�� �����ؾ� �ϴ� ��ġ�� �ִ� �������� ������ �ڷ� ����� �۾��� �Ѵ�.
            node->keys[i] = node->keys[i - 1]; // Ű�� �ڷ� �� ĭ�� ���� �۾�.
            node->children[i + 1] = node->children[i]; // �ڽĵ� ��������.
        }

        node->keys[pos].key = val; // val�� �����ؾ� �ϴ� ��ġ�� val ����.
        node->num_keys++; // �ϳ� ���������� Ű ���� ����
        if (node->num_keys == MAX_KEYS + 1) { // leaf ��尡 �� á���� �и��� ���ش�.
            node = splitNode(parent_pos, node, parent);
        }
    }
    return node; // node�� ���� �־��ִϱ� �� node�� ��ȯ�ؾ� ��. �׷��� ��� ����Ǿ ���� ��� ����.

}

void BT_insert_key(struct BTNode** root, struct KeyPair k)
{
    if (!*root) { // root�� ������
        *root = BT_create_node(); // root�� ������.
        (*root)->leaf = true; // ó�� ��������°Ŵϱ� root���� leaf ���.
        return;
    }
    else { // ��Ʈ�� ������
        *root = insertNode(0, k.key, *root, *root); // ó������ root�� �θ����� �������.
    }
}


//// ������ �� ��ġ�� �Լ�
//void mergeNode(struct BTNode* par_node, int node_pos, int mer_node_pos) {
//    // ���� ��带 ������ ������ ���� merge�Ǵ� ��ü�� ���� ��尡 �ǰ� ������ ����.(���� ����) / node_pos�� ������ Ű�� �����ִ� ��尡 �� ���� �ְ� �ȵɼ��� ����.
//    int merge_idx = par_node->children[mer_node_pos]->num_children; // merge�� ��ü ����ʿ� merge�� Ű ��ġ�� �����Ѵ�.
//    par_node->child[mer_node_pos]->key[merge_idx] = par_node->key[mer_node_pos]; //�θ����� Ű�� merge��.
//    par_node->child[mer_node_pos]->cnt_key++;
//
//    for (int i = 0; i < par_node->child[node_pos]->cnt_key; i++) { // ����� ��忡�� �� Ű�� ������ �ּ� Ű ���� ������ �ȵ� �� ����. ���� Ű ���� merge�� ���� �Űܾ� ��. ����Ű�� ������ �ƿ� for���� �ȵ��ư�.
//        par_node->child[mer_node_pos]->key[merge_idx + 1 + i] = par_node->child[node_pos]->key[i]; // Ű ������ 2�� -> i�� 0, merge idx ������ Ű/ Ű ������ �þ�� i�� �ϳ��� �� �þ. 
//        par_node->child[mer_node_pos]->cnt_key++;
//    }
//
//    int merge_childidx = par_node->child[mer_node_pos]->cnt_child; //merge�� ��� ���κ����� �Űܾ� �ϴ� ���κ� idx ����.
//    for (int i = 0; i < par_node->child[node_pos]->cnt_child; i++) { // ����� ��忡�� Ű�� ����� ���� �ڽ��� ������. �³׸� merge�� ���� �Űܾ� �Ѵ�.
//        par_node->child[mer_node_pos]->child[merge_childidx + i] = par_node->child[node_pos]->child[i];
//        par_node->child[mer_node_pos]->cnt_child++;
//    }
//
//    free(par_node->child[node_pos]); // merge �ǰ� ���� �ݴ��� ���� �ʿ������ �޸𸮿��� ������
//
//    for (int i = mer_node_pos; i < (par_node->cnt_key) - 1; i++) { //�θ� ����� Ű �ϳ��� �̹� �ڽ� ���� ���յǾ����� �������ؾ� ��.
//        par_node->key[i] = par_node->key[i + 1];
//    }
//    par_node->cnt_key--;
//
//    for (int i = mer_node_pos + 1; i < (par_node->cnt_child) - 1; i++) { // �θ� ��忡�� ���� �� �� ���� �� �ڿ� �ٸ� �ڽĵ� ���� �� ������ ��迭 �ؾ� ��. merge�� ��� �� ������ ����� ��.
//        par_node->child[i] = par_node->child[i + 1];
//    }
//    par_node->cnt_child--;
//}
//
//// ���ʿ��� ������ �Լ�
//void borrowFromLeft(struct BTNode* par_node, int cur_node_pos) { // �θ� ���� ���� ��� pos��ġ�� ���ڷ� ����. �̹� ���� ����� Ű�� ��������. cnt_key�� �ּһ��� Ȥ�� �̸��ϰ���.
//    int tenant_idx = 0; // ������ ��ü��忡 �����ִ� Ű�� ������ ��ġ.
//
//    // ������ ���� �ڸ��� �����ؾ� �ϱ� ������ �����ִ°Ÿ� ��ĭ �� �ڷ� �̷�� ��.(�����ִ°� ��� ����). borrowFromRight�� �ٸ��� ���� �ڸ� ������ �ؾ� ��.
//    for (int i = 0; i < par_node->child[cur_node_pos]->cnt_key; i++) {
//        par_node->child[cur_node_pos]->key[i + 1] = par_node->child[cur_node_pos]->key[i];
//    }
//    par_node->child[cur_node_pos]->key[tenant_idx] = par_node->key[cur_node_pos - 1]; // ������ Ű�� �������� �� �θ𿡰Լ� �����´�. ���⼭�� ���ʿ��� �������ϱ�, cur_node_pos(������ �ڽ� ����Ű�� ��ġ)���� 1�� ���� �θ��� Ű ��ġ�� ��. �׸��� ��������� Ű�� ���� �ö󰡴� ��.
//    par_node->child[cur_node_pos]->cnt_key++;
//
//    int idx_from_sib_topar = (par_node->child[cur_node_pos - 1]->cnt_key) - 1; // �θ��� Ű�� ������ �������ϱ� ���� ��� �� ������ Ű�� �ϳ� �θ�� �÷��� ��. 
//    par_node->key[cur_node_pos - 1] = par_node->child[cur_node_pos - 1]->key[idx_from_sib_topar]; // �θ���� ������ ��������� Ű�� ���´�. ���⼱ ���� �������.
//    par_node->child[cur_node_pos - 1]->cnt_key--;
//
//    // �������� Ű�� ���������� �ڽĵ� ������ �ؾ���. �ڽ� ��ġ ������ ����.(�� ĭ�� ������ �����)
//    if (par_node->child[cur_node_pos - 1]->cnt_child > 0) { // ���� ��� �ڽ��� �ִ� ��쿡��
//        int tenant_childidx = (par_node->child[cur_node_pos - 1]->cnt_child) - 1; // tenant ��忡�ٰ� ��������� ���� �� �ڽ� ��ġ�� ��ߵǱ� ������ �� �ε��� ����.
//        // �ڽ� ��ġ ����. borrowFromRight�� �ٸ��� �ű�� �� �̸� �����ؾ� ��. 
//        for (int i = par_node->child[cur_node_pos]->cnt_child; i > 0; i--) { // �ڿ������� �����ؼ� �տ��� �ڷ� �ű�� ����.
//            par_node->child[cur_node_pos]->child[i] = par_node->child[cur_node_pos]->child[i - 1];
//        }
//
//        par_node->child[cur_node_pos]->child[0] = par_node->child[cur_node_pos - 1]->child[tenant_childidx]; // ���� ��� �ڽ� ������. ������ �ڽ� ���� ��ġ�� ���� ����� 1��° ��ġ.
//        par_node->child[cur_node_pos]->cnt_child++;
//
//        par_node->child[cur_node_pos - 1]->cnt_child--;
//    }
//}
//
//// �����ʿ��� ������ �Լ�
//void borrowFromRight(struct BTNode* par_node, int cur_node_pos) { // �θ� ���� ���� ��� pos��ġ�� ���ڷ� ����. �̹� ���� ����� Ű�� ��������. cnt_key�� �ּһ��� Ȥ�� �̸��ϰ���.
//    int tenant_idx = par_node->child[cur_node_pos]->cnt_key; // ������ ��ü��忡 �����ִ� Ű�� ������ ��ġ.
//    par_node->child[cur_node_pos]->key[tenant_idx] = par_node->key[cur_node_pos]; // ������ Ű�� �������� �� �θ𿡰Լ� �����´�. �׸��� ��������� Ű�� ���� �ö󰡴� ��.
//    par_node->child[cur_node_pos]->cnt_key++;
//
//    int idx_from_sib_topar = 0;
//    par_node->key[cur_node_pos] = par_node->child[cur_node_pos + 1]->key[idx_from_sib_topar]; // �θ���� ������ ��������� Ű�� ���´�.
//
//    // // �������� Ű�� ���������ϱ� ��ġ ������ �ؾ� �Ѵ�.(�� ĭ�� ������ �����)
//    for (int i = 0; i < (par_node->child[cur_node_pos + 1]->cnt_key) - 1; i++) {
//        par_node->child[cur_node_pos + 1]->key[i] = par_node->child[cur_node_pos + 1]->key[i + 1];
//    }
//    par_node->child[cur_node_pos + 1]->cnt_key--;
//
//    int idx_from_sib = 0;
//    // �������� Ű�� ���������� �ڽĵ� ������ �ؾ���. �ڽ� ��ġ ������ ����.(�� ĭ�� ������ �����)
//    if (par_node->child[cur_node_pos + 1]->cnt_child > 0) { // �ڽ��� �ִ� ��쿡��
//        int tenant_childidx = par_node->child[cur_node_pos]->cnt_child; // tenant ��忡 ������ �ڽ��� �� ��ġ
//        par_node->child[cur_node_pos]->child[tenant_childidx] = par_node->child[cur_node_pos + 1]->child[idx_from_sib]; // ���� ��� �ڽ� ������.
//        par_node->child[cur_node_pos]->cnt_child++;
//
//        // �ڽ� ��ġ ���� 
//        for (int i = 0; i < par_node->child[cur_node_pos + 1]->cnt_child - 1; i++) {
//            par_node->child[cur_node_pos + 1]->child[i] = par_node->child[cur_node_pos + 1]->child[i + 1];
//        }
//        par_node->child[cur_node_pos + 1]->cnt_child--;
//    }
//}
//
//void balanceNode(struct BTNode* node, int child_pos) { // ���� ���� �ڽĳ�忡���� ��ġ�� ���ڷ� �޴� �Լ�(������,������ ����)
//    if (child_pos == 0) { // �ڽĳ�� Ű ��ġ�� �� �����϶��� ������ �θ�,������ ���� ��.
//        if (node->child[child_pos + 1]->cnt_key > min_keys) { // (�ڽĳ�� ����) ������ Ű������ �ּҼ��� ���� �� �μ�����
//            borrowFromRight(node, child_pos);
//        }
//        else { // ������ Ű������ �ּҼ��� ���� �μ�����
//            mergeNode(node, child_pos + 1, child_pos); // �θ���(������)�� �ڽ� ��ġ�� �ڱ� ���� ��ġ�� ���� �Ѱ���.  
//        }
//        return;
//    }
//
//    else if (child_pos == (node->cnt_key)) { // �ڽĳ�� Ű ��ġ�� �� �������� ���� ���� �θ�, ���� ���� ��.
//        if (node->child[child_pos - 1]->cnt_key > min_keys) { // �ڽĳ�� ����, ���� ������ Ű������ �ּҼ��� ���� �� �μ��� ��
//            borrowFromLeft(node, child_pos);
//        }
//        else { // �ּҼ��� ���� �μ��� ��
//            mergeNode(node, child_pos, child_pos - 1); // �θ���(������)�� ����� ���� ���մ�� ��� ��ġ�� ���� �Ѱ���.  
//        }
//        return;
//    }
//    else { // �� ����,�� ������ ���� �� �̿�
//        if (node->child[child_pos - 1]->cnt_key > min_keys) {
//            borrowFromLeft(node, child_pos);
//        }
//        else if (node->child[child_pos + 1]->cnt_key > min_keys) {
//            borrowFromRight(node, child_pos);
//        }
//        else {
//            mergeNode(node, child_pos, child_pos - 1); // �شܿ� �ִ� �ڽ� ���� �� �� ������ ��ġ�� ������ ������ ��
//        }
//        return;
//    }
//}
//
//// ���γ�� �������� �ڽĵ��� merge�ؾ��ϴ� ���̽�
//int mergeChildNode(struct BTNode* par_node, int cur_node_pos) {
//    int mergeidx = par_node->child[cur_node_pos]->cnt_key; // merge�� ���� �������� �ϴµ� �ڽĳ�忡�� ������ ��ġ ����.
//    // �ٷ� ������ �ʰ� ��ġ���� �ϴ� ��忡 ������� �ϴ� �θ���(���γ��)�� ���� ��ħ. �ֳ�? �� ������ �ٷ� ����� �ڽĳ�常 ��ġ��, ��ġ���� �ϴ� ��� �ؿ� �� �ڽĳ�尡 ���� ��쿡�� �ڽ� 1���� �������� ��. 
//    // �׷��� �ϴ� �θ����� ���� �ְ� �ű⼭ �� ��ͷ� ���� �� �ڽ��� ��ġ���� �������� �ؼ� �ڽļ��� �ذ��ؾ� ��.
//    int val_par_node = par_node->key[cur_node_pos]; // ������� �θ� ����� ���� ���
//    par_node->child[cur_node_pos]->key[mergeidx] = par_node->key[cur_node_pos];
//    par_node->child[cur_node_pos]->cnt_key++;
//
//    // ��ġ���� ��忡 ���� ��� ���� ������ ��. (���� �Լ��� ����)
//    for (int i = 0; i < par_node->child[cur_node_pos + 1]->cnt_key; i++) {
//        par_node->child[cur_node_pos]->key[mergeidx + 1 + i] = par_node->child[cur_node_pos + 1]->key[i];
//        par_node->child[cur_node_pos]->cnt_key++;
//    }
//    // ������� �ڽĵ� ���;� ��.
//    for (int i = 0; i < par_node->child[cur_node_pos + 1]->cnt_child; i++) {
//        par_node->child[cur_node_pos]->child[mergeidx + 1 + i] = par_node->child[cur_node_pos + 1]->child[i];
//        par_node->child[cur_node_pos]->cnt_child++;
//    }
//
//    // �θ���(���γ��)�� Ű�� �����ϱ� ��迭 & �ڽĵ� ��迭
//    for (int i = cur_node_pos; i < par_node->cnt_key; i++) {
//        par_node->key[i] = par_node->key[i + 1];
//        par_node->cnt_key--;
//    }
//    for (int i = cur_node_pos + 1; i < par_node->cnt_child; i++) {
//        par_node->child[i] = par_node->child[i + 1];
//        par_node->cnt_child--;
//    }
//    return val_par_node; // �θ��忡�� ���ȴ� ���� ����� ���� �ϴ� ����. ���� �����ϴ� �Լ����� ������ ����.
//
//}
//
//// predecessor ã�� �Լ�
//int findPredecessor(struct BTNode* cur_node) {
//    int predecessor;
//    if (cur_node->leaf) { //���� Ž����尡 �����̸�, ã�� �� ����.
//        return cur_node->key[cur_node->cnt_key - 1]; //���� ��忡�� ���� ū Ű �ָ� ��.(predecessor ����)
//    }
//    return findPredecessor(cur_node->child[(cur_node->cnt_child) - 1]); // Ž���� ������ ū �� �ڽ����� Ž���ؾ� ��.
//}
//// predecessor ã�Ƽ� ���γ�忡 ������ �Լ�
//int overrideWithPredecessor(struct BTNode* par_node, int pos_std_search) {
//    int predecessor = findPredecessor(par_node->child[pos_std_search]); // predecessor�� ��ͷ� �� �������� ã�� �Լ� ȣ��. �θ� ���� Ÿ�� ������ ��ġ�� ���ڷ� ��.
//    par_node->key[pos_std_search] = predecessor; // ������� �ϴ� ���γ���� ���� ã�� predecessor�� ��ü ����.
//    return predecessor;
//}
//
//// suceessor ã�� �Լ�
//int findSuccessor(struct BTNode* cur_node) {
//    int successor;
//    if (cur_node->leaf) { //���� Ž����尡 �����̸�, ã�� �� ����.
//        return cur_node->key[0]; //���� ��忡�� ���� ���� Ű �ָ� ��.(successor ����)
//    }
//    return findSuccessor(cur_node->child[0]); // Ž���� ������ ���� �� �ڽ����� Ž���ؾ� ��.
//}
//// successor ã�Ƽ� ���γ�忡 ������ �Լ�
//int overrideWithSuccessor(struct BTNode* par_node, int pos_std_search) {
//    int successor = findSuccessor(par_node->child[pos_std_search + 1]); // successor�� ��ͷ� �� �������� ã�� �Լ� ȣ��. �θ� ���� Ÿ�� ������ ��ġ(���������� ���ߵ�.)�� ���ڷ� ��.
//    par_node->key[pos_std_search] = successor; // ������� �ϴ� ���γ���� ���� ã�� successor�� ��ü ����.
//    return successor;
//}
//
//// ���� ��忡�� ���� ����� �Լ�
//void deleteInnerNode(struct BTNode* cur_node, int cur_node_pos) {
//    int cessor = 0; // predecessor Ȥ�� successor�� ���� ��� || merge�� ����� ã�� ���� ���� ����
//    int deletion_for_merge = 0;
//    // ���� ������ �� ����� �ڽ��� �� ������ Ȯ��, predeȤ�� successor�� ã�ƾ� �ϱ⶧��. ���� ������ ������ ���� ���� ����.
//    if (cur_node->child[cur_node_pos]->cnt_key >= cur_node->child[cur_node_pos + 1]->cnt_key) {
//        if (cur_node->child[cur_node_pos]->cnt_key > min_keys) { // �ڽ� Ű������ �ּҹ��� �ν��� ������ predecessor ã�� ����.
//            cessor = overrideWithPredecessor(cur_node, cur_node_pos);
//            deleteValFromNode(cessor, cur_node->child[cur_node_pos]); // ã�� predecessor�� ���� �÷��� ��. �ٵ� �� ������ �ᱹ �ش� ������忡�� ���� ����°� ȿ���� �����ϴ� �Լ� ȣ��.
//        }
//        else {
//            deletion_for_merge = mergeChildNode(cur_node, cur_node_pos);
//            deleteValFromNode(deletion_for_merge, cur_node->child[cur_node_pos]);
//        }
//    }
//    else {
//        if (cur_node->child[cur_node_pos + 1]->cnt_key > min_keys) {
//            cessor = overrideWithSuccessor(cur_node, cur_node_pos);
//            deleteValFromNode(cessor, cur_node->child[cur_node_pos + 1]); // successor ã���� �̰͵� ������忡�� ����� ȿ���� ���� ��.
//        }
//        else {
//            deletion_for_merge = mergeChildNode(cur_node, cur_node_pos);
//            deleteValFromNode(deletion_for_merge, cur_node->child[cur_node_pos]);
//        }
//
//    }
//
//}
//
//// ���� ����� ���� �־��ָ� ����� �Լ�.
//int deleteValFromNode(int val, struct BTNode* root) {
//    int pos; // ������� ����� ������ ��ġ�� ����.    
//    int flag = false; // ���������� ���θ� �˱� ���� flag�� ����
//    for (pos = 0; pos < root->cnt_key; pos++) { // val�� ���������ϴ� �� ��ġ�� ã�ƾ� ��.���� ����� Ű ������ŭ Ž��
//        if (val == root->key[pos]) { // ���� ����� Ű �迭���� pos�� val�� ������
//            flag = true; // ã�Ҵٴ� ǥ��
//            break;
//        }
//        else if (val < root->key[pos]) { // Ű �迭�� pos ��ġ ���� val���� ũ�� �� ��ġ���� �����. �ű⿡�� �Ʒ��� �� ���� �Ѵ�.
//            break;
//        }
//    } // �̰� �����ٴ°� �� ��忡�� (�߰� Ž����) pos��ġ�� �������ٴ� �� 
//    if (flag) { // flag�� true�̸� ������ �����ϴ� �۾� �ǽ�
//        if (root->leaf) {  // �������� �����ؾ� �ϸ�
//            for (int i = pos; i < root->cnt_key; i++) { // ����� pos��ġ���� Ű ������ŭ Ž��
//                node->key[i] = node->key[i + 1]; // ������� Ű ��ġ�� �� ���� Ű�� �����
//            }
//            root->cnt_key--;
//        }
//        else { // ���ο��� �����ؾ� �ϸ�
//            deleteInnerNode(root, pos); //���� ����� ���� �����ϴ� �Լ� ����. ���� ���� �����忡���� �� ��ġ�� ���ڷ� �ѱ�.
//        }
//        return flag;
//    }
//    else { // flag�� false�̸�(������� ���� ��ã�� ��)
//        if (root->leaf) { //leaf ����̸�
//            return flag;
//        }
//        else { // ������� ���� �� ã�Ҵµ� ���� ����̸� �� ������.
//            flag = deleteValFromNode(val, root->child[pos]); //val�̶� �������� pos��° �ڽ� �Ѱܼ� flag �ޱ�
//        }
//    }
//    if (root->child[pos]->cnt_key < min_keys) { // (��Ͱ� ������ �ٽ� �ö�µ�)����ó���ߴ� �ڽ� ����� Ű ������ �ּҼ��� ���� �μ����� ��
//        balanceNode(node, pos); // ������, �����ϴ� �ϴ� �Լ� ���� (���� ���� �ڽĳ���� pos��ġ�� ���ڷ�)
//    }
//
//    return flag;
//}

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