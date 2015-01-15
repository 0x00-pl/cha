
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "fs.h"
#include "dt.h"
#include "clsfile.h"
#include "clsld.h"
#include "err.h"
#include "instructions.h"

#define CHA_ERR_UPDATE_CORRUPTED_CLASS(err) \
    cha_err_update(err, CHA_ERR_CLSLD, "Error: Corrupted class file");

#define CHA_CLSLD_CHECK(handler, err_code, cond) if(1){ \
    if(cond){ \
      CHA_ERR_UPDATE_CORRUPTED_CLASS(err); \
      ret = err_code; \
      handler; \
    }}else{}


#define CHA_CLSLD_CALL_CHECK(handler, function_exp) if(1){ \
      if((ret = function_exp) != 0) \
      {handler;} \
    }else{}

#define CHA_CLSLD_NEW(handler, name_of_type) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_MALLOC, ((new_##name_of_type = cha_##name_of_type##_new()) == NULL))

#define CHA_CLSLD_NEW_ARR(handler, name_of_type, size) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_MALLOC, ((new_##name_of_type = cha_##name_of_type##_new(size)) == NULL))

#define CHA_CLSLD_CHECK_IO(handler) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_CLSLD, io->reach_eof)

#define CHA_CLSLD_READ_CHECK_u1(handler, dest, io) if(1){ \
      (dest) = io_read_u1(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }else{}

#define CHA_CLSLD_READ_CHECK_u2(handler, dest, io) if(1){ \
      (dest) = io_read_u2(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }else{}

#define CHA_CLSLD_READ_CHECK_u4(handler, dest, io) if(1){ \
      (dest) = io_read_u4(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }else{}

#define CHA_CLSLD_READ_CHECK_u8(handler, dest, io) if(1){ \
      (dest) = io_read_u8(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }else{}

#define CHA_CLSLD_READ_CHECK_bytes(handler, dest, size, io) if(1){ \
      io_read_bytes(io, dest, size); \
      CHA_CLSLD_CHECK_IO(handler); \
    }else{}

//


static int clsld_attribute_info_constantvalue( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_constantvalue_t **attribute_info_constantvalue_out)
{
    int ret = 0;
    cha_attribute_info_constantvalue_t *new_attribute_info_constantvalue = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_constantvalue);

    CHA_CLSLD_READ_CHECK_u1(goto fail, new_attribute_info_constantvalue->constantvalue_index, io);

    *attribute_info_constantvalue_out = new_attribute_info_constantvalue;

    goto done;
fail:
    cha_attribute_info_constantvalue_destroy(new_attribute_info_constantvalue);
done:
    return ret;
}


static int clsld_attribute_info_code_exception_table( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_code_exception_table_t **out,
        size_t count)
{
    int ret = 0;
    cha_attribute_info_code_exception_table_t *new_attribute_info_code_exception_table = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_code_exception_table, count);

    for(i = 0; i<count; i++){
      new_attribute_info_code_exception_table[i].start_pc = io_read_u2(io);
      new_attribute_info_code_exception_table[i].end_pc = io_read_u2(io);
      new_attribute_info_code_exception_table[i].handler_pc = io_read_u2(io);
      new_attribute_info_code_exception_table[i].catch_type = io_read_u2(io);
      CHA_CLSLD_CHECK_IO(break);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_code_exception_table;

    goto done;
fail:
    cha_attribute_info_code_exception_table_destroy(new_attribute_info_code_exception_table, count);
done:
    return ret;
}


static int clsld_attribute_info_code( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_t *cp_info, u2 cp_count, \
        cha_attribute_info_code_t **out)
{
    int ret = 0;
    cha_attribute_info_code_t *new_attribute_info_code = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_code);
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->max_stack, io);
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->max_locals, io);

    CHA_CLSLD_READ_CHECK_u4(goto fail, new_attribute_info_code->code_length, io);
    new_attribute_info_code->code = (u1*)malloc(new_attribute_info_code->code_length * sizeof(u1)); //TODO
    CHA_CLSLD_READ_CHECK_bytes(goto fail,
                               new_attribute_info_code->code,
                               new_attribute_info_code->code_length,
                               io);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->exception_table_length, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_code_exception_table(
	err, io,
	&new_attribute_info_code->exception_table,
	new_attribute_info_code->exception_table_length));
    
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->attributes_count, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attributes_info(
	err, io, cp_info, cp_count,
	&new_attribute_info_code->attributes,
	new_attribute_info_code->attributes_count));
    
    *out = new_attribute_info_code;

    goto done;
fail:
    cha_attribute_info_code_destroy(new_attribute_info_code);
done:
    return ret;
}

static int clsld_attribute_info_stack_map_frame_verification( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_stack_map_frame_verification_t **out,
        size_t count)
{
    int ret = 0;
    cha_attribute_info_stack_map_frame_verification_t *new_attribute_info_stack_map_frame_verification = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_stack_map_frame_verification, count);

    for(i = 0; i<count; i++){
      new_attribute_info_stack_map_frame_verification[i].tag = io_read_u1(io);
      CHA_CLSLD_CHECK_IO(break);
      switch(new_attribute_info_stack_map_frame_verification[i].tag){
        case ITEM_Top:
        case ITEM_Integer:
        case ITEM_Float:
        case ITEM_Double:
        case ITEM_Long:
        case ITEM_Null:
        case ITEM_UninitializedThis:
          break;
        case ITEM_Object:
          new_attribute_info_stack_map_frame_verification[i].info.object_variable_part.cpool_index = io_read_u2(io);
          break;
        case ITEM_Uninitialized:
          new_attribute_info_stack_map_frame_verification[i].info.uninitialized_variable_part.offset = io_read_u2(io);
          break;
        default:
          break;
      }
      CHA_CLSLD_CHECK_IO(break);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_stack_map_frame_verification;

    goto done;
fail:
    cha_attribute_info_stack_map_frame_verification_destroy(new_attribute_info_stack_map_frame_verification, count);
done:
    return ret;
}

