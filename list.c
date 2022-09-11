#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "token.h"
#include "in_file.h"
#include "ch_type.h"

/******************************************************
 ******************************************************
list.c module
#include <stdio.h>

function:
*li_addlist();
 malloc_check();
 li_length();
*li_delete();
*li_head_list();
 li_print();
 check_list();
*li_lack_check();
 get_digit();
*get_variable();
*li_lack_insert();
variable_compare();
*******************************************************
*******************************************************/


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
/*
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
*/

/*******************************************************
struct LIST init

parameter : struct LIST *list

return : void
*******************************************************/

struct LIST li = {
		  0.0,
		  NULL,
		  NULL,
		  TO_ETC,
		  (struct LIST *)0,
		  (struct LIST *)0,
		  (struct LIST *)0,
		  (struct LIST *)0,
		  (struct LIST *)0,
		  (struct LIST *)0
};

/*******************************************************
struct KAZU_INFO{
kazu     : float num
kazu_str : copy kazu
};
*******************************************************/

struct KAZU_INFO{
  float kazu;
  char *kazu_str;
};

/*******************************************************
Global pointer of struct LI_PREFIX_STACK 
*******************************************************/
struct LI_PREFIX_STACK *PREFIX_STACK;



static int    get_digit(struct KAZU_INFO *kazu);
static char   *get_variable(void);
static void   malloc_check(struct LIST *list);
static int    variable_compare(char *a);
void   		  li_print(struct LIST *list);
static int 	  li_some_var_check(struct LIST *list);
static struct LIST *li_resolve(struct LIST *list, struct LIST *pre, 
								int flag, int some_var);
static struct LIST *li_lack_insert(struct LIST *list, struct LIST *pre);
static struct KAZU_INFO *li_init_kazu_info(void);
static struct KAZU_INFO *li_free_kazu_info(struct KAZU_INFO *kz);
struct LIST *li_delete_list(struct LIST *list);
struct LI_PREFIX_STACK  *li_init_prefix_stack(void); 
struct LI_PREFIX_STACK  *li_free_prefix_stack(struct LI_PREFIX_STACK *prest);
static void li_push_prefix_stack(struct LI_PREFIX_STACK *prest,
				 struct LIST *data,
				 enum TOKEN_TYPE pre_type);
static void li_max_prefix_count(int address_index);
static void li_delete_prefix_stack(struct LI_PREFIX_STACK *prest);
static void li_delete_prefix_address(struct LI_PREFIX_STACK *prest);
static struct LIST *li_pop_prefix_address(struct LI_PREFIX_STACK *prest);
static int  li_check_prefix_address(struct LI_PREFIX_STACK *prest);
static int  li_check_prefix_stack(struct LI_PREFIX_STACK *prest);
int  li_check_type(struct LIST *lst); 
 

/*******************************************************
*li_add_list()

parameter : struct LIST *list,
            enum TOKEN_TYPE token

return : not exist (struct LIST *)head
         exists    (struct LIST *)head
*******************************************************/

