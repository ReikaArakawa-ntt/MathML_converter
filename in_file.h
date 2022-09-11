#ifndef _IN_FILE_H
#define _IN_FILE_H

/*******************************************************
in_file  module

function:

in_open()
in_close()
in_cur_file()
in_cur_char()
in_pre_char()
in_read_char()
in_print()
 ******************************************************/


/*******************************************************
in_open()

parameter : file name

return : 0 not open
         1 can open
 ******************************************************/

extern int in_open(const char name[]);

/*******************************************************
in_close

parameter : void

return : nothing
 ******************************************************/

extern void in_close(void);

/*******************************************************
in_cur_file()

parameter : void

return : (FILE *) in_file.fp
 ******************************************************/

//extern FILE *in_cur_file(void);


/*******************************************************
in_cur_char()

parameter : void

return : in_file.cur_char
 ******************************************************/

extern int in_cur_char(void);


/*******************************************************
in_pre_char()

parameter : void

return : in_file.pre_char
 ******************************************************/

extern int in_pre_char(void);


/*******************************************************
in_cur_ptr()

parameter : void
return : (int pointer) in_file.cur_ptr
 ******************************************************/

extern int *in_cur_ptr(void);


/*******************************************************
in_char_ptr()

parameter: void

return: (char pointer) in_file.char_ptr
 ******************************************************/

extern char *in_char_ptr(void);


/*******************************************************
in_print

parameter : void

return : in_file.cur_char
 ******************************************************/

extern void in_print(void);

/*******************************************************
in_read_char()

parameter : void

return : in_file.cur_char
 ******************************************************/

extern void in_read_char(void);



#endif