static int clsld_attribute_info_stack_map_frame( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_stack_map_frame_t **out,
        size_t count)
{
    int ret = 0;
    cha_attribute_info_stack_map_frame_t *new_attribute_info_stack_map_frame = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_stack_map_frame, count);

    for(i = 0; i<count; i++){
      new_attribute_info_stack_map_frame[i].frame_type = io_read_u1(io);
      CHA_CLSLD_CHECK_IO(break);
      u1 frame_type = new_attribute_info_stack_map_frame[i].frame_type;
      if(frame_type <= FRAME_TYPE_SAME_Height){
        /* empty */
      }
      else if(FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Low <= frame_type &&
        frame_type <= FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Height
      ){
	CHA_CLSLD_CALL_CHECK(
	  break,
	  clsld_attribute_info_stack_map_frame_verification(
	    err, io,
	    &new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_part.stack_1,
	    1));
      }
      else if(frame_type == FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED){
        new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_extended_part.offset_delta = io_read_u2(io);

        CHA_CLSLD_CALL_CHECK(
	  break,
	  clsld_attribute_info_stack_map_frame_verification(
	    err, io,
	    &new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_extended_part.stack_1,
	    1));
      }
      else if(FRAME_TYPE_CHOP_Low <= frame_type &&
        frame_type <= FRAME_TYPE_CHOP_Height
      ){
        new_attribute_info_stack_map_frame[i].info.chop_frame_part.offset_delta = io_read_u2(io);
      }
      else if(frame_type == FRAME_TYPE_SAME_FRAME_EXTENDED){
        new_attribute_info_stack_map_frame[i].info.same_frame_extended_part.offset_delta = io_read_u2(io);
      }
      else if(FRAME_TYPE_APPEND_Low <= frame_type &&
        frame_type <= FRAME_TYPE_APPEND_Height
      ){
        new_attribute_info_stack_map_frame[i].info.append_frame_part.offset_delta = io_read_u2(io);

        CHA_CLSLD_CALL_CHECK(
	  break,
	  clsld_attribute_info_stack_map_frame_verification(
	    err, io,
	    &new_attribute_info_stack_map_frame[i].info.append_frame_part.locals,
	    (size_t)(frame_type-251)));
      }
      else if(frame_type == FRAME_TYPE_FULL_FRAME){
        new_attribute_info_stack_map_frame[i].info.full_frame_part.offset_delta = io_read_u2(io);
        
        new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_locals = io_read_u2(io);

        CHA_CLSLD_CALL_CHECK(
	  break,
	  clsld_attribute_info_stack_map_frame_verification(
	    err, io,
	    &new_attribute_info_stack_map_frame[i].info.full_frame_part.locals,
	    new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_locals));
        
        new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_stack_items = io_read_u2(io);

        CHA_CLSLD_CALL_CHECK(
	  break,
	  clsld_attribute_info_stack_map_frame_verification(
	    err, io,
	    &new_attribute_info_stack_map_frame[i].info.full_frame_part.stack,
	    new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_stack_items));
      }
      else{
        CHA_ERR_UPDATE_CORRUPTED_CLASS(err);
        break;
      }
      CHA_CLSLD_CHECK_IO(break);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_stack_map_frame;

    goto done;
fail:
    cha_attribute_info_stack_map_frame_destroy(new_attribute_info_stack_map_frame, count);
done:
    return ret;
}


static int clsld_attribute_info_stack_map_table( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_stack_map_table_t **out)
{
    int ret = 0;
    cha_attribute_info_stack_map_table_t *new_attribute_info_stack_map_table = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_stack_map_table);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_stack_map_table->number_of_entries, io);
    if((ret = clsld_attribute_info_stack_map_frame(
      err, io,
      &new_attribute_info_stack_map_table->entries,
      new_attribute_info_stack_map_table->number_of_entries
    )) != 0)
    {goto fail;}

    *out = new_attribute_info_stack_map_table;

    goto done;
fail:
    cha_attribute_info_stack_map_table_destroy(new_attribute_info_stack_map_table);
done:
    return ret;
}

static int clsld_attribute_info_exceptions( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_exceptions_t **out)
{
    int ret = 0;
    cha_attribute_info_exceptions_t *new_attribute_info_exceptions = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_exceptions);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_exceptions->number_of_exceptions, io);
    
    new_attribute_info_exceptions->exception_index_table = (u2*)malloc(
      new_attribute_info_exceptions->number_of_exceptions * sizeof(u2));
    if(new_attribute_info_exceptions->exception_index_table == NULL) {
      CHA_ERR_UPDATE_CORRUPTED_CLASS(err);
      goto fail;
    }
    
    CHA_CLSLD_READ_CHECK_bytes(
      goto fail,
      new_attribute_info_exceptions->exception_index_table,
      (new_attribute_info_exceptions->number_of_exceptions * sizeof(u2)),
      io);
    
    *out = new_attribute_info_exceptions;

    goto done;
fail:
    cha_attribute_info_exceptions_destroy(new_attribute_info_exceptions);
done:
    return ret;
}


static int clsld_attribute_info_inner_class( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_inner_class_t **out,
        size_t count)
{
    int ret = 0;
    cha_attribute_info_inner_class_t *new_attribute_info_inner_class = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_inner_class, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_inner_class[i].inner_class_info_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_inner_class[i].outer_class_info_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_inner_class[i].inner_name_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_inner_class[i].inner_class_access_flags, io);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_inner_class;

    goto done;
fail:
    cha_attribute_info_inner_class_destroy(new_attribute_info_inner_class, count);
done:
    return ret;
}

static int clsld_attribute_info_inner_classes( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_inner_classes_t **out)
{
    int ret = 0;
    cha_attribute_info_inner_classes_t *new_attribute_info_inner_classes = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_inner_classes);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_inner_classes->number_of_classes, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_inner_class(
	err, io,
	&new_attribute_info_inner_classes->classes,
	new_attribute_info_inner_classes->number_of_classes));

    *out = new_attribute_info_inner_classes;

    goto done;
fail:
    cha_attribute_info_inner_classes_destroy(new_attribute_info_inner_classes);
done:
    return ret;
}

static int clsld_attribute_info_enclosing_method( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_enclosing_method_t **out)
{
    int ret = 0;
    cha_attribute_info_enclosing_method_t *new_attribute_info_enclosing_method = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_enclosing_method);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_enclosing_method->class_index, io);
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_enclosing_method->method_index, io);

    *out = new_attribute_info_enclosing_method;

    goto done;
fail:
    cha_attribute_info_enclosing_method_destroy(new_attribute_info_enclosing_method);
