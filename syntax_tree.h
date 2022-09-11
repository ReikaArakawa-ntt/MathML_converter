#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_

#include "list.h"
#include "token.h"

/*******************************************************
syn_syntax_tree()

parameter : (struct LIST **)stack;
            
return : (struct LIST *)root;
*******************************************************/

extern struct LIST *syn_make_tree(struct LIST **stack);


/*******************************************************
syn_converter()

parameter : (struct LIST **)root
            
return : void
*******************************************************/

extern void syn_converter(struct LIST *root);

/*******************************************************
syn_option_print()

parameter : void
            
return : void
*******************************************************/

extern void syn_option_print(struct LIST *root);


#endif
