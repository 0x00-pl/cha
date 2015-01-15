#include "instructions.h"

instruction *instruction_table_get()
{
  static instruction table[]={
    {0x00, 1, "nop"},
    {0x01, 1, "aconst_null"},
    {0x02, 1, "iconst_m1"},
    {0x03, 1, "iconst_0"},
    {0x04, 1, "iconst_1"},
    {0x05, 1, "iconst_2"},
    {0x06, 1, "iconst_3"},
    {0x07, 1, "iconst_4"},
    {0x08, 1, "iconst_5"},
    {0x09, 1, "lconst_0"},
    {0x0b, 1, "fconst_0"},
    {0x0c, 1, "fconst_1"},
    {0x0d, 1, "fconst_2"},
    {0x10, 1, "lconst_1"},
    {0x10, 2, "bipush"},
    {0x11, 3, "sipush"},
    {0x12, 2, "ldc"},
    {0x13, 3, "ldc_w"},
    {0x14, 1, "dconst_0"},
    {0x14, 3, "ldc2_w"},
    {0x15, 1, "dconst_1"},
    {0x15, 2, "iload"},
    {0x16, 2, "lload"},
    {0x17, 2, "fload"},
    {0x18, 2, "dload"},
    {0x19, 2, "aload"},
    {0x1a, 1, "iload_0"},
    {0x1b, 1, "iload_1"},
    {0x1c, 1, "iload_2"},
    {0x1d, 1, "iload_3"},
    {0x1e, 1, "lload_0"},
    {0x1f, 1, "lload_1"},
    {0x20, 1, "lload_2"},
    {0x21, 1, "lload_3"},
    {0x26, 1, "dload_0"},
    {0x27, 1, "dload_1"},
    {0x28, 1, "dload_2"},
    {0x29, 1, "dload_3"},
    {0x2a, 1, "aload_0"},
    {0x2b, 1, "aload_1"},
    {0x2c, 1, "aload_2"},
    {0x2d, 1, "aload_3"},
    {0x2e, 1, "iaload"},
    {0x2f, 1, "laload"},
    {0x30, 1, "faload"},
    {0x31, 1, "daload"},
    {0x32, 1, "aaload"},
    {0x33, 1, "baload"},
    {0x34, 1, "caload"},
    {0x34, 1, "fload_0"},
    {0x35, 1, "fload_1"},
    {0x35, 1, "saload"},
    {0x36, 1, "fload_2"},
    {0x36, 2, "istore"},
    {0x37, 1, "fload_3"},
    {0x37, 2, "lstore"},
    {0x38, 2, "fstore"},
    {0x39, 2, "dstore"},
    {0x3a, 2, "astore"},
    {0x3b, 1, "istore_0"},
    {0x3c, 1, "istore_1"},
    {0x3d, 1, "istore_2"},
    {0x3e, 1, "istore_3"},
    {0x3f, 1, "lstore_0"},
    {0x40, 1, "lstore_1"},
    {0x41, 1, "lstore_2"},
    {0x42, 1, "lstore_3"},
    {0x43, 1, "fstore_0"},
    {0x44, 1, "fstore_1"},
    {0x45, 1, "fstore_2"},
    {0x46, 1, "fstore_3"},
    {0x4b, 1, "astore_0"},
    {0x4c, 1, "astore_1"},
    {0x4d, 1, "astore_2"},
    {0x4e, 1, "astore_3"},
    {0x4f, 1, "iastore"},
    {0x50, 1, "lastore"},
    {0x51, 1, "fastore"},
    {0x52, 1, "dastore"},
    {0x53, 1, "aastore"},
    {0x54, 1, "bastore"},
    {0x55, 1, "castore"},
    {0x56, 1, "sastore"},
    {0x57, 1, "pop"},
    {0x58, 1, "pop2"},
    {0x59, 1, "dup"},
    {0x5a, 1, "dup_x1"},
    {0x5b, 1, "dup_x2"},
    {0x5c, 1, "dup2"},
    {0x5f, 1, "swap"},
    {0x60, 1, "iadd"},
    {0x61, 1, "ladd"},
    {0x62, 1, "fadd"},
    {0x63, 1, "dadd"},
    {0x64, 1, "isub"},
    {0x65, 1, "lsub"},
    {0x66, 1, "fsub"},
    {0x67, 1, "dsub"},
    {0x68, 1, "imul"},
    {0x69, 1, "lmul"},
    {0x6a, 1, "fmul"},
    {0x6b, 1, "dmul"},
    {0x6c, 1, "idiv"},
    {0x6d, 1, "ldiv"},
    {0x6e, 1, "fdiv"},
    {0x6f, 1, "ddiv"},
    {0x70, 1, "irem"},
    {0x71, 1, "dstore_0"},
    {0x71, 1, "lrem"},
    {0x72, 1, "dstore_1"},
    {0x72, 1, "frem"},
    {0x73, 1, "drem"},
    {0x73, 1, "dstore_2"},
    {0x74, 1, "dstore_3"},
    {0x74, 1, "ineg"},
    {0x75, 1, "lneg"},
    {0x76, 1, "fneg"},
    {0x77, 1, "dneg"},
    {0x78, 1, "ishl"},
    {0x79, 1, "lshl"},
    {0x7a, 1, "ishr"},
    {0x7b, 1, "lshr"},
    {0x7c, 1, "iushr"},
    {0x7d, 1, "lushr"},
    {0x7e, 1, "iand"},
    {0x7f, 1, "land"},
    {0x80, 1, "ior"},
    {0x81, 1, "lor"},
    {0x82, 1, "ixor"},
    {0x83, 1, "lxor"},
    {0x84, 3, "iinc"},
    {0x85, 1, "i2l"},
    {0x86, 1, "i2f"},
    {0x87, 1, "i2d"},
    {0x88, 1, "l2i"},
    {0x89, 1, "l2f"},
    {0x8a, 1, "l2d"},
    {0x8b, 1, "f2i"},
    {0x8c, 1, "f2l"},
    {0x8d, 1, "f2d"},
    {0x8e, 1, "d2i"},
    {0x8f, 1, "d2l"},
    {0x90, 1, "d2f"},
    {0x91, 1, "i2b"},
    {0x92, 1, "i2c"},
    {0x93, 1, "dup2_x1"},
    {0x93, 1, "i2s"},
    {0x94, 1, "dup2_x2"},
    {0x94, 1, "lcmp"},
    {0x95, 1, "fcmpl"},
    {0x96, 1, "fcmpg"},
    {0x97, 1, "dcmpl"},
    {0x98, 1, "dcmpg"},
    {0x99, 3, "ifeq"},
    {0x9a, 3, "ifne"},
    {0x9b, 3, "iflt"},
    {0x9c, 3, "ifge"},
    {0x9d, 3, "ifgt"},
    {0x9e, 3, "ifle"},
    {0x9f, 3, "if_icmpeq"},
    {0xa0, 3, "if_icmpne"},
    {0xa1, 3, "if_icmplt"},
    {0xa2, 3, "if_icmpge"},
    {0xa3, 3, "if_icmpgt"},
    {0xa4, 3, "if_icmple"},
    {0xa5, 3, "if_acmpeq"},
    {0xa6, 3, "if_acmpne"},
    {0xa7, 3, "goto"},
    {0xa8, 3, "jsr"},
    {0xa9, 2, "ret"},
    {0xaa, 255, "tableswitch"},
    {0xab, 255, "lookupswitch"},
    {0xac, 1, "ireturn"},
    {0xad, 1, "lreturn"},
    {0xae, 1, "freturn"},
    {0xaf, 1, "dreturn"},
    {0xb0, 1, "areturn"},
    {0xb1, 1, "return"},
    {0xb2, 3, "getstatic"},
    {0xb3, 3, "putstatic"},
    {0xb4, 3, "getfield"},
    {0xb5, 3, "putfield"},
    {0xb6, 3, "invokevirtual"},
    {0xb7, 3, "invokespecial"},
    {0xb8, 3, "invokestatic"},
    {0xb9, 5, "invokeinterface"},
    {0xba, 5, "invokedynamic"},
    {0xbb, 3, "new"},
    {0xbc, 2, "newarray"},
    {0xbd, 3, "anewarray"},
    {0xbe, 1, "arraylength"},
    {0xbf, 1, "athrow"},
    {0xc0, 3, "checkcast"},
    {0xc1, 3, "instanceof"},
    {0xc2, 1, "monitorenter"},
    {0xc3, 1, "monitorexit"},
    {0xc4, 255, "wide"},
    {0xc5, 4, "multianewarray"},
    {0xc6, 3, "ifnull"},
    {0xc7, 3, "ifnonnull"},
    {0xc8, 5, "goto_w"},
    {0xc9, 5, "jsr_w"},
  };
  return table;
}

