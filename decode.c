#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

/*
   opens the source file and returns file indicator to fptr src image

 */

Status open_src_file(DecodeInfo *decInfo)
{
    /*
       Src Image file
     */
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    /*
       Do Error handling
     */
    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        return e_failure;
    }
    return e_success;
}

/*
   opens output file and returns file indicator to fptr output
 */

Status open_output_file(DecodeInfo *decInfo)
{
    /*
       Stego Image file
     */
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    /*
       Do Error handling
     */
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

        return e_failure;
    }

    /*
       No failure return e_success
     */
    return e_success;
}

/*
   checks and validate the number of arguments passed through command line
   stores the provided filenames into structure
   checks the given file types
input : argc, argv
output : stores the arguments to structure
 */

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo  *decInfo)
{
    if(argc == 3 || argc == 4)
    {
        if(strstr(argv[2], ".bmp"))
        {
            decInfo->src_image_fname = argv[2];
        }
        else
        {
            printf("INFO : Invalid source file\n");
            return e_failure;
        }
        if(argc == 4)
        {
            if(strchr(argv[3], '.'))
            {
                printf("INFO : Please provide output filename without extention\n");
                return e_failure;
            }
            else
            {
                /*
                   stores user given output filename
                 */
                strcpy(decInfo->output_fname, argv[3]);
            }
        }
        else
        {
            /* 
               stores a default name 
             */
            strcpy(decInfo->output_fname, "output");
        }
    }
    else
    {
        printf("INFO : Invalid no.of arguments\n");
        return e_failure;
    }
    return e_success;
}

/*
   function decodes the data calls sub functions
   calls each functions on the success of previous function
   check each operations return value and prints the status
input : all functions to do decoding
output : prints messages on success or failure of operation
 */

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_src_file(decInfo) == e_success)
    {
        printf("INFO : Open source file successfull\n");
        if(decode_magic_string_length(decInfo) == e_success)
        {
            printf("INFO : decode magic string length successfull\n");
            if(read_magic_string(decInfo->usr_magic_string) == e_success)
            {
                printf("INFO : Read magic string successfull\n");
                if(decode_magic_string_check(decInfo->magic_string_size, decInfo->magic_string, decInfo) == e_success)
                {
                    printf("INFO : decode magic string and check completed\n");
                    if(decode_secret_file_extn_size(decInfo) == e_success)
                    {
                        printf("INFO : decode secet file extn size successfull\n");
                        if(decode_secret_file_extn(decInfo->secret_extn_size, decInfo) == e_success)
                        {
                            printf("INFO : decode secret file extn completed\n");
                            if(open_output_file(decInfo) == e_success)
                            {
                                printf("INFO : open output file successful\n");
                                if(decode_secret_file_size(decInfo) == e_success)
                                {
                                    printf("INFO : decode secret file size completed\n");
                                    
                                    if(decode_secret_file_data(decInfo->size_secret_file, decInfo) == e_success)
                                    {
                                        printf("INFO : decode secret file data successfull\n");
                                        return e_success;
                                    }
                                    else
                                        printf("INFO : decode secret file data failed\n");
                                    return e_failure;
                                }
                                else
                                    printf("INFO : decode secret file size failed\n");
                                return e_failure;
                            }
                            else
                                printf("INFO : Open output file failed\n");
                            return e_failure;
                        }
                        else
                            printf("INFO : Decode secret file extn failed\n");
                        return e_failure;
                    }
                    else
                        printf("INFO : Decode secet file extn size failed\n");
                    return e_failure;
                }
                else
                    printf("INFO : Decode magic string and check failed\n");
                return e_failure;
            }
            else
                printf("INFO : Read magic string failed\n");
            return e_failure;
        }
        else
            printf("INFO : Decode magic string length failed\n");
        return e_failure;
    }
    else
        printf("INFO : Open source file failed\n");
    return e_failure;
}

/*
   function decodes magic string length from the provided image file
   fseek is used to skip the header contents 
   fetched string length is stored to the structure variable
input : fseek deckinfo
output : returns e_success on successfull storing of length
 */

