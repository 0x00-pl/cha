/* Class File Format
   Copyright (c) 2014 Cheryl Natsu 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */

#ifndef _CLSFILE_H_
#define _CLSFILE_H_

#include <stdio.h>

#include "dt.h"



/* The Constant Pool */

enum
{
    CONSTANT_Unknown = 0,
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_String = 8,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_NameAndType = 12,
    CONSTANT_Utf8 = 1,
    CONSTANT_MethodHandle = 15,
    CONSTANT_MethodType = 16,
    CONSTANT_InvokeDynamic = 18,
};

/* cp_info : CONSTANT_Class */

typedef struct 
{
    u2 name_index;
} cha_cp_info_class_t;
cha_cp_info_class_t *cha_cp_info_class_new(void);
int cha_cp_info_class_destroy(cha_cp_info_class_t *cp_info_class);

/* cp_info : CONSTANT_Utf8 */

typedef struct 
{
    u2 length;
    u1 *bytes;
} cha_cp_info_utf8_t;
cha_cp_info_utf8_t *cha_cp_info_utf8_new(void);
int cha_cp_info_utf8_destroy(cha_cp_info_utf8_t *cp_info_utf8);

/* cp_info : CONSTANT_Integer */

typedef struct 
{
    u4 bytes;
} cha_cp_info_integer_t;
cha_cp_info_integer_t *cha_cp_info_integer_new(void);
int cha_cp_info_integer_destroy(cha_cp_info_integer_t *cp_info_integer);

/* cp_info : CONSTANT_Float */

typedef struct 
{
    u4 bytes;
} cha_cp_info_float_t;
cha_cp_info_float_t *cha_cp_info_float_new(void);
int cha_cp_info_float_destroy(cha_cp_info_float_t *cp_info_float);

/* cp_info : CONSTANT_Long */

typedef struct 
{
    u4 high_bytes;
    u4 low_bytes;
} cha_cp_info_long_t;
cha_cp_info_long_t *cha_cp_info_long_new(void);
int cha_cp_info_long_destroy(cha_cp_info_long_t *cp_info_long);

/* cp_info : CONSTANT_Double */

typedef struct 
{
    u4 high_bytes;
    u4 low_bytes;
} cha_cp_info_double_t;
cha_cp_info_double_t *cha_cp_info_double_new(void);
int cha_cp_info_double_destroy(cha_cp_info_double_t *cp_info_double);

/* cp_info : 
 *   CONSTANT_Fieldref 
 *   CONSTANT_Methodref 
 *   CONSTANT_Interface_Methodref 
 */

typedef struct 
{
    u2 class_index;
    u2 name_and_type_index;
} cha_cp_info_ref_t;
cha_cp_info_ref_t *cha_cp_info_ref_new(void);
int cha_cp_info_ref_destroy(cha_cp_info_ref_t *cp_info_ref);

/* cp_info : CONSTANT_String */

typedef struct 
{
    u2 index;
} cha_cp_info_string_t;
cha_cp_info_string_t *cha_cp_info_string_new(void);
int cha_cp_info_string_destroy(cha_cp_info_string_t *cp_info_string);

/* cp_info : CONSTANT_NameAndType */

typedef struct 
{
    u2 name_index;
    u2 descriptor_index;
} cha_cp_info_name_and_type_t;
cha_cp_info_name_and_type_t *cha_cp_info_name_and_type_new(void);
int cha_cp_info_name_and_type_destroy(cha_cp_info_name_and_type_t *cp_info_name_and_type);

/* cp_info : CONSTANT_MethodHandle */

typedef struct 
{
    u2 reference_kind;
    u2 reference_index;
} cha_cp_info_method_handle_t;
cha_cp_info_method_handle_t *cha_cp_info_method_handle_new(void);
int cha_cp_info_method_handle_destroy(cha_cp_info_method_handle_t *cp_info_method_handle);

