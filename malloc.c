#include "malloc.h"

#include <unistd.h>
#include <stdbool.h>

typedef struct memory_control_block {
    size_t size;   // 该控制块所控制内存的大小
    bool is_free;  // 该块内存是否被使用
    struct memory_control_block *prev;
}MCB;

MCB *mcb_top = NULL;
#define MCB_SIZE sizeof(MCB)

void *mini_malloc(size_t size) {
    if (!mcb_top) {
        mcb_top = (MCB *)sbrk(MCB_SIZE+size);
        mcb_top->size = size;
        mcb_top->is_free = false;
        mcb_top->prev = NULL;

        return (mcb_top+1);
    }

    MCB *m = mcb_top;
    while ( m != NULL) {
        if (m->is_free && m->size >= size) {
            m->is_free = false;
            break;
        }
        m = m->prev;
    }
    if (m != NULL) {
        return (m+1);
    }

    m = (MCB *)sbrk(MCB_SIZE+size);
    m->size = size;
    m->is_free = false;
    m->prev = mcb_top;
    mcb_top = m;

    return (mcb_top+1);        
}

void mini_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    MCB *m = (MCB *)ptr - 1;
    m->is_free = true;
    MCB *tmp = mcb_top;
    while (tmp->is_free && tmp->prev != NULL) {
        tmp = tmp->prev;
    }
    if (tmp->is_free) {
        mcb_top = tmp->prev;
        // 释放从tmp开始的所有内存
        brk(tmp);
    }else {
        mcb_top = tmp;
        // 释放tmp后面的节点开始的所有内存
        brk((char *)tmp+MCB_SIZE+tmp->size);
    }
}
