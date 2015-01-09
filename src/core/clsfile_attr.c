#include <stdlib.h>
#include "clsfile.h"


/* Attribute : ConstantValue */

cha_attribute_info_constantvalue_t *cha_attribute_info_constantvalue_new(void)
{
  cha_attribute_info_constantvalue_t *new_attribute_info_constantvalue= NULL;
  
  new_attribute_info_constantvalue= (cha_attribute_info_constantvalue_t*)malloc(sizeof(cha_attribute_info_constantvalue_t));
  if(new_attribute_info_constantvalue == NULL) {goto fail;}
  new_attribute_info_constantvalue->constantvalue_index=0;
  
  goto done;
fail:
done:
  return new_attribute_info_constantvalue;
}

int cha_attribute_info_constantvalue_destroy(cha_attribute_info_constantvalue_t *attribute_info_constantvalue)
{
  free(attribute_info_constantvalue);
  return 0;
}

/* Attribute : Code.exception_table */

cha_attribute_info_code_exception_table_t *cha_attribute_info_code_exception_table_new(size_t size)
{
  cha_attribute_info_code_exception_table_t *new_attribute_info_code_exception_table= NULL;
  
  new_attribute_info_code_exception_table= (cha_attribute_info_code_exception_table_t*)malloc(size*sizeof(cha_attribute_info_code_exception_table_t));
  if(new_attribute_info_code_exception_table == NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_code_exception_table[size].start_pc= 0;
    new_attribute_info_code_exception_table[size].end_pc= 0;
    new_attribute_info_code_exception_table[size].handler_pc= 0;
    new_attribute_info_code_exception_table[size].catch_type= 0;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_code_exception_table;
}

int cha_attribute_info_code_exception_table_destroy(cha_attribute_info_code_exception_table_t *attribute_info_code_exception_table, size_t size)
{
  (void)size;
  free(attribute_info_code_exception_table);
  return 0;
}

/* Attribute : Code */

cha_attribute_info_code_t *cha_attribute_info_code_new(void){
  cha_attribute_info_code_t *new_attribute_info_code= NULL;
  
  new_attribute_info_code= (cha_attribute_info_code_t*)malloc(sizeof(cha_attribute_info_code_t));
  if(new_attribute_info_code == NULL) {goto fail;}
  new_attribute_info_code->max_stack= 0;
  new_attribute_info_code->max_locals= 0;
  new_attribute_info_code->code_length= 0;
  new_attribute_info_code->code= NULL;
  new_attribute_info_code->exception_table_length= 0;
  new_attribute_info_code->exception_table= NULL;
  new_attribute_info_code->attributes_count= 0;
  new_attribute_info_code->attributes= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_code;
}

int cha_attribute_info_code_destroy(cha_attribute_info_code_t *attribute_info_code){
  if(attribute_info_code==NULL) {return 0;}
  
  if(attribute_info_code->code!=NULL) {free(attribute_info_code->code);} //TODO @code
  if(attribute_info_code->exception_table!=NULL) 
  {
    cha_attribute_info_code_exception_table_destroy(
      attribute_info_code->exception_table, 
      attribute_info_code->exception_table_length);
  }
  if(attribute_info_code->attributes!=NULL) 
  {
    cha_attribute_info_destroy(
      attribute_info_code->attributes, 
      attribute_info_code->attributes_count);
  }
  free(attribute_info_code);
  
  return 0;
}

/* Attribute : StackMapTable.stack_map_frame.verification_type_info */

cha_attribute_info_stack_map_frame_verification_t *cha_attribute_info_stack_map_frame_verification_new(size_t size)
{
  cha_attribute_info_stack_map_frame_verification_t *new_attribute_info_stack_map_frame_verification= NULL;
  
  new_attribute_info_stack_map_frame_verification= (cha_attribute_info_stack_map_frame_verification_t*)malloc(size*sizeof(cha_attribute_info_stack_map_frame_verification_t));
  if(new_attribute_info_stack_map_frame_verification==NULL) {goto fail;}
  while(size-->0)
  {
    new_attribute_info_stack_map_frame_verification[size].tag= ITEM_Unknow;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_stack_map_frame_verification;
}

int cha_attribute_info_stack_map_frame_verification_destroy(cha_attribute_info_stack_map_frame_verification_t *attribute_info_stack_map_frame_verification, size_t size)
{
  (void)size;
  free(attribute_info_stack_map_frame_verification);
  return 0;
}

/* Attribute : StackMapTable.stack_map_frame */

cha_attribute_info_stack_map_frame_t *cha_attribute_info_stack_map_frame_new(size_t size)
{
  cha_attribute_info_stack_map_frame_t *new_attribute_info_stack_map_frame= NULL;
  
  new_attribute_info_stack_map_frame= (cha_attribute_info_stack_map_frame_t*)malloc(size*sizeof(cha_attribute_info_stack_map_frame_t));
  if(new_attribute_info_stack_map_frame==NULL) {goto fail;}
  while(size-->0)
  {
    new_attribute_info_stack_map_frame[size].frame_type= 0;
  }
  
  goto done;
fail:
done: 
  return new_attribute_info_stack_map_frame;
}

int cha_attribute_info_stack_map_frame_destroy(cha_attribute_info_stack_map_frame_t *attribute_info_stack_map_frame, size_t size)
{
  while(size-->0)
  {
    size_t frame_type= attribute_info_stack_map_frame[size].frame_type;
    if(/* FRAME_TYPE_SAME_Low<=frame_type ###always_true###  && */frame_type<=FRAME_TYPE_SAME_Height)
    {}
    else if(FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Low<=frame_type && 
      frame_type<=FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Height)
    {
      cha_attribute_info_stack_map_frame_verification_destroy(
	attribute_info_stack_map_frame[size].info.same_locals_1_stack_item_frame_part.stack_1, 1);
    }
    else if(frame_type==FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED)
    {
      cha_attribute_info_stack_map_frame_verification_destroy(
	attribute_info_stack_map_frame[size].info.same_locals_1_stack_item_frame_extended_part.stack_1, 1);
    }
    else if(FRAME_TYPE_CHOP_Low<=frame_type && 
      frame_type<=FRAME_TYPE_CHOP_Height)
    {}
    else if(frame_type==FRAME_TYPE_SAME_FRAME_EXTENDED)
    {}    
    else if(FRAME_TYPE_APPEND_Low<=frame_type && 
      frame_type<=FRAME_TYPE_APPEND_Height)
    {
      cha_attribute_info_stack_map_frame_verification_destroy(
	attribute_info_stack_map_frame[size].info.append_frame_part.locals,
	frame_type-251);
    }
    else if(frame_type==FRAME_TYPE_FULL_FRAME)
    {
      cha_attribute_info_stack_map_frame_verification_destroy(
	attribute_info_stack_map_frame[size].info.full_frame_part.locals,
	attribute_info_stack_map_frame[size].info.full_frame_part.number_of_locals);
      cha_attribute_info_stack_map_frame_verification_destroy(
	attribute_info_stack_map_frame[size].info.full_frame_part.stack,
	attribute_info_stack_map_frame[size].info.full_frame_part.number_of_stack_items);
    }
    else
    {}
  }
  free(attribute_info_stack_map_frame);
  return 0;
}

/* Attribute : StackMapTable */

cha_attribute_info_stack_map_table_t *cha_attribute_info_stack_map_table_new(void)
{
  cha_attribute_info_stack_map_table_t *new_attribute_info_stack_map_table= NULL;
  
  new_attribute_info_stack_map_table= (cha_attribute_info_stack_map_table_t*)malloc(sizeof(cha_attribute_info_stack_map_table_t));
  if(new_attribute_info_stack_map_table==NULL) {goto fail;}
  new_attribute_info_stack_map_table->number_of_entries= 0;
  new_attribute_info_stack_map_table->exception_table= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_stack_map_table;
}

int cha_attribute_info_stack_map_table_destroy(cha_attribute_info_stack_map_table_t *attribute_info_stack_map_table)
{
  if(attribute_info_stack_map_table==NULL) {return 0;}
  cha_attribute_info_stack_map_frame_destroy(
    attribute_info_stack_map_table->exception_table,
    attribute_info_stack_map_table->number_of_entries
  );
  return 0;
}

/* Attribute : Exceptions */

cha_attribute_info_exceptions_t *cha_attribute_info_exceptions_new(void)
{
  cha_attribute_info_exceptions_t *new_attribute_info_exceptions= NULL;
  
  new_attribute_info_exceptions= (cha_attribute_info_exceptions_t*)malloc(sizeof(cha_attribute_info_exceptions_t));
  if(new_attribute_info_exceptions==NULL) {goto fail;}
  new_attribute_info_exceptions->number_of_exceptions= 0;
  new_attribute_info_exceptions->exception_index_table= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_exceptions;
}

int cha_attribute_info_exceptions_destroy(cha_attribute_info_exceptions_t *attribute_info_exceptions)
{
  if(attribute_info_exceptions==NULL) {return 0;}
  free(attribute_info_exceptions->exception_index_table);
  return 0;
}

/* Attribute : InnerClasses.classes */

cha_attribute_info_inner_class_t *cha_attribute_info_inner_class_new(size_t size)
{
  cha_attribute_info_inner_class_t *new_attribute_info_inner_class= NULL;
  
  new_attribute_info_inner_class= (cha_attribute_info_inner_class_t*)malloc(size*sizeof(cha_attribute_info_inner_class_t));
  if(new_attribute_info_inner_class==NULL) {goto fail;}
  while(size-->0)
  {
    new_attribute_info_inner_class[size].inner_class_info_index= 0;
    new_attribute_info_inner_class[size].outer_class_info_index= 0;
    new_attribute_info_inner_class[size].inner_name_index= 0;
    new_attribute_info_inner_class[size].inner_class_access_flags= 0;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_inner_class;
}

int cha_attribute_info_inner_class_destroy(cha_attribute_info_inner_class_t *attribute_info_inner_class, size_t size)
{
  (void)size;
  if(attribute_info_inner_class==NULL) {return 0;}
  free(attribute_info_inner_class);
  return 0;
}

/* Attribute : InnerClasses */

cha_attribute_info_inner_classes_t *cha_attribute_info_inner_classes_new(void)
{
  cha_attribute_info_inner_classes_t *new_attribute_info_inner_classes= NULL;
  
  new_attribute_info_inner_classes= (cha_attribute_info_inner_classes_t*)malloc(sizeof(cha_attribute_info_inner_classes_t));
  if(new_attribute_info_inner_classes==NULL) {goto fail;}
  new_attribute_info_inner_classes->number_of_classes= 0;
  new_attribute_info_inner_classes->classes= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_inner_classes;
}

int cha_attribute_info_inner_classes_destroy(cha_attribute_info_inner_classes_t *attribute_info_inner_classes)
{
  if(attribute_info_inner_classes==NULL) {return 0;}
  cha_attribute_info_inner_class_destroy(
    attribute_info_inner_classes->classes,
    attribute_info_inner_classes->number_of_classes
  );
  free(attribute_info_inner_classes);
  return 0;
}

/* Attribute : EnclosingMethod */

cha_attribute_info_enclosing_method_t *cha_attribute_info_enclosing_method_new(void)
{
  cha_attribute_info_enclosing_method_t *new_attribute_info_enclosing_method= NULL;
  
  new_attribute_info_enclosing_method= (cha_attribute_info_enclosing_method_t*)malloc(sizeof(cha_attribute_info_enclosing_method_t));
  if(new_attribute_info_enclosing_method==NULL) {goto fail;}
  new_attribute_info_enclosing_method->class_index= 0;
  new_attribute_info_enclosing_method->method_index= 0;
  
  goto done;
fail:
done:
  return new_attribute_info_enclosing_method;  
}

int cha_attribute_info_enclosing_method_destroy(cha_attribute_info_enclosing_method_t *attribute_info_enclosing_method)
{
  if(attribute_info_enclosing_method==NULL) {return 0;}
  free(attribute_info_enclosing_method);
  return 0;
}

/* Attribute : Synthetic */
/* empty struct */
// cha_attribute_info_synthetic_t *cha_attribute_info_synthetic_new(void) {return NULL;}
// int cha_attribute_info_synthetic_destroy(cha_attribute_info_synthetic_t *attribute_info_synthetic) {}

/* Attribute : Signature */

cha_attribute_info_signature_t *cha_attribute_info_signature_new(void)
{ 
  cha_attribute_info_signature_t *new_attribute_info_signature= NULL;
  
  new_attribute_info_signature= (cha_attribute_info_signature_t*)malloc(sizeof(cha_attribute_info_signature_t));
  if(new_attribute_info_signature==NULL) {goto fail;}
  new_attribute_info_signature->signature_index= 0;
  
  goto done;
fail:
done:
  return new_attribute_info_signature; 
}

int cha_attribute_info_signature_destroy(cha_attribute_info_signature_t *attribute_info_signature)
{
  if(attribute_info_signature==NULL) {return 0;}
  free(attribute_info_signature);
  return 0;
}


/* Attribute : SourceFile */

cha_attribute_info_source_file_t *cha_attribute_info_source_file_new(void)
{
  cha_attribute_info_source_file_t *new_attribute_info_source_file= NULL;
  
  new_attribute_info_source_file= (cha_attribute_info_source_file_t*)malloc(sizeof(cha_attribute_info_source_file_t));
  if(new_attribute_info_source_file==NULL) {goto fail;}
  new_attribute_info_source_file->sourcefile_index= 0;
  
  goto done;
fail:
done:
  return new_attribute_info_source_file;
}

int cha_attribute_info_source_file_destroy(cha_attribute_info_source_file_t *attribute_info_source_file)
{
  if(attribute_info_source_file==NULL) {return 0;}
  free(attribute_info_source_file);
  return 0;
}


/* Attribute : SourceDebugExtension */

cha_attribute_info_source_debug_extension_t *cha_attribute_info_source_debug_extension_new(void)
{
  cha_attribute_info_source_debug_extension_t *nwe_attribute_info_source_debug_extension= NULL;
  
  nwe_attribute_info_source_debug_extension= (cha_attribute_info_source_debug_extension_t*)malloc(sizeof(cha_attribute_info_source_debug_extension_t));
  if(nwe_attribute_info_source_debug_extension==NULL) {goto fail;}
  nwe_attribute_info_source_debug_extension->debug_extension= NULL;
  
  goto done;
fail:
done:
  return nwe_attribute_info_source_debug_extension;
}

int cha_attribute_info_source_debug_extension_destroy(cha_attribute_info_source_debug_extension_t *attribute_info_source_debug_extension)
{
  if(attribute_info_source_debug_extension==NULL) {return 0;}
  free(attribute_info_source_debug_extension->debug_extension);
  free(attribute_info_source_debug_extension);
  return 0;
}


/* Attribute : LineNumberTable.line_number */

cha_attribute_info_line_number_t *cha_attribute_info_line_number_new(size_t size)
{
  cha_attribute_info_line_number_t *new_attribute_info_line_number= NULL;
  
  new_attribute_info_line_number= (cha_attribute_info_line_number_t*)malloc(size*sizeof(cha_attribute_info_line_number_t));
  if(new_attribute_info_line_number==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_line_number[size].start_pc= 0;
    new_attribute_info_line_number[size].line_number= 0;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_line_number;
}
int cha_attribute_info_line_number_destroy(cha_attribute_info_line_number_t *attribute_info_line_number, size_t size)
{
  (void)size;
  if(attribute_info_line_number==NULL) {return 0;}
  free(attribute_info_line_number);
  return 0;
}

/* Attribute : LineNumberTable */

cha_attribute_info_line_number_table_t *cha_attribute_info_line_number_table_new(void)
{
  cha_attribute_info_line_number_table_t *new_attribute_info_line_number_table= NULL;
  
  new_attribute_info_line_number_table= (cha_attribute_info_line_number_table_t*)malloc(sizeof(cha_attribute_info_line_number_table_t));
  if(new_attribute_info_line_number_table==NULL) {goto fail;}
  new_attribute_info_line_number_table->line_number_table_length= 0;
  new_attribute_info_line_number_table->line_number_table= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_line_number_table;
}

int cha_attribute_info_line_number_table_destroy(cha_attribute_info_line_number_table_t *attribute_info_line_number_table)
{
  if(attribute_info_line_number_table==NULL) {return 0;}
  cha_attribute_info_line_number_destroy(
    attribute_info_line_number_table->line_number_table,
    attribute_info_line_number_table->line_number_table_length
  );
  free(attribute_info_line_number_table);
  return 0;
}


/* Attribute : LocalVariableTable.local_variable */

cha_attribute_info_local_variable_t *cha_attribute_info_local_variable_new(size_t size)
{
  cha_attribute_info_local_variable_t *new_attribute_info_local_variable= NULL;
  
  new_attribute_info_local_variable= (cha_attribute_info_local_variable_t*)malloc(size*sizeof(cha_attribute_info_local_variable_t));
  if(new_attribute_info_local_variable==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_local_variable[size].start_pc= 0;
    new_attribute_info_local_variable[size].length= 0;
    new_attribute_info_local_variable[size].name_index= 0;
    new_attribute_info_local_variable[size].descriptor_index= 0;
    new_attribute_info_local_variable[size].index= 0;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_local_variable;
}

int cha_attribute_info_local_variable_destroy(cha_attribute_info_local_variable_t *attribute_info_local_variable, size_t size)
{
  (void)size;
  if(attribute_info_local_variable==NULL) {return 0;}
  free(attribute_info_local_variable);
  return 0;
}

/* Attribute : LocalVariableTable */

cha_attribute_info_local_variable_table_t *cha_attribute_info_local_variable_table_new(void)
{
  cha_attribute_info_local_variable_table_t *new_attribute_info_local_variable_table= NULL;
  
  new_attribute_info_local_variable_table= (cha_attribute_info_local_variable_table_t*)malloc(sizeof(cha_attribute_info_local_variable_table_t));
  if(new_attribute_info_local_variable_table==NULL) {goto fail;}
  new_attribute_info_local_variable_table->local_variable_table_length= 0;
  new_attribute_info_local_variable_table->local_variable_table= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_local_variable_table;
}
int cha_attribute_info_local_variable_table_destroy(cha_attribute_info_local_variable_table_t *attribute_info_local_variable_table)
{
  if(attribute_info_local_variable_table==NULL) {return 0;}
  cha_attribute_info_local_variable_destroy(
    attribute_info_local_variable_table->local_variable_table,
    attribute_info_local_variable_table->local_variable_table_length
  );
  free(attribute_info_local_variable_table);
  return 0;
}

/* Attribute : LocalVariableTypeTable.local_variable_type */

cha_attribute_info_local_variable_type_t *cha_attribute_info_local_variable_type_new(size_t size)
{
  cha_attribute_info_local_variable_type_t *new_attribute_info_local_variable_type= NULL;
  
  new_attribute_info_local_variable_type= (cha_attribute_info_local_variable_type_t*)malloc(size*sizeof(cha_attribute_info_local_variable_type_t));
  if(new_attribute_info_local_variable_type==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_local_variable_type[size].start_pc= 0;
    new_attribute_info_local_variable_type[size].length= 0;
    new_attribute_info_local_variable_type[size].name_index= 0;
    new_attribute_info_local_variable_type[size].signature_index= 0;
    new_attribute_info_local_variable_type[size].index= 0;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_local_variable_type; 
}
int cha_attribute_info_local_variable_type_destroy(cha_attribute_info_local_variable_type_t *attribute_info_local_variable_type, size_t size)
{
  (void)size;
  if(attribute_info_local_variable_type==NULL) {return 0;}
  free(attribute_info_local_variable_type);
  return 0;
}

/* Attribute : LocalVariableTypeTable */

cha_attribute_info_local_variable_type_table_t *cha_attribute_info_local_variable_type_table_new(void)
{  
  cha_attribute_info_local_variable_type_table_t *new_attribute_info_local_variable_type_table= NULL;
  
  new_attribute_info_local_variable_type_table= (cha_attribute_info_local_variable_type_table_t*)malloc(sizeof(cha_attribute_info_local_variable_type_table_t));
  if(new_attribute_info_local_variable_type_table==NULL) {goto fail;}
  new_attribute_info_local_variable_type_table->local_variable_type_table_length= 0;
  new_attribute_info_local_variable_type_table->local_variable_type_table= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_local_variable_type_table;
}
int cha_attribute_info_local_variable_type_table_destroy(cha_attribute_info_local_variable_type_table_t *attribute_info_local_variable_type_table)
{
  if(attribute_info_local_variable_type_table==NULL) {return 0;}
  cha_attribute_info_local_variable_type_destroy(
    attribute_info_local_variable_type_table->local_variable_type_table,
    attribute_info_local_variable_type_table->local_variable_type_table_length
  );
  free(attribute_info_local_variable_type_table);
  return 0;
}


/* Attribute : Deprecated */
// cha_attribute_info_deprecated_t *cha_attribute_info_deprecated_new(void){return NULL;}
// int cha_attribute_info_deprecated_destroy(cha_attribute_info_deprecated_t *attribute_info_deprecated){return 0;}

/* Attribute : Annotation.element_value_pair */

struct cha_attribute_info_element_value_t_decl;

cha_attribute_info_element_value_pair_t *cha_attribute_info_element_value_pair_new(size_t size)
{
  cha_attribute_info_element_value_pair_t *new_attribute_info_element_value_pair= NULL;
  
  new_attribute_info_element_value_pair= (cha_attribute_info_element_value_pair_t*)malloc(size*sizeof(cha_attribute_info_element_value_pair_t));
  if(new_attribute_info_element_value_pair==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_element_value_pair[size].element_name_index= 0;
    new_attribute_info_element_value_pair[size].value= NULL;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_element_value_pair;
}
int cha_attribute_info_element_value_pair_destroy(cha_attribute_info_element_value_pair_t* attribute_info_element_value_pair, size_t size)
{
  if(attribute_info_element_value_pair==NULL) {return 0;}
  while(size-->0)
  {
    cha_attribute_info_element_value_destroy(
      attribute_info_element_value_pair[size].value, 1);
  }
  free(attribute_info_element_value_pair);
  return 0;
}

/* Attribute : Annotation */

cha_attribute_info_annotation_t *cha_attribute_info_annotation_new(size_t size)
{
  cha_attribute_info_annotation_t *new_attribute_info_annotation= NULL;
  
  new_attribute_info_annotation= (cha_attribute_info_annotation_t*)malloc(size*sizeof(cha_attribute_info_annotation_t));
  if(new_attribute_info_annotation==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_annotation[size].type_index= 0;
    new_attribute_info_annotation[size].num_element_value_pairs= 0;
    new_attribute_info_annotation[size].element_value_pairs= NULL;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_annotation;
}
int cha_attribute_info_annotation_destroy(cha_attribute_info_annotation_t *attribute_info_annotation, size_t size)
{
  if(attribute_info_annotation==NULL) {return 0;}
  while(size-->0)
  {
    cha_attribute_info_element_value_pair_destroy(
      attribute_info_annotation[size].element_value_pairs,
      attribute_info_annotation[size].num_element_value_pairs
    );
  }
  free(attribute_info_annotation);
  return 0;
}

/* Attribute : Annotation.element_value */

cha_attribute_info_element_value_t *cha_attribute_info_element_value_new(size_t size)
{
  cha_attribute_info_element_value_t *new_attribute_info_element_value= NULL;
  
  new_attribute_info_element_value= (cha_attribute_info_element_value_t*)malloc(size*sizeof(cha_attribute_info_element_value_t));
  if(new_attribute_info_element_value==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_element_value[size].tag= '\0';
  }
  
  goto done;
fail:
done:
  return new_attribute_info_element_value;
}
int cha_attribute_info_element_value_destroy(cha_attribute_info_element_value_t *attribute_info_element_value, size_t size)
{
  if(attribute_info_element_value==NULL) {return 0;}
  while(size-->0)
  {
    switch(attribute_info_element_value[size].tag)
    {
      case 'B':
      case 'C':
      case 'D':
      case 'F':
      case 'I':
      case 'J':
      case 'S':
      case 'Z':
      case 's':
	/* const_value_index_part */
	break;
      case 'e':
	/* enum_const_value_part */
	break;
      case 'c':
	/* class_info_index_part */
	break;
      case '@':
	/* annotation_value_part */
	cha_attribute_info_annotation_destroy(
	  attribute_info_element_value[size].value.annotation_value_part, 1);
	break;
      case '[':
	/* array_value_part */
	cha_attribute_info_element_value_destroy(
	  attribute_info_element_value[size].value.array_value_part.values,
	  attribute_info_element_value[size].value.array_value_part.num_values
	);
	break;
      default:
	break;
    }
  }
  free(attribute_info_element_value);
  return 0;
}


/* Attribute : RuntimeVisibleAnnotations */

cha_attribute_info_runtime_visible_annotations_t *cha_attribute_info_runtime_visible_annotations_new(void)
{
  cha_attribute_info_runtime_visible_annotations_t *new_attribute_info_runtime_visible_annotations= NULL;
  
  new_attribute_info_runtime_visible_annotations= (cha_attribute_info_runtime_visible_annotations_t*)malloc(sizeof(cha_attribute_info_runtime_visible_annotations_t));
  if(new_attribute_info_runtime_visible_annotations==NULL) {goto fail;}
  new_attribute_info_runtime_visible_annotations->num_annotations= 0;
  new_attribute_info_runtime_visible_annotations->annotations= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_runtime_visible_annotations;
}
int cha_attribute_info_runtime_visible_annotations_destroy(cha_attribute_info_runtime_visible_annotations_t *attribute_info_runtime_visible_annotations)
{
  if(attribute_info_runtime_visible_annotations==NULL) {return 0;}
  cha_attribute_info_annotation_destroy(
    attribute_info_runtime_visible_annotations->annotations,
    attribute_info_runtime_visible_annotations->num_annotations
  );
  free(attribute_info_runtime_visible_annotations);
  return 0;
}

/* Attribute : RuntimeInvisibleAnnotations */

cha_attribute_info_runtime_invisible_annotations_t *cha_attribute_info_runtime_invisible_annotations_new(void)
{
  cha_attribute_info_runtime_invisible_annotations_t *new_attribute_info_runtime_invisible_annotations= NULL;
  
  new_attribute_info_runtime_invisible_annotations= (cha_attribute_info_runtime_invisible_annotations_t*)malloc(sizeof(cha_attribute_info_runtime_invisible_annotations_t));
  if(new_attribute_info_runtime_invisible_annotations==NULL) {goto fail;}
  new_attribute_info_runtime_invisible_annotations->num_annotations= 0;
  new_attribute_info_runtime_invisible_annotations->annotations= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_runtime_invisible_annotations;
}
int cha_attribute_info_runtime_invisible_annotations_destroy(cha_attribute_info_runtime_invisible_annotations_t *attribute_info_runtime_invisible_annotations)
{
  if(attribute_info_runtime_invisible_annotations==NULL) {return 0;}
  cha_attribute_info_annotation_destroy(
    attribute_info_runtime_invisible_annotations->annotations,
    attribute_info_runtime_invisible_annotations->num_annotations
  );
  free(attribute_info_runtime_invisible_annotations);
  return 0;
}

/* Attribute : ParameterAnnotations */

cha_attribute_info_parameter_annotation_t *cha_attribute_info_attribute_info_parameter_annotation_new(size_t size)
{
  cha_attribute_info_parameter_annotation_t *new_attribute_info_parameter_annotation= NULL;
  
  new_attribute_info_parameter_annotation= (cha_attribute_info_parameter_annotation_t*)malloc(size*sizeof(cha_attribute_info_parameter_annotation_t));
  if(new_attribute_info_parameter_annotation==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info_parameter_annotation[size].num_annotations= 0;
    new_attribute_info_parameter_annotation[size].annotations= NULL;
  }
  goto done;
fail:
done:
  return new_attribute_info_parameter_annotation;
}
int cha_attribute_info_parameter_annotation_destroy(cha_attribute_info_parameter_annotation_t *attribute_info_parameter_annotation, size_t size)
{
  if(attribute_info_parameter_annotation==NULL) {return 0;}
  while(size-->0){
    cha_attribute_info_annotation_destroy(
      attribute_info_parameter_annotation[size].annotations,
      attribute_info_parameter_annotation[size].num_annotations
    );
  }
  free(attribute_info_parameter_annotation);
  return 0;
}

/* Attribute : RuntimeVisibleParameterAnnotations */

cha_attribute_info_runtime_visible_parameter_annotations_t *cha_attribute_info_runtime_visible_parameter_annotations_new(void)
{
  cha_attribute_info_runtime_visible_parameter_annotations_t *new_attribute_info_runtime_visible_parameter_annotations= NULL;
  
  new_attribute_info_runtime_visible_parameter_annotations= (cha_attribute_info_runtime_visible_parameter_annotations_t*)malloc(sizeof(cha_attribute_info_runtime_visible_parameter_annotations_t));
  if(new_attribute_info_runtime_visible_parameter_annotations==NULL) {goto fail;}
  new_attribute_info_runtime_visible_parameter_annotations->num_parameters= 0;
  new_attribute_info_runtime_visible_parameter_annotations->parameter_annotations= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_runtime_visible_parameter_annotations;
}
int cha_attribute_info_runtime_visible_parameter_annotations_destroy(cha_attribute_info_runtime_visible_parameter_annotations_t *attribute_info_runtime_visible_parameter_annotations)
{
  if(attribute_info_runtime_visible_parameter_annotations==NULL) {return 0;}
  cha_attribute_info_parameter_annotation_destroy(
    attribute_info_runtime_visible_parameter_annotations->parameter_annotations,
    attribute_info_runtime_visible_parameter_annotations->num_parameters
  );
  free(attribute_info_runtime_visible_parameter_annotations);
  return 0;
}

/* Attribute : RuntimeInisibleParameterAnnotations */

cha_attribute_info_runtime_invisible_parameter_annotations_t *cha_attribute_info_runtime_invisible_parameter_annotations_new(void)
{
  cha_attribute_info_runtime_invisible_parameter_annotations_t *new_attribute_info_runtime_invisible_parameter_annotations= NULL;
  
  new_attribute_info_runtime_invisible_parameter_annotations= (cha_attribute_info_runtime_invisible_parameter_annotations_t*)malloc(sizeof(cha_attribute_info_runtime_invisible_parameter_annotations_t));
  if(new_attribute_info_runtime_invisible_parameter_annotations==NULL) {goto fail;}
  new_attribute_info_runtime_invisible_parameter_annotations->num_parameters= 0;
  new_attribute_info_runtime_invisible_parameter_annotations->parameter_annotations= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_runtime_invisible_parameter_annotations;
}
int cha_attribute_info_runtime_invisible_parameter_annotations_destroy(cha_attribute_info_runtime_invisible_parameter_annotations_t *attribute_info_runtime_invisible_parameter_annotations)
{
  if(attribute_info_runtime_invisible_parameter_annotations==NULL) {return 0;}
  cha_attribute_info_parameter_annotation_destroy(
    attribute_info_runtime_invisible_parameter_annotations->parameter_annotations,
    attribute_info_runtime_invisible_parameter_annotations->num_parameters
  );
  free(attribute_info_runtime_invisible_parameter_annotations);
  return 0;
}


/* Attribute : AnnotationDefault */

cha_attribute_info_annotation_default_t *cha_attribute_info_annotation_default_new(void)
{
  cha_attribute_info_annotation_default_t *new_attribute_info_annotation_default= NULL;
  
  new_attribute_info_annotation_default= (cha_attribute_info_annotation_default_t*)malloc(sizeof(cha_attribute_info_annotation_default_t));
  if(new_attribute_info_annotation_default==NULL) {goto fail;}
  new_attribute_info_annotation_default->default_value= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_annotation_default;
}
int cha_attribute_info_annotation_default_destroy(cha_attribute_info_annotation_default_t *attribute_info_annotation_default)
{
  if(attribute_info_annotation_default==NULL) {return 0;}
  cha_attribute_info_element_value_destroy(
    attribute_info_annotation_default->default_value, 1);
  free(attribute_info_annotation_default);
  return 0;
}

/* Attribute : BootstrapMethods.bootstrap_method */

cha_attribute_info_bootstrap_method_t *cha_attribute_info_attribute_info_bootstrap_method_new(size_t size)
{
  cha_attribute_info_bootstrap_method_t *new_attribute_info_bootstrap_method= NULL;
  
  new_attribute_info_bootstrap_method= (cha_attribute_info_bootstrap_method_t*)malloc(size*sizeof(cha_attribute_info_bootstrap_method_t));
  if(new_attribute_info_bootstrap_method==NULL) {goto fail;}
  while(size-->0)
  {
    new_attribute_info_bootstrap_method[size].bootstrap_method_ref= 0;
    new_attribute_info_bootstrap_method[size].bootstrap_arguments= 0;
    new_attribute_info_bootstrap_method[size].bootstrap_arguments= NULL;
  }
  
  goto done;
fail:
done:
  return new_attribute_info_bootstrap_method;
}
int cha_attribute_info_bootstrap_method_destroy(cha_attribute_info_bootstrap_method_t *attribute_info_bootstrap_method, size_t size)
{
  if(attribute_info_bootstrap_method==NULL) {return 0;}
  while(size-->0)
  {
    free(attribute_info_bootstrap_method->bootstrap_arguments);
  }
  free(attribute_info_bootstrap_method);
  return 0;
}

/* Attribute : BootstrapMethods */

cha_attribute_info_bootstrap_methods_t *cha_attribute_info_bootstrap_methods_new(void)
{
  cha_attribute_info_bootstrap_methods_t *new_attribute_info_bootstrap_methods= NULL;
  
  new_attribute_info_bootstrap_methods= (cha_attribute_info_bootstrap_methods_t*)malloc(sizeof(cha_attribute_info_bootstrap_methods_t));
  if(new_attribute_info_bootstrap_methods==NULL) {goto fail;}
  new_attribute_info_bootstrap_methods->num_bootstrap_methods= 0;
  new_attribute_info_bootstrap_methods->bootstrap_methods= NULL;
  
  goto done;
fail:
done:
  return new_attribute_info_bootstrap_methods;
}
int cha_attribute_info_bootstrap_methods_destroy(cha_attribute_info_bootstrap_methods_t *attribute_info_bootstrap_methods)
{
  if(attribute_info_bootstrap_methods==NULL) {return 0;}
  cha_attribute_info_bootstrap_method_destroy(
    attribute_info_bootstrap_methods->bootstrap_methods,
    attribute_info_bootstrap_methods->num_bootstrap_methods
  );
  free(attribute_info_bootstrap_methods);
  return 0;
}


cha_attribute_info_t *cha_attribute_info_new(size_t size)
{
  cha_attribute_info_t *new_attribute_info= NULL;
  
  new_attribute_info= (cha_attribute_info_t*)malloc(size*sizeof(cha_attribute_info_t));
  if(new_attribute_info==NULL) {goto fail;}
  while(size-->0){
    new_attribute_info[size].attributes_name_index= 0;
    new_attribute_info[size].attributes_length= 0;
    new_attribute_info[size].info.unknow= NULL;
    new_attribute_info[size].attributes_type_= ATTRIBUTE_TYPE_Unknow;
  }
  
  goto done;
fail:
done:
  return new_attribute_info;
}
int cha_attribute_info_destroy(cha_attribute_info_t *attribute_info, size_t size)
{
  if(attribute_info==NULL) {return 0;}
  while(size-->0){
    if(attribute_info[size].attributes_type_==ATTRIBUTE_TYPE_Unknow || attribute_info[size].info.unknow==NULL) {continue;}
    switch(attribute_info[size].attributes_type_){
      case ATTRIBUTE_TYPE_ConstantValue:
	cha_attribute_info_constantvalue_destroy(attribute_info[size].info.constantvalue_part);
	break;
      case ATTRIBUTE_TYPE_Code:
	cha_attribute_info_code_destroy(attribute_info[size].info.code_part);
	break;
      case ATTRIBUTE_TYPE_StackMapTable:
	cha_attribute_info_stack_map_table_destroy(attribute_info[size].info.stack_map_table_part);
	break;
      case ATTRIBUTE_TYPE_Exceptions:
	cha_attribute_info_exceptions_destroy(attribute_info[size].info.exceptions_part);
	break;
      case ATTRIBUTE_TYPE_InnerClasses:
	cha_attribute_info_inner_classes_destroy(attribute_info[size].info.inner_classes_part);
	break;
      case ATTRIBUTE_TYPE_EnclosingMethod:
	cha_attribute_info_enclosing_method_destroy(attribute_info[size].info.enclosing_method_part);
	break;
      case ATTRIBUTE_TYPE_Synthetic:
	/* empty */
	break;
      case ATTRIBUTE_TYPE_Signature:
	cha_attribute_info_signature_destroy(attribute_info[size].info.signature_part);
	break;
      case ATTRIBUTE_TYPE_SourceFile:
	cha_attribute_info_source_file_destroy(attribute_info[size].info.source_file_part);
	break;
      case ATTRIBUTE_TYPE_SourceDebugExtension:
	cha_attribute_info_source_debug_extension_destroy(attribute_info[size].info.source_debug_extension_part);
	break;
      case ATTRIBUTE_TYPE_LineNumberTable:
	cha_attribute_info_line_number_table_destroy(attribute_info[size].info.line_number_table_part);
	break;
      case ATTRIBUTE_TYPE_LocalVariableTable:
	cha_attribute_info_local_variable_table_destroy(attribute_info[size].info.local_variable_table_part);
	break;
      case ATTRIBUTE_TYPE_LocalVariableTypeTable:
	cha_attribute_info_local_variable_type_table_destroy(attribute_info[size].info.local_variable_type_table_part);
	break;
      case ATTRIBUTE_TYPE_Deprecated:
	/* empty */
	break;
      case ATTRIBUTE_TYPE_RuntimeVisibleAnnotations:
	cha_attribute_info_runtime_visible_annotations_destroy(attribute_info[size].info.runtime_visible_annotations_part);
	break;
      case ATTRIBUTE_TYPE_RuntimeInvisibleAnnotations:
	cha_attribute_info_runtime_invisible_annotations_destroy(attribute_info[size].info.runtime_invisible_annotations_part);
	break;
      case ATTRIBUTE_TYPE_RuntimeVisibleParameterAnnotations:
	cha_attribute_info_runtime_visible_parameter_annotations_destroy(attribute_info[size].info.runtime_visible_parameter_annotations_part);
	break;
      case ATTRIBUTE_TYPE_RuntimeInisibleParameterAnnotations:
	cha_attribute_info_runtime_invisible_parameter_annotations_destroy(attribute_info[size].info.runtime_invisible_parameter_annotations_part);
	break;
      case ATTRIBUTE_TYPE_AnnotationDefault:
	cha_attribute_info_annotation_default_destroy(attribute_info[size].info.annotation_default_part);
	break;
      case ATTRIBUTE_TYPE_BootstrapMethods:
	cha_attribute_info_bootstrap_methods_destroy(attribute_info[size].info.bootstrap_methods_part);
	break;
      case ATTRIBUTE_TYPE_Unknow:
      default:
	break;
    }
  }
  free(attribute_info);
  return 0;
}