done:
    return ret;
}

/* Attribute : Synthetic */
/* empty */

static int clsld_attribute_info_signature( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_signature_t **out)
{
    int ret = 0;
    cha_attribute_info_signature_t *new_attribute_info_signature = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_signature);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_signature->signature_index, io);

    *out = new_attribute_info_signature;

    goto done;
fail:
    cha_attribute_info_signature_destroy(new_attribute_info_signature);
done:
    return ret;
}

static int clsld_attribute_info_source_file( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_source_file_t **out)
{
    int ret = 0;
    cha_attribute_info_source_file_t *new_attribute_info_source_file = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_source_file);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_source_file->sourcefile_index, io);

    *out = new_attribute_info_source_file;

    goto done;
fail:
    cha_attribute_info_source_file_destroy(new_attribute_info_source_file);
done:
    return ret;
}

static int clsld_attribute_info_source_debug_extension( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_source_debug_extension_t **out, \
	size_t size)
{
    int ret = 0;
    cha_attribute_info_source_debug_extension_t *new_attribute_info_source_debug_extension = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_source_debug_extension);

    new_attribute_info_source_debug_extension->debug_extension = (u1*)malloc(size);
    if(new_attribute_info_source_debug_extension->debug_extension == NULL) {
      CHA_ERR_UPDATE_CORRUPTED_CLASS(err);
      goto fail;
    }
    
    CHA_CLSLD_READ_CHECK_bytes(
      goto fail,
      new_attribute_info_source_debug_extension->debug_extension,
      (size * sizeof(u1)),
      io);

    *out = new_attribute_info_source_debug_extension;

    goto done;
fail:
    cha_attribute_info_source_debug_extension_destroy(new_attribute_info_source_debug_extension);
done:
    return ret;
}

static int clsld_attribute_info_line_number( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_line_number_t **out,
        size_t count)
{
    int ret = 0;
    cha_attribute_info_line_number_t *new_attribute_info_line_number = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_line_number, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_line_number[i].start_pc, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_line_number[i].line_number, io);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_line_number;

    goto done;
fail:
    cha_attribute_info_line_number_destroy(new_attribute_info_line_number, count);
done:
    return ret;
}


static int clsld_attribute_info_line_number_table( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_line_number_table_t **out)
{
    int ret = 0;
    cha_attribute_info_line_number_table_t *new_attribute_info_line_number_table = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_line_number_table);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_line_number_table->line_number_table_length, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_line_number(
	err,io,
	&new_attribute_info_line_number_table->line_number_table,
	new_attribute_info_line_number_table->line_number_table_length
      ));

    *out = new_attribute_info_line_number_table;

    goto done;
fail:
    cha_attribute_info_line_number_table_destroy(new_attribute_info_line_number_table);
done:
    return ret;
}

static int clsld_attribute_info_local_variable( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_local_variable_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_local_variable_t *new_attribute_info_local_variable = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_local_variable, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable[i].start_pc, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable[i].length, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable[i].name_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable[i].descriptor_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable[i].index, io);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_local_variable;

    goto done;
fail:
    cha_attribute_info_local_variable_destroy(new_attribute_info_local_variable, count);
done:
    return ret;
}

static int clsld_attribute_info_local_variable_table( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_local_variable_table_t **out)
{
    int ret = 0;
    cha_attribute_info_local_variable_table_t *new_attribute_info_local_variable_table = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_local_variable_table);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_local_variable_table->local_variable_table_length, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_local_variable(
	err,io,
	&new_attribute_info_local_variable_table->local_variable_table,
	new_attribute_info_local_variable_table->local_variable_table_length
      ));

    *out = new_attribute_info_local_variable_table;

    goto done;
fail:
    cha_attribute_info_local_variable_table_destroy(new_attribute_info_local_variable_table);
done:
    return ret;
}

static int clsld_attribute_info_local_variable_type( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_local_variable_type_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_local_variable_type_t *new_attribute_info_local_variable_type = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_local_variable_type, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable_type[i].start_pc, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable_type[i].length, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable_type[i].name_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable_type[i].signature_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_local_variable_type[i].index, io);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_local_variable_type;

    goto done;
fail:
    cha_attribute_info_local_variable_type_destroy(new_attribute_info_local_variable_type, count);
done:
    return ret;
}

static int clsld_attribute_info_local_variable_type_table( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_local_variable_type_table_t **out)
{
    int ret = 0;
    cha_attribute_info_local_variable_type_table_t *new_attribute_info_local_variable_type_table = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_local_variable_type_table);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_local_variable_type_table->local_variable_type_table_length, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_local_variable_type(
	err,io,
	&new_attribute_info_local_variable_type_table->local_variable_type_table,
	new_attribute_info_local_variable_type_table->local_variable_type_table_length
      ));

    *out = new_attribute_info_local_variable_type_table;

    goto done;
fail:
    cha_attribute_info_local_variable_type_table_destroy(new_attribute_info_local_variable_type_table);
done:
    return ret;
}


/* Attribute : Deprecated */
/* empty */

static int clsld_attribute_info_element_value( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_element_value_t **out, \
        size_t count);

static int clsld_attribute_info_element_value_pair( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_element_value_pair_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_element_value_pair_t *new_attribute_info_element_value_pair = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_element_value_pair, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_element_value_pair[i].element_name_index, io);
      
      CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_element_value(
	err,io,
	&(new_attribute_info_element_value_pair[i].value),
	new_attribute_info_element_value_pair[i].element_name_index
      ));

    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_element_value_pair;

    goto done;
fail:
    cha_attribute_info_element_value_pair_destroy(new_attribute_info_element_value_pair, count);
done:
    return ret;
}

static int clsld_attribute_info_annotation( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_annotation_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_annotation_t *new_attribute_info_annotation = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_annotation, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_annotation[i].type_index, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_annotation[i].num_element_value_pairs, io);
         
      CHA_CLSLD_CALL_CHECK(
      break,
      clsld_attribute_info_element_value_pair(
	err,io,
	&new_attribute_info_annotation[i].element_value_pairs,
	new_attribute_info_annotation[i].num_element_value_pairs
      ));
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_annotation;

    goto done;
fail:
    cha_attribute_info_annotation_destroy(new_attribute_info_annotation, count);
done:
    return ret;
}


