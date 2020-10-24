#pragma once

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


const int REALLOC_VALUE = 2;
const int INCREASE_LEVEL = 2;
const int DECREASE_LEVEL = 4;

#ifdef BARANOV_V_V_DEBUG
typedef uint64_t stack_canary_t;
typedef int64_t hash_t;
const stack_canary_t STACK_CANARY = 0xBAAADCFEFEBDCBFE;
#endif // BARANOV_V_V_DEBUG

/*!
Error codes
!*/
enum Error_t {
    SUCCESS        = 0,
    LENGTH_ERROR   = 1,
    PUSH_ERROR     = 2,
    TOP_ERROR      = 3,
    NO_INCREASE    = 4,
    NO_DECREASE    = 5,
    POP_ERROR      = 6,
    SIZE_ERROR     = 7,
    CAPACITY_ERROR = 8
};

/*!
Error codes for stack
!*/
enum StackErrors {
    OK                       = 0,
    INVALID_SIZE             = 1,
    INVALID_CAPACITY         = 2,
    POISON_ERROR             = 3,
    DATA_NULL                = 4,
    STACK_OVERFLOW           = 5,
    BEGIN_DATA_CANARY_ERROR  = 6,
    END_DATA_CANARY_ERROR    = 7,
    BEGIN_STACK_CANARY_ERROR = 8,
    END_STACK_CANARY_ERROR   = 9,
    STACK_HASH_ERROR         = 10,
    DATA_HASH_ERROR          = 11,
};

static const char* ErrorNames[] = {
    "OK",
    "SIZE_ERROR",
    "CAPACITY_ERROR",
    "POISON_ERROR",
    "DATA IS NULL",
    "STACK OVERFLOW",
    "LEFT DATA CANARY VALUE IS CHANGED",
    "RIGHT DATA CANARY VALUE IS CHANGED",
    "LEFT STACK CANARY VALUE IS CHANGED",
    "RIGHT STACK CANARY VALUE IS CHANGED",
    "STACK HASH VALUES DOES NOT MATCH",
    "DATA HASH VALUES DOES NOT MATCH"
};

#define DUMP(stack, StackErrors) {                                                \
    const char* file_name = "Stack_(" #stack ")_Dump.txt";                       \
    FILE* fp = fopen(file_name, "w");                                            \
    StackDump(stack, file_name, fp, StackErrors);                                \
    fclose(fp); \
    }

static StackErrors error_no;

#ifdef BARANOV_V_V_DEBUG

#define CONSTRUCT(stack, new_size)   \
    Construct(new_size, &stack);     \
    stack.stack_name_ = #stack;      \
    stack.current_line = __LINE__;   \
    stack.current_file = __FILE__;

#define ASSERT_OK(stack)                                \
    error_no = StackOk(stack);                          \
    if (error_no != OK) {                               \
        DUMP(stack, error_no);                          \
        assert(!"OK");                                  \
    }

#define HASHSTACK(stack) \
    HashStack(stack);

#else

#define CONSTRUCT(stack)

#define ASSERT_OK(stack)                                           \
    error_no = StackOk(stack);                                     \
    if (error_no != OK) {                                          \
        DUMP(stack, error_no);                                     \
        fprintf(stderr, "Error with code %d occured", error_no);   \
    }

#define HASHSTACK(stack)

#endif // MYDEBUG


/*!
type to store size and capacity
!*/
typedef int64_t int_t;

/*!
type which will be used in stack as value type
!*/
typedef float Type_t;

struct StackArray {
    #ifdef BARANOV_V_V_DEBUG
    stack_canary_t canary_begin;
    #endif // BARANOV_V_V_DEBUG

    #ifdef BARANOV_V_V_DEBUG
    char* stack_name_;
    char* current_file;
    int current_line;

    hash_t data_hash;
    hash_t stack_hash;
    #endif

    int_t size_;
    int_t capacity_;
    Type_t* data_;

    #ifdef BARANOV_V_V_DEBUG
    stack_canary_t canary_end;
    #endif // BARANOV_V_V_DEBUG
};