int clsld_classfile_verbose_instruction(size_t indentation, u1 *cur_code, u4 *code_length_out)
{
  size_t i;
  u4 code_length;
  u4 switch_default, switch_low, switch_height, switch_size, jump_key, jump_offset;
  u1 *table_start;
  instruction *instruction_table;
  
  instruction_table = instruction_table_get();
  switch(*cur_code)
  {
    case 0xaa: //tableswitch
      table_start= (u1*)((((size_t)cur_code) + 3) & (size_t)(~3));
      switch_default = (u4)((table_start[0]  << 24) | (table_start[1]  << 16) | (table_start[2]  << 8) | table_start[3]);
      switch_low     = (u4)((table_start[4]  << 24) | (table_start[5]  << 16) | (table_start[6]  << 8) | table_start[7]);
      switch_height  = (u4)((table_start[8]  << 24) | (table_start[9]  << 16) | (table_start[10] << 8) | table_start[11]);
      switch_size    = (u4)(switch_height - switch_low + 1);
      for(i=0; i<indentation; i++) {printf(" ");}
      printf(
        "%s %u to %u:\n",
        instruction_table[*cur_code].name,
        switch_low, switch_height
      );
      
      for(i=0; i<switch_size; i++)
      {
        jump_offset = (u4)((table_start[i*4 + 12] << 24) | (table_start[i*4 + 13] << 16) | (table_start[i*4 + 14] << 8) | table_start[i*4 + 15]);
        for(i=0; i<indentation+2; i++) {printf(" ");}
        printf("%zu: %u", i, jump_offset);
      }
      
      for(i=0; i<indentation+2; i++) {printf(" ");}
      printf("default: %u", switch_default);
      
      *code_length_out = (u4)((table_start - cur_code) + 12 + (switch_size * 4));
      break;
      
    case 0xab: //lookupswitch
      table_start= (u1*)((((size_t)cur_code) + 3) & (size_t)(~3));
      switch_default = (u4)((table_start[0]  << 24) | (table_start[1]  << 16) | (table_start[2]  << 8) | table_start[3]);
      switch_size    = (u4)((table_start[4]  << 24) | (table_start[5]  << 16) | (table_start[6]  << 8) | table_start[7]);
      for(i=0; i<indentation; i++) {printf(" ");}
      printf(
        "%s %u:\n",
        instruction_table[*cur_code].name,
        switch_size
      );
      
      for(i=0; i<switch_size; i++)
      {
        jump_key    = (u4)((table_start[i*8 + 8]  << 24) | (table_start[i*8 + 9]  << 16) | (table_start[i*8 + 10] << 8) | table_start[i*8 + 11]);
        jump_offset = (u4)((table_start[i*8 + 12] << 24) | (table_start[i*8 + 13] << 16) | (table_start[i*8 + 14] << 8) | table_start[i*8 + 15]);
        for(i=0; i<indentation+2; i++) {printf(" ");}
        printf("%u: %u", jump_key, jump_offset);
      }
      
      for(i=0; i<indentation+2; i++) {printf(" ");}
      printf("default: %u", switch_default);
      
      *code_length_out = (u4)((table_start - cur_code) + 8 + (switch_size * 8));
      break;
      
    case 0xc4: //wide
      if(cur_code[1]==0x84) //iinc
      {
        for(i=0; i<indentation; i++) {printf(" ");}
        printf(
          "%s iinc #%u, #%u\n",
          instruction_table[*cur_code].name,
          ( (cur_code[2]<<8) | (cur_code[3]) ),
          ( (cur_code[4]<<8) | (cur_code[5]) )
        );
        *code_length_out = 6;
      }
      else
      {
        for(i=0; i<indentation; i++) {printf(" ");}
        printf(
          "%s %s #%u\n",
          instruction_table[cur_code[0]].name,
          instruction_table[cur_code[1]].name,
          ( (cur_code[2]<<8) | (cur_code[3]) )
        );
        *code_length_out = 4;
      }
      break;
      
    default:
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("%s ", instruction_table[*cur_code].name);
      code_length = instruction_table[*cur_code].length;
      
      for(i=1; i<code_length; i++)
      {
        printf("0x%x ", cur_code[i]);
      }
      printf("\n");
      
      *code_length_out = code_length;
      break;
  }
  return 0;
}

