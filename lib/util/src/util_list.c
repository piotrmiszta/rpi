#include "util_list.h"
#include "error_codes.h"
#include <stdlib.h>

typedef struct NodeS{
    struct NodeS* next;
    struct NodeS* prev;
    void* data;
}NodeS;

typedef struct ListS{
    void (*free_fun)(void*);
    int (*comp_fun)(void* a, void* b);
    size_t size;
    NodeS* head;
    NodeS* tail;
}ListS;

ListS* util_list_create(void (*free_fun)(void*), int (*comp_fun)(void* a, void* b)) {
    ListS* list = malloc(sizeof(ListS));
    assert_ss(list);
    list->comp_fun = comp_fun;
    list->free_fun = free_fun;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

int util_list_destroy(ListS* list) {
    if(list) {
        while(list->size) {
            NodeS* node = list->head;
            list->head = list->head->next;
            list->free_fun(node->data);
            free(node);
            list->size--;
        }
        free(list);
        return SUCCESS;
    }
    return SUCCESS;
}

int util_list_push_back(ListS* list, void* data) {
    if(list) {
        NodeS* node = malloc(sizeof(NodeS));
        assert_ss(node);
        node->data = data;
        node->next = NULL;
        if(list->tail) {
            node->prev = list->tail;
            list->tail->next = node;
            list->tail = node;
            list->size ++;
            return SUCCESS;
        }
        else {
            assert_ss( (!list->tail) && (!list->head) );
            assert_ss(list->size == 0);
            node->prev = NULL;
            list->tail = node;
            list->head = node;
            list->size ++;
            return SUCCESS;
        }
    }
    else {
        return ELARG;
    }
}

int util_list_push_front(ListS* list, void* data) {
    if(list) {
        NodeS* node = malloc(sizeof(NodeS));
        assert_ss(node);
        node->data = data;
        node->prev = NULL;
        if(list->tail) {
            node->next = list->head;
            list->head->prev = node;
            list->head = node;
            list->size ++;
            return SUCCESS;
        }
        else {
            assert_ss( (!list->tail) && (!list->head) );
            assert_ss(list->size == 0);
            node->next = NULL;
            list->tail = node;
            list->head = node;
            list->size ++;
            return SUCCESS;
        }
    }
    else {
        return ELARG;
    }
}

int util_list_push_index(ListS* list, void* data, size_t index) {
    if(list) {
        if(index < list->size) {
            if(index == list->size - 1) {
                util_list_push_back(list, data);
                return SUCCESS;
            }
            else if(index == 0) {
                util_list_push_front(list, data);
                return SUCCESS;
            }
            else {
                NodeS* node = list->head;
                for(size_t i = 0; i < index - 1; i++) {
                    node = node->next;
                }
                NodeS* newNode = malloc(sizeof(NodeS));
                assert_ss(newNode);
                newNode->prev = node;
                newNode->next = node->next;
                newNode->data = data;
                node->next->prev = newNode;
                node->next = newNode;
                list->size++;
                return SUCCESS;
            }
        }
        else{
            return ELRAN;
        }
    }
    else {
        return ELARG;
    }
}

void* util_list_pop_back(ListS* list) {
    if(list) {
        void* ret = NULL;
        if(list->tail) {
            if(list->head == list->tail) {
                assert_ss(list->size == 1);
                ret = list->tail->data;
                free(list->tail);
                list->head = NULL;
                list->tail = NULL;
                list->size--;
                return ret;
            }
            else {
                assert_ss(list->size > 1);
                assert_ss(!list->tail->next);
                assert_ss(list->tail->prev);
                ret = list->tail->data;
                NodeS* temp = list->tail->prev;
                temp->next = NULL;
                free(list->tail);
                list->tail = temp;
                list->size--;
                return ret;
            }
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

void* util_list_pop_front(ListS* list) {
    if(list) {
        void* ret = NULL;
        if(list->head) {
            if(list->head == list->tail) {
                assert_ss(list->size == 1);
                ret = list->head->data;
                free(list->head);
                list->head = NULL;
                list->tail = NULL;
                list->size --;
                return ret;
            }
            else {
                assert_ss(list->size > 1);
                assert_ss(!list->head->prev);
                assert_ss(list->head->next);
                ret = list->head->data;
                NodeS* temp = list->head->next;
                temp->prev = NULL;
                free(list->head);
                list->head = temp;
                list->size --;

                return ret;
            }
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

void* util_list_pop_index(ListS* list, size_t index) {
    if(list) {
        if(index < list->size) {
            if(index == 0) {
                return util_list_pop_front(list);
            }
            else if(index == list->size-1) {
                return util_list_pop_back(list);
            }
            else {
                NodeS* temp = list->head;
                for(size_t i = 0; i < index; i++) {
                    temp = temp->next;
                }
                void* ret = temp->data;
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                free(temp);
                list->size--;
                return ret;
            }
        }
        else {
            return NULL;
        }
    }
    else{
        return NULL;
    }
}

void* util_list_get_back(ListS* list) {
    if(list) {
        if(list->tail) {
            return list->tail->data;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

void* util_list_get_front(ListS* list) {
    if(list) {
        if(list->head) {
            return list->head->data;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

void* util_list_get_index(ListS* list, size_t index) {
    if(list) {
        if(index < list->size) {
            if(index == 0) {
                return util_list_get_front(list);
            }
            else if(index == list->size-1) {
                return util_list_get_back(list);
            }
            else {
                NodeS* temp = list->head;
                for(size_t i = 0; i < index; i++) {
                    temp = temp->next;
                }
                return temp->data;
            }
        }
        else {
            return NULL;
        }
    }
    else{
        return NULL;
    }
}

index_t util_list_find(ListS* list, void* data) {
    if(list) {
        if(list->comp_fun == NULL) {
            return -1;
        }
        NodeS* node = list->head;
        for(size_t i = 0; i < list->size; i++) {
            int result = list->comp_fun(node->data, data);
            node = node->next;
            if(result == 0) {
                return (long long int)i;
            }
        }
    }
    else{
        return -1;
    }
    return -1;
}

size_t util_list_get_size(ListS* list) {
    if(list) {
        return list->size;
    }
    else {
        return 0;
    }
}
