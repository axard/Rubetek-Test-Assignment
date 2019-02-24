#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Приватные макросы */
/* ========================================================================== */

/* Размер заголовка */
#define HEADER_SIZE sizeof(Header_t)

#ifndef HEAP_SIZE
#warning "HEAP_SIZE not defined. Use default value: 1024"
#define HEAP_SIZE 1024
#endif /* HEAP_SIZE */

/* Приватные типы */
/* ========================================================================== */

typedef struct _Header_t Header_t;
struct _Header_t {
        Header_t *next; /*!< Следующий свободный блок в цепочке, NULL для последнего */
        size_t    size; /*!< Размер блока памяти */
};

/* Приватные переменные */
/* ========================================================================== */

static uint8_t heap[ HEAP_SIZE + 2*HEADER_SIZE ];
static Header_t begin;
static Header_t *end = NULL;
static size_t freeSize = HEAP_SIZE;

/* Приватные функции */
/* ========================================================================== */

static
void init(void)
{
        end = (void*)((size_t)heap + HEAP_SIZE);
        end->next = NULL;
        end->size = 0;

        Header_t *first = (void*)heap;
        first->next = end;
        first->size = HEAP_SIZE;

        fprintf(stderr, "HEADER_SIZE = %lu\n", HEADER_SIZE);
        fprintf(stderr, "heap = %lu\n", (size_t)heap);

        begin.next = first;
        begin.size = 0;

        freeSize = first->size;
}

/* Публичные функции */
/* ========================================================================== */

void* dmm_malloc(size_t size)
{
        if (end == NULL) {
                init();
        }

        if (size > freeSize) {
                return NULL;
        }

        Header_t *prev = &begin;
        Header_t *curr = begin.next;

        while ((curr->size < size) && (curr->next != NULL)) {
                prev = curr;
                curr = curr->next;
        }

        if (curr == end) {
                return NULL;
        }

        fprintf(stderr, "curr = %lu\n", (size_t)curr);
        void *result = (void*)((size_t)curr + HEADER_SIZE);

        if ((curr->size - size) > HEADER_SIZE) {
                Header_t *newBlock = (void*)((size_t)curr + HEADER_SIZE + size);
                fprintf(stderr, "newBlock = %lu\n", (size_t)newBlock);
                newBlock->next = curr->next;
                newBlock->size = curr->size - size - HEADER_SIZE;
                curr->size = size;
                prev->next = newBlock;
                freeSize -= size + HEADER_SIZE;
                fprintf(stderr, "freeSize = %lu\n", freeSize);
        } else {
                prev->next = curr->next;
                freeSize -= size;
        }

        return result;
}

void dmm_free(void *ptr)
{
        if (ptr == NULL) {
                return;
        }

        Header_t *header = (void*)((size_t)ptr - HEADER_SIZE);
        Header_t *iter = &begin;
        size_t headerAddr = (size_t)header;
        while ((iter->next < header) && (iter->next != NULL)) {
                iter = iter->next;
        }

        if (iter == end) {
                return;
        }

        header->next = iter->next;
        iter->next = header;

        size_t iterAddr = (size_t)iter;
        size_t nextAddr = (size_t)(header->next);
        if ((iterAddr + HEADER_SIZE + iter->size) == headerAddr) {
                freeSize += header->size + HEADER_SIZE;
                iter->size += header->size + HEADER_SIZE;
                iter->next = header->next;
        }

        if ((headerAddr + HEADER_SIZE + header->size) == nextAddr) {
                freeSize += header->size + HEADER_SIZE;
                header->size += header->next->size + HEADER_SIZE;
                header->next = header->next->next;
        }
}

size_t dmm_avail(void)
{
        if (end == NULL) {
                init();
        }

        return freeSize;
}

