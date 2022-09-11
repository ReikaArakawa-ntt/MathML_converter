#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "token.h"
#include "ch_type.h"
#include "in_file.h"


/******************************************************
*******************************************************
token.c module

function:
enum TOKEN_TYPE get_token_type();
enum TOKEN_TYPE get_operator();
eunm TOKEN_TYPE mathml();
*******************************************************
*******************************************************/

static int attribution_compare(char *a);
static int operator_compare(char *a);

/*******************************************************
get_token_type()

parameter : void

return : token_type
*******************************************************/

enum TOKEN_TYPE get_token_type(){
  
  if(in_cur_char() == EOF)//in_cur_char() return (in_file.cur_char)
    return (TO_EOF);
      
  switch(get_char_type(in_cur_char())){
    //get_char_type(in_cur_cahr()) return  key_table[ in_cur_char() ]
    //in_cur_char() return in_file.cur_char
    
  case CH_OPEN: //'<'
    return (TO_OPEN);
   
  case CH_CLOSE: //'>'
    return (TO_CLOSE);   

  case CH_SPACE:
    return (TO_SPACE);
    
  default:
    return (TO_ETC);

  }//switch  
}


/*******************************************************
mathml()

parameter : void

return : token type
         TO_ETC
*******************************************************/
extern FILE *outfp;


enum TOKEN_TYPE mathml(){
   char *str, copy[10] = {'\0'};
   int length = 0;
   
  if(in_cur_char() == EOF) return (TO_EOF);

  switch(get_char_type(in_cur_char())){
    
  case CH_SLASH:
       in_read_char();
       if(in_cur_char() == 'm'){
	 
	 do{
	   copy[length++] = in_cur_char();
	   in_read_char();
	   if(length == 9) break;
	   if(get_token_type() == TO_EOF)break;
	 }while(get_token_type() != TO_CLOSE);

	 copy[length++] = '\0';
	 str = (char *)malloc(length);
	 strcpy(str, copy);
	 length = 0;
	
	 switch(attribution_compare(str)){
	   free(str);
	 case 0:
	   return (TO_E_MATH);   // '/math'
	 case 1:
	   return (TO_E_MROW);   // '/mrow'
	 case 2:	      
	   return (TO_E_MN);     // '/mn'
	 case 3:	      
	   return (TO_E_MI);     // '/mi'
	 case 4:	     
	   return (TO_E_MO);     // '/mo'
	 case 5:	      
	   return (TO_E_MSUP);   // '/msup'
	 case 6:	      
	   return (TO_E_MSUB);   // '/msub'
	 case 7:
	   return (TO_E_MFRAC);  // '/mfrac'
	 case 8:	      
	   return (TO_E_MFENCED);// '/mfenced'
	 case 9:
	   return (TO_E_MSQRT);  // '/msqrt'
	 case 10:
	   return (TO_E_MROOT);  // '/mroot'
	 default:
	   return (TO_ETC_SLA);
	 }

       }else{
	 return (TO_ETC_SLA);
       }



    
  case CH_ALPHABET:
       if(in_cur_char() == 'm'){//first character matches with 'm'
	 
            do{
	      copy[length++] = in_cur_char();
	      in_read_char();
	      if(length == 9)break;
	      if(get_token_type() == TO_SPACE) break;//if standard white space
	    }while(get_token_type() != TO_CLOSE);

	    copy[length++] = '\0';
	    str = (char *)malloc(length);
	    strcpy(str, copy);
	    length = 0;
	    
	    switch(attribution_compare(str)){
	      free(str);
	    case 0:
	      return (TO_S_MATH);
	    case 1:
	      return (TO_MROW);
	    case 2:	      
	      return (TO_MN);
	    case 3:	      
	      return (TO_MI);
	    case 4:	     
	      return (TO_MO);
	    case 5:	      
	      return (TO_MSUP);
	    case 6:	      
	      return (TO_MSUB);
	    case 7:
	      return (TO_MFRAC);
	    case 8:	      
	      return (TO_MFENCED);
	    case 9:
	      return (TO_MSQRT);
	    case 10:
	      return (TO_MROOT);
	    default:
	      return (TO_ETC);
	    }

       }else{//first character doesn't match with 'm'
	 return (TO_ETC);
       }

    
  default:
    return (TO_ETC);
  }
  
}