static int clsld_attribute_info_element_value( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_element_value_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_element_value_t *new_attribute_info_element_value = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_element_value, count);

    
    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u1(break, new_attribute_info_element_value[i].tag, io);
      
      switch(new_attribute_info_element_value[i].tag){
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
	  CHA_CLSLD_READ_CHECK_u2(
	    break, 
	    new_attribute_info_element_value[i].value.const_value_index_part, 
	    io);
	  break;
	case 'e':
	  /* enum_const_value_part */
	  CHA_CLSLD_READ_CHECK_u2(
	    break, 
	    new_attribute_info_element_value[i].value.enum_const_value_part.type_name_index, 
	    io);
	  CHA_CLSLD_READ_CHECK_u2(
	    break, 
	    new_attribute_info_element_value[i].value.enum_const_value_part.const_name_index,
	    io);
	  break;
	case 'c':
	  /* class_info_index_part */
	  CHA_CLSLD_READ_CHECK_u2(
	    break, 
	    new_attribute_info_element_value[i].value.class_info_index_part, 
	    io);
	  break;
	case '@':
	  /* annotation_value_part */
	  CHA_CLSLD_CALL_CHECK(
	    break,
	    clsld_attribute_info_annotation(
	      err,io,
	      &(new_attribute_info_element_value[i].value.annotation_value_part),
	      1
	    ));
	  break;
	case '[':
	  /* array_value_part */
	  CHA_CLSLD_READ_CHECK_u2(
	    break, 
	    new_attribute_info_element_value[i].value.array_value_part.num_values, 
	    io);
	  CHA_CLSLD_CALL_CHECK(
	    break,
	    clsld_attribute_info_element_value(
	      err,io,
	      &(new_attribute_info_element_value[i].value.array_value_part.values),
	      new_attribute_info_element_value[i].value.array_value_part.num_values
	    ));
	  break;
	default:
	  break;
      }
      if(ret != 0) {break;}
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_element_value;

    goto done;
fail:
    cha_attribute_info_element_value_destroy(new_attribute_info_element_value, count);
done:
    return ret;
}

static int clsld_attribute_info_runtime_visible_annotations( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_runtime_visible_annotations_t **out)
{
    int ret = 0;
    cha_attribute_info_runtime_visible_annotations_t *new_attribute_info_runtime_visible_annotations = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_runtime_visible_annotations);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_runtime_visible_annotations->num_annotations, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_annotation(
	err,io,
	&new_attribute_info_runtime_visible_annotations->annotations,
	new_attribute_info_runtime_visible_annotations->num_annotations
      ));

    *out = new_attribute_info_runtime_visible_annotations;

    goto done;
fail:
    cha_attribute_info_runtime_visible_annotations_destroy(new_attribute_info_runtime_visible_annotations);
done:
    return ret;
}

static int clsld_attribute_info_runtime_invisible_annotations( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_runtime_invisible_annotations_t **out)
{
    int ret = 0;
    cha_attribute_info_runtime_invisible_annotations_t *new_attribute_info_runtime_invisible_annotations = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_runtime_invisible_annotations);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_runtime_invisible_annotations->num_annotations, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_annotation(
	err,io,
	&new_attribute_info_runtime_invisible_annotations->annotations,
	new_attribute_info_runtime_invisible_annotations->num_annotations
      ));

    *out = new_attribute_info_runtime_invisible_annotations;

    goto done;
fail:
    cha_attribute_info_runtime_invisible_annotations_destroy(new_attribute_info_runtime_invisible_annotations);
done:
    return ret;
}

static int clsld_attribute_info_parameter_annotation( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_parameter_annotation_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_parameter_annotation_t *new_attribute_info_parameter_annotation = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_parameter_annotation, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_parameter_annotation[i].num_annotations, io);
      CHA_CLSLD_CALL_CHECK(
	goto fail,
	clsld_attribute_info_annotation(
	  err,io,
	  &new_attribute_info_parameter_annotation[i].annotations,
	  new_attribute_info_parameter_annotation[i].num_annotations
	));
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_parameter_annotation;

    goto done;
fail:
    cha_attribute_info_parameter_annotation_destroy(new_attribute_info_parameter_annotation, count);
done:
    return ret;
}

static int clsld_attribute_info_runtime_visible_parameter_annotations( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_runtime_visible_parameter_annotations_t **out)
{
    int ret = 0;
    cha_attribute_info_runtime_visible_parameter_annotations_t *new_attribute_info_runtime_visible_parameter_annotations = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_runtime_visible_parameter_annotations);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_runtime_visible_parameter_annotations->num_parameters, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_parameter_annotation(
	err,io,
	&new_attribute_info_runtime_visible_parameter_annotations->parameter_annotations,
	new_attribute_info_runtime_visible_parameter_annotations->num_parameters
      ));

    *out = new_attribute_info_runtime_visible_parameter_annotations;

    goto done;
fail:
    cha_attribute_info_runtime_visible_parameter_annotations_destroy(new_attribute_info_runtime_visible_parameter_annotations);
done:
    return ret;
}

static int clsld_attribute_info_runtime_invisible_parameter_annotations( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_runtime_invisible_parameter_annotations_t **out)
{
    int ret = 0;
    cha_attribute_info_runtime_invisible_parameter_annotations_t *new_attribute_info_runtime_invisible_parameter_annotations = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_runtime_invisible_parameter_annotations);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_runtime_invisible_parameter_annotations->num_parameters, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_parameter_annotation(
	err,io,
	&new_attribute_info_runtime_invisible_parameter_annotations->parameter_annotations,
	new_attribute_info_runtime_invisible_parameter_annotations->num_parameters
      ));

    *out = new_attribute_info_runtime_invisible_parameter_annotations;

    goto done;
fail:
    cha_attribute_info_runtime_invisible_parameter_annotations_destroy(new_attribute_info_runtime_invisible_parameter_annotations);
done:
    return ret;
}

static int clsld_attribute_info_annotation_default( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_annotation_default_t **out)
{
    int ret = 0;
    cha_attribute_info_annotation_default_t *new_attribute_info_annotation_default = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_annotation_default);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_element_value(
	err,io,
	&new_attribute_info_annotation_default->default_value,
	1
      ));

    *out = new_attribute_info_annotation_default;

    goto done;
