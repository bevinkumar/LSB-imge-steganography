#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    /*
       Seek to 18th byte
    */
    fseek(fptr_image, 18, SEEK_SET);

    /*
       Read the width (an int)
    */
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    /*
       Read the height (an int)
     */
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    /*
       Return image capacity
     */
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    /*
       Src Image file
     */
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    /*
       Do Error handling
     */
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    /*
       Secret file
     */
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    /*
       Do Error handling
     */
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    /* Stego Image file */
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    /* Do Error handling */
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    /* No failure return e_success */
    return e_success;
}

/*
   Reading the passed argumeents through command line
   checking the number of arguments, their extentions
   saving thearguments to respected pointers for further operation
input : arguments count, arguments
output : pointer to each argument for further operation
returns e_success on successfull checking completion else e_failure
 */

Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo  *encInfo)
{
    /* checking argument count */
    if(argc == 4 || argc == 5)
    {
        /* checking argument 2 is .bmp file or not (image file) */
        if(strstr(argv[2], ".bmp"))
        {
            /* saving file name */
            encInfo->src_image_fname = argv[2];
        }
        else
        {
            perror("strstr");
            printf("INFO : provided argument 2 is not .bmp file Invalid source file\n");
            return e_failure;
        }
        /* checking type of secret file and storing to extn_secret_file */
        if(strchr(argv[3], '.'))
        {
            strcpy(encInfo->extn_secret_file, strchr(argv[3], '.'));
            encInfo->secret_fname = argv[3];
        }
        else
        {
            printf("INFO : Invalid secret file\n");
            return e_failure;
        }
        /* checking for output filename */
        if(argc == 5)
        {
            /* cecking if user provided a valid .bmp filename */
            if(strstr(argv[4], ".bmp"))
            {
                encInfo->stego_image_fname = argv[4];
            }
            else
            {
                printf("INFO : Invalid output filename\n");
                return e_failure;
            }
        }
        else
        {
            /* default file name */
            printf("INFO : Output File not mentioned Creating output.bmp as default\n");
            encInfo->stego_image_fname =  "output.bmp";
        }
        return e_success;
    }
    else
    {
        printf("INFO : Invalid no.of arguments\n./lsb_steg: Encoding: ./lsb_stege <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }
}

/*
   doing encoding process
   calling particular function for a processs and checks its return value
   prints error for a particular operation faiilure
   prints success message for each successfull operation
inpout : all functions related to program and required arguments
output : on successful completion of all functions returns e_success
else returns error message and terminates the program
 */

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO : Open files success\n");
        if(encode_read_magic_string(encInfo->magic_string) == e_success)
        {
            printf("INFO : Read magic string data completed\n");
            if(check_capacity(encInfo) == e_success)
            {
                printf("INFO : Check capacity successfull\n");
                if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                {
                    printf("INFO : Copy bmp header successfull\n");
                    if(encode_magic_string_size(strlen(encInfo->magic_string), encInfo) == e_success)
                    {
                        printf("INFO : Encode magic string length completed\n");
                        if(encode_magic_string(encInfo->magic_string, encInfo) == e_success)
                        {
                            printf("INFO : Encode magic string completed\n");
                            if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                            {
                                printf("INFO : Encode secret file exten size completed\n");
                                if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                                {
                                    printf("INFO : Encode secret file extn completed\n");

                                    if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                                    {
                                        printf("INFO : Encode secret file size completed\n");
                                        if(encode_secret_file_data(encInfo) == e_success)
                                        {
                                            printf("INFO : Encode secret file data successfull\n");
                                            if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                            {
                                                printf("INFO : Copy remaining data successfull\n");
                                                return e_success;
                                            }
                                            else
                                            {
                                                printf("INFO : Copy remaining data failed\n");
                                                return e_failure;
                                            }
                                        }
                                        else
                                        {
                                            printf("INFO : Encode secret file data failed\n");
                                            return e_failure;
                                        }
                                    }
                                    else
                                    {
                                        printf("INFO : Encode secret file size failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("INFO : Encode secret file extn failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("INFO : Encode secret file exten size failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("INFO : Encode magic string failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("INFO : Encode magic string length failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("INFO : Check capacity failed\n");
                    return e_failure;
                }
            }
            else
            {
                printf("INFO : Copy bmp header failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("INFO : Read magic string data failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO : Open files failed\n");
        return e_failure;
    }
}

/*
   ensuring the source file contains enough size to store all datas related to scret file
   by comparing the all data size and file size
input : secret file size, secret file exn size, magic string size, length of magic string,
secret file,secret file extn, source image size
output : on successfull checking returns e_success else e_failure
 */

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(((encInfo->size_secret_file + sizeof(int) + sizeof(int) + sizeof(int) + strlen(encInfo->magic_string) + strlen(encInfo->extn_secret_file)) * 8) < get_image_size_for_bmp(encInfo->fptr_src_image))
    {
        return e_success;
    }
    else
    {
        printf("INFO : Sorce file size is insufficient to store data\n");
        return e_failure;
    }
}

/* to fet file size passed 
returns the return value of ftell which is size of file */

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/*
   function copies header file contants from source image to output image
   no change needed for header contants so directly copying using fread and fwrite functions
   before copying fseek is used to bring the file indicator back to begining position
input : sorce file pointer, output file pointer
output : retun e_success
 */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image, 0, SEEK_SET);
    char buffer[HEADER_SIZE];
    fread(buffer, HEADER_SIZE, 1, fptr_src_image);
    fwrite(buffer, HEADER_SIZE, 1, fptr_dest_image);
    return e_success;
}

/*
   reads  magic string from user and saves on structure variable
   magic string is like a password its protection for the secret content
input : magic string pointer
output : returns e_success on successfull reading else e_failure
 */

Status encode_read_magic_string(char *magic_string)
{
    printf("INFO : Enter Magic String : \n");
    if(scanf("%s", magic_string))
    {
        return e_success;
    }
    else
    {
        printf("INFO : Read from user faiiled\n");
        return e_failure;
    }
}

/*
   encodes size of the entered magic string to fetched bytes of source file
   32 bytes from source file is copied and replaces lsb of each byte with the each bit of size value
   and stores in output image
input : encode_size_to_image, size
output : returns e_success on successfull encoding
 */

Status encode_magic_string_size(long size, EncodeInfo *encInfo)
{
    if(encode_size_to_image(size, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   encodes magic string to the copy bytes from source image
   and stores in output image
input : magic string data and length, file pointer source and output
output : returns e_success
 */

Status encode_magic_string(char *magic_string,  EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;    
    }
    else
    {
        return e_failure;
    }
}

/*
   function will fetch 8 bytes of data from source image * size times
   each time the lsb of 8 bytes will be replaced by the each bit of onecharacter from given string
   writes the edited data to the output image
input : buffer, length, string, source and output pointers
output : returns e_success
 */

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_dest_image)
{
    int i, val1, val2;
    char image_buffer[eight_bytes];
    for(i = 0;i < size;++i)
    {
        val1 = fread(image_buffer, eight_bytes, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        val2 = fwrite(image_buffer, eight_bytes, 1, fptr_dest_image);
        if(val1 != val2)
        {
            printf("INFO : Error in reading and writing string\n");
            return e_failure;
        }
    }
    return e_success;
}

/*
   function replaces the lsb bit of each element of passed array with each bit of data
   8 elements passed 8 bits of data(character) is replaced
input : data, buffer
output : modified buffer
 */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;
    for(i = 7;i >= 0;--i)
    {
        image_buffer[i] = ((image_buffer[i] & 0xfe) | (((U)data >> i) & 1));
    }
}

/*
   encodes secret file extension size to copied data and stores to output image
 */

Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_size_to_image(file_size, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   function reads 32 byte from source image and saves to buffer
   which is passed to size to lsb fn and writes the modified buffer to output image
input : size, buffer
output : writes to output, return e_success
 */

Status encode_size_to_image(long file_size, EncodeInfo *encInfo)
{
    char size_buffer[thirtytwo_bytes];
    fread(size_buffer, thirtytwo_bytes, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(size_buffer, file_size, encInfo);
    fwrite(size_buffer, thirtytwo_bytes, 1, encInfo->fptr_stego_image);
    return e_success;
}

/*
   modifies the input buffer 
   each element of buffer array is modified with bits of given size
input : size, buffer
output : modified buffer
 */

Status encode_size_to_lsb(char *size_buffer, long file_size, EncodeInfo *encInfo)
{
    int i;
    for(i = 31;i >= 0;--i)
    {
        size_buffer[i] = (size_buffer[i] & 0xfe) | (((U)file_size >> i) & 1);
    }
    return e_success;
}

/*
   encodes secret file extension to output image
 */

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(encInfo->extn_secret_file, strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   encodes secret file size to output image
 */

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_size_to_image(file_size, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   encodes secret file data to output image
   fseek makes file indicator of secret file point to begining
   reads the secret data to a temperory buffer secret_data
   ehich is then replaced with lsb bit of source image bytes
input : secre_data,pointer to source and output image
output : modified data will be copied to output image
 */

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    char secret_data[encInfo->size_secret_file];
    fread(secret_data, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    if(encode_data_to_image(secret_data, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   copies remaining data of source image to output image
   without any modification 
   bcoz all needed datas are encoded already
 */

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) != 0)
    {
        fwrite(&ch, 1, 1,fptr_dest);
    }
    return e_success;
}
OperationType check_operation_type(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(!(strcmp(argv[1], "-e")))
        {
            return e_encode;
        }
        else if(!(strcmp(argv[1], "-d")))
        {
            return e_decode;
        }
        else
        {
            return e_unsupported;
        }
    }
    else
    {
        printf("INFO : Please provide arguments\n");
        return e_unsupported;
    }
}



