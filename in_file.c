#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "in_file.h"

/*******************************************************
 *******************************************************
in_file  module

function:

in_open()
in_close()
in_file()
in_cur_char()
in_pre_char()
in_char_ptr()
in_read_char()
in_print()
*******************************************************
 ******************************************************/



/*******************************************************
typedef struct input_file

file pointer         : fp
temp array           : temp[128]
temp pointer         : *char_ptr
current char pointer : *cur_ptr
now character        : cur_char
next character       : next_char
 ******************************************************/

struct input_file{
  FILE *fp;
  char temp[128];
  char *char_ptr;// pointer of temp
  int *cur_ptr; // pointer of current char
  int cur_char;  // current char
  int pre_char;  // previous char 

};

static struct input_file in_file;

/*******************************************************
init struct input_file

file pointer         : NULL
temp array           : ""
temp pointer         : NULL
current char pointer : NULL
now character        : '\0'
next character       : '\0'
 ******************************************************/

static struct input_file in_file={
				  NULL,
				  "",
				  NULL,
				  NULL,
				  '\0',
				  '\0'
};

/*******************************************************
in_open()

parameter : file name

return : 0 not open
         1 can open
 ******************************************************/

int in_open(const char name[]){

  //cann't open
  if((in_file.fp = fopen(name,"r"))==NULL){
    printf("cann't file open");
    return 0;
  }

  //if can open
  in_file.cur_char = fgetc(in_file.fp);
  in_file.cur_ptr = &(in_file.cur_char);//current pointer of file
  in_file.char_ptr = in_file.temp;
  return 1;
}


/*******************************************************
in_close()

parameter : void

return : void
 ******************************************************/

void in_close(void){
  if(in_file.fp != NULL){
    fclose(in_file.fp);
    
  //init FILE *fp
  in_file.fp = NULL;
  }
}


/*******************************************************
in_read_char()

parameter : void

return : in_file.cur_char
 ******************************************************/

void in_read_char(void){

  //*in_file.char_ptr = in_file.cur_char;
  //++in_file.char_ptr;

  in_file.pre_char = in_file.cur_char;
  in_file.cur_char = fgetc(in_file.fp);
  
}


/*******************************************************
in_cur_file()

parameter : void

return : (FILE *) in_file.fp
 ******************************************************/

FILE *in_cur_file(void){
  return (in_file.fp);
}


/*******************************************************
in_cur_char()

parameter : void

return : in_file.cur_char
 ******************************************************/

int in_cur_char(void){
  return (in_file.cur_char);
}

/*******************************************************
in_pre_char()

parameter : void

return : in_file.pre_char
 ******************************************************/

int in_pre_char(void){
  return (in_file.pre_char);
}

/*******************************************************
in_cur_ptr()

parameter : void
return : (int pointer) in_file.cur_ptr
 ******************************************************/

int *in_cur_ptr(void){
  return (in_file.cur_ptr);
} 



/*******************************************************
in_char_ptr()  pointer of temp[128]

parameter : int c
return : (char pointer) in_file.char_ptr
 ******************************************************/

char *in_char_ptr(void){
  return (in_file.char_ptr);
} 



/*******************************************************
in_print

parameter : void

return : void
 ******************************************************/


void in_print(void){
  printf("%c",in_cur_char());
}

