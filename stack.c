#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "token.h"

#define OUT_STACK_SIZE 9
#define OPE_STACK_SIZE 5

/*******************************************************
********************************************************
stack module

function : 
st_stack()
st_init_stack()
st_frech_temp_list()
st_print()
st_push_out()
st_prior()
st_compare_prior()
st_pop_ope()
st_free_stack()
********************************************************
*******************************************************/


/*******************************************************
struct STACK{
out_num : index of out_stack[]
ope_num : index of ope_stack[]
};
*******************************************************/

struct STACK{
  int out_num;
  int ope_num;
};

/*******************************************************
struct PREFIX_CHECK{
prefix_check[] : struct LIST **
prefix_check_index : index of prefix_check[]
};
*******************************************************/

struct PREFIX_CHECK{
  struct LIST **prefix_check;
  int  *out_flag;
  int prefix_check_index;
};

/*******************************************************
enum PRIORITY{
     PR1 : TO_IT
     PR2 : TO_PLUS, TO_MINUS...
     PR3 : TO_EQUAL...
};
*******************************************************/

enum PRIORITY{
	      PR1,
	      PR2,
	      PR3
};

/*******************************************************
prototype decaration
*******************************************************/

static struct LIST **out = NULL;
static struct LIST **ope = NULL;

static struct STACK *st_init_stack(void);
static void   st_fresh_out(struct STACK *stk, struct LIST **out_p);
static void   st_fresh_ope(struct STACK *stk, struct LIST **ope_p);

//prefix_check
static struct PREFIX_CHECK *st_init_prefix_check(void);
static void   st_push_prefix_check(struct PREFIX_CHECK *check,struct LIST *list);
static int    st_check_out_flag(struct PREFIX_CHECK *check,struct LIST *list);
static void   st_change_out_flag(struct PREFIX_CHECK *check, struct LIST *list);
static void   st_free_prefix_check(struct PREFIX_CHECK *check);
static void   st_connect_out(struct LIST **out_p, struct STACK *stk);
static struct LIST *st_connect_to_prefix(struct LIST **out_p,
                                          struct STACK *stk,
                                          struct LIST *pre,
                                          int flag);

static void   st_print(struct LIST **out);
static void   st_push_out(struct STACK *stk, struct LIST **out_p,
                          struct LIST  *list);
static void   st_push_ope(struct STACK *stk, struct LIST **ope_p,
                          struct LIST  *list);
static enum   PRIORITY st_prior(struct LIST *list);
static int    st_compare_prior(enum PRIORITY pre_pr, enum PRIORITY cur_pr);
static struct LIST *st_pop_ope(struct STACK *stk, struct LIST **ope_p);
static struct LIST **st_free_ope(struct LIST **ope_stack);

/*******************************************************
st_stack()

parameter : struct LIST *list

return : (struct LIST **)out
*******************************************************/

struct LIST **st_stack(struct LIST *list){
  struct STACK *stk   = NULL;
  struct LIST  *pop   = (struct LIST *)0;
  struct LIST  *pre   = (struct LIST *)0;   

  //default set
  struct LIST **address = NULL;
	int     		  *flag   = NULL;
	int 	   *denomi_flag = NULL;
	int 	  address_index = 0;

  //reconnect list by out stack
  struct PREFIX_CHECK *check = NULL;
  
  //create pointer area of struct STACK
  stk  =  st_init_stack();

