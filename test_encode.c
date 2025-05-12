/*
Name        : K.Bevin kumar
Description : This project hides secret messages within images using the Least Significant Bit (LSB) technique. It involves embedding data in the image's pixel bits, making changes imperceptible to the human eye. The project includes encoding to hide the message and decoding to retrieve it, ensuring the original image's appearance remains unchanged.
*/
#include <stdio.h>
#include"decode.h"
#include "encode.h"
#include "types.h"
#include <string.h>

int main(int argc, char *argv[])
{
    char ret;
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if( argc == 1 )
    {
        
        printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
        return 0;
    }
    if(!strcmp(argv[1],"-e") && argc < 4)
    {
         printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
         return 0;
        
    }
    else if(!strcmp(argv[1],"-d") &&  argc < 3)
     {
        printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
        return 0;
     }
    
     // Checking encode or decode operation based on user input
    ret = check_operation_type(argc, argv);
    if(ret == e_encode)
    {
        printf("INFO : ## Encoding procedure started ##\n");

         // Call the function to validate the given areguements are correct or not
        
        if(read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
            printf("INFO : Read and validate successfull\n");
        }
        else
        {
            printf("INFO : Read and validate failed\n");
            return -1;
        }
        
        // Call the function to do the encoding
        if(do_encoding(&encInfo) == e_success)
        {
            printf("## Encoding successfull ##\n");
        }
        else
        {
            printf("## Encoding failed ##\n");
            return -1;
        }
    }
    else if(ret == e_decode)
    {
        printf("INFO : Decoding procedure started ##\n");
       
        // Call the function to validate the given areguements are correct or not
        if(read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            printf("INFO : Read and validate successfull\n");
        }
        else
        {
            printf("INFO : Read and validate failed\n");
            return -1;
        }

        // Call the function to do the decoding
        if(do_decoding(&decInfo) == e_success)
        {
            printf("## Decoding successfull ##\n");
        }
        else
        {
            printf("INFO : decoding failed\n");
            return -1;
        }
    }
    
    else
    {
        printf("INFO : No operator selected\n");
    }

    return 0;
}

