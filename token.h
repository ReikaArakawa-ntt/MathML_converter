#ifndef _TOKEN_H_
#define _TOKEN_H_

/*******************************************************
 *******************************************************
token module

function:
get_token_type()
token_digit()
*******************************************************
*******************************************************/


/*******************************************************
TOKEN_TYPE

enum TOKEN_TYPE
*******************************************************/

enum TOKEN_TYPE{
		TO_EOF,     // EOF
		TO_ETC,     // read through token
		TO_ETC_SLA, // </annotation>...
		TO_INTNUM,  //  (mn) int number
		TO_VARIABLE,//  (mi) variable
		TO_PLUS,    // '+'
		TO_MINUS,   // '-' or '&Minus;'
		TO_EQUAL,   // '='
		TO_PLUSMINUS,// 'plsuminus' or '&plusmn;'
		TO_TIMES,   // 'multiply'   or '&times;'
		TO_DEVIDE,  // 'devide'     or '&devide;'
		TO_S_MATH,  // 'math'
		TO_E_MATH,  // '/math'
		TO_MROW,    // 'mrow'
		TO_E_MROW,  // '/mrow'
		TO_MN,      // 'mn'
		TO_E_MN,    // '/mn'
		TO_MI,      // 'mi'
		TO_E_MI,    // '/mi'
		TO_MO,      // 'mo'
		TO_E_MO,    // '/mo'
		TO_MSUP,    // 'msup'
		TO_E_MSUP,  // '/msup'
		TO_MSUB,    // 'msub'
		TO_E_MSUB,  // '/msub'
		TO_MFRAC,   // 'mfrac'
		TO_E_MFRAC, // '/mfrac'
		TO_MFENCED, // 'mfenced'
		TO_E_MFENCED,// '/mfenced'
		TO_MSQRT,   // 'msqrt'
		TO_E_MSQRT, // '/msqrt'
		TO_AF,      // '&ApplyFunction;' or '&af;'
		TO_IT,      // '&InvisibleTimes;' or '&it;'
		TO_OPEN,    // '<'
		TO_CLOSE,   // '>'
		TO_SLASH,   // '/'
		TO_SPACE,   // standard white space characters
		TO_PI,      // '&pi;'
		TO_GT,      // '&gt;' '>'
		TO_GE,      // '&ge;' 'greater or equal'
		TO_LT,      // '&lt;' '<'
		TO_LE,      // '&le;' 'less or equal'
		TO_NEQ,     // 'not equal'
		TO_MOD,     // 'mod'
		TO_MROOT,   // 'mroot'
		TO_E_MROOT, // '/mroot'
		TO_EXPONENTIAL_E, // 'exponentialE;'
		TO_SUM,           // '&sum;'
		TO_INTEGRAL,      // '&int;'
		TO_DIFFERENTIAL_D,// '&DifferentialD;'
		TO_RIGHTARROW     // '&RightArrow;'
		
		
};


/*******************************************************
get_token_type()

parameter : void

return : (enum TOKEN_TYPE) token_type
 ******************************************************/
extern enum TOKEN_TYPE get_token_type(void);

/*******************************************************
get_token_type()

parameter : void

return : (enum TOKEN_TYPE) token_type
 ******************************************************/
extern enum TOKEN_TYPE mathml(void);


/*******************************************************
get_operator()  get operator data

parameter : void

return : (enum TOKEN_TYPE) operator
*******************************************************/

extern enum TOKEN_TYPE get_operator(void);




#endif