/*!
returns size of stack
@param[in] stack stack with size to return
@param[in] variable to put size

@return size size of stack
!*/
Error_t Size(StackArray* stack, int_t* size);

/*!
returns capacity of stack
@param[in] stack stack with capacity to return
@param[in] capacity variable to put capacity
@return error code (0 if poped succesfully)
!*/
Error_t Capacity(StackArray* stack, int_t* capacity);

/*!
 Increases stack in capacity by its Increase_value_
@param[in] stack stack with capacity to Increase
@return error_code (0 if increased successfully)
!*/
Error_t StackIncrease(struct StackArray* stack);

/*!
 Decreases stack in capacity by its increase_value_
@param[in] stack stack with capacity to decrease
@return error_code (0 if increased successfully)
!*/
Error_t StackDecrease(struct StackArray* stack);

/*!
creates new stack with given parameters
@param[in] start_size start size of stack to create
@return stack
!*/
Error_t Construct(int start_size, struct StackArray* new_stack);

/*!
Pushes elem in stack with increases in capacity if needed
@param[in] stack stack to push
@param[in] value variable to push
@return error code (0 if pushed successfully)
!*/
Error_t Push(struct StackArray* stack,Type_t value);

/*!
Gets last element of stack if it has
@param[in] stack stack from witch take element
@param[in] value variable to put top element
@return error code (0 if poped succesfully)
!*/
Error_t Top(struct StackArray* stack, Type_t* value);

/*!
Pops elem from stack if it has one
@param[in] stack stack to pop
@return error code (0 if poped succesfully)
!*/
Error_t Pop(struct StackArray* stack);

/*!
 Destroys stack so it can not be used again
@param[in] stack stack to destroy
@return error code (0 if poped succesfully)
!*/
Error_t Destroy(struct StackArray* stack);

StackErrors StackOk(struct StackArray* stack);

void StackDump(struct StackArray* stack,const char* file_name, FILE* fp, StackErrors err_no);

#ifdef BARANOV_V_V_DEBUG
hash_t RotateLeft(hash_t value, int shift);

hash_t MakeHash(void* data, int_t size);

void HashStack(struct StackArray* stack);
#endif // BARANOV_V_V_DEBUG

void StackRealloc(struct StackArray* stack, int_t new_capacity);

Error_t ShrinkToFit(struct StackArray* stack);


#ifdef BARANOV_V_V_DEBUG

hash_t RotateLeft(hash_t value, int shift) {
    assert(shift >= 0);
    const int BITS = 8;

    return (value << shift) | (value >> (sizeof(hash_t) * BITS - shift));
}

hash_t MakeHash(void* data, int_t size) {
    assert(data != NULL);
    assert(size >= 0);

    if (size == 0) {
        return 0;
    }

    hash_t hash_value = *((char*)data + 0);

    for(int_t i = 1; i < size; i++) {
        hash_value += RotateLeft(hash_value, i) ^ *((char*)data + i);
    }

    return hash_value;
}

void HashStack(struct StackArray* stack) {
    assert(stack != NULL);

    stack->data_hash = 0;
    stack->stack_hash = 0;

    stack->stack_hash = MakeHash(stack, sizeof(struct StackArray));
    stack->data_hash = MakeHash(stack->data_, stack->capacity_ * sizeof(Type_t));

}

#endif // BARANOV_V_V_DEBUG