/* cp_info : CONSTANT_MethodType */

typedef struct 
{
    u2 descriptor_index;
} cha_cp_info_method_type_t;
cha_cp_info_method_type_t *cha_cp_info_method_type_new(void);
int cha_cp_info_method_type_destroy(cha_cp_info_method_type_t *cp_info_method_type);

/* cp_info : CONSTANT_InvokeDynamic */

typedef struct 
{
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} cha_cp_info_invoke_dynamic_t;
cha_cp_info_invoke_dynamic_t *cha_cp_info_invoke_dynamic_new(void);
int cha_cp_info_invoke_dynamic_destroy(cha_cp_info_invoke_dynamic_t *cp_info_invoke_dynamic);

typedef struct 
{
    u1 tag;
    union
    {
        cha_cp_info_class_t *class_part;
        cha_cp_info_utf8_t *utf8_part;
        cha_cp_info_integer_t *integer_part;
        cha_cp_info_float_t *float_part;
        cha_cp_info_long_t *long_part;
        cha_cp_info_double_t *double_part;
        cha_cp_info_ref_t *ref_part;
        cha_cp_info_string_t *string_part;
        cha_cp_info_name_and_type_t *name_and_type_part;
        cha_cp_info_method_handle_t *method_handle_part;
        cha_cp_info_method_type_t *method_type_part;
        cha_cp_info_invoke_dynamic_t *invoke_dynamic_part;
    } info;
} cha_cp_info_t;
cha_cp_info_t *cha_cp_info_new(size_t size);
int cha_cp_info_destroy(cha_cp_info_t *cp_info, size_t size);


/* Access Flags */
enum
{
    ACC_PUBLIC = 0x0001,
    ACC_PRIVATE = 0x0002,
    ACC_PROTECTED = 0x0004,
    ACC_STATIC = 0x0008,
    ACC_FINAL = 0x0010,
    ACC_SUPER = 0x0020,
    ACC_VOLATILE = 0x0040,
    /*ACC_BRIDGE = 0x0040,*/
    ACC_TRANSIENT = 0x0080,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400,
    ACC_STRICT = 0x0800,
    ACC_SYNTHETIC = 0x1000,
    ACC_ANNOTATION = 0x2000,
    ACC_ENUM = 0x4000,
};


/* Attributes */

struct cha_attribute_info_t_decl;

/* Attribute : ConstantValue */

typedef struct
{
  u2 constantvalue_index;
} cha_attribute_info_constantvalue_t;
cha_attribute_info_constantvalue_t *cha_attribute_info_constantvalue_new(void);
int cha_attribute_info_constantvalue_destroy(cha_attribute_info_constantvalue_t *attribute_info_constantvalue);

/* Attribute : Code.exception_table */

typedef struct
{
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type;
} cha_attribute_info_code_exception_table_t;
cha_attribute_info_code_exception_table_t *cha_attribute_info_code_exception_table_new(size_t size);
int cha_attribute_info_code_exception_table_destroy(cha_attribute_info_code_exception_table_t *attribute_info_code_exception_table, size_t size);

/* Attribute : Code */

typedef struct
{
  u2 max_stack;
  u2 max_locals;
  u4 code_length;
  u1 *code; //TODO
  u2 exception_table_length;
  cha_attribute_info_code_exception_table_t *exception_table;
  u2 attributes_count;
  struct cha_attribute_info_t_decl *attributes;
} cha_attribute_info_code_t;
cha_attribute_info_code_t *cha_attribute_info_code_new(void);
int cha_attribute_info_code_destroy(cha_attribute_info_code_t *attribute_info_code);

/* Attribute : StackMapTable.stack_map_frame.verification_type_info */
enum{
  ITEM_Top = 0,
  ITEM_Integer = 1,
  ITEM_Float = 2,
  ITEM_Double = 3,
  ITEM_Long = 4,
  ITEM_Null = 5,
  ITEM_UninitializedThis = 6,
  ITEM_Object = 7,
  ITEM_Uninitialized = 8,
  ITEM_Unknow
};