fail:
    cha_attribute_info_annotation_default_destroy(new_attribute_info_annotation_default);
done:
    return ret;
}

static int clsld_attribute_info_bootstrap_method( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_bootstrap_method_t **out, \
        size_t count)
{
    int ret = 0;
    cha_attribute_info_bootstrap_method_t *new_attribute_info_bootstrap_method = NULL;
    size_t i;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info_bootstrap_method, count);

    for(i = 0; i<count; i++){
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_bootstrap_method[i].bootstrap_method_ref, io);
      CHA_CLSLD_READ_CHECK_u2(break, new_attribute_info_bootstrap_method[i].num_bootstrap_arguments, io);

      new_attribute_info_bootstrap_method[i].bootstrap_arguments = (u2*)malloc(
	new_attribute_info_bootstrap_method[i].num_bootstrap_arguments * sizeof(u2));
      CHA_CLSLD_READ_CHECK_bytes(
	goto fail,
	new_attribute_info_bootstrap_method[i].bootstrap_arguments,
	(new_attribute_info_bootstrap_method[i].num_bootstrap_arguments * sizeof(u2)),
	io);
    }
    if(ret != 0) {goto fail;}

    *out = new_attribute_info_bootstrap_method;

    goto done;
fail:
    cha_attribute_info_bootstrap_method_destroy(new_attribute_info_bootstrap_method, count);
done:
    return ret;
}

static int clsld_attribute_info_bootstrap_methods( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_bootstrap_methods_t **out)
{
    int ret = 0;
    cha_attribute_info_bootstrap_methods_t *new_attribute_info_bootstrap_methods = NULL;

    CHA_CLSLD_NEW(goto fail, attribute_info_bootstrap_methods);

    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_bootstrap_methods->num_bootstrap_methods, io);
    
    CHA_CLSLD_CALL_CHECK(
      goto fail,
      clsld_attribute_info_bootstrap_method(
	err,io,
	&new_attribute_info_bootstrap_methods->bootstrap_methods,
	new_attribute_info_bootstrap_methods->num_bootstrap_methods
      ));

    *out = new_attribute_info_bootstrap_methods;

    goto done;
fail:
    cha_attribute_info_bootstrap_methods_destroy(new_attribute_info_bootstrap_methods);
done:
    return ret;
}



#define CLSLD_ATTRIBUTES_INFO_IN_CASE(str) \
	if(clsld_classfile_match_utf8( \
	  cp_info, cp_count, \
	  new_attribute_info[i].attributes_name_index, \
	  str, strlen(str)))
	  
