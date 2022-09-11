#ifndef _LIST_H_
#define _LIST_H_

#include "token.h"
#include "in_file.h"


/*******************************************************
typedef struct LIST
float               : kazu
char                : *figures //copy kazu
char pointer        : *var
enum TOKEN_TYPE     : type
struct LIST pointer : *pre     //interactive LIST
struct LIST pointer : *next    //interactive next
struct LIST pointer : *nume    // only use prefix
struct LIST pointer : *denomi  // only use prefix
struct LIST pointer : *right
struct LIST pointer : *left
*******************************************************/

struct LIST{
  float  kazu;
  char   *figures;
  char   *var;
  enum   TOKEN_TYPE type;
  struct LIST *pre;
  struct LIST *next;
  struct LIST *nume;
  struct LIST *denomi;
  struct LIST *right;
  struct LIST *left;
};


#define PREFIX_STACK_SIZE 6

/*******************************************************
struct LI_PREFIX_STACK

enum : TO_TYPE array
struct LIST** : PREFIX_ADDRESS 
int  : STACK_ST_LEVEL
int  : STACK_AD_LEVEL
INT * : ADDLESS_FLAG
*******************************************************/

struct LI_PREFIX_STACK{
  enum TOKEN_TYPE *prefix_stack;
  struct LIST  **prefix_address;
  int           prefix_st_level;
  int           prefix_ad_level;
  int           *address_flag;
};

#define ON 1
#define OFF 0

/*******************************************************
*add_list()

parameter : (struct LIST)list
            (enum TOKEN_TYPE) token
            (struct LI_PREFIX_STACK) *prest

return : (struct LIST )head pointer
*******************************************************/

extern struct LIST *li_add_list(struct LIST *list,
				enum TOKEN_TYPE token,
				struct LI_PREFIX_STACK *prest);

/*******************************************************
li_print()

parameter : struct LIST *list
            
return : void
*******************************************************/

extern void li_print(struct LIST *list);

/*******************************************************
li_back_print()

parameter : struct LIST *list
            
return : void
*******************************************************/

extern void li_back_print(struct LIST *list);


/*******************************************************
li_delete_list()

parameter : struct LIST *list

return : (struct LIST *)NULL
*******************************************************/

extern struct LIST *li_delete_list(struct LIST *list);




/*******************************************************
li_length()

parameter : struct LIST *list
            
return : (int)length
*******************************************************/

extern int li_length(struct LIST *list);


/*******************************************************
li_back_length()

parameter : struct LIST *list
            
return : void
*******************************************************/

extern void li_back_length(struct LIST *list);




/*******************************************************
 *li_var_check()

parameter : struct LIST *list 
            
return : (struct LIST *)head
*******************************************************/

extern struct LIST *li_var_check(struct LIST *list);



/*******************************************************
li_lack_check()

parameter : struct LIST *list

return : (int)check_count
*******************************************************/

extern struct LIST *li_lack_check(struct LIST *list);

/*******************************************************
li_check_type()

parameter : struct LIST *list

return : (int)check_count
*******************************************************/

extern int li_check_type(struct LIST *list);


/*******************************************************
li_get_max_prefix_count()

parameter : void

return : (int)max_prefix_level
*******************************************************/

extern int li_get_max_prefix_count(void);


/*******************************************************
li_reset_max_prefix_count()

parameter : (int)address_index

return : void
*******************************************************/

extern void li_reset_max_prefix_count(void);

/*******************************************************
li_init_prefix_stack()

parameter : struct LI_REFIX_STACK*
            
return : void
********************************************************/

extern struct LI_PREFIX_STACK *li_init_prefix_stack(void);


/*******************************************************
li_free_prefix_stack()

parameter : struct LI_REFIX_STACK li_pre_st
            
return : (struct LI_PREFIX_STACK *)0
********************************************************/

extern struct LI_PREFIX_STACK *li_free_prefix_stack(struct
						    LI_PREFIX_STACK
						    *li_pre_st);

#endif