StackErrors StackOk(struct StackArray* stack) {
    assert(stack != NULL);

    #ifdef BARANOV_V_V_DEBUG
    hash_t data_hash_tmp = stack->data_hash;
    hash_t stack_hash_tmp = stack->stack_hash;
    #endif // BARANOV_V_V_DEBUG

    HASHSTACK(stack);

    if (stack->size_ < 0) {
        return INVALID_SIZE;
    }
    if (stack->capacity_ < 0) {
        return INVALID_CAPACITY;
    }
    if (stack->size_ > stack->capacity_) {
        return STACK_OVERFLOW;
    }
    if (stack->data_ == NULL) {
        return DATA_NULL;
    }

    #ifdef BARANOV_V_V_DEBUG
    if(*(stack_canary_t*)((char*)stack->data_ - sizeof(stack_canary_t)) != STACK_CANARY) {
        return BEGIN_DATA_CANARY_ERROR;
    }
    if(*(stack_canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)) != STACK_CANARY) {
        return END_DATA_CANARY_ERROR;
    }
    if(stack->canary_begin != STACK_CANARY) {
        return BEGIN_STACK_CANARY_ERROR;
    }
    if(stack->canary_end != STACK_CANARY) {
        return END_STACK_CANARY_ERROR;
    }
    #endif // BARANOV_V_V_DEBUG

    for (int i = stack->size_; i < stack->capacity_; i++) {
        if ( isfinite(stack->data_[i]) != 0 ) {
            return POISON_ERROR;
        }
    }

    #ifdef BARANOV_V_V_DEBUG
    if (stack->data_hash != data_hash_tmp) {
        return DATA_HASH_ERROR;
    }
    if (stack->stack_hash != stack_hash_tmp) {
        return STACK_HASH_ERROR;
    }
    #endif // BARANOV_V_V_DEBUG

    return OK;
}

void StackRealloc(struct StackArray* stack, int_t new_capacity) {
    assert(stack != NULL);
    assert(new_capacity >= 0);

    #ifdef BARANOV_V_V_DEBUG

    void* data_tmp = realloc((char*)stack->data_ - sizeof(stack_canary_t), sizeof(Type_t) * new_capacity + 2 * sizeof(stack_canary_t));
    assert(data_tmp != NULL);

    stack->data_ = (Type_t*)((char*)data_tmp + sizeof(stack_canary_t));

    *(stack_canary_t*)((char*)stack->data_ - sizeof(stack_canary_t)) = STACK_CANARY;
    *(stack_canary_t*)((char*)stack->data_ + (new_capacity + 1) * sizeof(Type_t)) = STACK_CANARY;

    #else

    stack->data_ = (Type_t*) realloc(stack->data_, new_capacity * sizeof(Type_t));

    #endif
}

void StackDump(struct StackArray* stack, const char* file_name, FILE* fp, StackErrors err_no) {
    assert(stack != NULL);
    assert(fp != NULL);
    assert(file_name != NULL);

    #ifdef BARANOV_V_V_DEBUG
    fprintf(fp, "Stack name: <%s>\n\n", stack->stack_name_);
    fprintf(fp, "Stack made\n");
    fprintf(fp, "   ON line %d\n", stack->current_line);
    fprintf(fp, "   IN file %s\n\n", stack->current_file);
    #endif // DEBUG

    fprintf(fp, "Error code : %d (%s)\n\n", err_no, ErrorNames[err_no]);
    fprintf(fp, "adress of Stack: [%p]\n", stack);
    fprintf(fp, "{\n    size = %lld\n    capacity = %lld\n", stack->size_, stack->capacity_);
    fprintf(fp, "    data adress: [%p]\n\n", stack->data_);

    #ifdef BARANOV_V_V_DEBUG
    fprintf(fp, "    Data hash value: %lld\n", stack->data_hash);
    fprintf(fp, "    Stack hash value: %lld\n\n", stack->stack_hash);

    fprintf(fp,"    Canary type |  Initial value   |  Current value\n");
    fprintf(fp,"    Stack begin | %llX | %llX\n", STACK_CANARY, stack->canary_begin);
    fprintf(fp,"    Stack end   | %llX | %llX\n", STACK_CANARY, stack->canary_end);
    fprintf(fp,"    Data begin  | %llX | %llX\n", STACK_CANARY, *(stack_canary_t*)((char*)stack->data_ - sizeof(stack_canary_t)));
    fprintf(fp,"    Data end    | %llX | %llX\n", STACK_CANARY, *(stack_canary_t*)((char*)stack->data_ + (stack->capacity_ + 1) * sizeof(Type_t)));
    #endif // BARANOV_V_V_DEBUG

    fprintf(fp, "\n    {\n");
    for (int_t i = 0; i < stack->capacity_; i++) {
        if (i < stack->size_) {
            fprintf(fp, "       *[%lld] = %g\n",i ,stack->data_[i]);
        }
        else {
            fprintf(fp, "        [%lld] = %g (Poison!)\n", i, stack->data_[i]);
        }
    }
    fprintf(fp, "   }\n}\n");

}

