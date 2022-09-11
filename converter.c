#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "in_file.h"
#include "ch_type.h"
#include "token.h"
#include "list.h"
#include "stack.h"
#include "syntax_tree.h"


/*******************************************************
 *******************************************************
multi_module:

Makefile 
converter.c //main()
in_file.h 
in_file.c
ch_type.h
ch_type.c
token.h
token.c
list.c
list.h



compile command:
make

execution command:
./Converter ( (file that you wanna convert).html )
or 
Converter (inputfile)

if remove some files command:
make clean

*******************************************************
*******************************************************/


/*******************************************************
init_level()

parameter : void

return : void
*******************************************************/
static int math_level = 0;//math contents start
//static int mrow_level = 0;//mrow start
//static int prefix_level = 0;

static void init_level(void){
  math_level = 0;
  //mrow_level = 0;
  //prefix_level=0;
}

/*******************************************************
math_up(), math_down()

parameter : void

return : void
*******************************************************/
static void math_up(void){
  ++math_level;
}

static void math_down(void){
  --math_level;
}

/*******************************************************
mrow_up(), mrow_down()

parameter : void

return : void
*******************************************************/
void mrow_up(void){
  //++mrow_level;
}

void mrow_down(void){
  //--mrow_level;
}

/*******************************************************
prefix_level_up()

parameter : void
            
return : void
*******************************************************/


/*******************************************************
output_file_open()

parameter : FILE *fp

return : void
*******************************************************/

//output stream contents markup
FILE *outfp;

void output_file_open(void){
  
  //If the file does not exist, create it, 
  //and if it exists, empty its contents first.
  outfp = fopen("output/out.html","w+");
  if(outfp == NULL){
    printf("cannot open outfp");
    exit(0);
  }
}


/*******************************************************
do_file()

parameter : const char name

return : void
*******************************************************/
#define ON  1
#define OFF 0
extern int nume_button;
extern int denomi_button;

static void do_file(const char name[]){
 enum TOKEN_TYPE current_token;
 enum TOKEN_TYPE token;
 struct LIST *list      = (struct LIST *)0;
 struct LIST *list_head = (struct LIST *)0;//copy head pointer of list
 int check  = 0;
 struct LIST **out_stack = (struct LIST **)0;//out stack pointer os pointer
 struct LIST *root       = (struct LIST *)0;
 struct LI_PREFIX_STACK *prest = (struct LI_PREFIX_STACK *)0;
 
 init_char_type();
 init_level();
 printf("\n");

 //open file pointer of output Contents Markup
 output_file_open();
 
  while(1){
    
    current_token = get_token_type();
         
    if(current_token == TO_OPEN){
      in_print();
      fprintf(outfp,"%c",in_cur_char());
      in_read_char();
      switch(mathml()){
	 case TO_S_MATH:             // 'math'
	   //printf("TO_S_MATH");
	   math_up();
	   break;
	 case TO_E_MATH:             // '/math'
	   math_down();
	   break;
         case TO_ETC_SLA:
	   printf("/");
	   printf("%c",in_cur_char());
	   fprintf(outfp,"/");
	   fprintf(outfp,"%c",in_cur_char());
	   break;
	 default:
	   in_print();
	   fprintf(outfp,"%c",in_cur_char());
	   break;
	 }
	 
    }else{
      in_print();
      fprintf(outfp,"%c",in_cur_char());
   }   
 
    //printf("%d ",math_level);
    
    if(math_level == 1){//start math contents

      //create prefix_stack and init
      prest = li_init_prefix_stack();
      
      do{	
	        current_token = get_token_type();

	          if(current_token == TO_OPEN){
                    in_read_char();
                  while(get_char_type(in_cur_char())==CH_SPACE){
                        in_read_char();
                  }
                  token = mathml();
                  switch(token){
                  case TO_E_MATH:
                      //printf("TO_E_MATH");
                      math_down();
                      break;
                  case TO_ETC:
                  break;	    
                  default:
                      //make list of  mathematical formula
                      list = (struct LIST *)li_add_list(list, token, prest);
                  break;
                      }
                  }
                    if(math_level == 0) break;
	          in_read_char();
         }while(in_cur_char() != EOF);
      }

    
    if(list != NULL && math_level == 0){//******************

      //free LI_PREFIX_STACK area
      prest = li_free_prefix_stack(prest);
      
      //copy head pointer 
      // if you use pointer of list, pointer does not indicate head.
      list_head = list;

      //li_print(list)******************************
      //li_print(list);  
      
      //list = list_head;

      //if variable consists of some alphabet*******
      list = li_var_check(list);
      list_head = list;
      //li_print(list);
      
      
      //check lack & insert lack data
      list = li_lack_check(list);
      li_print(list);
      list = list_head;
      

      /*************************10/23****/
      //printf("ここでセグってます！！！\n");
      //---postfix notation
      out_stack    = st_stack(list);
      //reset max_prefix_count == 0
      li_reset_max_prefix_count();
    
     
      //---make syntax_tree from out_stack
      root         = syn_make_tree(out_stack);
      
      //---output to Contents Markup
      printf("\n\n\n");
      syn_option_print(root);
      printf("\n\n");


      out_stack    = st_free_stack(out_stack);

      /********************10/23*********/



      root     = (struct LIST *)0;
      
      //delete list(list = NULL)
      list = list_head;
      list = li_delete_list(list);
      list_head = (struct LIST * )0;
    }//******************************************************

    
    in_read_char();
    
    if(in_cur_char() == EOF){
      printf("\n\n");
      return;
    }
  }
  fclose(outfp);
  return;
}



/*******************************************************
deliver_file()

parameter : const char name

return : (int) 1 success
               0 fail 
 ******************************************************/

static int deliver_file(const char *const input_file){

  if(in_open(input_file)){
    printf("Success : You can open input_file!\n\n");
    do_file(input_file);
    in_close();
    return 1;
    
  }else{//file cannot open
    in_close();
    printf("Error : Could not open file[ %s ]\n",input_file);
    return 0;
  }
    
}


/*******************************************************
main()

parameter : stdin int argc, char *argv[]

return : (int) 1 normal success
               0 fail 
*******************************************************/

int main(int argc, char *argv[]){
  
  if(argc<2 || argc >= 3){
    printf("argc is not less or more\n");
    exit(201);
  }
  

  if(argc == 2){
    if(deliver_file(argv[argc-1])){
      return (1);
    }else{
      return (0);
    }
  }

  
}