struct LIST *li_add_list(struct LIST *list, enum TOKEN_TYPE token,
			 struct LI_PREFIX_STACK *prest){
  
  struct LIST *head;
  struct LIST *data = (struct LIST *)0;
  float  digit      = 0.0;
  char   *variable  = NULL;
  struct KAZU_INFO *kz;
  
  //copy head of list pointer
  head = list;

  //calloc area(Structual elements are float num and char* num)
  kz = li_init_kazu_info();
  kz->kazu     = 0.0;
  kz->kazu_str = NULL;

  
  if(list == (struct LIST *)0){
       switch(token){
       case TO_MN:
	 	data = (struct LIST *)malloc(sizeof(struct LIST));
	 	malloc_check(data);
	 	in_read_char();
	 	digit = get_digit(kz);
	  	 if(digit != 65535 && digit != 65534){
	   	  data->kazu    = kz->kazu;}
	  	 //if you don't create data->figures calloc area,
	  	 //error will occurs
		data->figures = calloc(1,strlen(kz->kazu_str)+1);
		if(data->figures == NULL)exit(120);
		strcpy(data->figures, kz->kazu_str);
		data->var     = NULL;
		data->type    = TO_MN;
		data->pre     = (struct LIST *)0;
		data->next    = (struct LIST *)0;
		data->nume    = (struct LIST *)0;
		data->denomi  = (struct LIST *)0;
		data->right   = (struct LIST *)0;
		data->left    = (struct LIST *)0;
		break;
		
		case TO_MI:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		in_read_char();
		data->kazu      = 0.0;
		data->figures   = NULL;
		data->type      = TO_MI;
		variable        = get_variable();
		if(*variable != '\0'){
			data->var = calloc(1,strlen(variable)+1);
			strcpy(data->var, variable);
		}
		data->pre       = (struct LIST *)0;
		data->next      = (struct LIST *)0;
		data->nume      = (struct LIST *)0;
		data->denomi    = (struct LIST *)0;
		data->right     = (struct LIST *)0;
		data->left      = (struct LIST *)0;
		break;
		
		case TO_MO:
			in_read_char();
				switch(get_operator()){
			case TO_PLUS:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_PLUS;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
		 
			case TO_MINUS:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_MINUS;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
			
			case TO_EQUAL:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_EQUAL;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
		 
			case TO_IT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_IT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
		
			case TO_GT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_GT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_LT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_LT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_GE:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_GE;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_LE:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_LE;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_NEQ:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_NEQ;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
		  
			case TO_MOD:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_MOD;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
			
			default:
			data = NULL;
			break;
		}
		break;
		
		//prefix notation token	
		case TO_MSUP:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSUP;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data,TO_MSUP);
	 	break;

       	case TO_MSUB:
		data = (struct LIST *)malloc(sizeof(struct LIST)) ;
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSUB;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MSUB);
	 	break;
	 
       	case TO_MFRAC:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MFRAC;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MFRAC);
	 	break;

       	case TO_MFENCED:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MFENCED;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MFENCED);
	 	break;

      	case TO_MSQRT:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSQRT;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MSQRT);
	 	break;

       	case TO_MROOT:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MROOT;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MROOT);
		break;

	 
		case TO_MROW:
		data = NULL;
		break;
		
		case TO_E_MROW:
		data = NULL;
		break;
		
		case TO_E_MSUP:
		data = NULL;
		break;
		
		case TO_E_MSUB:
		data = NULL;
		break;
		
		case TO_E_MFRAC:
		data = NULL;
		
		break;

		case TO_E_MSQRT:
		data = NULL;
		
		break;
		case TO_E_MROOT:
		data = NULL;
		
		break;
		
		default:
		data = NULL;
		break;
		}
		
       //free KAZU_INFO area
       kz = li_free_kazu_info(kz);
       
       head = data;//no need pre
       return (struct LIST *)head;
       
    
  }else{ //if list exists.......................................
      
       switch(token){
		case TO_MN:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		in_read_char();
		digit = get_digit(kz);
		if(digit != 65535 && digit != 65534){
		data->kazu    = kz->kazu;}
		data->figures = calloc(1,strlen(kz->kazu_str)+1);
		if(data->figures == NULL)exit(2);
		strcpy(data->figures, kz->kazu_str);
		data->var     = NULL;
		data->type    = TO_MN;
		data->pre     = (struct LIST *)0;
		data->next    = (struct LIST *)0;
		data->nume    = (struct LIST *)0;
		data->denomi  = (struct LIST *)0;
		data->right   = (struct LIST *)0;
		data->left    = (struct LIST *)0;
		break;
		
		case TO_MI:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		in_read_char();
		data->kazu      = 0.0;
		data->type      = TO_MI;
		variable        = get_variable();
		if(*variable != '\0'){
		data->var = calloc(1,strlen(variable)+1);
		strcpy(data->var, variable);
		}
		data->pre       = (struct LIST *)0;
		data->next      = (struct LIST *)0;
		data->nume      = (struct LIST *)0;
		data->denomi    = (struct LIST *)0;
		data->right     = (struct LIST *)0;
		data->left      = (struct LIST *)0;
		break;
		
		case TO_MO:
			in_read_char();
			switch(get_operator()){
			case TO_PLUS:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_PLUS;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
			
	       	case TO_MINUS:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_MINUS;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
	       
			case TO_EQUAL:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_EQUAL;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
	       
			case TO_IT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_IT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_GT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_GT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_LT:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_LT;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_GE:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_GE;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_LE:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_LE;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
		 	 break;

			case TO_NEQ:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_NEQ;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;

			case TO_MOD:
			data = (struct LIST *)malloc(sizeof(struct LIST));
			malloc_check(data);
			data->kazu   = 0.0;
			data->figures= NULL;
			data->var    = NULL;
			data->type   = TO_MOD;
			data->pre    = (struct LIST *)0;
			data->next   = (struct LIST *)0;
			data->nume   = (struct LIST *)0;
			data->denomi = (struct LIST *)0;
			data->right  = (struct LIST *)0;
			data->left   = (struct LIST *)0;
			break;
			
			default:
			data = NULL;
			break;
			}
			break;

		//prefix notation token	
		case TO_MSUP:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSUP;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest, data, TO_MSUP);
		break;

       	case TO_MSUB:
		data = (struct LIST *)malloc(sizeof(struct LIST)) ;
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSUB;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		data->figures   = 0;
		li_push_prefix_stack(prest,data, TO_MSUB);
		break;
	 
       	case TO_MFRAC:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MFRAC;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest,data, TO_MFRAC);
		break;

       	case TO_MFENCED:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MFENCED;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		break;

       case TO_MSQRT:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MSQRT;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest,data, TO_MSQRT);
		break;

		case TO_MROOT:
		data = (struct LIST *)malloc(sizeof(struct LIST));
		malloc_check(data);
		data->kazu   = 0.0;
		data->figures= NULL;
		data->var    = NULL;
		data->type   = TO_MROOT;
		data->pre    = (struct LIST *)0;
		data->next   = (struct LIST *)0;
		data->nume   = (struct LIST *)0;
		data->denomi = (struct LIST *)0;
		data->right  = (struct LIST *)0;
		data->left   = (struct LIST *)0;
		li_push_prefix_stack(prest,data, TO_MROOT);
		break;

		case TO_MROW:
		data = NULL;
		//TO_MROW isn't stored when prefix_st_level == 0
		if(prest->prefix_st_level > 0){
			li_push_prefix_stack(prest, data, TO_MROW);
		}
		break;
	 
		case TO_E_MROW:
		if(prest->prefix_st_level > 0){
			li_delete_prefix_stack(prest);
		}
		data = NULL;
		break;
		
		case TO_E_MSUP:
		data = NULL;
	 	li_delete_prefix_stack(prest);
	 	break;
	 
		case TO_E_MSUB:
		data = NULL;
		li_delete_prefix_stack(prest);
		break;
		
		case TO_E_MFRAC:
		data = NULL;
		li_delete_prefix_stack(prest);
		break;

		case TO_E_MSQRT:
		data = NULL;
		li_delete_prefix_stack(prest);
		break;
		
		case TO_E_MROOT:
		data = NULL;
		li_delete_prefix_stack(prest);
		break;
	
		default:
		data = NULL;
		break;
    }//switch

       //free KAZU_INFO area
       kz = li_free_kazu_info(kz);
	
       //no exist data
       if(data == NULL){
		   return (struct LIST *)head;
	   }


	//infix only
	if(prest->prefix_ad_level == 0){
		while(list->next != NULL){
		list = list->next;
		}
		list->next = data;
		return (struct LIST *)head;
	}

	//connect the first prefix data
	if(prest->prefix_ad_level == 1){
		if(li_check_type(data)==1){
			while(list->next != NULL){
				list = list->next;
			}
			list->next = data;
			return (struct LIST *)head;
		}			
	}
	
	//make  hierarchical list
	if(li_check_type(data) == 0){
		list = prest->prefix_address[prest->prefix_ad_level-1];

		if(prest->address_flag[prest->prefix_ad_level-1] == ON){
			if(list->nume==(struct LIST *)0){
				list->nume = data;
				if((prest->prefix_stack[prest->prefix_st_level-1]!=TO_MROW)
					&& (prest->prefix_stack[prest->prefix_st_level-1]!=TO_MSQRT)){
					prest->address_flag[prest->prefix_ad_level-1] = OFF;
				}
			}else{//list->nume != (struct LIST *)0
				list = list->nume;
				while(list->next != NULL){
					list = list->next;
				}
				list->next = data;
			}

		}else{//OFF
			if(list->denomi == (struct LIST *)0){
				list->denomi = data;
			}else{
				list = list->denomi;
				while(list->next != NULL){
					list = list->next;
				}
				list->next = data;
			}
		}
	}else{//li_check_type(data) == 1
		list = prest->prefix_address[prest->prefix_ad_level-2];

		if(prest->address_flag[prest->prefix_ad_level-2]==ON){
			if(list->nume == (struct LIST*)0){
				list->nume = data;
				if((prest->prefix_stack[prest->prefix_st_level-2]!=TO_MROW)
					&&(prest->prefix_stack[prest->prefix_st_level-2]!=TO_MSQRT)){
						prest->address_flag[prest->prefix_ad_level-2]=OFF;
				}
			}else{//list->nume != (struct LIST *)0
				list = list->nume;
				while(list->next != NULL){
					list = list->next;
				}
				list->next = data;
			}


		}else{//OFF
			if(list->denomi == (struct LIST *)0){
				list->denomi = data;
			}else{
				list = list->denomi;
				while(list->next!= NULL){
					list = list->next;
				}
				list->next = data;
			}
		}
	}



		
       return (struct LIST *)head;
  }   
}