/*******************************************************
compare()

parameter : array pointer

return : 1 matches
         0 not match
*******************************************************/

static int attribution_compare(char *a){
  //if static ,not match data error!
  const char *b[] = { "math",   //0
		      "mrow",   //1
		      "mn",     //2
		      "mi",     //3
		      "mo",     //4
		      "msup",   //5
		      "msub",   //6
		      "mfrac",  //7
		      "mfenced",//8
		      "msqrt",  //9
		      "mroot",  //10
		      "-->",    //11
		      NULL
  };
  //copy
  char *c;
  c = a;
  
  int i = 0;
  while(b[i]!=NULL){
    while(*a == *b[i]){
      if(*a == '\0'){
	return i;}
      a++;
      b[i]++;}
    i++;
    a = c;//go back to head
  }
    return -1;
}



/*******************************************************
get_operator()  get operator data ('+','-','=','&af;'...)

parameter : void

return : (char) operator
*******************************************************/

enum TOKEN_TYPE get_operator(void){
  char *str, operator[18] = {'\0'};
  int length = 0;

  if(in_cur_char()==EOF)return (TO_EOF);

  switch(get_char_type(in_cur_char())){
  case CH_PLUS:
    return (TO_PLUS);
  case CH_MINUS:
    return (TO_MINUS);
  case CH_EQUAL:
    return (TO_EQUAL);
  case CH_AND:
        do{
	  operator[length++] = in_cur_char();
	  in_read_char();
	  if(length == 16)break;
	}while(get_char_type(in_cur_char()) != CH_SEMICOLON);
    
	operator[length++] = '\0';
	str = (char *)malloc(length);
	strcpy(str,operator);
	length = 0;

	switch(operator_compare(str)){
	  free(str);
	case 0:
	case 1:
	case 12:
	  return (TO_AF);
	case 2:
	case 3:
	case 5:
	  return (TO_IT);
	case 4:
	case 15:
	  return (TO_TIMES);
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	  return (TO_MINUS);
	case 13:
	case 14:
	  return (TO_PLUSMINUS);
	case 16:
	  return (TO_DEVIDE);
	case 17:
	  return (TO_GT);
	case 18:
	case 23:
	  return (TO_GE);
	case 19:
	  return (TO_LT);
	case 20:
	case 24:
	  return (TO_LE);
	case 22:
	  return (TO_NEQ);
	case 21:
	  return (TO_INTEGRAL);
	
	default:
	  return (TO_ETC);
	}
    
  default:
    return (TO_ETC);
  }

}


/*******************************************************
operator_compare()

parameter : array pointer

return : j
*******************************************************/

static int operator_compare(char *a){
  
  const char *d[] = { "&ApplyFunction",  //0
		      "&af",             //1
		      "&InvisibleTimes", //2
		      "&it",             //3
		      "&times",          //4
		      "&#x2062",         //5  'invisible times'
		      "&minus",          //6
		      "&#x2212",         //7  'only minus'
		      "&#x02D7",         //8  'only minus'
		      "&#x002D",         //9  'minus & hyphen'
		      "&#xFF0D",         //10 'minus & hyphen'
		      "&#xFF63",         //11 'minus & hyphen'
		      "&#x2061",         //12 'function application'
		      "&#x00B1",         //13 'plusminus'
		      "&plusmn;",        //14 'plusminus'
		      "&#x00D7",         //15 'times'
		      "&devide;",        //16 'devide'
		      "&gt",             //17 'greater than'
		      "&ge",             //18 'greater or equal'
		      "&lt",             //19 'less than'
		      "&le",             //20 'less or equal'
		      "&int",            //21 'integral'
		      "&#x2260",         //22 'not equal'
		      "&#x2265",         //23 'greater or equal'
		      "&#x2264",         //24 'less or equal'
		      NULL
		      
  };
  
  //copy
  char *e;
  e = a;
  
  int j = 0;
  //printf("%s",a);
  
  while(d[j]!=NULL){
    while(*a == *d[j]){
      if(*a == '\0'){
	//printf("[j:%d]",j);
	return j;}
      a++;
      d[j]++;}
    j++;
    a = e;//go back to head
  }
    return -1;
}






