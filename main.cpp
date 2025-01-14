#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define StackAssert(stk) if (StackVerify(stk)) {StackDump(stk); assert(0);}

typedef int stackelem_t;

struct Stack {
    stackelem_t* data;
    int size;
    int capacity;
};

enum RESPONSE_CODES_FOR_CTOR {
    CREATE_SUCCESS = 0,
    INVALID_SIZE = 1,
    ZERO_SIZE = 2,
    INVALID_STACK = 3
};

enum RESPONCE_CODES_FOR_VERIFY {
    VERIFYOK = 0,
    STACKNOTDEFINED = 1,
    STACKBADSIZE = 2,
    STACKBADCAPACITY = 3,
    STACKOVERFLOW = 4
};

const stackelem_t POISON = 0xBADDED;

enum RESPONSE_CODES_FOR_CTOR StackCtor(struct Stack* stk, int size);
int StackPush(struct Stack* stk, stackelem_t elem);
int StackPop(struct Stack* stk, stackelem_t* target);
int StackDtor(struct Stack* stk);
enum RESPONCE_CODES_FOR_VERIFY StackVerify(struct Stack* stk);
int StackDump(struct Stack* stk);

int main()
{
    struct Stack stk = {};
    StackCtor(&stk, 10);
    StackPush(&stk, 10);
    StackPush(&stk, 20);
    StackPush(&stk, 30);
    StackDump(&stk);
    return 0;
}

enum RESPONSE_CODES_FOR_CTOR StackCtor(struct Stack* stk, int size) {
    if (size < 0) {
        printf("INVALID SIZE: %d", size);
    }
    if (size == 0) {
        printf("CREATED 0-SIZED STACK");
        stk->data = NULL;
        stk->size = 0;
        stk->capacity = 0;
        return ZERO_SIZE;
    }
    stk->data = (stackelem_t*) calloc(size, sizeof(stackelem_t));
    stk->size = 0;
    stk->capacity = size;
    return CREATE_SUCCESS;
}

int StackPush(struct Stack* stk, stackelem_t elem) {
    StackAssert(stk);
    if (stk->size >= stk->capacity) {
        stk->capacity *= 2;
        stk->data = (stackelem_t*) realloc(stk->data, stk->capacity * sizeof(stackelem_t));
    }
    stk->data[stk->size] = elem;
    stk->size++;
    return 0;
}

int StackPop(struct Stack* stk, stackelem_t* target) {
    StackAssert(stk);
    if (stk->capacity && stk->size <= stk->capacity / 4) {
        stk->capacity /= 2;
        stk->data = (stackelem_t*) realloc(stk->data, stk->capacity * sizeof(stackelem_t));
    }
    if (stk->size <= 0) {
        return 1;
    }
    stk->size--;
    *target = stk->data[stk->size];
    stk->data[stk->size] = POISON;
    return 0;
}

int StackDtor(struct Stack* stk) {
    free(&stk->data);
    return 0;
}

enum RESPONCE_CODES_FOR_VERIFY StackVerify(struct Stack* stk) {
    if (stk == NULL) {
        return STACKNOTDEFINED;
    }
    if (stk->size < 0) {
        return STACKBADSIZE;
    }
    if (stk->capacity < 0) {
        return STACKBADCAPACITY;
    }
    if (stk->size > stk->capacity) {
        return STACKOVERFLOW;
    }
    return VERIFYOK;
}

int StackDump(struct Stack* stk) {
    for (int i = 0; i < stk->size; i++) {
        printf("%d ", stk->data[i]);
    }
    printf("\n");
    return 0;
}