typedef struct
{
  u1 tag;
  union
  {
    struct{} top_variable_part;
    struct{} integer_variable_part;
    struct{} float_variable_part;
    struct{} long_variable_part;
    struct{} double_variable_part;
    struct{} null_variable_part;
    struct{} uninitializedThis_variable_part;
    struct{u2 cpool_index;} object_variable_part;
    struct{u2 offset;} uninitialized_variable_part;
  } info;
} cha_attribute_info_stack_map_frame_verification_t;
cha_attribute_info_stack_map_frame_verification_t *cha_attribute_info_stack_map_frame_verification_new(size_t size);
int cha_attribute_info_stack_map_frame_verification_destroy(cha_attribute_info_stack_map_frame_verification_t *attribute_info_stack_map_frame_verification, size_t size);

/* Attribute : StackMapTable.stack_map_frame */

enum{
  FRAME_TYPE_SAME_Low    = 0,
  FRAME_TYPE_SAME_Height = 63,
  FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Low    = 64,
  FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Height = 127,
  FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED = 247,
  FRAME_TYPE_CHOP_Low    = 248,
  FRAME_TYPE_CHOP_Height = 250,
  FRAME_TYPE_SAME_FRAME_EXTENDED = 251,
  FRAME_TYPE_APPEND_Low    = 252,
  FRAME_TYPE_APPEND_Height = 254,
  FRAME_TYPE_FULL_FRAME = 255
};

typedef struct
{
  u1 frame_type;
  union
  {
    struct{} same_frame_part;
    struct{
      cha_attribute_info_stack_map_frame_verification_t *stack_1;
    } same_locals_1_stack_item_frame_part;
    struct{
      u2 offset_delta;
      cha_attribute_info_stack_map_frame_verification_t *stack_1;
    } same_locals_1_stack_item_frame_extended_part;
    struct{
      u2 offset_delta;
    } chop_frame_part;
    struct{
      u2 offset_delta;
    } same_frame_extended_part;
    struct{
      u2 offset_delta;
      cha_attribute_info_stack_map_frame_verification_t *locals;
    } append_frame_part;
    struct{
      u2 offset_delta;
      u2 number_of_locals;
      cha_attribute_info_stack_map_frame_verification_t *locals;
      u2 number_of_stack_items;
      cha_attribute_info_stack_map_frame_verification_t *stack;
    } full_frame_part;
  } info;
} cha_attribute_info_stack_map_frame_t;
cha_attribute_info_stack_map_frame_t *cha_attribute_info_stack_map_frame_new(size_t size);
int cha_attribute_info_stack_map_frame_destroy(cha_attribute_info_stack_map_frame_t *attribute_info_stack_map_frame, size_t size);

/* Attribute : StackMapTable */

typedef struct
{
  u2 number_of_entries;
  cha_attribute_info_stack_map_frame_t *exception_table;
} cha_attribute_info_stack_map_table_t;
cha_attribute_info_stack_map_table_t *cha_attribute_info_stack_map_table_new(void);
int cha_attribute_info_stack_map_table_destroy(cha_attribute_info_stack_map_table_t *attribute_info_stack_map_table);

/* Attribute : Exceptions */

typedef struct
{
  u2 number_of_exceptions;
  u2 *exception_index_table;
} cha_attribute_info_exceptions_t;
cha_attribute_info_exceptions_t *cha_attribute_info_exceptions_new(void);
int cha_attribute_info_exceptions_destroy(cha_attribute_info_exceptions_t *attribute_info_exceptions);

/* Attribute : InnerClasses.classes */

