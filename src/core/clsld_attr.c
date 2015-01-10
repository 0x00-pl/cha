
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "fs.h"
#include "dt.h"
#include "clsfile.h"
#include "clsld.h"
#include "err.h"

#define CHA_ERR_UPDATE_CORRUPTED_CLASS(err) \
    cha_err_update(err, CHA_ERR_CLSLD, "Error: Corrupted class file");

#define CHA_CLSLD_CHECK(handler, err_code, cond) do{ \
    if(cond){ \
      CHA_ERR_UPDATE_CORRUPTED_CLASS(err); \
      ret = err_code; \
      handler; \
    }}while(0)
    
#define CHA_CLSLD_NEW(handler, name_of_type) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_MALLOC, ((new_##name_of_type = cha_##name_of_type##_new()) == NULL))

#define CHA_CLSLD_NEW_ARR(handler, name_of_type, size) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_MALLOC, ((new_##name_of_type = cha_##name_of_type##_new(size)) == NULL))
    
#define CHA_CLSLD_CHECK_IO(handler) \
    CHA_CLSLD_CHECK(handler, CHA_ERR_CLSLD, io->reach_eof)
 
#define CHA_CLSLD_READ_CHECK_u1(handler, dest, io) do{ \
      (dest) = io_read_u1(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }while(0)
 
#define CHA_CLSLD_READ_CHECK_u2(handler, dest, io) do{ \
      (dest) = io_read_u2(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }while(0)                                              

#define CHA_CLSLD_READ_CHECK_u4(handler, dest, io) do{ \
      (dest) = io_read_u4(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }while(0)

#define CHA_CLSLD_READ_CHECK_u8(handler, dest, io) do{ \
      (dest) = io_read_u8(io); \
      CHA_CLSLD_CHECK_IO(handler); \
    }while(0)
    