  //create out area  && crate ope area
  out = calloc(1, sizeof(struct LIST *)*OUT_STACK_SIZE);
  if(out == NULL){
    printf("error : not create malloc area");
    exit(113);}
  ope = calloc(1, sizeof(struct LIST *)*OPE_STACK_SIZE);
  if(ope == NULL){
    printf("error : not create malloc area");
    exit(113);}

  
  //**********************************
  //prefix no exist in a list
  if(li_get_max_prefix_count()==0){
        while(list != NULL){
          switch(list->type){
            case TO_MI:
            case TO_MN:
            case TO_MSQRT:
                //list  = list->next;
                st_push_out(stk, out, list);
                break;
            default:
                //list = list->next;
                st_push_ope(stk, ope, list);
                break;
          }
          pre = list;
          list = list->next;
        }
      //pop all of ope
      if(stk->ope_num != 0){
        do{
          stk->ope_num--;
          pop = ope[stk->ope_num];
          st_push_out(stk, out, pop);
        }while(stk->ope_num != 0);
      }

  }else{//prefix exists 
/* 2021/12/13
        //deault setting 
        address = calloc(1, sizeof(struct LIST *)*PREFIX_STACK_SIZE);
        if(address == NULL){
          printf("not create calloc area\n");
          exit(113);}
        flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
        if(flag == NULL){
          printf("not create calloc area\n");
          exit(113);}
        denomi_flag = calloc(1, sizeof(int)*PREFIX_STACK_SIZE);
        if(denomi_flag == NULL){
          printf("not create calloc area\n");
          exit(113);}
        for(int i=0; i<PREFIX_STACK_SIZE;i++){
          denomi_flag[i] = ON;
        }

        //reconnect list by prefix_check
        check = st_init_prefix_check();
        
	
  retry:
    if(li_check_type(list) == 0){
        if(list->next != NULL){
              switch(list->type){
              case TO_MI:
              case TO_MN:
                  st_push_out(stk, out, list);
                  break;
              default:
                  st_push_ope(stk, ope, list);
                  break;
              }
              list = list->next ;
              goto retry;
        }else{//list->next == (struct LIST *)0
              if(address_index == 0){
                    goto out;
              }else{
                    //
                    goto check;
              }
        }
      }else{//prefix type
          //if prefix not appear
          if(check->prefix_check[0] == (struct LIST *)0){
              address[address_index] = list;
              flag[address_index] = 0;
              ++(address_index);
              pre = list;
              list = list->nume;
              goto retry;
          }

          //check prefix_check is ON or OFF
          

          if(st_check_out_flag(check, list) == OFF){
                st_push_out(stk, out, list);
                goto retry;
          }else{//prefix_check[prefix_check_index] == ON then push both
                address[address_index] = list;
                flag[address_index] = 0;
                ++(address_index);

                //push to prefix_check
                st_push_prefix_check(check, list);

                //fresh array
                st_fresh_out(stk,out);
                st_fresh_ope(stk,ope);
                pre = list;
                list = list->nume;
                goto retry;
          }//prefix_check[prefix_check_index] == ON
      }//li_check_type(list)

	check:
		list = address[address_index-1];
    pre = list;
		if(flag[address_index-1] == 0){
        if(list->denomi != (struct LIST *)0){
            flag[address_index-1] = 1;
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

	out:
		free(address);
		free(flag);
		free(denomi_flag);

  //here is . low layer of list push to  out stack


2021/12/13  */

  }//prefix exists


  

  //st_print(out);
  
  free(stk);
  ope = st_free_ope(ope);
  //prefixがある場合
  //st_free_prefix_check(check);
  
  return (struct LIST **)out;
}


/*******************************************************
st_init_stack()
paremeter : void
return : struct STACK *stk
*******************************************************/
static struct STACK *st_init_stack(void){
  struct STACK *stk;

  stk = calloc(1, sizeof(struct STACK));
  if(stk == NULL){
    printf("error : not create calloc area");
    exit(0); }

  //init index
  stk->out_num = 0;
  stk->ope_num = 0;
  
  return (struct STACK *)stk;
}


/*******************************************************
st_print()
paremeter : struct LIST **out_stack
return : void
*******************************************************/
static void st_print(struct LIST **out_p){
  int i = 0;
  printf("\tout_stack\n");
  while(out_p[i]!= NULL){
    if(out_p[i] == (struct LIST *)0)break;
    printf("\t[out_p[%d] var: %6s, type: %d]\n",
	   i, out_p[i]->var, out_p[i]->type);
    ++i;
  }
  printf("\n");
  return;
}