typedef struct
{
  u2 inner_class_info_index;
  u2 outer_class_info_index;
  u2 inner_name_index;
  u2 inner_class_access_flags;
} cha_attribute_info_inner_class_t;
cha_attribute_info_inner_class_t *cha_attribute_info_inner_class_new(size_t size);
int cha_attribute_info_inner_class_destroy(cha_attribute_info_inner_class_t *attribute_info_inner_classes_classes, size_t size);

/* Attribute : InnerClasses */

typedef struct
{
  u2 number_of_classes;
  cha_attribute_info_inner_class_t *classes;
} cha_attribute_info_inner_classes_t;
cha_attribute_info_inner_classes_t *cha_attribute_info_inner_classes_new(void);
int cha_attribute_info_inner_classes_destroy(cha_attribute_info_inner_classes_t *attribute_info_inner_classes);

/* Attribute : EnclosingMethod */

typedef struct
{
  u2 class_index;
  u2 method_index;
} cha_attribute_info_enclosing_method_t;
cha_attribute_info_enclosing_method_t *cha_attribute_info_enclosing_method_new(void);
int cha_attribute_info_enclosing_method_destroy(cha_attribute_info_enclosing_method_t *attribute_info_enclosing_method);

/* Attribute : Synthetic */

typedef struct
{/* empty */} cha_attribute_info_synthetic_t;
// cha_attribute_info_synthetic_t *cha_attribute_info_synthetic_new(void);
// int cha_attribute_info_synthetic_destroy(cha_attribute_info_synthetic_t *attribute_info_synthetic);

/* Attribute : Signature */

typedef struct
{
  u2 signature_index;
} cha_attribute_info_signature_t;
cha_attribute_info_signature_t *cha_attribute_info_signature_new(void);
int cha_attribute_info_signature_destroy(cha_attribute_info_signature_t *attribute_info_signature);


/* Attribute : SourceFile */

typedef struct
{
  u2 sourcefile_index;
} cha_attribute_info_source_file_t;
cha_attribute_info_source_file_t *cha_attribute_info_source_file_new(void);
int cha_attribute_info_source_file_destroy(cha_attribute_info_source_file_t *attribute_info_source_file);


/* Attribute : SourceDebugExtension */

typedef struct
{
  u1 *debug_extension;
} cha_attribute_info_source_debug_extension_t;
cha_attribute_info_source_debug_extension_t *cha_attribute_info_source_debug_extension_new(void);
int cha_attribute_info_source_debug_extension_destroy(cha_attribute_info_source_debug_extension_t *attribute_info_source_debug_extension);


/* Attribute : LineNumberTable.line_number */

typedef struct
{
  u2 start_pc;
  u2 line_number;
} cha_attribute_info_line_number_t;
cha_attribute_info_line_number_t *cha_attribute_info_line_number_new(size_t size);
int cha_attribute_info_line_number_destroy(cha_attribute_info_line_number_t *attribute_info_line_number, size_t size);

/* Attribute : LineNumberTable */

typedef struct
{
  u2 line_number_table_length;
  cha_attribute_info_line_number_t *line_number_table;
} cha_attribute_info_line_number_table_t;
cha_attribute_info_line_number_table_t *cha_attribute_info_line_number_table_new(void);
int cha_attribute_info_line_number_table_destroy(cha_attribute_info_line_number_table_t *attribute_info_line_number_table);


/* Attribute : LocalVariableTable.local_variable */

typedef struct
{
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 descriptor_index;
  u2 index;
} cha_attribute_info_local_variable_t;
cha_attribute_info_local_variable_t *cha_attribute_info_local_variable_new(size_t size);
int cha_attribute_info_local_variable_destroy(cha_attribute_info_local_variable_t *attribute_info_local_variable, size_t size);

/* Attribute : LocalVariableTable */

typedef struct
{
  u2 local_variable_table_length;
  cha_attribute_info_local_variable_t *local_variable_table;
} cha_attribute_info_local_variable_table_t;
cha_attribute_info_local_variable_table_t *cha_attribute_info_local_variable_table_new(void);
int cha_attribute_info_local_variable_table_destroy(cha_attribute_info_local_variable_table_t *attribute_info_local_variable_table);