#define CHA_CLSLD_READ_CHECK_bytes(handler, dest, size, io) do{ \
      io_read_bytes(io, dest, size); \
      CHA_CLSLD_CHECK_IO(handler); \
    }while(0)
    
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
    
    for(i=0; i<count; i++){
      new_attribute_info_code_exception_table[i].start_pc= io_read_u2(io);
      new_attribute_info_code_exception_table[i].end_pc= io_read_u2(io);
      new_attribute_info_code_exception_table[i].handler_pc= io_read_u2(io);
      new_attribute_info_code_exception_table[i].catch_type= io_read_u2(io);
      CHA_CLSLD_CHECK_IO(break);
    }
    CHA_CLSLD_CHECK_IO(goto fail);

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
    new_attribute_info_code->code= (u1*)malloc(new_attribute_info_code->code_length * sizeof(u1)); //TODO
    CHA_CLSLD_READ_CHECK_bytes(goto fail,
			       new_attribute_info_code->code, 
			       new_attribute_info_code->code_length,
			       io);
    
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->exception_table_length, io);
    if((ret= clsld_attribute_info_code_exception_table(err, io,
      &new_attribute_info_code->exception_table,
      new_attribute_info_code->exception_table_length))!=0) 
    {goto fail;}
      
    CHA_CLSLD_READ_CHECK_u2(goto fail, new_attribute_info_code->attributes_count, io);
    if((ret= clsld_attributes_info(err, io, cp_info, cp_count,
      &new_attribute_info_code->attributes,
      new_attribute_info_code->attributes_count))!=0)
    {goto fail;}

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
    
    for(i=0; i<count; i++){
      new_attribute_info_stack_map_frame_verification[i].tag= io_read_u1(io);
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
	  new_attribute_info_stack_map_frame_verification[i].info.object_variable_part.cpool_index= io_read_u2(io);
	  break;
	case ITEM_Uninitialized:
	  new_attribute_info_stack_map_frame_verification[i].info.uninitialized_variable_part.offset= io_read_u2(io);
	  break;
	default:
	  break;
      }
      CHA_CLSLD_CHECK_IO(break);
    }
    CHA_CLSLD_CHECK_IO(goto fail);

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
    
    for(i=0; i<count; i++){
      new_attribute_info_stack_map_frame[i].frame_type= io_read_u1(io);
      CHA_CLSLD_CHECK_IO(break);
      u1 frame_type= new_attribute_info_stack_map_frame[i].frame_type;
      if(frame_type<=FRAME_TYPE_SAME_Height){
	/* empty */
      }
      else if(FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Low<=frame_type && 
	frame_type<=FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_Height
      ){
	if((ret= clsld_attribute_info_stack_map_frame_verification(
	  err, io,
	  &new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_part.stack_1,
	  1
	))!= 0)
	{break;}
      }
      else if(frame_type==FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED){
	new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_extended_part.offset_delta= io_read_u2(io);
      
	if((ret= clsld_attribute_info_stack_map_frame_verification(
	  err, io,
	  &new_attribute_info_stack_map_frame[i].info.same_locals_1_stack_item_frame_extended_part.stack_1,
	  1
	))!= 0)
	{break;}
      }
      else if(FRAME_TYPE_CHOP_Low<=frame_type && 
	frame_type<=FRAME_TYPE_CHOP_Height
      ){
	new_attribute_info_stack_map_frame[i].info.chop_frame_part.offset_delta= io_read_u2(io);
      }
      else if(frame_type==FRAME_TYPE_SAME_FRAME_EXTENDED){
	new_attribute_info_stack_map_frame[i].info.same_frame_extended_part.offset_delta= io_read_u2(io);
      }
      else if(FRAME_TYPE_APPEND_Low<=frame_type && 
	frame_type<=FRAME_TYPE_APPEND_Height
      ){
	new_attribute_info_stack_map_frame[i].info.append_frame_part.offset_delta= io_read_u2(io);
      
	if((ret= clsld_attribute_info_stack_map_frame_verification(
	  err, io,
	  &new_attribute_info_stack_map_frame[i].info.append_frame_part.locals,
	  (size_t)(frame_type-251)
	))!= 0)
	{break;}
      }
      else if(frame_type==FRAME_TYPE_FULL_FRAME){
	new_attribute_info_stack_map_frame[i].info.full_frame_part.offset_delta= io_read_u2(io);
	
	new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_locals= io_read_u2(io);
      
	if((ret= clsld_attribute_info_stack_map_frame_verification(
	  err, io,
	  &new_attribute_info_stack_map_frame[i].info.full_frame_part.locals,
	  new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_locals
	))!= 0)
	{break;}
	
	new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_stack_items= io_read_u2(io);
      
	if((ret= clsld_attribute_info_stack_map_frame_verification(
	  err, io,
	  &new_attribute_info_stack_map_frame[i].info.full_frame_part.stack,
	  new_attribute_info_stack_map_frame[i].info.full_frame_part.number_of_stack_items
	))!= 0)
	{break;}
      }
      else{
	CHA_ERR_UPDATE_CORRUPTED_CLASS(err);
	break;
      }
      CHA_CLSLD_CHECK_IO(break);
    }
    if(ret!=0) {goto fail;}
    CHA_CLSLD_CHECK_IO(goto fail);

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
    if((ret= clsld_attribute_info_stack_map_frame(
      err, io,
      &new_attribute_info_stack_map_table->exception_table,
      new_attribute_info_stack_map_table->number_of_entries
    ))!= 0)
    {goto fail;}
      
      
    *out = new_attribute_info_stack_map_table;

    goto done;
fail:
    cha_attribute_info_stack_map_table_destroy(new_attribute_info_stack_map_table);
done:
    return ret;
}



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

    CHA_CLSLD_NEW_ARR(goto fail, attribute_info, attributes_info_count);

    for (i = 0; i != attributes_info_count; i++)
    {
        new_attribute_info[i].attributes_name_index = io_read_u2(io);
        new_attribute_info[i].attributes_length = io_read_u4(io);
	CHA_CLSLD_CHECK_IO(break);
	//TODO
	(void)cp_info;
	(void)cp_count;
/*         if ((new_attribute_info[i].info = (u1 *)malloc( \
 *                         sizeof(u1) * new_attribute_info[i].attributes_length)) == NULL)
 *         { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }
 *         io_read_bytes(io, new_attribute_info[i].info, new_attribute_info[i].attributes_length);
*/
    }
    CHA_CLSLD_CHECK_IO(goto fail);

    *attribute_info_out = new_attribute_info;

    goto done;
fail:
    cha_attribute_info_destroy(new_attribute_info, attributes_info_count);
done:
    return ret;
}
    