/*******************************************************
struct KAZU_INFO init

parameter : void

return : struct KAZU_INFO
*******************************************************/

static struct KAZU_INFO *li_init_kazu_info(void){
  struct KAZU_INFO *kz;

  kz = calloc(1, sizeof(struct KAZU_INFO));
  if(kz == NULL){
    printf("error : not create valloc area(kazu_info)");
    exit(115);
  }
  return (struct KAZU_INFO *)kz;
}

/*******************************************************
struct KAZU_INFO free

parameter : void

return : NULL
*******************************************************/

static struct KAZU_INFO *li_free_kazu_info(struct KAZU_INFO *kz){
  free(kz);
  return NULL;
}

/*******************************************************
get_digit()  get digit data

parameter : void

return : digit
*******************************************************/

static int get_digit(struct KAZU_INFO *kz){
  int digit = 0; 
  float num = 0.0;
  int count = 1;
  float sum = 0.0;
  char sum_str[128];//copy of kazu
  int i     = 0;
  
  
  if(in_cur_char() == CH_EOF) return (65535);
  
  if(get_char_type(in_cur_char()) == CH_INTNUM){
          while(get_char_type(in_cur_char()) == CH_INTNUM){
	      sum_str[i++] = in_cur_char();
	      sum_str[i]   = 0;
	      digit        = digit * 10 + in_cur_char() - '0';// (integer)
	      in_read_char();
	      if(get_char_type(in_cur_char()) == CH_DOT)break;
	  	}
	  
	  if(get_char_type(in_cur_char()) == CH_DOT){// (decimal) '0.'
	      sum_str[i++] = in_cur_char();
	      sum_str[i]   = 0;
	      in_read_char();
	    while(get_char_type(in_cur_char()) == CH_INTNUM){
	      sum_str[i++] = in_cur_char();
	      sum_str[i]   = 0;
	        num        = num + (in_cur_char() - '0') * pow(0.1,count);
			++count;
			in_read_char();
	    }
	  }
	  
	  sum          = (float)digit + num;
	  kz->kazu_str = calloc(1, strlen(sum_str)+1);
	  strcpy(kz->kazu_str,sum_str);
	  kz->kazu     = sum;
	  return (1119);
    
  }else if(get_char_type(in_cur_char()) == CH_DOT){// '.5(0.5)'
         sum_str[i++]= in_cur_char();
         sum_str[i]=0;
         in_read_char();
         while(get_char_type(in_cur_char()) == CH_INTNUM){
	  		 	sum_str[i++] = in_cur_char();
	   			sum_str[i]   = 0;
	   			num          = num + (in_cur_char() - '0') * pow(0.1, count);
	   			++count;
	   			in_read_char();
	 	}
	 	kz->kazu     = num;
	 	kz->kazu_str = calloc(1,strlen(sum_str)+1);
	 	strcpy(kz->kazu_str,sum_str);
	 	return (1119);
	 
  }else if(get_char_type(in_cur_char())== CH_MINUS){// '<mn>-1</mn>'
        sum_str[i++]=in_cur_char();
        sum_str[i]=0;
        in_read_char();// read after '-'
	  do{
	      sum_str[i++] = in_cur_char();
	      sum_str[i]   = 0;
	      digit        = digit * 10 + in_cur_char() - '0';// (integer)
	      in_read_char();
	      if(get_char_type(in_cur_char()) == CH_DOT)break;
	  }while(get_char_type(in_cur_char()) == CH_INTNUM);
    
	  if(get_char_type(in_cur_char()) == CH_DOT){// (decimal)
	    sum_str[i++] = in_cur_char();
	    sum_str[i]   = 0;
	    in_read_char();
	    do{
	        sum_str[i++] = in_cur_char();
	        sum_str[i]   = 0;
	        num          = num + (in_cur_char() - '0') * pow(0.1,count);
			++count;
			in_read_char();
	    }while(get_char_type(in_cur_char()) == CH_INTNUM);
	  }
	  sum          = (float)digit + num;
	  sum          = (-1)*sum;// multiply minus
	  kz->kazu     = sum;
	  kz->kazu_str = calloc(1,strlen(sum_str)+1);
	  strcpy(kz->kazu_str,sum_str);
	  return (1119);
	  
  }else{
    return (65534);
  }
}


/*******************************************************
get_variable()  get variable data

parameter : void

return : *char
*******************************************************/

static char *get_variable(void){
  char *str, var[16] = {'\0'};
  int  length = 0;
  
  if(in_cur_char() == CH_EOF) exit(0);

  if(get_char_type(in_cur_char()) == CH_ALPHABET){ // x, bc...
       do{
	     var[length++] = in_cur_char();
	     if(length == 15)break;
	     in_read_char();
       }while(get_char_type(in_cur_char()) == CH_ALPHABET);
       
       var[length++] = '\0';
       str = (char *)malloc(length);
       strcpy(str, var);
       length = 0;
       return(str);
       
  }else if(get_char_type(in_cur_char()) == CH_AND){ //&infin, &sum...
       do{
	     var[length++] = in_cur_char();
	     if(length == 15)break;
	     in_read_char();
       }while(get_char_type(in_cur_char()) == CH_ALPHABET);
       
       var[length++] = '\0';
       str = (char *)malloc(length);
       strcpy(str, var);
       length = 0;
       return(str);

  }else if(get_char_type(in_cur_char())== CH_MINUS){// <mi>-x</mi>...
     do{
	     var[length++] = in_cur_char();
	     if(length == 15)break;
	     in_read_char();
       }while(get_char_type(in_cur_char()) == CH_ALPHABET);
       
       var[length++] = '\0';
       str = (char *)malloc(length);
       strcpy(str, var);
       length = 0;
       return(str);
       
  
  }else{//other
    return (NULL); 
  }
}



