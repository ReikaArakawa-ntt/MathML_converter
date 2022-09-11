#ifndef _STACK_H_
#define _STACK_H_

#include "list.h"
#include "token.h"


/*******************************************************
st_init_stack()

parameter : struct LIST *list
            
return : (struct STACK **)stk
*******************************************************/

extern struct LIST **st_stack(struct LIST *list);


/*******************************************************
st_free_stack()

parameter : (struct LIST **)out;
            
return : (struct LIST **)0;
*******************************************************/

extern struct LIST **st_free_stack(struct LIST **out_stack);


#endif 
