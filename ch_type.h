#ifndef _CH_TYPE_H_
#define _CH_TYPE_H_

/*******************************************************
CHAR_TYPE module

declare character type: enuum CHAR_TYPE

function:




*******************************************************/

enum CHAR_TYPE{
	       CH_ALPHABET,   //'a'~'z','A'~'Z'
	       CH_INTNUM,     // '0'~'9' 
	       CH_SLASH,      // '/'
	       CH_OPEN,       // '<'
	       CH_CLOSE,      // '>'
	       CH_EQUAL,      // '='
	       CH_PLUS,       // '+'
	       CH_MINUS,      // '-'
	       CH_AND,        // '&'
	       CH_QUOTATION,  // '"'
	       CH_SEMICOLON,  // ';'
	       CH_COLON,      // ':'
	       CH_DOT,        // '.'
	       CH_S_PAREN,    // '('
	       CH_E_PAREN,    // ')'
	       CH_ETC,
	       CH_EXCLAMATION,// '!'
	       CH_SPACE,      //Standard whitespace characters
	       CH_EOF         // EOF
	       

};




/*******************************************************
init_char_type()

parameter: void

return: void
 ******************************************************/

extern void init_char_type(void);


/*******************************************************
get_char_type()

parameter: int character

return: (enum CHAR_TYPE) char_type
 ******************************************************/

extern enum CHAR_TYPE get_char_type(int c);

/*******************************************************
is_char_type()

parameter: int character, (enum CHAR_TYPE)c_type

return: 1 match
        0 not match
 ******************************************************/

extern int is_char_type(int c, enum CHAR_TYPE c_type);



#endif
