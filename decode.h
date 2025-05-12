#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "common.h" //contains macros for constant

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    //Magic string
    char magic_string[20];
    long magic_string_size;
    char usr_magic_string[20];

    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;

    /* output File Info */
    char output_fname[20];
    FILE *fptr_output;
    char *extn_output_file;
	
    //secret file
    long secret_extn_size;
    long size_secret_file;

} DecodeInfo;


/* Decoding function prototype */

//opens sorce file
Status open_src_file(DecodeInfo *decInfo);

//opens output file
Status open_output_file(DecodeInfo *decInfo);

//decodes magic string and compares with user input
Status decode_magic_string_check(long size, char *magic_string, DecodeInfo *decInfo);

//decode data from sorce file
Status decode_image_to_data(char *data_buffer, int size, FILE *fptr_src_image);

//decodes lsb bit of source file data 
int decode_lsb_to_byte(char *image_buffer);

//decodes secret file extn size from source file
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

//decodes size from source image
int decode_image_to_size(DecodeInfo *decInfo);

//decodes lsb of 32 byte data to form ssize
int decode_lsb_to_size(char *size_buffer, DecodeInfo *decInfo);

//decodes output file extention
Status decode_secret_file_extn(long extn_size, DecodeInfo *decInfo);

//decodes secret file size
Status decode_secret_file_size(DecodeInfo *decInfo);

//decodes secret file data
Status decode_secret_file_data(long size, DecodeInfo *decInfo);


/* Check operation type */
OperationType check_operation_type(int argc, char *argv[]);

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

//decodes magic string length
Status decode_magic_string_length(DecodeInfo *decInfo);

//reads magic string from user
Status read_magic_string(char *magic_string);

#endif
