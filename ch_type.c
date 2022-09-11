#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch_type.h"

/*******************************************************
********************************************************/

#define MAX_CH 256

enum CHAR_TYPE key_table[MAX_CH];

/*******************************************************
init_char_type()

parameter : void
return : void
 ******************************************************/

void init_char_type(void){
  int i;
  
  for(i=0; i<MAX_CH; i++)
    key_table[i] = CH_ETC;
  for(i='0'; i<='9';i++)
    key_table[i] = CH_INTNUM;
  for(i='a'; i<='z'; i++)
    key_table[i] = key_table[i-0x20] = CH_ALPHABET;
  
  key_table['_'] = CH_ALPHABET;
  key_table['/'] = CH_SLASH;
  key_table['<'] = CH_OPEN;
  key_table['>'] = CH_CLOSE;
  key_table['='] = CH_EQUAL;
  key_table['+'] = CH_PLUS;
  key_table['-'] = CH_MINUS;
  key_table['&'] = CH_AND;
  key_table['"'] = CH_QUOTATION;
  key_table[';'] = CH_SEMICOLON;
  key_table[':'] = CH_COLON;
  key_table['.'] = CH_DOT;
  key_table['\t'] = CH_SPACE;
  key_table['\n'] = CH_SPACE;
  key_table[' '] = CH_SPACE;
  key_table['\v'] = CH_SPACE;
  key_table['\f'] = CH_SPACE;
  key_table['\r'] = CH_SPACE;
 
  
}

/*******************************************************
enum get_char_type()

parameter : int c
return : char_type
 ******************************************************/

enum CHAR_TYPE get_char_type(int c){

  if(c == EOF) return(CH_EOF);

  return (key_table[c]);

}

/*******************************************************
is_char_type()

parameter : int c, ch_type
return : enum ch_type
         else CH_ETC
 ******************************************************/

int is_char_type(int c, enum CHAR_TYPE c_type){

  if(get_char_type(c) == c_type){
    return 1;
  }else{
    return 0;
  }
}