int clsld_attributes_info( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_t *cp_info, u2 cp_count, \
        cha_attribute_info_t **attribute_info_out, \
        size_t attributes_info_count)
{
    int ret = 0;
    cha_attribute_info_t *new_attribute_info = NULL;
    size_t i;
    char *info_begin;

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info, attributes_info_count);

    for (i = 0; i != attributes_info_count; i++)
    {
        new_attribute_info[i].attributes_name_index = io_read_u2(io);
        new_attribute_info[i].attributes_length = io_read_u4(io);
        CHA_CLSLD_CHECK_IO(break);
        
        info_begin = io->ptr_cur;
        
        CLSLD_ATTRIBUTES_INFO_IN_CASE("ConstantValue"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_ConstantValue;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("Code"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Code;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("StackMapTable"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_StackMapTable;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("Exceptions"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Exceptions;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("InnerClasses"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_InnerClasses;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("EnclosingMethod"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_EnclosingMethod;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("Synthetic"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Synthetic;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("Signature"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Signature;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("SourceFile"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_SourceFile;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("SourceDebugExtension"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_SourceDebugExtension;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("LineNumberTable"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_LineNumberTable;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("LocalVariableTable"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_LocalVariableTable;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("LocalVariableTypeTable"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_LocalVariableTypeTable;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("Deprecated"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Deprecated;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("RuntimeVisibleAnnotations"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_RuntimeVisibleAnnotations;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("RuntimeInvisibleAnnotations"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_RuntimeInvisibleAnnotations;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("RuntimeVisibleParameterAnnotations"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_RuntimeVisibleParameterAnnotations;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("RuntimeInvisibleParameterAnnotations"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_RuntimeInisibleParameterAnnotations;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("AnnotationDefault"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_AnnotationDefault;
        }
        else CLSLD_ATTRIBUTES_INFO_IN_CASE("BootstrapMethods"){
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_BootstrapMethods;
        }else{
          new_attribute_info[i].attributes_type_ = ATTRIBUTE_TYPE_Unknow;
        }
        
        
        switch(new_attribute_info[i].attributes_type_){
          case ATTRIBUTE_TYPE_ConstantValue:
            ret = clsld_attribute_info_constantvalue(err, io, &(new_attribute_info[i].info.constantvalue_part));
            break;
          case ATTRIBUTE_TYPE_Code:
            ret = clsld_attribute_info_code(err, io, cp_info, cp_count, &(new_attribute_info[i].info.code_part));
            break;
          case ATTRIBUTE_TYPE_StackMapTable:
            ret = clsld_attribute_info_stack_map_table(err, io, &(new_attribute_info[i].info.stack_map_table_part));
            break;
          case ATTRIBUTE_TYPE_Exceptions:
            ret = clsld_attribute_info_exceptions(err, io, &(new_attribute_info[i].info.exceptions_part));
            break;
          case ATTRIBUTE_TYPE_InnerClasses:
            ret = clsld_attribute_info_inner_classes(err, io, &(new_attribute_info[i].info.inner_classes_part));
            break;
          case ATTRIBUTE_TYPE_EnclosingMethod:
            ret = clsld_attribute_info_enclosing_method(err, io, &(new_attribute_info[i].info.enclosing_method_part));
            break;
          case ATTRIBUTE_TYPE_Synthetic:
            /* empty */
            break;
          case ATTRIBUTE_TYPE_Signature:
            ret = clsld_attribute_info_signature(err, io, &(new_attribute_info[i].info.signature_part));
            break;
          case ATTRIBUTE_TYPE_SourceFile:
            ret = clsld_attribute_info_source_file(err, io, &(new_attribute_info[i].info.source_file_part));
            break;
          case ATTRIBUTE_TYPE_SourceDebugExtension:
            ret = clsld_attribute_info_source_debug_extension(
              err, io, 
              &(new_attribute_info[i].info.source_debug_extension_part),
              new_attribute_info[i].attributes_length);
            break;
          case ATTRIBUTE_TYPE_LineNumberTable:
            ret = clsld_attribute_info_line_number_table(err, io, &(new_attribute_info[i].info.line_number_table_part));
            break;
          case ATTRIBUTE_TYPE_LocalVariableTable:
            ret = clsld_attribute_info_local_variable_table(
              err, io, &(new_attribute_info[i].info.local_variable_table_part));
            break;
          case ATTRIBUTE_TYPE_LocalVariableTypeTable:
            ret = clsld_attribute_info_local_variable_type_table(
              err, io, &(new_attribute_info[i].info.local_variable_type_table_part));
            break;
          case ATTRIBUTE_TYPE_Deprecated:
            /* empty */
            break;
          case ATTRIBUTE_TYPE_RuntimeVisibleAnnotations:
            ret = clsld_attribute_info_runtime_visible_annotations(
              err, io, &(new_attribute_info[i].info.runtime_visible_annotations_part));
            break;
          case ATTRIBUTE_TYPE_RuntimeInvisibleAnnotations:
            ret = clsld_attribute_info_runtime_invisible_annotations(
              err, io, &(new_attribute_info[i].info.runtime_invisible_annotations_part));
            break;
          case ATTRIBUTE_TYPE_RuntimeVisibleParameterAnnotations:
            ret = clsld_attribute_info_runtime_visible_parameter_annotations(
              err, io, &(new_attribute_info[i].info.runtime_visible_parameter_annotations_part));
            break;
          case ATTRIBUTE_TYPE_RuntimeInisibleParameterAnnotations:
            ret = clsld_attribute_info_runtime_invisible_parameter_annotations(
              err, io, &(new_attribute_info[i].info.runtime_invisible_parameter_annotations_part));
            break;
          case ATTRIBUTE_TYPE_AnnotationDefault:
            ret = clsld_attribute_info_annotation_default(
              err, io, &(new_attribute_info[i].info.annotation_default_part));
            break;
          case ATTRIBUTE_TYPE_BootstrapMethods:
            ret = clsld_attribute_info_bootstrap_methods(
              err, io, &(new_attribute_info[i].info.bootstrap_methods_part));
            break;
          default:
            /* undefined attributes */
            /* ignore */
            io->ptr_cur += new_attribute_info[i].attributes_length;
        }
	
      if(ret != 0) {break;}
      if(io->ptr_cur - info_begin != new_attribute_info[i].attributes_length){
	CHA_ERR_UPDATE_CORRUPTED_CLASS(err);
	break;
      }
    }
    if(ret != 0) {goto fail;}

    *attribute_info_out = new_attribute_info;

    goto done;
fail:
    cha_attribute_info_destroy(new_attribute_info, attributes_info_count);
done:
    return ret;
}
#undef CLSLD_ATTRIBUTES_INFO_IN_CASE

static int clsld_classfile_verbose_cp_info(size_t indentation, cha_cp_info_t *constant_pool_info)
{
  size_t i;
  
  for(i=0; i<indentation; i++) {printf(" ");}
  
  switch (constant_pool_info->tag)
  {
    case CONSTANT_Unknown:
        printf("Unknown\n");
        break;
    case CONSTANT_Class:
        printf("Class              #%u\n", \
                constant_pool_info->info.class_part->name_index);
        break;
    case CONSTANT_Fieldref:
        printf("Fieldref           #%u.#%u\n", \
                constant_pool_info->info.ref_part->class_index, \
                constant_pool_info->info.ref_part->name_and_type_index);
        break;
    case CONSTANT_Methodref:
        printf("Methodref          #%u.#%u\n", \
                constant_pool_info->info.ref_part->class_index, \
                constant_pool_info->info.ref_part->name_and_type_index);
        break;
    case CONSTANT_InterfaceMethodref:
        printf("InterfaceMethodref #%u.#%u\n", \
                constant_pool_info->info.ref_part->class_index, \
                constant_pool_info->info.ref_part->name_and_type_index);
        break;
    case CONSTANT_String:
        printf("String             #%u\n", \
                constant_pool_info->info.string_part->index);
        break;
    case CONSTANT_Integer:
        printf("Integer            #%u\n", \
                constant_pool_info->info.integer_part->bytes);
        break;
    case CONSTANT_Float:
        printf("Float              #%u\n", \
                constant_pool_info->info.float_part->bytes);
        break;
    case CONSTANT_Long:
        printf("Long               #%u.#%u\n", \
                constant_pool_info->info.long_part->high_bytes, \
                constant_pool_info->info.long_part->low_bytes);
        break;
    case CONSTANT_Double:
        printf("Double             #%u.#%u\n", \
                constant_pool_info->info.double_part->high_bytes, \
                constant_pool_info->info.double_part->low_bytes);
        break;
    case CONSTANT_NameAndType:
        printf("NameAndType        #%u:#%u\n", \
                constant_pool_info->info.name_and_type_part->name_index, \
                constant_pool_info->info.name_and_type_part->descriptor_index);
        break;
    case CONSTANT_Utf8:
        printf("Utf8               ");
        fwrite(constant_pool_info->info.utf8_part->bytes, \
                constant_pool_info->info.utf8_part->length, \
                1, \
                stdout);
        printf("\n");
        break;
    case CONSTANT_MethodHandle:
        printf("MethodHandle       #%u.#%u\n", \
                constant_pool_info->info.method_handle_part->reference_kind, \
                constant_pool_info->info.method_handle_part->reference_index);
        break;
    case CONSTANT_MethodType:
        printf("MethodType         #%u\n", \
                constant_pool_info->info.method_type_part->descriptor_index);
        break;
    case CONSTANT_InvokeDynamic:
        printf("InvokeDynamic      #%u.#%u\n", \
                constant_pool_info->info.invoke_dynamic_part->bootstrap_method_attr_index, \
                constant_pool_info->info.invoke_dynamic_part->name_and_type_index);
        break;
    default:
        printf("\n");
        break;
  }
  return 0;
}


int clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
  size_t indentation, 
  cha_attribute_info_stack_map_frame_verification_t *attribute_info_stack_map_frame_verification)
{
  size_t i;
  
  for(i=0; i<indentation; i++) {printf(" ");}
  
  switch(attribute_info_stack_map_frame_verification->tag){
    case ITEM_Top:
      printf("ITEM_Top\n");
      break;
    case ITEM_Integer:
      printf("ITEM_Integer\n");
      break;
    case ITEM_Float:
      printf("ITEM_Float\n");
      break;
    case ITEM_Double:
      printf("ITEM_Double\n");
      break;
    case ITEM_Long:
      printf("ITEM_Long\n");
      break;
    case ITEM_Null:
      printf("ITEM_Null\n");
      break;
    case ITEM_UninitializedThis:
      printf("ITEM_UninitializedThis\n");
      break;
    case ITEM_Object:
      printf(
        "ITEM_Object #%u\n",
        attribute_info_stack_map_frame_verification->info.object_variable_part.cpool_index);
      break;
    case ITEM_Uninitialized:
      printf(
        "ITEM_Uninitialized %u\n",
        attribute_info_stack_map_frame_verification->info.uninitialized_variable_part.offset);
      break;
    case ITEM_Unknow:
    default:
      printf("ITEM_Unknow\n");
      break;
  }
  
  return 0;
}


int clsld_classfile_vervose_attribute_info_stack_map_frame(size_t indentation, 
                                                           cha_attribute_info_stack_map_frame_t *attribute_info_stack_map_frame)
{
  size_t i;
  u2 loop_num;
  
  u1 frame_type = attribute_info_stack_map_frame->frame_type;
  if(/* FRAME_TYPE_SAME_Low <= frame_type ###always_true###  && */frame_type <= FRAME_TYPE_SAME_Height)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("SAME\n");
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("offset_delta: %u\n", frame_type);
  }
  else if(FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Low <= frame_type &&
    frame_type <= FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Height)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("SAME_LOCALS_1_STACK_ITEM\n");
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("offset_delta: %u\n", frame_type-64);
    
    clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
      indentation,
      attribute_info_stack_map_frame->info.same_locals_1_stack_item_frame_part.stack_1
    );
  }
  else if(frame_type == FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("SAME_LOCALS_1_STACK_ITEM_EXTENDED\n");
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf(
      "offset_delta: %u\n",
      attribute_info_stack_map_frame->info.same_locals_1_stack_item_frame_extended_part.offset_delta
    );
    
    clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
      indentation,
      attribute_info_stack_map_frame->info.same_locals_1_stack_item_frame_extended_part.stack_1
    );
  }
  else if(FRAME_TYPE_CHOP_Low <= frame_type &&
    frame_type <= FRAME_TYPE_CHOP_Height)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("CHOP[-%u]\n", 251-frame_type);
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf(
      "offset_delta: %u\n",
      attribute_info_stack_map_frame->info.chop_frame_part.offset_delta
    );
  }
  else if(frame_type == FRAME_TYPE_SAME_FRAME_EXTENDED)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("SAME_FRAME_EXTENDED\n");
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf(
      "offset_delta: %u\n",
      attribute_info_stack_map_frame->info.same_frame_extended_part.offset_delta
    );
  }
  else if(FRAME_TYPE_APPEND_Low <= frame_type &&
    frame_type <= FRAME_TYPE_APPEND_Height)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("APPEND[%u]\n", frame_type-251);
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf(
      "offset_delta: %u\n",
      attribute_info_stack_map_frame->info.append_frame_part.offset_delta
    );
    
    loop_num = (u2)(frame_type - 251);
    for(i=0; i<loop_num; i++){
      clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
        indentation,
        &attribute_info_stack_map_frame->info.append_frame_part.locals[i]
      );
    }
  }
  else if(frame_type == FRAME_TYPE_FULL_FRAME)
  {
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("FULL_FRAME\n");
    
    for(i=0; i<indentation; i++) {printf(" ");}
    printf(
      "offset_delta: %u\n",
      attribute_info_stack_map_frame->info.full_frame_part.offset_delta
    );
    
    loop_num = attribute_info_stack_map_frame->info.full_frame_part.number_of_locals;
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("locals[%u]:\n", loop_num);
    
    for(i=0; i<loop_num; i++){
      clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
        indentation+2,
        &attribute_info_stack_map_frame->info.full_frame_part.locals[i]
      );
    }
    
    loop_num = attribute_info_stack_map_frame->info.full_frame_part.number_of_stack_items;
    for(i=0; i<indentation; i++) {printf(" ");}
    printf("stack[%u]:\n", loop_num);
    
    for(i=0; i<loop_num; i++){
      clsld_classfile_vervose_attribute_info_stack_map_frame_verification(
        indentation+2,
        &attribute_info_stack_map_frame->info.full_frame_part.stack[i]
      );
    }
  }
  else
  {}
  return 0;
}

int clsld_classfile_verbose_attribute_info(size_t indentation, cha_class_file_t *class_file, cha_attribute_info_t *attribute_info){
  
  cha_cp_info_t *cp_info = class_file->constant_pool;
  u2 cp_index;
  size_t i;
  size_t p;
  u4 code_length;
  u2 table_length;
  cha_attribute_info_t *attributes;
  cha_attribute_info_code_exception_table_t *exception_table;
  cha_attribute_info_stack_map_frame_t *stack_map_frame;
  cha_attribute_info_line_number_t *line_number_table;
  u2 *exception_index_table;
  
  for(i=0; i<indentation; i++) {printf(" ");}
  cp_index = attribute_info->attributes_name_index;
  fwrite(cp_info[cp_index-1].info.utf8_part->bytes, \
        cp_info[cp_index-1].info.utf8_part->length, \
        1, \
        stdout);
  printf(":\n");
  indentation += 2;
  
  switch(attribute_info->attributes_type_)
  {
    case ATTRIBUTE_TYPE_ConstantValue:
      cp_index = attribute_info->info.constantvalue_part->constantvalue_index;
      clsld_classfile_verbose_cp_info(indentation, &cp_info[cp_index-1]);
      break;
      
    case ATTRIBUTE_TYPE_Code:
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("max_stack: %u\n", attribute_info->info.code_part->max_stack);
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("max_locals: %u\n", attribute_info->info.code_part->max_locals);
      
      /* Code */
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("Code[%u]:\n", attribute_info->info.code_part->code_length);
      for(i=0; i<attribute_info->info.code_part->code_length; i+=code_length)
      {
        clsld_classfile_verbose_instruction(
          indentation+2,
          &attribute_info->info.code_part->code[i],
          &code_length
        );
      }
      
      /* Exception */
      table_length = attribute_info->info.code_part->exception_table_length;
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("exception_table[%u]:\n", table_length);
      
      exception_table = attribute_info->info.code_part->exception_table;
      for(p=0; p<table_length; p++)
      {
        for(i=0; i<indentation; i++) {printf(" ");}
        printf("%u: start_pc:%u, end_pc:%u, handler_pc:%u \n", (uint)p, \
                exception_table[p].start_pc, \
                exception_table[p].end_pc, \
                exception_table[p].handler_pc);
        
        for(i=0; i<indentation; i++) {printf(" ");}
        printf("catch_type:\n");
        cp_index = exception_table[p].catch_type;
        clsld_classfile_verbose_cp_info(indentation+2, &cp_info[cp_index-1]);
      }
      
      /* Attributes */
      table_length = attribute_info->info.code_part->attributes_count;
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("attributes[%u]:\n", table_length);
      
      attributes = attribute_info->info.code_part->attributes;
      for(p=0; p<table_length; p++)
      {
        clsld_classfile_verbose_attribute_info(indentation+2, class_file, &attributes[p]);
      }
      break;
      
    case ATTRIBUTE_TYPE_StackMapTable:
      table_length = attribute_info->info.stack_map_table_part->number_of_entries;
      stack_map_frame = attribute_info->info.stack_map_table_part->entries;
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("entries[%u]:\n", table_length);
      for(p=0; p<table_length; p++)
      {
        for(i=0; i<indentation; i++) {printf(" ");}
        clsld_classfile_vervose_attribute_info_stack_map_frame(
          indentation+2,
          &stack_map_frame[p]);
      }
      break;
      
    case ATTRIBUTE_TYPE_Exceptions:
      table_length = attribute_info->info.exceptions_part->number_of_exceptions;
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("exception_index_table[%u]:\n", table_length);
      
      exception_index_table = attribute_info->info.exceptions_part->exception_index_table;
      for(p=0; p<table_length; p++)
      {
        cp_index = exception_index_table[p];
        clsld_classfile_verbose_cp_info(indentation+2, &cp_info[cp_index-1]);
      }
      break;
      
#if 0
    case ATTRIBUTE_TYPE_InnerClasses:
      ret = clsld_attribute_info_inner_classes(err, io, &(new_attribute_info[i].info.inner_classes_part));
      break;
    case ATTRIBUTE_TYPE_EnclosingMethod:
      ret = clsld_attribute_info_enclosing_method(err, io, &(new_attribute_info[i].info.enclosing_method_part));
      break;
    case ATTRIBUTE_TYPE_Synthetic:
      /* empty */
      break;
    case ATTRIBUTE_TYPE_Signature:
      ret = clsld_attribute_info_signature(err, io, &(new_attribute_info[i].info.signature_part));
      break;
#endif
    case ATTRIBUTE_TYPE_SourceFile:
      cp_index = attribute_info->info.source_file_part->sourcefile_index;
      clsld_classfile_verbose_cp_info(indentation, &cp_info[cp_index-1]);
      break;
      
#if 0
    case ATTRIBUTE_TYPE_SourceDebugExtension:
      ret = clsld_attribute_info_source_debug_extension(
        err, io, 
        &(new_attribute_info[i].info.source_debug_extension_part),
        new_attribute_info[i].attributes_length);
      break;
#endif
    case ATTRIBUTE_TYPE_LineNumberTable:
      table_length = attribute_info->info.line_number_table_part->line_number_table_length;
      line_number_table = attribute_info->info.line_number_table_part->line_number_table;
      
      for(i=0; i<indentation; i++) {printf(" ");}
      printf("line_number_table[%u]:\n", table_length);
      
      for(p=0; p<table_length; p++)
      {
        for(i=0; i<indentation; i++) {printf(" ");}
        printf("start_pc: %u, line_number: %u\n",
               line_number_table[p].start_pc,
               line_number_table[p].line_number
        );
      }
      
      break;
      
#if 0
    case ATTRIBUTE_TYPE_LocalVariableTable:
      ret = clsld_attribute_info_local_variable_table(
        err, io, &(new_attribute_info[i].info.local_variable_table_part));
      break;
    case ATTRIBUTE_TYPE_LocalVariableTypeTable:
      ret = clsld_attribute_info_local_variable_type_table(
        err, io, &(new_attribute_info[i].info.local_variable_type_table_part));
      break;
    case ATTRIBUTE_TYPE_Deprecated:
      /* empty */
      break;
    case ATTRIBUTE_TYPE_RuntimeVisibleAnnotations:
      ret = clsld_attribute_info_runtime_visible_annotations(
        err, io, &(new_attribute_info[i].info.runtime_visible_annotations_part));
      break;
    case ATTRIBUTE_TYPE_RuntimeInvisibleAnnotations:
      ret = clsld_attribute_info_runtime_invisible_annotations(
        err, io, &(new_attribute_info[i].info.runtime_invisible_annotations_part));
      break;
    case ATTRIBUTE_TYPE_RuntimeVisibleParameterAnnotations:
      ret = clsld_attribute_info_runtime_visible_parameter_annotations(
        err, io, &(new_attribute_info[i].info.runtime_visible_parameter_annotations_part));
      break;
    case ATTRIBUTE_TYPE_RuntimeInisibleParameterAnnotations:
      ret = clsld_attribute_info_runtime_invisible_parameter_annotations(
        err, io, &(new_attribute_info[i].info.runtime_invisible_parameter_annotations_part));
      break;
    case ATTRIBUTE_TYPE_AnnotationDefault:
      ret = clsld_attribute_info_annotation_default(
        err, io, &(new_attribute_info[i].info.annotation_default_part));
      break;
    case ATTRIBUTE_TYPE_BootstrapMethods:
      ret = clsld_attribute_info_bootstrap_methods(
        err, io, &(new_attribute_info[i].info.bootstrap_methods_part));
      break;
    default:
      /* undefined attributes */
      /* ignore */
      io->ptr_cur += new_attribute_info[i].attributes_length;
#endif
  }
  return 0;
}