/*******************************************************
malloc_check()

parameter : struct LIST *list 
            
return : void
*******************************************************/

static void  malloc_check(struct LIST *list){
  if(list == NULL){
    printf("error malloc");
    exit(0);
  }
  return;
}



/*******************************************************
li_print()

parameter : struct LIST *list
            
return : void
*******************************************************/

void li_print(struct LIST *lst){
	struct LIST **address = NULL;
	int     address_index = 0;
	int     *flag = NULL;
	int 	*denomi_flag  = NULL;
	
	
	printf("\n\tLIST PRINT\n");

	if(lst == NULL){
		printf("nothing list to be printed\n");
		return;
	}

	//create address and  init all (struct LIST*)0
	address = calloc(1, sizeof(struct LIST*)*PREFIX_STACK_SIZE);
	if(address == NULL){
		printf("not create calloc area\n");
		exit(112);}	
	for(int i=0; i<PREFIX_STACK_SIZE; i++){
		address[i] = (struct LIST*)0;
	}
	//create flag area and init all
	flag = calloc(1,sizeof(int)*PREFIX_STACK_SIZE);
	if(flag == NULL){
		printf("not create calloc area");
		exit(112);}
	for(int i=0;i<PREFIX_STACK_SIZE; i++){
		flag[i] = 0;
	}
	denomi_flag = calloc(1,sizeof(int)*PREFIX_STACK_SIZE);
	if(denomi_flag == NULL){
		printf("not create calloc area");
		exit(112);}
	for(int i=0;i<PREFIX_STACK_SIZE;i++){
		denomi_flag[i] = ON;
	}
	//init address_index 
	address_index = 0;
	
	
retry:
	if(li_check_type(lst)==0){
		for(int i=0;i<address_index;i++){
				printf("\t");
		}
		if(lst->type==TO_MI){
			printf("\t\t[type:%d, %s]\n",lst->type,lst->var);
		}else{
			printf("\t\t[type:%d]\n",lst->type);
		}
		if(lst->next != NULL){
			lst = lst->next;
			goto retry;
		}else{//lst->next == NULL
			if(address_index == 0){
				goto out;
			}else{
				goto check;	
			}
		}
		
	}else{//type of prefix
		for(int i=0;i<address_index;i++){
			printf("\t");
		}
		printf("\t\t[type:%d]**\n",lst->type);
		//PUSH address and setting
		address[address_index]=lst;
		flag[address_index] = 0;
		++address_index;
		lst = lst->nume;
		goto retry;
	}

	check:
			lst = address[address_index-1];
			if(flag[address_index-1] == 0){
				if(lst->denomi!= (struct LIST *)0){
					flag[address_index-1] = 1;
					lst = lst->denomi;
					denomi_flag[address_index-1]=OFF;
					goto retry;
				}else{//denomi == (struct LIST *)0
					flag[address_index-1] = 2;
					switch(flag[address_index-2]){
					case 0:
						flag[address_index-2] = 1;
						break;
					case 1:
						flag[address_index-2] = 2;
						break;
					default:
						break;
					}
					goto check;
				}
			}else if(flag[address_index-1] == 1){
				flag[address_index-1] = 2;
					if(lst->denomi != (struct LIST *)0){
						if(denomi_flag[address_index-1] == ON){
							lst = lst->denomi;
							goto retry;
						}else{
							goto check;
						}
					}else{
						goto check;
					}
				
			}else{//flag[address_index-1] == 2 
				if(lst->next != (struct LIST *)0){
					lst = lst->next;
					address[address_index-1] = (struct LIST *)0;
					flag[address_index-1] = 0;
					denomi_flag[address_index-1] = ON;
					--(address_index);
					goto retry;
				}else{
					if(address_index == 1){
						goto out;
					}else{
						address[address_index-1] = (struct LIST *)0;
						flag[address_index-1] = 0;
						denomi_flag[address_index-1] = ON;
						--(address_index);
						goto check;
					}
				}
			}

	
	out:
		//lst = head;
		free(address);
		free(flag);
		free(denomi_flag);
return;
}




/*******************************************************
li_back_print()

parameter : struct LIST *list
            
return : void
*******************************************************/

void li_back_print(struct LIST *list){
  struct LIST *li;
  struct LIST *tail;
  printf("li_back_print\n");
  li = list;
  
  while(li != NULL){
    if(li->next == NULL)break;
    li = li->next;
  }

  tail = li;

  while(tail != NULL){
    printf("\t[digit : %f, var : %s, type : %d]\n",
	   tail->kazu, tail->var, tail->type);
    if(tail->pre == NULL)break;
    tail = tail->pre;
  }
  printf("\n");
  return;
}


/*******************************************************
li_delete_list()

parameter : struct LIST *list
            
return : (struct LIST *) NULL
*******************************************************/
struct LIST *li_delete_list(struct LIST *lst){
	//use prefix exists
	struct LIST **address = NULL;
	int    address_index  = 0;
	int    *flag          = NULL;
	int    *denomi_flag   = NULL;
	struct LIST *tmp      = (struct LIST *)0;

	if(lst == NULL){
		return NULL;
	}

	//*********************************************
	//create address and  init all (struct LIST*)0
	address = calloc(1, sizeof(struct LIST*)*PREFIX_STACK_SIZE);
	if(address == NULL){
		printf("not create calloc area\n");
		exit(112);}	
	for(int i=0; i<PREFIX_STACK_SIZE; i++){
		address[i] = (struct LIST*)0;
	}
	//create flag area and init all
	flag = calloc(1,sizeof(int)*PREFIX_STACK_SIZE);
	if(flag == NULL){
		printf("not create calloc area");
		exit(112);}
	for(int i=0;i<PREFIX_STACK_SIZE; i++){
		flag[i] = 0;
	}
	denomi_flag = calloc(1,sizeof(int)*PREFIX_STACK_SIZE);
	if(denomi_flag == NULL){
		printf("not create calloc area");
		exit(112);}
	for(int i=0;i<PREFIX_STACK_SIZE;i++){
		denomi_flag[i] = ON;
	}
	//init address_index 
	address_index = 0;
	
	
retry:
	if(li_check_type(lst)==0){
		if(lst->next != NULL){
			tmp = lst->next;
			if(lst->type==TO_MI){
				//printf("\tDELETE:%d, %s\n",lst->type,lst->var);
			}else{
				//printf("\tDELETE:%d\n",lst->type);
			}
			free(lst);
			lst = tmp;
			goto retry;
		}else{//lst->next == NULL
			if(lst->type==TO_MI){
				//printf("\tDELETE:%d, %s\n",lst->type,lst->var);
			}else{
				//printf("\tDELETE:%d\n",lst->type);
			}
			free(lst);
			if(address_index == 0){
				goto out;
			}else{
				goto check;	
			}
		}
		
	}else{//type of prefix
		//PUSH address and setting
		address[address_index]=lst;
		flag[address_index] = 0;
		++address_index;
		lst = lst->nume;
		goto retry;
	}