/* Attribute : LocalVariableTypeTable.local_variable_type */

typedef struct
{
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 signature_index;
  u2 index;
} cha_attribute_info_local_variable_type_t;
cha_attribute_info_local_variable_type_t *cha_attribute_info_local_variable_type_new(size_t size);
int cha_attribute_info_local_variable_type_destroy(cha_attribute_info_local_variable_type_t *attribute_info_local_variable_type, size_t size);

/* Attribute : LocalVariableTypeTable */

typedef struct
{
  u2 local_variable_type_table_length;
  cha_attribute_info_local_variable_type_t *local_variable_type_table;
} cha_attribute_info_local_variable_type_table_t;
cha_attribute_info_local_variable_type_table_t *cha_attribute_info_local_variable_type_table_new(void);
int cha_attribute_info_local_variable_type_table_destroy(cha_attribute_info_local_variable_type_table_t *attribute_info_local_variable_type_table);


/* Attribute : Deprecated */

typedef struct
{/* empty */} cha_attribute_info_deprecated_t;
// cha_attribute_info_deprecated_t *cha_attribute_info_deprecated_new(void);
// int cha_attribute_info_deprecated_destroy(cha_attribute_info_deprecated_t *attribute_info_deprecated);

/* Attribute : Annotation.element_value_pair */

struct cha_attribute_info_element_value_t_decl;
typedef struct
{
    u2 element_name_index;
    struct cha_attribute_info_element_value_t_decl *value;
} cha_attribute_info_element_value_pair_t;
cha_attribute_info_element_value_pair_t *cha_attribute_info_element_value_pair_new(size_t size);
int cha_attribute_info_element_value_pair_destroy(cha_attribute_info_element_value_pair_t* attribute_info_element_value_pair, size_t size);


/* Attribute : Annotation */

typedef struct
{
  u2 type_index;
  u2 num_element_value_pairs;
  cha_attribute_info_element_value_pair_t *element_value_pairs;
} cha_attribute_info_annotation_t;
cha_attribute_info_annotation_t *cha_attribute_info_annotation_new(size_t size);
int cha_attribute_info_annotation_destroy(cha_attribute_info_annotation_t *attribute_info_annotation, size_t size);

/* Attribute : Annotation.element_value */

typedef struct
{
  u2 type_name_index;
  u2 const_name_index;
} enum_const_value_t;
    
typedef struct cha_attribute_info_element_value_t_decl
{
  u1 tag;
  union {
    u2 const_value_index_part;

    enum_const_value_t enum_const_value_part;

    u2 class_info_index_part;

    cha_attribute_info_annotation_t *annotation_value_part;

    struct {
      u2 num_values;
      struct cha_attribute_info_element_value_t_decl *values;
    } array_value_part;
  } value;
} cha_attribute_info_element_value_t;
cha_attribute_info_element_value_t *cha_attribute_info_element_value_new(size_t size);
int cha_attribute_info_element_value_destroy(cha_attribute_info_element_value_t *attribute_info_element_value, size_t size);


/* Attribute : RuntimeVisibleAnnotations */

typedef struct
{
  u2 num_annotations;
  cha_attribute_info_annotation_t *annotations;
} cha_attribute_info_runtime_visible_annotations_t;
cha_attribute_info_runtime_visible_annotations_t *cha_attribute_info_runtime_visible_annotations_new(void);
int cha_attribute_info_runtime_visible_annotations_destroy(cha_attribute_info_runtime_visible_annotations_t *attribute_info_runtime_visible_annotations);

/* Attribute : RuntimeInvisibleAnnotations */

typedef struct
{
  u2 num_annotations;
  cha_attribute_info_annotation_t *annotations;
} cha_attribute_info_runtime_invisible_annotations_t;
cha_attribute_info_runtime_invisible_annotations_t *cha_attribute_info_runtime_invisible_annotations_new(void);
int cha_attribute_info_runtime_invisible_annotations_destroy(cha_attribute_info_runtime_invisible_annotations_t *attribute_info_runtime_invisible_annotations);

