const long long poison = -666;
const int max_value = 2147483637;
const long long canary = 0x2237E4177;

typedef struct{
	long long chop1;
	int capacity;
	int size;
	long long hash_summ;
	long long* elem;
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

void push(Stack* stack, long long elem);
void constructor(Stack* stack);
int verification(Stack* stack);
void stack_dump(Stack* stack, int error, int str_num);
void cap_change(Stack* stack);
long long pop(Stack* stack);
void destructor(Stack* stack);