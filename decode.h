#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "common.h"

/* Structure to store information required to
 * decode secret file from stego image.
 */

typedef struct _DecodeInfo
{
    /* Decode  File Info */
    char *decode_fname;
    FILE *fptr_decode;
    char decode_file_extn[MAX_SECRET_FILE_EXTN];
    long decode_file_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint width, height;
    uint bpp; //bits per pixel

} DecodeInfo;


/* decode function prototype */
/*Check operation type function is executed in encode.c so no need to implement here */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/*Decode Magic String*/
Status decode_magic_String (const char* magic_string, DecodeInfo* decInfo);

/*Decode size of encode_file_extension */
Status decode_size_of_encoded_file_extn (DecodeInfo* decInfo);

/*Decode extension of encode_file*/
Status decode_extension_of_encoded_file(DecodeInfo* decInfo);

/*Decoding Secret file size*/
int decode_secret_file_size(DecodeInfo* decInfo);

/*Decoding secret file data*/
Status decode_secret_file_data (DecodeInfo* decInfo, int decode_file_size);

/*Decode byte from LSB*/
int decode_byte_from_lsb (char* img_buffer);

/*Decode Size from LSB*/
int decode_size_from_lsb (char* img_buffer);

/* my_strcmp */
int my_strcmp (char* str1, char* str2);

/*my strlen*/
int my_strlen (const char* str);

/*My strstr*/
char* my_strstr (char* str1, char* str2);
int compare(char* str1, char* str2);

#endif
