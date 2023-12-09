#include <stdio.h>
#include "decode.h"

/* Read and validate decode arguments
Prototype => Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
Return type => e_success if argv[2] is .bmp file and argv[3] is .txt file
			   e_failure if any of the above conditions are failed 
*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	char* ptr;
	ptr = my_strstr(argv[2], ".bmp");
	if (ptr != NULL)
	{
		//.bmp is a substring of argv[2] but not the extension
		if ( my_strcmp(ptr, ".bmp") != 0)
		{
			return e_failure;
		}
		//assigning decInfo -> stego_image_fname as argv[2]
		else
		{
			decInfo -> stego_image_fname = argv[2];
		}
	}
	//if .bmp not a substring of argv[2] and my_strstr return NULL
	else		
	{
		return e_failure;
	}
	//if argv[3] is not null that means decode file name is passed
    if (argv[3] != NULL)
    {
        decInfo->decode_fname = argv[3];
    } 
    //if stego file name not passed then assigning default value as stego.bmp
    else
    {
        decInfo->decode_fname = "decode.txt";
    } 
    //if we have reached till this point then return e_success
    return e_success;
}

/* Description: Open files
 * Inputs: Stego Image file name and Decode file
 * Output: FILE pointers for above files
 * Return Value:
 *  e_success - If all files are opened,
 *  e_failure - On file errors
 */
Status open_decode_files(DecodeInfo *decInfo)
{
	//stego image
	decInfo->fptr_stego_image = fopen (decInfo->stego_image_fname, "r");
	//validation fptr_stego_image file pointer
	if (decInfo -> fptr_stego_image == NULL)
	{
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
	}

	// decode file
    decInfo->fptr_decode = fopen(decInfo->decode_fname, "w");
    //Do Error handling
    if (decInfo->fptr_decode == NULL)
    {
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);
    	return e_failure;
    }

    //all files open 
    return e_success;
}

Status do_decoding (DecodeInfo *decInfo)
{
	//1. Open decode files
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files" );
        return e_failure;
    }

    else
    {
    	//Print success message for open decode file succcess
    	printf("INFO: Open decode files function is a success\n");
    	//2. Skip BMP header: skip 54 BYTES from stego_image.bmp
    	fseek(decInfo->fptr_stego_image,54L,SEEK_SET);
    	//3. Decode Magic string
    	if(decode_magic_String (MAGIC_STRING, decInfo) == e_success)
    	{
    		printf("INFO: STEGO IMAGE detected!!!! Decoding of Magic String is successful\n");
    		//3. Decode encode_file extension size
    		if (decode_size_of_encoded_file_extn (decInfo) == e_success)
    		{
				printf("INFO: Decoding of Encode_file extension size successful\n");
				//4. Decode encode_file extension 
				if (decode_extension_of_encoded_file(decInfo) == e_success)
				{
					printf("INFO: Decoding of Encoded_file extension is successful\n");
					//5. Decoding Secret file size
					int decode_file_size = decode_secret_file_size(decInfo);
					printf("INFO: Decoding of Secret file size is successful\n");
					//6. Decode encoded_file_data
					if (decode_secret_file_data (decInfo, decode_file_size) == e_success)
					{
						printf("INFO: Decoding of Secret file Data is successful\n");
						return e_success;
					}
					else
					{
						printf("ERROR: Decoding of Secret file Data failed\n");
						return e_failure;
					}
					/*--------Decoding of secret file data over---------------*/
				}
				else
				{
					printf("ERROR: Decoding of Encoded_file extension failed\n");
					return e_failure;
				}

    		/*----------Decoding of encode file extension over ------------*/
    		}
    		else
    		{
				printf("ERROR: Decoding of Encode_file extension size failed\n");
    			return e_failure;
    		}
    		/*-----------------Decoding of Encode file extension size over---------*/
    	}
    	else
    	{
    		printf("ERROR: Decoding of Magic String failed\n");
    		return e_failure;
    	}
    	/*----------------Decoding of Magic string over------------*/

    }
    /*-----Open decode file over----------------*/
}