/* Attribute : ParameterAnnotations */

typedef struct
{
  u2 num_annotations;
  cha_attribute_info_annotation_t *annotations;
} cha_attribute_info_parameter_annotation_t;
cha_attribute_info_parameter_annotation_t *cha_attribute_info_attribute_info_parameter_annotation_new(size_t size);
int cha_attribute_info_parameter_annotation_destroy(cha_attribute_info_parameter_annotation_t *attribute_info_parameter_annotation, size_t size);

/* Attribute : RuntimeVisibleParameterAnnotations */

typedef struct
{
  u1 num_parameters;
  cha_attribute_info_parameter_annotation_t *parameter_annotations;
} cha_attribute_info_runtime_visible_parameter_annotations_t;
cha_attribute_info_runtime_visible_parameter_annotations_t *cha_attribute_info_runtime_visible_parameter_annotations_new(void);
int cha_attribute_info_runtime_visible_parameter_annotations_destroy(cha_attribute_info_runtime_visible_parameter_annotations_t *attribute_info_runtime_visible_parameter_annotations);

/* Attribute : RuntimeInisibleParameterAnnotations */

typedef struct
{
  u1 num_parameters;
  cha_attribute_info_parameter_annotation_t *parameter_annotations;
} cha_attribute_info_runtime_invisible_parameter_annotations_t;
cha_attribute_info_runtime_invisible_parameter_annotations_t *cha_attribute_info_runtime_invisible_parameter_annotations_new(void);
int cha_attribute_info_runtime_invisible_parameter_annotations_destroy(cha_attribute_info_runtime_invisible_parameter_annotations_t *attribute_info_runtime_invisible_parameter_annotations);


/* Attribute : AnnotationDefault */

typedef struct
{
  cha_attribute_info_element_value_t *default_value;
} cha_attribute_info_annotation_default_t;
cha_attribute_info_annotation_default_t *cha_attribute_info_annotation_default_new(void);
int cha_attribute_info_annotation_default_destroy(cha_attribute_info_annotation_default_t *attribute_info_annotation_default);

/* Attribute : BootstrapMethods.bootstrap_method */

typedef struct
{
  u2 bootstrap_method_ref;
  u2 num_bootstrap_arguments;
  u2 *bootstrap_arguments;
} cha_attribute_info_bootstrap_method_t;
cha_attribute_info_bootstrap_method_t *cha_attribute_info_attribute_info_bootstrap_method_new(size_t size);
int cha_attribute_info_bootstrap_method_destroy(cha_attribute_info_bootstrap_method_t *attribute_info_bootstrap_method, size_t size);

/* Attribute : BootstrapMethods */

typedef struct
{
  u2 num_bootstrap_methods;
  cha_attribute_info_bootstrap_method_t *bootstrap_methods;
} cha_attribute_info_bootstrap_methods_t;
cha_attribute_info_bootstrap_methods_t *cha_attribute_info_bootstrap_methods_new(void);
int cha_attribute_info_bootstrap_methods_destroy(cha_attribute_info_bootstrap_methods_t *attribute_info_bootstrap_methods);