	check:
			lst = address[address_index-1];
			if(flag[address_index-1] == 0){
				if(lst->denomi!= (struct LIST *)0){
					flag[address_index-1] = 1;
					lst = lst->denomi;
					denomi_flag[address_index-1]=OFF;
					goto retry;
				}else{//denomi == (struct LIST *)0
					flag[address_index-1] = 2;
					switch(flag[address_index-2]){
					case 0:
						flag[address_index-2] = 1;
						break;
					case 1:
						flag[address_index-2] = 2;
						break;
					default:
						break;
					}
					goto check;
				}
			}else if(flag[address_index-1] == 1){
				flag[address_index-1] = 2;
					if(lst->denomi != (struct LIST *)0){
						if(denomi_flag[address_index-1] == ON){
							lst = lst->denomi;
							goto retry;
						}else{
							goto check;
						}
					}else{
						goto check;
					}
				
			}else{//flag[address_index-1] == 2 
				if(lst->next != (struct LIST *)0){
					lst = lst->next;
					tmp = address[address_index-1];
					address[address_index-1] = (struct LIST *)0;
					//printf("\tDELETE**:%d\n",tmp->type);
					free(tmp);
					flag[address_index-1] = 0;
					denomi_flag[address_index-1] = ON;
					--(address_index);
					goto retry;
				}else{
					if(address_index == 1){
						//printf("\tDELETE**:%d\n",address[address_index-1]->type);
						goto out;
					}else{
						tmp = address[address_index-1];
						address[address_index-1] = (struct LIST *)0;
						//printf("\tDELETE**:%d\n",tmp->type);
						free(tmp);
						flag[address_index-1] = 0;
						denomi_flag[address_index-1] = ON;
						--(address_index);
						goto check;
					}
				}
			}

	
	out:
		free(address);
		free(flag);
		free(denomi_flag);
		printf("\tFREE ALL LIST\n");

return NULL;
}


/*******************************************************
li_length()   

parameter : struct LIST *list
            
return : number of list length
*******************************************************/

int li_length(struct LIST *list){
  struct LIST *li;
  int length = 0;
  li = list;
  
  while(li != NULL){
    ++length;
    li = li->next;
  }
  if(length >0)printf("list_length: %d\n",length);
  return length;
}


/*******************************************************
li_back_length()   

parameter : struct LIST *list
            
return : number of list length
*******************************************************/

void li_back_length(struct LIST *list){
  int length = 0;
  struct LIST *tail;
  
  while(list != NULL){
    if(list->next == NULL)break;
    list = list->next;
  }
  tail = list;

  while(tail != NULL){
    ++length;
    if(tail->pre == NULL)break;
    tail = tail->pre;
  }
  
  if(length >0)printf("list_length: %d\n",length);
  return;
}


/*******************************************************
li_var_check()

parameter : struct LIST *list 
            
return : (int)
*******************************************************/

struct LIST *li_var_check(struct LIST *list){
  int  some			    = 0;
  struct LIST *pre	    = (struct LIST*)0;
  struct LIST *head     = (struct LIST*)0;
  //use hierarchiacal list 
  struct LIST **address = NULL;
  int     		  *flag = NULL;
  int 	   *denomi_flag = NULL;
  int 	  address_index = 0;

	head = list;

	address = calloc(1, sizeof(struct LIST *)*PREFIX_STACK_SIZE);
	if(address == NULL){
		printf("not create calloc area\n");
		exit(112);}
	flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
	if(flag == NULL){
		printf("not create calloc area\n");
		exit(112);}
	denomi_flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
	if(denomi_flag == NULL){
		printf("not create calloc area\n");
		exit(112);}
	for(int i=0; i<PREFIX_STACK_SIZE;i++){
		denomi_flag[i] = ON;
	}

	//**********************************************
retry:
	if(li_check_type(list) == 0){
		if(list->next != NULL){
			if((list->type == TO_MI) && (strlen(list->var)>1)){
				some = li_some_var_check(list);
				if(some!=0){
					list = li_resolve(list, pre, flag[address_index-1],some);
					if(pre == (struct LIST*)0){
						head = list;
					}
				}
			}//if(list->type==TO_MI)
			pre = list;
			list = list->next ;
			goto retry;
		}else{//list->next == (struct LIST *)0
			if((list->type==TO_MI) && (strlen(list->var)>1)){
				some = li_some_var_check(list);
				if(some!=0){
					list = li_resolve(list, pre, flag[address_index-1], some);
					if(pre == (struct LIST*)0){
						head = list;
					}
				}	
			}
			if(address_index == 0){
				goto out;
			}else{
				goto check;
			}
		}
	}else{//prefix type
		address[address_index] = list;
		flag[address_index] = 0;
		++(address_index);
		pre = list;
		list = list->nume;
		goto retry;
	}

