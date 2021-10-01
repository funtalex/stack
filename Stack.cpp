#include <iostream>
#include <stdlib.h>
#include <assert.h>

const int START_SIZE = 10;
const int UNDEFINED = -1;
const int POISON = 12324567890;
const int DAMAGED_STACK = -888;


struct Stack {
	int max_size;
	int size;
	int* element;
	int error;
};

bool is_damaged(Stack* st) {
	assert(st);
	if (st->error == DAMAGED_STACK) {
		printf("Stack is damaged. You can't access it. Please use other stack\n");
		return true;
	}
	return false;
}

void Ctor(Stack* st) {
	st->max_size = START_SIZE;
	st->size = 0;
	st->element = (int*)calloc(st->max_size, sizeof(int));
	assert(st->element);
	st->error = 0;
}

void Dtor(Stack* st) {
	assert(st);
	st->max_size = UNDEFINED;
	st->size = UNDEFINED;
	st->error = DAMAGED_STACK;
	assert(st->element);
	free(st->element);
}

int push(Stack* st, int value) {
	assert(st);
	if (is_damaged(st))
		return st->error;
	if (st->size == st->max_size) {
		st->max_size *= 2;
		st->element = (int*)realloc(st->element, sizeof(int) * st->max_size);
		assert(st->element);
	}
	st->element[st->size] = value;
	st->size++;
	return 0;
}

int pop(Stack* st) {
	assert(st);
	if (is_damaged(st))
		return DAMAGED_STACK;
	if (st->size == 0) {
		printf("Error. Stack is empty. There is no elements.\n");
		st->error = DAMAGED_STACK;
		return DAMAGED_STACK;
	}
	int value = st->element[--st->size];
	if (st->max_size > START_SIZE && st->max_size / st->size >= 4) {
		st->max_size /= 2;
		st->element = (int*)realloc(st->element, sizeof(int) * st->max_size / 2);
		assert(st->element);
	}
	return value;
}

void clear(Stack* st) {
	assert(st);
	if (is_damaged(st))
		return;
	st->max_size = START_SIZE;
	st->size = 0;
	st->element = NULL;
	st->element = (int*)realloc(st->element, st->max_size * sizeof(char));
}

int size(Stack* st) {
	assert(st);
	return st->size;
}

void test_1() {
	Stack st;
	Ctor(&st);
	if (DAMAGED_STACK != pop(&st)) {
		printf("Mistake in test_1. You cannot pop from empty stack\n");
		return;
	}
	printf("Test_1 is ok\n");
	Dtor(&st);
}

void test_2() {
	Stack st;
	Ctor(&st);
	for (int i = 0; i < 15; ++i) {
		if (DAMAGED_STACK == push(&st, i)) {
			printf("Mistake in test_2. Wrong push\n");
			return;
		}
		if (size(&st) != i + 1) {
			printf("Mistake in test_2. Wrong size\n");
			return;
		}
	}
	for (int i = 0; i < 5; ++i) {
		if (pop(&st) != 14 - i) {
			printf("Mistake in test_2. Wrong pop\n");
		}
	}
	if (st.max_size != 20) {
		printf("Mistake in test_3. Wrong capacity (max size)");
		return;
	}
	printf("Test_2 is ok\n");
	Dtor(&st);
}

void test_3() {
	Stack st;
	Ctor(&st);
	for (int i = 0; i < 50; ++i) {
		push(&st, i * 50);
	}
	clear(&st);
	if (size(&st) != 0) {
		printf("Mistake in test_3. Size must equal zero after clear function/\n");
		return;
	}
	if (st.max_size != 10) {
		printf("Mistake in test_3. Max size (capacity) must equal zero after clear function/\n");
		return;
	}
	if (DAMAGED_STACK != pop(&st)) {
		printf("Mistake in test_3. Stack must be empty after clear function\n");
		return;
	}
	printf("Test_3 is ok\n");
	Dtor(&st);
}

void unit_test() {
	test_1();
	test_2();
	test_3();
}


int main() {
	unit_test();
	return 0;
}