Status decode_magic_string_length(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image, HEADER_SIZE, SEEK_SET);
    if(decInfo->magic_string_size = decode_image_to_size(decInfo))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   reads the magic string from user and stores to pointer
 */

Status read_magic_string(char *magic_string)
{
    printf("INFO : Enter the Magic String : \n");
    if(scanf("%s", magic_string))
    {
        return e_success;
    }
    else
    {
        printf("INFO : Error in reading magic string\n");
        return e_failure;
    }
}

/*
   function decodes the magic string encoded to the image file
   and compare with the user given string 
input : size and data of input magic string
output : return success on matching string else prints error message
 */

Status decode_magic_string_check(long size, char *magic_string,  DecodeInfo *decInfo)
{
    decode_image_to_data(magic_string, size, decInfo->fptr_src_image);
    if((strcmp(decInfo->usr_magic_string, magic_string)))
    {
        printf("INFO : Magic string not matching\n");
        return e_failure;    
    }
    else
    {
        return e_success;
    }
}

/*
   decodes the lsb of 8 bytess from source image which represents a charecter of secret data
   the fetched data is stored to passed data buffer
input : data buffer length of string
output : length * 8 bytes of data will be fetched from image and lsb is collected to form secret data
 */

Status decode_image_to_data(char *data_buffer, int size, FILE *fptr_src_image)
{
    int i;
    char data;
    char image_buffer[eight_bytes];
    for(i = 0;i < size;++i)
    {
        fread(image_buffer, eight_bytes, 1, fptr_src_image);
        data = decode_lsb_to_byte(image_buffer);
     
        data_buffer[i] = data;
    }
    data_buffer[i] = '\0';
    return e_success;
}

/*
   decodes 8 lsb bit from passed array and | with a variable wich represens 1 character of secret data
 */

int decode_lsb_to_byte(char *image_buffer)
{
    int i;
    char data = 0;
    for(i = 7;i >= 0;i--)
    {
        data = (data | ((image_buffer[i] & 0x01) << i));
    }
    return data;
}

/*
   decodes secret file extension size from source image and store to structure variable
 */

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    if(decInfo->secret_extn_size = decode_image_to_size(decInfo))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   decodes lsb 32 bytes from source image and convert to an integer
 */

int decode_image_to_size(DecodeInfo *decInfo)
{
    char size_buffer[thirtytwo_bytes];
    fread(size_buffer, thirtytwo_bytes, 1, decInfo->fptr_src_image);
    return decode_lsb_to_size(size_buffer, decInfo);
}

/*
   decodes size from lsb of given array
 */

int decode_lsb_to_size(char *size_buffer, DecodeInfo *decInfo)
{
    int i, data = 0;
    for(i = 31;i >= 0;--i)
    {
        data = (data | ((size_buffer[i] & 0x1) << i));
    }

    return data;
}

/*
   decodes secret file extn from the source file according to the given length
   stores the extn to structure variable
   merge the fetched extension with the user given file name and store to structure
input : size of  extns, data buffer
output : merge extn with filename
 */

Status decode_secret_file_extn(long extn_size, DecodeInfo *decInfo)
{
    char data_buffer[extn_size ];
    data_buffer[extn_size] = '\0';

    decode_image_to_data(data_buffer, extn_size, decInfo->fptr_src_image);
    decInfo->extn_output_file = data_buffer;
    if(data_buffer[0] == '.')
    {
        strcat(decInfo->output_fname, decInfo->extn_output_file);
        return e_success;
    }
    else
    {
        printf("INFO : Invalid extension\n");
        return e_failure;
    }
}

/*
   decodes secret file size from the source file
 */

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    if(decInfo->size_secret_file = decode_image_to_size(decInfo))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*
   decodes secret file data from source file
input : size for secret data, databuffer freed
output : reads length amount of data from source file and store to data buffer
and writes to output file
 */

Status decode_secret_file_data(long size, DecodeInfo *decInfo)
{
    char data_buffer[size];
    data_buffer[size] = '\0';
   decode_image_to_data(data_buffer, size, decInfo->fptr_src_image);
    if(fwrite(data_buffer, size,1 , decInfo->fptr_output) )
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
   
}