	check:
		list = address[address_index-1];
		if(flag[address_index-1] == 0){
			if(list->denomi != (struct LIST *)0){
				flag[address_index-1] = 1;
				pre = list;
				list = list->denomi;
				denomi_flag[address_index-1] = OFF;
				goto retry;
			}else{//denomi == (struct LIST *)0
					flag[address_index-1] = 2;
					switch(flag[address_index-2]){
						case 0:
							flag[address_index-2] = 1;
							break;
						case 1:
							flag[address_index-2] = 2;
							break;
						default:
							break;
					}
					goto check;
			}
		}else if(flag[address_index-1] == 1){
			flag[address_index-1] = 2;
			if(list->denomi !=(struct LIST *)0){
				if(denomi_flag[address_index-1] == ON){
					pre = list;
					list = list->denomi;
					goto retry;
				}else{//denomi_flag == OFF
					goto check;
				}
			}else{
				goto check;
			}
		}else{//flag[address_index-1]==2
			if(list->next != (struct LIST *)0){
				pre = list;
				list = list->next;
				address[address_index-1] = (struct LIST*)0;
				flag[address_index-1]=0;
				--(address_index);
				goto retry;
			}else{
				if(address_index == 1){
					goto out;
				}else{
					address[address_index-1] = (struct LIST *)0;
					flag[address_index-1] = 0;
					--(address_index);
					goto check;
				}
			}
		}
//
	out:
		free(address);
		free(flag);
		free(denomi_flag);
		printf("\tALL LIST->VAR CHECKED IN ONE LIST\n");
	return (struct LIST *)head;
}


/*******************************************************
static variable_compare()

parameter : array pointer

return : i
*******************************************************/

static int variable_compare(char *a){
 
  const char *d[] = { "&pi",             //0
		      "&ExponentialE",   //1
		      "e",               //2 
		      "&sum",            //3
		      "&#x222b"          //4 'integral'
		      "log",             //5
		      "sin",             //6
		      "cos",             //7
		      "tan",             //8
		      "&ImaginaryI",     //9
		      "&infin",          //10
		      "&sum"             //11
		      "lim",             //12
		      NULL
  };
  //copy
  char *e;
  e = a;
  
  int i = 0;
  while(d[i]!=NULL){
    while(*a == *d[i]){
      	if(*a == '\0'){
			return i;
		}
      	a++;
    	d[i]++;
	}
    i++;
    a = e;//go back to head
  }
    return -1;
}

/*******************************************************
li_some_var_check()  check string mi element

parameter : char *str, struct LIST *list
            
return : (int)a number of varible : undefined string
			  0                   : defined string
*******************************************************/
static int li_some_var_check(struct LIST *list){
	char *str   = NULL;
	int some_var = 0;

	str = calloc(1, strlen(list->var)+1);
	if(str == NULL){
		printf("not create calloc area\n");
		exit(112);
	}	
	strcpy(str, list->var);

	switch(variable_compare(str)){
	case -1: // bc, xy...
		if(strlen(str) > 1){
			if(str[0]=='-'){ //<mi>-x</mi>...
				printf("\t> UNARY OPERATR :%s\n",str);
				break;}
			some_var = strlen(str);
			printf("\tSOME VARIABLES :%s\n",list->var);
		}
		break;
	default: // &infin,e,sin...
		break;
}
	free(str);
	return (int)some_var;
}


/*******************************************************
*li_resolve()

parameter : struct LIST *list
			struct LIST *pre
			int    flag
			int    some_var

return : (struct LIST *)list
*******************************************************/

static struct LIST *li_resolve(struct LIST *list, struct LIST *pre, 
						int flag, int some_var){
  struct LIST *data[some_var];
  char *str;
  
	//create resolved data
	for(int i=0;i<some_var;i++){
		data[i] = calloc(1, sizeof(struct LIST));
		if(data[i] == NULL){
		printf("error : not create calloc area");
		exit(112);
		}
		data[i]->kazu = 0.0;
		data[i]->var  = NULL;
		data[i]->type = TO_MI;
	}

  	//link resolved data
	for(int k=0;k<some_var;k++){
		if(k == 0){                      //first data[0]
			data[k]->next = data[k+1];
		}else if(k == (some_var-1)){     //the last data[some_var]
			data[k]->next = (struct LIST *)0;
		}else{            
			data[k]->next = data[k+1];
		}
	}

	//copy string variables into *str
	str = calloc(1, sizeof(strlen(list->var))+1);
	if(str == NULL){
		printf("not create calloc area\n");
		exit(112);	}
	strcpy(str, list->var);

  //insert a character into a data resolved
	for(int j=0; j<some_var;j++){
		data[j]->var = calloc(1, sizeof(char)+1);
		if(data[j]->var == NULL){
			printf("error: not create calloc area");
			exit(112);
		}
		strncpy(data[j]->var, &str[j], 1);
		printf("\tdata[%d]->var : %s\n", j, data[j]->var);
	}

	//connect resolved list into list

	//list is the first data of list, return head
	if(pre == (struct LIST *)0){
		data[some_var-1]->next = list->next;
		free(list);
		free(str);
		return (struct LIST *)data[0];
	}

	//pre is not equal to NULL
	if(li_check_type(pre) == 0){
		pre->next = data[0];
		data[some_var-1]->next = list->next;
	}else{//li_check_type(pre)==1
		if(flag == 0){
			pre->nume = data[0];
		}else if(flag == 1){
			pre->denomi = data[0];
		}else{
			pre->next = data[0];
		}
	}
		
	free(list);//delete data of list('xyz')
	free(str);
	//return the end of resolved list pointer
	return (struct LIST *)data[some_var-1];
}


/*******************************************************
li_lack_check()  lack point exists or not

parameter : struct LIST *list
            
return : (int)check_count
*******************************************************/

struct LIST *li_lack_check(struct LIST *list){
	struct LIST *head       = (struct LIST *)0;
	struct LIST *pre	    = (struct LIST*)0;
  	
  	//***********************
	//use hierarchiacal list 
	struct LIST **address = NULL;
	int     		  *flag = NULL;
	int 	   *denomi_flag = NULL;
	int 	  address_index = 0;
	head = list;

	address = calloc(1, sizeof(struct LIST *)*PREFIX_STACK_SIZE);
	if(address == NULL){
		printf("not create calloc area\n");
		exit(112);}
	flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
	if(flag == NULL){
		printf("not create calloc area\n");
		exit(112);}
	denomi_flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
	if(denomi_flag == NULL){
		printf("not create calloc area\n");
		exit(112);}
	for(int i=0; i<PREFIX_STACK_SIZE;i++){
		denomi_flag[i] = ON;
	}

	
retry:
	if(li_check_type(list) == 0){
		if(list->next != NULL){
			if(pre != (struct LIST*)0){
				if(list->type == TO_MI){
					if(pre->type == TO_MI || pre->type == TO_MN){
						list = li_lack_insert(list, pre);
					} 
				}
			}//pre != (struct LIST*)0
			pre = list;
			list = list->next ;
			goto retry;
		}else{//list->next == (struct LIST *)0
			if(pre !=(struct LIST *)0){
				if(list->type == TO_MI){
					if(pre->type == TO_MI || pre->type ==TO_MN){
						list = li_lack_insert(list,pre);
					}
				}
			}//pre != (struct LIST*)0
			if(address_index == 0){
				goto out;
			}else{
				goto check;
			}
		}
	}else{//prefix type
		address[address_index] = list;
		flag[address_index] = 0;
		++(address_index);
		pre = list;
		list = list->nume;
		goto retry;
	}

