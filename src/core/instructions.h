#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <stdio.h>
#include "dt.h"

typedef struct{
  u1 mnemonic;
  u1 length;
  const char *name;
  
}instruction;

instruction *instruction_table_get();



int clsld_classfile_verbose_instruction(size_t indentation, u1 *cur_code, u4 *code_length_out);


#endif