enum{
  ATTRIBUTE_TYPE_ConstantValue,
  ATTRIBUTE_TYPE_Code,
  ATTRIBUTE_TYPE_StackMapTable,
  ATTRIBUTE_TYPE_Exceptions,
  ATTRIBUTE_TYPE_InnerClasses,
  ATTRIBUTE_TYPE_EnclosingMethod,
  ATTRIBUTE_TYPE_Synthetic,
  ATTRIBUTE_TYPE_Signature,
  ATTRIBUTE_TYPE_SourceFile,
  ATTRIBUTE_TYPE_SourceDebugExtension,
  ATTRIBUTE_TYPE_LineNumberTable,
  ATTRIBUTE_TYPE_LocalVariableTable,
  ATTRIBUTE_TYPE_LocalVariableTypeTable,
  ATTRIBUTE_TYPE_Deprecated,
  ATTRIBUTE_TYPE_RuntimeVisibleAnnotations,
  ATTRIBUTE_TYPE_RuntimeInvisibleAnnotations,
  ATTRIBUTE_TYPE_RuntimeVisibleParameterAnnotations,
  ATTRIBUTE_TYPE_RuntimeInisibleParameterAnnotations,
  ATTRIBUTE_TYPE_AnnotationDefault,
  ATTRIBUTE_TYPE_BootstrapMethods,
  ATTRIBUTE_TYPE_Unknow
};

typedef struct cha_attribute_info_t_decl
{
    u2 attributes_name_index;
    u4 attributes_length;
    union
    {
      cha_attribute_info_constantvalue_t                             *constantvalue_part;
      cha_attribute_info_code_t                                      *code_part;
      cha_attribute_info_stack_map_table_t                           *stack_map_table_part;
      cha_attribute_info_exceptions_t                                *exceptions_part;
      cha_attribute_info_inner_classes_t                             *inner_classes_part;
      cha_attribute_info_enclosing_method_t                          *enclosing_method_part;
      cha_attribute_info_synthetic_t                                 *synthetic_part;
      cha_attribute_info_signature_t                                 *signature_part;
      cha_attribute_info_source_file_t                               *source_file_part;
      cha_attribute_info_source_debug_extension_t                    *source_debug_extension_part;
      cha_attribute_info_line_number_table_t                         *line_number_table_part;
      cha_attribute_info_local_variable_table_t                      *local_variable_table_part;
      cha_attribute_info_local_variable_type_table_t                 *local_variable_type_table_part;
      cha_attribute_info_deprecated_t                                *deprecated_part;
      cha_attribute_info_runtime_visible_annotations_t               *runtime_visible_annotations_part;
      cha_attribute_info_runtime_invisible_annotations_t             *runtime_invisible_annotations_part;
      cha_attribute_info_runtime_visible_parameter_annotations_t     *runtime_visible_parameter_annotations_part;
      cha_attribute_info_runtime_invisible_parameter_annotations_t   *runtime_invisible_parameter_annotations_part;
      cha_attribute_info_annotation_default_t                        *annotation_default_part;
      cha_attribute_info_bootstrap_methods_t                         *bootstrap_methods_part;
      void                                                           *unknow;
    } info;
    int attributes_type_;
} cha_attribute_info_t;
cha_attribute_info_t *cha_attribute_info_new(size_t size);
int cha_attribute_info_destroy(cha_attribute_info_t *attribute_info, size_t size);


/* Fields */

typedef struct 
{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_field_info_t;
cha_field_info_t *cha_field_info_new(size_t size);
int cha_field_info_destroy(cha_field_info_t *field_info, size_t size);


/* Methods */

typedef struct 
{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_method_info_t;
cha_method_info_t *cha_method_info_new(size_t size);
int cha_method_info_destroy(cha_method_info_t *method_info, size_t size);


/* Extra Info */

typedef struct 
{
    char *filename;
    size_t len;
} cha_extra_info_t;
cha_extra_info_t *cha_extra_info_new(const char *filename, const size_t filename_len);
int cha_extra_info_destroy(cha_extra_info_t *extra_info);


/* Class File */

typedef struct 
{
    cha_extra_info_t *extra_info;

    u4 magic; /* 0xCAFEBABE */
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    cha_cp_info_t *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    cha_field_info_t *fields;
    u2 methods_count;
    cha_method_info_t *methods;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_class_file_t;
cha_class_file_t *cha_class_file_new(void);
int cha_class_file_destroy(cha_class_file_t *class_file);


#endif