	check:
		list = address[address_index-1];
		if(flag[address_index-1] == 0){
			if(list->denomi != (struct LIST *)0){
				flag[address_index-1] = 1;
				pre = list;
				list = list->denomi;
				denomi_flag[address_index-1] = OFF;
				goto retry;
			}else{//denomi == (struct LIST *)0
					flag[address_index-1] = 2;
					switch(flag[address_index-2]){
						case 0:
							flag[address_index-2] = 1;
							break;
						case 1:
							flag[address_index-2] = 2;
							break;
						default:
							break;
					}
					goto check;
			}
		}else if(flag[address_index-1] == 1){
			flag[address_index-1] = 2;
			if(list->denomi !=(struct LIST *)0){
				if(denomi_flag[address_index-1] == ON){
					pre = list;
					list = list->denomi;
					goto retry;
				}else{//denomi_flag == OFF
					goto check;
				}
			}else{
				goto check;
			}
		}else{//flag[address_index-1]==2
			if(list->next != (struct LIST *)0){
				pre = list;
				list = list->next;
				address[address_index-1] = (struct LIST*)0;
				flag[address_index-1]=0;
				--(address_index);
				goto retry;
			}else{
				if(address_index == 1){
					goto out;
				}else{
					address[address_index-1] = (struct LIST *)0;
					flag[address_index-1] = 0;
					--(address_index);
					goto check;
				}
			}
		}
//
	out:
		free(address);
		free(flag);
		free(denomi_flag);

	
    return (struct LIST *)head;
}


/*******************************************************
li_lack_insert()  insert data to lack point

parameter : struct LIST *list
            
return : struct LIST *head
*******************************************************/
static struct LIST *li_lack_insert(struct LIST *list,struct LIST *pre){
  struct LIST *insert_data = (struct LIST *)0;
  
  //create operartionl data (invisible times)
	insert_data = calloc(1,sizeof(struct LIST));
	if(insert_data == NULL){
		printf("not create calloc area\n");
		exit(112);
	}
	insert_data->kazu = 0.0;
	insert_data->var = NULL;
	insert_data->type = TO_IT;

	pre->next = insert_data;
	insert_data->next = list;

  return (struct LIST *)list;
}


/*******************************************************
li_init_prefix_stack()

parameter : void

return : (struct LI_PREFIX_STACK *)li_pre_stack;
*******************************************************/

struct LI_PREFIX_STACK *li_init_prefix_stack(void){
  struct LI_PREFIX_STACK *prest = NULL;
  
  //create area of structure LI_PREFIX_STACK
  prest = calloc(1, sizeof(struct LI_PREFIX_STACK));
  if(prest == NULL){
    printf("li_error : not create calloc area");
    exit(112);}

  //create area of prefix_stack, and init all TO_ETC
  prest->prefix_stack = calloc(1, sizeof(enum TOKEN_TYPE)*PREFIX_STACK_SIZE);
  if(prest->prefix_stack == NULL){
    printf("li_error : not create calloc area");
    exit(112); }
  for(int i=0; i<PREFIX_STACK_SIZE; i++){
	  prest->prefix_stack[i] = TO_ETC;
  }

  //create area of prefix_address, and init all (struct LIST *)0
  prest->prefix_address = calloc(1,sizeof(struct LIST*)*PREFIX_STACK_SIZE);
  if(prest->prefix_address == NULL){
    printf("li_error : not create calloc area");
    exit(112); }
  for(int i=0; i<PREFIX_STACK_SIZE; i++){
    prest->prefix_address[i] = (struct LIST *)0;
  }

   //calloc area of address_flag,and init all 0
prest->address_flag = calloc(1,sizeof(int)*PREFIX_STACK_SIZE);
	if(prest->address_flag == NULL){
		printf("li_error : not create calloc area");
		exit(112);
	}
	for(int i=0;i<PREFIX_STACK_SIZE;i++){
		prest->address_flag[i]=ON;
	}
  
  
  //init prefix_st_level
  prest->prefix_st_level = 0;
  prest->prefix_ad_level = 0;
  
  return (struct LI_PREFIX_STACK *)prest;
}


/*******************************************************
li_push_prefix_stack()

parameter : struct LI_REFIX_STACK*
            struct LIST**
            enum TOKEN_TYPE
            
return : void
*******************************************************/

static void li_push_prefix_stack(struct LI_PREFIX_STACK *prest,
				 				struct LIST *data,
				 				enum TOKEN_TYPE prefix_type){

  //push prefix_type into prefix_stack
	if(prest->prefix_st_level < PREFIX_STACK_SIZE){
			//push prefix_stack
			prest->prefix_stack[prest->prefix_st_level] = prefix_type;
			++(prest->prefix_st_level);
		
			//push prefix_address
			if(prefix_type != TO_MROW){
				prest->prefix_address[prest->prefix_ad_level] = data;
				++(prest->prefix_ad_level);
				li_max_prefix_count(prest->prefix_ad_level);
			}
			//print level
  		printf("\tPUSH  (st_level,ad_level) = (%d, %d)\n",
  				prest->prefix_st_level, prest->prefix_ad_level);
  
			
	}else{
		printf("li_error :(PUSH) Over PREFIX_STACK_SIZE\n");
	}

  
  //print prefix_address
  	
  	/* for(int j=0; j<(prest->prefix_st_level); j++){
   	 	printf("\t\tprefix_stack[%d]: %d\n",j, prest->prefix_stack[j]);
	}
	for(int j=0;j<(prest->prefix_ad_level);j++){
		printf("\t\tprefix_address[%d]: %d\n",j, prest->prefix_address[j]->type);
	} */
  
  return;
}

/*******************************************************
li_max_prefix_count()

parameter : (int)address_index

return : max_prefix_level
*******************************************************/
static int max_prefix_level = 0;

static void li_max_prefix_count(int address_index){
	if(max_prefix_level < address_index){
		max_prefix_level = address_index;
	}
	return;
}