/*******************************************************
st_fresh_out()
parameter : struct STACK *stk
            struct LIST  **out_p      
return : void
*******************************************************/
static void st_fresh_out(struct STACK *stk, struct LIST **out_p){
  for(int i=0; i<OUT_STACK_SIZE; i++){
    out_p[i] = (struct LIST *)0;
  }
  stk->out_num = 0;
  return;
}

/*******************************************************
st_fresh_ope()
parameter : struct STACK *stk
            struct LIST  **ope_p
return : void
*******************************************************/
static void st_fresh_ope(struct STACK *stk, struct LIST **ope_p){
  for(int i=0; i<OPE_STACK_SIZE; i++){
    ope_p[i] = (struct LIST *)0;
  }
  stk->ope_num = 0;
  return;
}

/*******************************************************
st_init_prefix_check()
parameter : void
return : struct PREFIX_CHECK *check
*******************************************************/
static struct PREFIX_CHECK *st_init_prefix_check(void){
  struct PREFIX_CHECK *check = NULL;

  check = calloc(1,sizeof(struct PREFIX_CHECK));
  if(check == NULL){
    printf("not create calloc area\n");
    exit(113);}
  check->prefix_check = calloc(1, sizeof(struct LIST *)*OUT_STACK_SIZE);
  if(check->prefix_check == NULL){
    printf("not create calloc area\n");
    exit(113);}
  check->out_flag = calloc(1, sizeof(int)*OUT_STACK_SIZE);
  if(check->out_flag == NULL){
    printf("not create calloc area\n");
    exit(113);}

  check->prefix_check_index = 0;

  return (struct PREFIX_CHECK *)check;
}

/*******************************************************
st_push_prefix_check()
parameter : struct LIST *list
            struct PREFIX_CHECK *check
return : void
*******************************************************/
static void st_push_prefix_check(struct PREFIX_CHECK *check,struct LIST *list){
  check->prefix_check[check->prefix_check_index] = list;
  ++(check->prefix_check_index);
  return;
}

/*******************************************************
st_check_out_flag()
parameter : struct PREFIX_CHECK *check
return : int out_flag[]
*******************************************************/
static int st_check_out_flag(struct PREFIX_CHECK *check, struct LIST *list){
  int i = 0;
  while(i < OUT_STACK_SIZE){
    if(check->prefix_check[i] == list) break;
    ++i;
  }
  return (int)check->out_flag[i];
}
/*******************************************************
st_change_out_flag()
parameter : struct PREFIX_CHECK *check
return : int out_flag[]
*******************************************************/
static void st_change_out_flag(struct PREFIX_CHECK *check, struct LIST *list){
  int i = 0;
  while(i < OUT_STACK_SIZE){
    if(check->prefix_check[i]==list)break;
    ++i;
  }
  switch(check->out_flag[i]){
    case 0:
      check->out_flag[i] = 1;
      break;
    case 1:
      check->out_flag[i] = 2;
      break;
    case 2:
      break;
    default:
      break;
  }
  
  return;
}

/*******************************************************
st_free_prefix_check()
parameter : struct PREFIX_CHECK *check
return : void
*******************************************************/
static void st_free_prefix_check(struct PREFIX_CHECK *check){
  free(check->prefix_check);
  free(check->out_flag);
  free(check);
  return;
}

/*******************************************************
st_reconnect_out()
paremeter : struct LIST **out
            struct STACK *stk
return : struct LIST *list
*******************************************************/
static void st_connect_out(struct LIST **out_p,struct STACK *stk){
    int i = 0;
    i = stk->out_num-1;

    while(i > 0){
      out_p[i]->next = out_p[i-1];
      --i;
    }
    out_p[0]->next = (struct LIST *)0;
    return;
}

/*******************************************************
st_connect_to_prefix()
paremeter : struct LIST **out
            struct STACK *stk
            int   flag[address_index-1]
return : struct LIST *list
*******************************************************/
static struct LIST *st_connect_to_prefix(struct LIST **out_p, 
                                    struct STACK *stk,
                                    struct LIST *pre,
                                    int flag){
   if(flag == 0){
     pre->nume = out_p[stk->out_num-1];
   }else if(flag == 1){
     pre->denomi = out_p[stk->out_num-1];
   }else{
     //do nothig
   }
  return (struct LIST *)out_p[0];
}


