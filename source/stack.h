typedef struct 
{
    int cap;
    int * arr;
    int top;
} Stack;

void stack_init ( Stack * );
void stack_push ( Stack *, int );
int stack_size ( Stack * );
int stack_pop ( Stack * );
int stack_peek ( const Stack * );
int stack_empty ( const Stack * );
void stack_free ( Stack * );
