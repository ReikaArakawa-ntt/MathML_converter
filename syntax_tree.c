#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "token.h"

/*******************************************************
********************************************************
syntax_tree module

function:
syn_make_tree()
count_stack()
*index_stack()
syn_push()
syn_pop()
*syn_free()
syn_stack_print()
syn_print_tree()
syn_contents_print()
syn_converter()
*******************************************************
*******************************************************/


/*******************************************************
prototype declare
*******************************************************/

void               syn_converter(struct LIST *tree);
static int         count_stack(struct LIST **out_stack);
static struct LIST **create_stack(int out_stack_num);
static struct LIST **Parse(struct LIST **out_stack, int i,
			  struct LIST **stack);
static void        syn_push(struct LIST *top, struct LIST **stack);
static struct LIST *syn_pop(struct LIST **stack);
static void        syn_free(struct LIST **stack,int out_stack_num);
int                syn_prefix_check(struct LIST *root);

//int count_prefix = 0;//use syn_prefix_check()


/*******************************************************
syn_make_tree()

parameter : (struct LIST **)stack
            
return : (struct LIST **)root
*******************************************************/

struct  LIST  *syn_make_tree(struct LIST **out_stack){
  struct LIST *root    = (struct LIST *)0;
  struct LIST **stack  = (struct LIST **)0;
  int    out_stack_num = 0;
 
  out_stack_num  = count_stack(out_stack);
  stack          = create_stack(out_stack_num);
  out_stack_num  = out_stack_num - 1; //no count NULL
  
  for(int i=0; i<out_stack_num; i++)
    stack = Parse(out_stack, i, stack);
  
  root    = syn_pop(stack);
  
  syn_free(stack, out_stack_num);
  
  return (struct LIST *)root;
}


/*******************************************************
syn_count_stack()

parameter : (struct LIST **)stack
            
return : (int)count
*******************************************************/

static int count_stack(struct LIST **out_stack){
  int stack_count = 1;
  int i           = 0;
  
  do{
    ++i;
    ++stack_count;  
  }while(out_stack[i] != NULL);

  return (int)stack_count;
}


/*******************************************************
create_stack()

parameter : (int)out_stack_num
            
return :(struct LIST **)stack
*******************************************************/

static struct LIST **create_stack(int out_stack_num){
  struct LIST **stack = (struct LIST **)0;
  
  stack = (struct LIST **)malloc(sizeof(struct LIST *)*out_stack_num);
  if(stack == NULL){
    printf("error : not create malloc area");
    exit(0);    
  }
  for(int j=0; j<out_stack_num; j++){
    stack[j] = (struct LIST *)0;
  }
  return (struct LIST **)stack;
}


/*******************************************************
Parse()

parameter : struct LIST **out_stack, int num, struct LIST *stack
            
return : void
*******************************************************/

static struct LIST **Parse(struct LIST **out_stack,
			   int num,
			   struct LIST **stack){

  switch(out_stack[num]->type){
  case TO_MI:
  case TO_MN:
  case TO_MSQRT:
  case TO_MFENCED:
  case TO_MSUP:
  case TO_MSUB:
  case TO_MROOT:
  case TO_MFRAC:
    out_stack[num]->right = (struct LIST *)0;
    out_stack[num]->left  = (struct LIST *)0;
    break;
    
  default:
    out_stack[num]->right = syn_pop(stack);
    out_stack[num]->left  = syn_pop(stack);
    break;
  }
  syn_push(out_stack[num], stack);
  
  return (struct LIST **)stack;
}


/*******************************************************
syn_push()

parameter : struct LIST *top, struct LIST **stack
            
return : void
*******************************************************/

static void syn_push(struct LIST *top, struct LIST **stack){
  int i = 0;
  
  while(stack[i] != (struct LIST *)0)
    ++i;

  stack[i]   = (struct LIST *)top;
}

/*******************************************************
syn_pop()

parameter : struct LIST **stack
            
return : struct LIST *pop
*******************************************************/

static struct LIST *syn_pop(struct LIST **stack){
  struct LIST *pop = (struct LIST *)0;
  int i            = 0;

  while(stack[i] != (struct LIST *)0)
    ++i;
  
  pop        = stack[i-1];
  stack[i-1] = (struct LIST *)0;
 
  return (struct LIST *)pop;
}


/*******************************************************
syn_free()

parameter : int *index_stack
            
return : (int *)0
*******************************************************/

static void syn_free(struct LIST **stack, int out_stack_num){
  for(int i=0; i<out_stack_num; i++)
    free(stack[i]);
}


/*******************************************************
syn_converter()

parameter : (struct LIST *)root
            
return : void  \n
*******************************************************/
int depth = -1;

