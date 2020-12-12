#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once

const long long poison = -666;
const int max_value = 2147483637;
const long long canary = 0x2237EA177;
const int canary_size = 4; 
const int decrement = 5;

typedef struct{
	long long chop1;
	int capacity;
	int size;
	int* elem;
	long long hash_summ;
	long long chop2;
}Stack;

enum error {
	stack_ok,
	w_value_cap,
	w_value_size,
	size_lrg_cap,
	w_value_chop1,
	w_value_chop2,
	null_ptr,
	w_value_canary1,
	w_value_canary2,
	w_val_hsh_sm
};


////The function create stack////
void constructor(Stack* stack);

////The function checks the stack data////
int verification(Stack* stack);

////The function prints stack data and errors////
void stack_dump(Stack* stack, int error);

////The function change stack capacity////
void cap_change(Stack* stack);

////The function puts an element on the stack////
void push(Stack* stack, int elem);

////The function returns an element from the stack////
int pop(Stack* stack);

////The function destruct stack data////
void destructor(Stack* stack);
