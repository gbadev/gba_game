#include<stdio.h>
#include<stdlib.h>

#include"stack.h"

void stack_init ( Stack * sptr )
{
    sptr->cap = 5;
    sptr->arr = malloc ( sizeof (int) * sptr->cap );
    sptr->top = -1;
}
void stack_push ( Stack * sptr, int arg )
{
    if ( ( sptr->top + 1 ) >= sptr->cap )
    {
       sptr->cap += 5;
       sptr->arr =  realloc ( sptr->arr, sizeof(int) * sptr->cap );
    }
    sptr->arr[ ++(sptr->top) ] = arg;

}
int stack_size ( Stack * sptr )
{
    return ( sptr->top + 1 );
}
int stack_pop ( Stack * sptr )
{
    int rval = stack_peek ( sptr );
    --(sptr->top);
    return rval;
}
int stack_peek ( const Stack * sptr )
{
    if ( stack_empty ( sptr ) )
    {
        fprintf ( stderr, "peek error\n" );
        exit(8);
    }
    return sptr->arr[sptr->top];
}
int stack_empty ( const Stack * sptr )
{
    return ( sptr->top < 0 );
}
void stack_free ( Stack * sptr )
{
    free ( sptr->arr );
}