extern FILE *outfp;

void syn_converter(struct LIST *tree){
  
  
  if(tree != (struct LIST *)0){
    ++depth;

    //printf("%d",depth);
    
    for(int j=0; j<=depth; j++){
	printf("\t");
	fprintf(outfp,"\t");
    }
    if(tree->type == TO_MI ||  tree->type == TO_MN){
      //no needed write <apply>
    }else{
      printf("<apply>");
      fprintf(outfp,"<apply>");
    }
    
    switch(tree->type){
      case TO_IT:
      printf("<times/>\n");
      fprintf(outfp,"<times/>\n");
      break;
    case TO_EQUAL:
      printf("<eq/>\n");
      fprintf(outfp,"<eq/>\n");
      break;
    case TO_PLUS:
      printf("<plus/>\n");
      fprintf(outfp,"<plus/>\n");
      break;
    case TO_MINUS:
      printf("<minus/>\n");
      fprintf(outfp,"<minus/>\n");
      break;
    case TO_GT:
      printf("<gt/>\n");
      fprintf(outfp,"<gt/>\n");
      break;
    case TO_LT:
      printf("<lt/>\n");
      fprintf(outfp,"<lt/>\n");
      break;
    case TO_GE:
      printf("<geq/>\n");
      fprintf(outfp,"<geq/>\n");
      break;
    case TO_LE:
      printf("<leq/>\n");
      fprintf(outfp,"<leq/>\n");
      break;
    case TO_NEQ:
      printf("<neq/>\n");
      fprintf(outfp,"<neq/>\n");
      break;
    case TO_MOD:
      printf("<rem/>\n");
      fprintf(outfp,"<rem/>\n");
    default:
      break;
    }

    //recursion
    syn_converter(tree->left);
    
    switch(tree->type){
    case TO_MI:
      // monomial <mi>-x</mi>-----------------------------
      if(tree->var[0] == '-'){ 
	printf("<apply><minus/>\n");
	fprintf(outfp,"<apply><minus/>\n");
	  for(int j=0; j<=depth+1; j++){
	    printf("\t");
	    fprintf(outfp,"\t");
	  }
	tree->var = &(tree->var[1]);
	printf("<ci>%s</ci>\n",tree->var);
	fprintf(outfp,"<ci>%s</ci>\n",tree->var);
	  for(int j=0; j<=depth; j++){
	    printf("\t");
	    fprintf(outfp,"\t");
	  }
	printf("</apply>\n");
	fprintf(outfp,"</apply>\n");
	//------------------------------------------------
	
      }else{	
	printf("<ci>%s</ci>\n",tree->var);
	fprintf(outfp,"<ci>%s</ci>\n",tree->var);
      }
      break;

      
    case TO_MN:
      //monomial <mn>-0.5</mn>----------------------------
      if(tree->kazu < 0){
	   printf("<apply><minus/>\n");
	   fprintf(outfp,"<apply><minus/>\n");
	   
	   for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
	   }
	   
	   printf("<cn>%s</cn>\n",&(tree->figures[1]));
	   fprintf(outfp,"<cn>%s</cn>\n",&(tree->figures[1]));
	   
	   for(int j=0; j<=depth; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
	   }
	   printf("</apply>\n");
	   fprintf(outfp,"</apply>\n");
     //monomial-------------------------------------------
	   
      }else{
	   printf("<cn>%s</cn>\n",tree->figures);
	   fprintf(outfp,"<cn>%s</cn>\n",tree->figures);
      }
      break;


    //prefix ******************************************
    case TO_MSQRT:
      printf("<root/>\n");
      fprintf(outfp,"<root/>\n");

      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      
      if(tree->nume->next == (struct LIST *)0){
	  if(tree->nume->type == TO_MI){
	    printf("<ci>%s</ci>\n", tree->nume->var);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->var);
	  }else if(tree->nume->type == TO_MN){
	    printf("<cn>%s</cn>\n",tree->nume->figures);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->figures);
	  }else{
	  //do nothing
	  }
      }else{
	//recursion syn_converter(tree->nume);
      }
      
      for(int j=0; j<=depth; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      printf("</apply>\n");
      fprintf(outfp,"</apply>\n");
      break;

      
    case TO_MFENCED:
      break;
      
    case TO_MSUP:
      printf("<power/>\n");
      fprintf(outfp,"<power/>\n");
      
      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      
      if(tree->nume->next == (struct LIST *)0){
	  if(tree->nume->type == TO_MI){
	    printf("<ci>%s</ci>\n", tree->nume->var);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->var);
	  }else if(tree->nume->type == TO_MN){
	    printf("<cn>%s</cn>\n",tree->nume->figures);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->figures);
	  }else{
	  //do nothing
	  }
      }else{
	//recursion syn_converter(tree->nume);
      }
      
      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }

      if(tree->denomi->next == (struct LIST *)0){
	   if(tree->denomi->type == TO_MI){
	     printf("<ci>%s</ci>\n",tree->denomi->var);
	     fprintf(outfp, "<ci>%s</ci>\n",tree->denomi->var);
	   }else if(tree->denomi->type == TO_MN){
	     printf("<cn>%s</cn>\n",tree->denomi->figures);
	     fprintf(outfp,"<cn>%s</cn>\n",tree->denomi->figures);
	   }else{
	     //do nothing
	   }

      }else{
	//recursion syn_converter(tree->denomi)
      }

      for(int j=0; j<=depth; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      printf("</apply>\n");
      fprintf(outfp,"</apply>\n");
      break;
      
    case TO_MSUB:
      
      break;
    case TO_MFRAC:
      printf("<devide/>\n");
      fprintf(outfp,"<devide/>\n");

      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      
      if(tree->nume->next == (struct LIST *)0){
	  if(tree->nume->type == TO_MI){
	    printf("<ci>%s</ci>\n", tree->nume->var);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->var);
	  }else if(tree->nume->type == TO_MN){
	    printf("<cn>%s</cn>\n",tree->nume->figures);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->figures);
	  }else{
	  //do nothing
	  }
      }else{
	//recursion syn_converter(tree->nume);
      }
      
      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }

      if(tree->denomi->next == (struct LIST *)0){
	   if(tree->denomi->type == TO_MI){
	     printf("<ci>%s</ci>\n",tree->denomi->var);
	     fprintf(outfp, "<ci>%s</ci>\n",tree->denomi->var);
	   }else if(tree->denomi->type == TO_MN){
	     printf("<cn>%s</cn>\n",tree->denomi->figures);
	     fprintf(outfp,"<cn>%s</cn>\n",tree->denomi->figures);
	   }else{
	     //do nothing
	   }

      }else{
	//recursion syn_converter(tree->denomi)
      }

      for(int j=0; j<=depth; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      printf("</apply>\n");
      fprintf(outfp,"</apply>\n");

      
      break;
    case TO_MROOT:
      printf("<root/>\n");
      fprintf(outfp,"<root/>\n");


      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      
      if(tree->nume->next == (struct LIST *)0){
	  if(tree->nume->type == TO_MI){
	    printf("<ci>%s</ci>\n", tree->nume->var);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->var);
	  }else if(tree->nume->type == TO_MN){
	    printf("<cn>%s</cn>\n",tree->nume->figures);
	    fprintf(outfp,"<cn>%s</cn>\n",tree->nume->figures);
	  }else{
	  //do nothing
	  }
      }else{
	//recursion syn_converter(tree->nume);
      }
      
      for(int j=0; j<=depth+1; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }

      if(tree->denomi->next == (struct LIST *)0){
	   if(tree->denomi->type == TO_MI){
	     printf("<ci>%s</ci>\n",tree->denomi->var);
	     fprintf(outfp, "<ci>%s</ci>\n",tree->denomi->var);
	   }else if(tree->denomi->type == TO_MN){
	     printf("<cn>%s</cn>\n",tree->denomi->figures);
	     fprintf(outfp,"<cn>%s</cn>\n",tree->denomi->figures);
	   }else{
	     //do nothing
	   }

      }else{
	//recursion syn_converter(tree->denomi)
      }

      for(int j=0; j<=depth; j++){
	     printf("\t");
	     fprintf(outfp,"\t");
      }
      printf("</apply>\n");
      fprintf(outfp,"</apply>\n");
      break;
       
    default:
      break;
    }
     
    syn_converter(tree->right);

     
    if(tree->right != (struct LIST *)0){
      for(int j=0; j<=depth; j++){
	printf("\t");
	fprintf(outfp,"\t");
      }
      printf("</apply>\n");
      fprintf(outfp,"</apply>\n");
    }
    --depth;   
  }
  
}


/*******************************************************
syn_option_print()

parameter : void
            
return : void
*******************************************************/

void syn_option_print(struct LIST *root){
  
  fprintf(outfp,"math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n");
  fprintf(outfp,"<semantics>\n");
  
  syn_converter(root);
 
  fprintf(outfp,"</semantics>\n");
  fprintf(outfp,"</math>");
  
}


/*******************************************************
syn_prefix_check()

parameter : (struct LIST *)root
            
return : (int)count_prefix
*******************************************************/