/*******************************************************
li_get_max_prefix_count()

parameter : void

return : (int)max_prefix_level
*******************************************************/

int li_get_max_prefix_count(void){
	return (int)max_prefix_level;
}

/*******************************************************
li_reset_max_prefix_count()

parameter : (int)address_index
return : void
*******************************************************/
void li_reset_max_prefix_count(){
	max_prefix_level = 0;
}


/*******************************************************
li_delete_prefix_stack()

parameter : struct LI_REFIX_STACK*
            enum TOKEN_TYPE
            
return : void
*******************************************************/

static void li_delete_prefix_stack(struct LI_PREFIX_STACK *prest){
  enum TOKEN_TYPE init_type = TO_ETC;
  	
  if(prest->prefix_st_level == 1){
    prest->prefix_stack[prest->prefix_st_level-1] = init_type;
    --(prest->prefix_st_level);
    li_delete_prefix_address(prest);
	printf("\tDELETE (st_level, ad_level) = (%d, %d)\n",
			prest->prefix_st_level,prest->prefix_ad_level);
	/* for(int j=0; j<(prest->prefix_st_level); j++){
   	 printf("\t\tprefix_stack[%d]: %d\n",j, prest->prefix_stack[j]);
	} */
    return;
  }
  
  if(prest->prefix_stack[prest->prefix_st_level-1] == TO_MROW){
    prest->prefix_stack[prest->prefix_st_level-1] = init_type;
    --(prest->prefix_st_level);


	if(li_check_prefix_address(prest) == 2){
		if(prest->address_flag[prest->prefix_ad_level-1]==ON){
			prest->address_flag[prest->prefix_ad_level-1] = OFF;
		}
	}

	printf("\tDELETE (st_level, ad_level) = (%d, %d)\n",
			prest->prefix_st_level,prest->prefix_ad_level); 
	/* for(int j=0; j<(prest->prefix_st_level); j++){
   	 printf("\t\tprefix_stack[%d]: %d\n",j, prest->prefix_stack[j]);
	} */
  }else{
    prest->prefix_stack[prest->prefix_st_level-1] = init_type;
    --(prest->prefix_st_level);
    li_delete_prefix_address(prest);
	printf("\tDELETE (st_level, ad_level) = (%d, %d)\n",
			prest->prefix_st_level,prest->prefix_ad_level);
	/* for(int j=0; j<(prest->prefix_st_level); j++){
   	 printf("\t\tprefix_stack[%d]: %d\n",j, prest->prefix_stack[j]);
	} */
  }
  return;
}


/*******************************************************
li_delete_prefix_address()

parameter : struct LI_REFIX_STACK*
      
return : void
*******************************************************/

static void li_delete_prefix_address(struct LI_PREFIX_STACK *prest){
  /*
	printf("DELETE ADDRESS\n");
	for(int i=0;i<prest->prefix_ad_level;i++){
		printf("prefix_address[%d]:%p\n",i,prest->prefix_address[i]);
	}*/
	//delete prefix_address[]
    prest->prefix_address[prest->prefix_ad_level-1] = (struct LIST *)0;
	//init address_flag[]
	prest->address_flag[prest->prefix_ad_level-1] = ON;

    --(prest->prefix_ad_level);
	
  return;
}

/*******************************************************
li_pop_prefix_address()

parameter : struct LI_REFIX_STACK*
      
return : (struct LIST *)address of data
*******************************************************/

static struct LIST *li_pop_prefix_address(struct LI_PREFIX_STACK *prest){
  struct LIST *pop_address = (struct LIST *)0;
  
  if(prest->prefix_ad_level > 0){
    pop_address = prest->prefix_address[prest->prefix_ad_level-1];
	printf("\t\tpop_address[%d]->type:%d\n",
			prest->prefix_ad_level-1,pop_address->type);
  }else{
    printf("li_pop_prefix_address error\n");
  }
  return (struct LIST *)pop_address;
}


/*******************************************************
li_check_prefix_address()     [prefix_address]

parameter : struct LI_REFIX_STACK*
            
return : (int)prefix_type 
*******************************************************/

static int li_check_prefix_address(struct LI_PREFIX_STACK *prest){
  struct LIST *address = (struct LIST *)0;
  int   discrimination = 0;
  
  	address = prest->prefix_address[prest->prefix_ad_level-1];
    
    switch(address->type){
    case TO_MSUP:
    case TO_MSUB:
    case TO_MFRAC:
    case TO_MROOT:
      //the number of argument is 2
      discrimination = 2;
      break;
    case TO_MSQRT:
      //the number of argument is 1
      discrimination = 1;
      break;
    default:
      break;
    }
  return (int)discrimination;
}


/*******************************************************
li_check_prefix_stack()     [prefix_stack]

parameter : struct LI_REFIX_STACK*
            
return : (int)prefix_type 
*******************************************************/

static int li_check_prefix_stack(struct LI_PREFIX_STACK *prest){
  enum TOKEN_TYPE token_type = TO_ETC;
  int         discrimination = 0;

  //top of prefix_stack
  token_type = prest->prefix_stack[prest->prefix_st_level-1];

  switch(token_type){
  case TO_MSUP:
  case TO_MSUB:
  case TO_MFRAC:
  case TO_MROOT:
    //the number of argument is 2
    discrimination = 2;
    break;
  case TO_MSQRT:
    //the number of argument is 1
    discrimination = 1;
    break;
  case TO_MROW:
    //the number of argument is 3
    discrimination = 3;
    break;
  default:
    break;
  }
  return (int)discrimination;
}

/*******************************************************
li_check_type()     pop data

parameter : struct LIST*
            
return : (int)prefix_type 
*******************************************************/

int li_check_type(struct LIST *lst){
  int discrimination = 0;
  switch(lst->type){
  case TO_MSUP:
  case TO_MSUB:
  case TO_MFRAC:
  case TO_MROOT:
  case TO_MSQRT:
    discrimination = 1;
    break;
  default:
    break;
  }
  return (int)discrimination;
}



/*******************************************************
li_free_prefix_stack()

parameter : struct LI_REFIX_STACK *li_pre_st
            
return : (struct LI_PREFIX_STACK *)0
********************************************************/

struct LI_PREFIX_STACK *li_free_prefix_stack(struct
					     LI_PREFIX_STACK *li_pre_st){
  free(li_pre_st->prefix_stack);
  free(li_pre_st->prefix_address);
  free(li_pre_st);
  return NULL;
}