Error_t Size(StackArray* stack, int_t* size) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    *size = stack->size_;

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Capacity(StackArray* stack, int_t* capacity) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->capacity_ >= 0) {
        *capacity = stack->capacity_;
    }
    else {
        return CAPACITY_ERROR;
    }


    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackIncrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    int_t old_capacity = stack->capacity_;

    if (stack->capacity_ == 0) {
        stack->capacity_ = 1;
    }
    else if (stack->size_ == stack->capacity_) {
        stack->capacity_ = (int_t) stack->capacity_ * REALLOC_VALUE;
    }
    else {
        return NO_INCREASE;
    }

    StackRealloc(stack, stack->capacity_);

    for(int i = old_capacity; i < stack->capacity_; ++i) {
        stack->data_[i] = NAN;
    }

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t StackDecrease(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0 && stack->capacity_ / stack->size_ >= DECREASE_LEVEL && stack->capacity_ >= DECREASE_LEVEL) {

        stack->capacity_ = (int_t) stack->capacity_ / REALLOC_VALUE;
        StackRealloc(stack, stack->capacity_);

    }
    else {
        return NO_DECREASE;
    }

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Construct(int start_size, struct StackArray* new_stack) {
    assert(start_size >= 0);

    #ifdef BARANOV_V_V_DEBGU
    new_stack->canary_begin = CANARY;
    new_stack->canary_end = CANARY;
    #endif // BARANOV_V_V_DEBGU

    new_stack->size_ = 0;
    new_stack->capacity_ = start_size;

    #ifdef BARANOV_V_V_DEBUG

    void* data_tmp = calloc(1, sizeof(Type_t) * start_size + 2 * sizeof(stack_canary_t));
    assert(data_tmp != NULL);

    new_stack->data_ = (Type_t*)((char*)data_tmp + sizeof(stack_canary_t));

    *(stack_canary_t*)((char*)new_stack->data_ - sizeof(stack_canary_t)) = STACK_CANARY;
    *(stack_canary_t*)((char*)new_stack->data_ + (start_size + 1) * sizeof(Type_t)) = STACK_CANARY;

    #else

    new_stack->data_ = (Type_t*) calloc(start_size, sizeof(Type_t));

    #endif // BARANOV_V_V_DEBUG


    for(int_t i = 0; i < start_size; ++i) {
        new_stack->data_[i] = NAN;
    }

    HASHSTACK(new_stack);
    ASSERT_OK(new_stack);
    return SUCCESS;
}

Error_t ShrinkToFit(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    stack->capacity_ = stack->size_;
    StackRealloc(stack, stack->capacity_);

    HASHSTACK(stack);
    ASSERT_OK(stack);

    return SUCCESS;
}

Error_t Push(struct StackArray* stack, Type_t value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    StackIncrease(stack);
    (stack->data_)[stack->size_++] = value;

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Top(struct StackArray* stack, Type_t* value) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    if (stack->size_ > 0) {
        *value = stack->data_[stack->size_ - 1];
        return SUCCESS;
    }

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return TOP_ERROR;
}

Error_t Pop(struct StackArray* stack) {
    assert(stack != NULL);
    ASSERT_OK(stack);

    StackDecrease(stack);
    if (stack->size_ > 0) {
        stack->data_[--stack->size_] = NAN;
    }
    else {
        return POP_ERROR;
    }

    HASHSTACK(stack);
    ASSERT_OK(stack);
    return SUCCESS;
}

Error_t Destroy(struct StackArray* stack) {
    assert(stack != NULL);

    stack->size_ = -1;
    stack->capacity_ = -1;

    #ifdef BARANOV_V_V_DEBUG

    free(stack->data_ - sizeof(stack_canary_t));

    #else

    free(stack->data_);

    #endif // BARANOV_V_V_DEBUG

    stack->data_ = NULL;

    return SUCCESS;
}