/*******************************************************
st_push_out()

parameter : struct STACK *stk
            struct LIST  **out_p
            struct LIST  *list

return : void
*******************************************************/
static void st_push_out(struct STACK *stk,
			struct LIST  **out_p,
			struct LIST  *list){
  
  out_p[stk->out_num] = list;
  ++(stk->out_num);
  
  return;
}


/*******************************************************
st_push_ope()

parameter : struct STACK *stk
            struct LIST  **ope_p
            struct LIST  *list

return : void
*******************************************************/
static void st_push_ope(struct STACK *stk,struct LIST  **ope_p,
                        struct LIST  *list){
  enum   PRIORITY pre_pr = PR3;
  enum   PRIORITY cur_pr = PR3;
  struct LIST     *li    = (struct LIST *)0;
  
  if(stk->ope_num == 0){
    ope_p[stk->ope_num] = list;
    stk->ope_num++;
    
  }else{
    pre_pr = st_prior(ope_p[stk->ope_num-1]);
    cur_pr = st_prior(list);
  
    switch(st_compare_prior(pre_pr, cur_pr)){
    case 1://pre > cur
      li = st_pop_ope(stk, ope);
      st_push_out(stk, out, li);  
      st_push_ope(stk, ope_p, list);//recursive st_push_ope()
      break;
      
    case 2://pre < cur
      ope_p[stk->ope_num] = list;
      stk->ope_num++;
      break;
      
    case 3://pre = cur
      li  = st_pop_ope(stk, ope);
      st_push_out(stk, out, li);
      st_push_ope(stk, ope_p, list);//recursive st_push_ope()
      break;
    }    
  }
  return;
}


/*******************************************************
st_prior()

parameter : struct LIST *list

return : (enum PRIORITY) type
*******************************************************/

static enum PRIORITY st_prior(struct LIST *list){
  //need parameter
  switch(list->type){
  case TO_IT:
       return PR1;
  case TO_PLUS:
  case TO_MINUS:
       return PR2;
  case TO_EQUAL:
  case TO_GT:
  case TO_LT:
  case TO_GE:
  case TO_LE:
  case TO_NEQ:
  case TO_MOD:
       return PR3;
  default:
       return PR3;
  }
}


/*******************************************************
st_compare_prior()

parameter : enum PRIORITY pre_pr
            enum PRIORITY cur_pr

return : (int) 1: pre is prior
               2: cur is prior
               3: prior equal
*******************************************************/

static int st_compare_prior(enum PRIORITY pre, enum PRIORITY cur){

    //pre > cur
  if((pre == PR1 && cur == PR2) ||
     (pre == PR1 && cur == PR3) ||
     (pre == PR2 && cur == PR3)){
    return 1;

    //pre < cur
  }else if((pre == PR2 && cur == PR1) ||
	   (pre == PR3 && cur == PR1) ||
	   (pre == PR3 && cur == PR2)){
    return 2;

    //pre = cur
  }else{
    return 3;
  }
}

/*******************************************************
st_pop_ope()

parameter : struct STACK *stk
            struct LIST  **ope_p
            
return : (struct LIST *)list;
*******************************************************/

static struct LIST *st_pop_ope(struct STACK *stk,
			       struct LIST  **ope_p){
  struct LIST *li;

  li    = ope_p[stk->ope_num-1];
  ope_p[stk->ope_num-1] = (struct LIST *)0;
  stk->ope_num--;
  
  return (struct LIST *)li;
}



/*******************************************************
st_free_stack()

parameter : (struct LIST **)out;
            
return : (struct LIST **)0;
*******************************************************/

struct LIST **st_free_stack(struct LIST **out_stack){
   free(out_stack);
  return (struct LIST **)0;
}


/*******************************************************
st_free_ope()

parameter : struct LIST **ope

return : NULL
*******************************************************/

static struct LIST **st_free_ope(struct LIST **ope){
  free(ope);
  return NULL;
}