/*Decode MAGIC string
Prototype => Status decode_magic_String (const char* magic_string, DecodeInfo* decInfo);
*/
Status decode_magic_String (const char* magic_string, DecodeInfo* decInfo)
{
	int decoded_val;
	for (int i = 0; i < my_strlen(magic_string); i++)
	{
		char buffer[8];
		//take 8B from stego image
		fread (buffer, 8, 1, decInfo->fptr_stego_image);
		//sending element at ith index of magic string to function along value of buffer just read
		decoded_val = decode_byte_from_lsb (buffer);
		if (decoded_val != magic_string[i])
		{
			return e_failure;
		}
	}
	//if decoded magic string 
	return e_success;
}
/*decode decode_file extension
Prototype => Status decode_size_of_decode_file_extn (DecodeInfo* decInfo);
*/
Status decode_size_of_encoded_file_extn (DecodeInfo* decInfo)
{
	//buffer => 32 because size will be int ie 4B so neeDecodeInfo* decInfod; to get 4*8B from stego image to decode
	char buffer[32];
	//read 32B from stego image
	fread (buffer, 32, 1, decInfo->fptr_stego_image);
	decode_size_from_lsb (buffer);
	return e_success;
}
/* Decode decode_file extension size
Prototype => Status decode_extension_of_decode_file(DecodeInfo* decInfo);
*/
Status decode_extension_of_encoded_file(DecodeInfo* decInfo)
{
	//size of extension will be 4 => .txt
	for (int i = 0; i < 4; i++)
	{
		char buffer[8];
		//take 8B from stego image
		fread (buffer, 8, 1, decInfo->fptr_stego_image);
		//sending element at ith index of magic string to function along value of buffer just read
		decode_byte_from_lsb (buffer);
	}
	return e_success;
} 
/* Decode Secret file size
It returns the size of secret file size that was encoded during encoding stage
*/
int decode_secret_file_size(DecodeInfo* decInfo)
{
	//buffer => 32 because size will be int ie 4B => 4*8B from stego image to decode
	char buffer[32];
	//read 32B from stego image
	fread (buffer, 32, 1, decInfo->fptr_stego_image);
	return decode_size_from_lsb (buffer);
}
/*Encode Secret file data
Prototype => Status decode_secret_file_data (DecodeInfo* decInfo, int decode_file_size)
*/
Status decode_secret_file_data (DecodeInfo* decInfo, int decode_file_size)
{
	for ( int i = 0; i < decode_file_size; i++)
	{
		char buffer[8];
		//reading 1 element of 8 bytes from fptr_stego and writing into the buffer 
		fread (buffer, 8, 1, decInfo->fptr_stego_image);
		char decode_val = decode_byte_from_lsb (buffer);
		//writing decode_val into the decode_text file 
		fwrite(&decode_val, 1, 1, decInfo->fptr_decode);
	}
	return e_success;
}

/*decode byte from LSB
Prototype => decode_byte_from_lsb (char* data, char* img_buffer);
Return => will return value of number formed from lsb of image_buffer[i] 
*/
int decode_byte_from_lsb (char* img_buffer)
{
	int decoded_val = 0;
	for (int i = 0; i < 8; i++)
	{
		//get lsb from img_buffer[i] and store in decode_value. Because we get binary value therefore we do * 2
		decoded_val = (decoded_val * 2) + (img_buffer[i] & 1);
	}
	return decoded_val;
}

/*decode size from LSB
Prototype => int decode_byte_from_lsb (char* data, char* img_buffer);
Return => will return e_success if number formed from lsb of image_buffer[i] is equal to data
		else return e_failure
*/
int decode_size_from_lsb (char* img_buffer)
{
	int decoded_val = 0;
	for (int i = 0; i < 32; i++)
	{
		//get lsb from img_buffer[i] and store in decode_value. Because we get binary value therefore we do * 2
		decoded_val = (decoded_val * 2) + (img_buffer[i] & 1);
	}
	return decoded_val;
}