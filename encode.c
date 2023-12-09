#include <stdio.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Operation type
Prototye => OperationType check_operation_type(char *argv[]);
Return => e_encode: If argv[1] is -e
          e_decode: If argv[1] is -d
          e_unsupported: If argv[1] is anything except -e or -d
*/
OperationType check_operation_type(char *argv[])
{
    //argv[1] is -e return e_encode
    if ( my_strcmp (argv[1],"-e") == 0)
    {
        return e_encode;
    }
    else if ( my_strcmp (argv[1],"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

}
/*My strcpy*/
void my_strcpy (char* dest, char* src)
{
    while (*dest++ = *src++);
}

/* my string compare function*/
int my_strcmp (char* str1, char* str2)
{
    int indx;
    for (indx = 0; str1[indx] && str2[indx]; indx++)
    {
        if (str1[indx] < str2[indx])
        {
            return -1;
        } 

        else if (str1[indx] > str2[indx])
        {
            return 1;
        }    
    }

    /*At this point there are 3 possibilities
    1. str1 is over
    2. str2 is over
    3. both are equal
    */
    //comapring both the strings
    if (str1[indx] < str2[indx])
    {
        return -1;
    } 

    else if (str1[indx] > str2[indx])
    {
        return 1;
    }

    //both are equal
    else
    { 
        return 0;
    }        

}
int compare(char* str1, char* str2)
{
    while (*str1 && *str2)
    {
        //value at str1 addr is not equal to value at str2 addr
        if (*str1 != *str2)
        {
            return 0;
        }
        str1++;
        str2++;
    }

    //at this point it is possible that either *str1 or *str2 is '\0'. If *str2 is '\0' and we havent returned till now it means str2 is a substring of str1
    return (*str2 == '\0');
}

/*my strstr function*/
char* my_strstr (char* str1, char* str2)
{
    while (*str1)
    {
        if ( (*str1 == *str2) && compare(str1,str2) )
        {
            return str1;
        }
        //else inc str1
        str1++;
    }

    //Till this point if we havent returned it means that str2 is not a substring of str1
    return NULL;
}

/*my_strlen*/
int my_strlen (const char* str)
{
    int count = 0;
    while (str[count] != '\0')
    {
        count++;
    }
    return count;
}
/* Read and validate command line arguments
Prototype => Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);
Return => e_success when argv[2] is .bmp file and argv[3] is .txt file and if argv[4] is present then store that file in stego_file_image
          if any above condition is false then return e_failure  
*/
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char* ptr;
    //finding substring .bmp in argv[2]
    ptr = my_strstr( argv[2], ".bmp" );
    if ( ptr != NULL){
        // now comaring ptr and .bmp. If function returns non zero value that means ptr and ".txt" are not equal
        if ( my_strcmp( ptr, ".bmp") != 0)
        {
            return e_failure;
        }

        else
        {
            //storing the argv[2] file in encInfo->src_image_fname
            encInfo->src_image_fname = argv[2];
        }
    }
    //if .bmp is not present as a substring in argv[1] then throw error
    else
    {
        return e_failure;
    }        

    //finding substring .txt in argv[3]
    ptr = my_strstr( argv[3], ".txt" );
    if ( ptr != NULL){
        // now comaring ptr and .txt. If function returns non zero value that means ptr and ".txt" are not equal
        if ( my_strcmp( ptr, ".txt") != 0)
        {
            return e_failure;
        }
        else
        {
            //storing the argv[3] file in encInfo->secret_fname
            encInfo->secret_fname = argv[3];
            //assigning .txt to encInfo->secret_file_extn
            my_strcpy(encInfo->secret_file_extn,".txt");
        }
    }
    //if .txt is not present as a substring in argv[1] then throw error
    else
    {
        return e_failure;
    }     
    //if argv[4] is not null that means stego_image name is passed
    if (argv[4] != NULL)
    {
        encInfo->stego_image_fname = argv[4];
    } 
    //if stego file name not passed then assigning default value as stego.bmp
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    } 

    //if reached till here means everything is fine 
    return e_success;   
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in case of RGB color image)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 * bpp is at offset 28, size is 2 bytes
 */
long get_image_size_for_bmp(EncodeInfo *encInfo)
{
    FILE *fptr_src_image = encInfo->fptr_src_image; // Copy to local var. Just to shorten the code
    uint width, height;
    unsigned short bpp;
    long img_size;

    // Seek to 18th byte
    fseek(fptr_src_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_src_image);
    // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_src_image);
    // printf("height = %u\n", height);

    // Get bits per pixel
    fseek(fptr_src_image, 28, SEEK_SET);
    fread(&bpp, sizeof(short), 1, fptr_src_image);
    // printf("bpp = %hu\n", bpp);

    // Store values in encInfo
    encInfo->width = width;
    encInfo->height = height;
    encInfo->bpp = bpp;

    img_size = width * height * bpp / 8; // Calculate image size
    // printf("%ld\n", img_size);
    return img_size;
}

/* Description: Open files
 * Inputs: Source image filename, Secret filename and Stego Image file name
 * Output: FILE pointers for above files
 * Return Value:
 *  e_success - If all files are opened,
 *  e_failure - On file errors
 */
Status open_encode_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // All files opened, return e_success
    return e_success;

}

Status do_encoding (EncodeInfo* encInfo)
{
    //1. Open encode files
    if (open_encode_files(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files" );
        return e_failure;
    }
    else
    {
        printf("INFO: %s function completed Successfully\n", "open_files" );
        //2. Check image capacity
        if (check_capacity (encInfo) == e_success)
        {
            printf("INFO: Check Capacity is a Success\n");
            //3. Copy BMP Header
            if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                printf("INFO: BMP header copy is a Success\n");
                //4. Encode Magic String
                if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("INFO: Magic String Encoding successful\n");
                    //5. Encode secret file extension size
                    if (encode_size_of_secret_file_extension(MAX_SECRET_FILE_EXTN, encInfo) == e_success)
                    {
                        printf("INFO: Secret File Extension size encoding Successful\n");
                        //6. Encode secret file extension
                        if (encode_secret_file_extn (encInfo->secret_file_extn, encInfo) == e_success)
                        {
                            printf ("INFO: Secret File extension Encoding Successful\n");
                            //7. Encoding Secret file size
                            if (encode_secret_file_size(encInfo->secret_file_size, encInfo) == e_success)
                            {
                                printf("INFO: Secret file size Encoding successful\n");
                                //8. Encoding secret file data
                                if (encode_secret_file_data (encInfo) == e_success)
                                {
                                    printf("INFO: Secret file data Encoding successful\n");
                                    //9. Copy remaining data
                                    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("INFO: Copying remaining Image data successful\n");
                                        return e_success;

                                    }
                                    else
                                    {
                                        printf ("ERROR: Copying remaining Image data Failed\n");
                                        return e_failure;
                                    }
                                    /*----------------Copy remaining data over-----------*/

                                }
                                else
                                {
                                    printf("INFO: Secret file data Encoding failed\n");
                                    return e_failure;
                                }
                                /*-----------Secret file encoding over----------*/
                            }
                            else
                            {
                                printf("ERROR: Secret file size Encoding failed\n");
                                return e_failure;
                            }
                            /*-------------Secret file size encoding over------------*/
                        }
                        else
                        {
                            printf ("INFO: Secret File extension Encoding Failed\n");
                            return e_failure;
                        }
                        /*-------------Secret file extension encoding over------------*/

                    }
                    else
                    {
                        printf("ERROR: Secret File Extension size encoding failed\n");
                        return e_failure;
                    }
                     /*-------------Secret file extension size encoding over------------*/

                }
                else
                {
                    printf("INFO: Magic String Failed\n");
                    return e_failure;
                }
               /*-------------Magic String encoding over------------*/

            }
            else
            {
                printf("Error: Copy Image header Failed");
                return e_failure;
            }
            /*--------------------Copy image header over--------------*/

        }
        else
        {
            printf("ERROR: Insufficient capacity in Source Image\n");
            return e_failure;

        }
        /*---------------------Capacity check for source image over-------------*/
    }
   /*--------------------------Open encode file function over------------------*/ 
}


/*Check image capacity function
Prototype => Status check_capacity(EncodeInfo *encInfo);
Return => e_success if image size is >= 54 + 16 + 32 + 32 +secret_file_size*8 
          else e_failure
*/            
Status check_capacity(EncodeInfo *encInfo)
{
    //storing function return value in local variable
    uint image_size = get_image_size_for_bmp(encInfo);
    //getting size of secret file size
    encInfo->secret_file_size = get_secret_file_size (encInfo->fptr_secret);

    if (image_size >= (54 + 16 + 32 + 32 + encInfo->secret_file_size*8))
    {
        return e_success;
    }

    else
    {
        return e_failure;
    }    
}

/* get secret file size function
Prototype => get_secret_file_size (FILE* fptr_secret);
Return => Return size of Secret file size
*/
long get_secret_file_size (FILE* fptr_secret)
{
    //Use fseek ftell rewind function. Move fptr to end of file using fseek. Use ftell to get offset position which is size of file
    fseek(fptr_secret, 0L, SEEK_END);
    //now fptr_secret is at the end of file. SO ftell will give size of secret file in BYTES.
    //returning ftell(fptr_secret)
    return ftell(fptr_secret);
}

/*Copy BMP header
Prototype => Status copy_bmp_header (FILE* fptr_src_image, FILE* fptr_src_image);
*/
Status copy_bmp_header (FILE* fptr_src_image, FILE* fptr_stego_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread (buffer, 54, 1, fptr_src_image);
    fwrite (buffer, 54, 1, fptr_stego_image);
    return e_success;
}

/* Encode Magic string 
Prototype=> Status encode_magic string (char* magic_string, EncodeInfo *encInfo);
*/
Status encode_magic_string(const char* magic_string, EncodeInfo *encInfo)
{
    //calling encode_data_to_image function
    encode_data_to_image (magic_string, my_strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

/* Encode size of secret file extension
Prototype => Status encode_size_of_file_extension (int size, EncodeInfo* encInfo)
*/
Status encode_size_of_secret_file_extension (int secret_file_size, EncodeInfo* encInfo)
{
    // .txt => size is 4B => 4*8 bits
    char buffer[32];
    //reading 32B from fptr_src_image
    fread (buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb (secret_file_size, buffer);
    fwrite (buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}
/* Encode secret file extension function
Prototye: Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);
*/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image (file_extn, my_strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

/* encoding secret file size
prototype: Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);
*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{ 
    char buffer[32];
    fread (buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb (file_size, buffer);
    fwrite (buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/*encoding secret file data
Prototype => Status encode_secret_file_data(EncodeInfo *encInfo);
*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int file_size = encInfo->secret_file_size;
    //bringing fptr-> secret poniter to beginning of the secter.txt file
    rewind (encInfo->fptr_secret);
    // printf("%ld\n",ftell(encInfo -> fptr_src_image));
    for (int i = 0; i < file_size;i++)
    {
        char ch, buffer[8];
        //read 1 byte from secret file
        fread (&ch, 1, 1, encInfo->fptr_secret);
        //read 8 byte from src_img
        fread (buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch, buffer);
        fwrite (buffer, 8, 1, encInfo->fptr_stego_image);  
    }
    return e_success;
}

/* Copy remaining image bytes from src to stego image after encoding 
Prototype => Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    //Read from fptr_src
    // Write to fptr_dest
    char buffer[1];
    while (fread(buffer,1, 1, fptr_src) > 0)
    {
        fwrite (buffer, 1, 1, fptr_dest);
    }
    return e_success;
}
Status encode_data_to_image (const char* data, int size, FILE* fptr_src_image, FILE* fptr_stego_image)
{
    //running a loop till size parameter
    for (int i = 0; i < size; i++)
    {
        char buffer[8];
        //getting 8 bytes from fptr_src_image and storing in buffer
        fread (buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i],buffer);
        //writing modified buffer value to fptr_stego_image
        fwrite (buffer, 8, 1, fptr_stego_image);
    }
}

Status encode_byte_to_lsb(char data,char* img_buffer)
{
    /* assume data is # == 0x23 == 0010 0011
    0 will be put in img_buffer[0] lsb bit
    0 will be put in img_buffer[1] lsb bit
    1 will be put in img_buffer[2] lsb bit
    0 will be put in img_buffer[3] lsb bit
    0 will be put in img_buffer[4] lsb bit
    0 will be put in img_buffer[5] lsb bit
    1 will be put in img_buffer[6] lsb bit
    1 will be put in img_buffer[7] lsb bit
    */
    char lsb_bit;
    for (int i = 0; i < 8; i++)
    {
        //getting (7 - i)th bit from data and storing in img_buffer[i] lsb bit
        lsb_bit = (data >> (7-i)) & 1;
        //clearing lsb bit in img_buffer[i]
        img_buffer[i] = img_buffer[i] & ~(1);
        //setting lsb_bit value at img_buffer[i] lsb
        img_buffer[i] |= lsb_bit; 
    }
    return e_success;
}

Status encode_size_to_lsb(int size,char* img_buffer)
{
    /* assume size is 4 == 0x00 00 00 04 == 00000000 00000000 00000000 00000100
    0 will be put in img_buffer[0] lsb bit
    0 will be put in img_buffer[1] lsb bit
    0 will be put in img_buffer[2] lsb bit
    0 will be put in img_buffer[3] lsb bit
    0 will be put in img_buffer[4] lsb bit
    0 will be put in img_buffer[5] lsb bit
    0 will be put in img_buffer[6] lsb bit
    0 will be put in img_buffer[7] lsb bit
    0 will be put in img_buffer[8] lsb bit
    0 will be put in img_buffer[9] lsb bit
    0 will be put in img_buffer[10] lsb bit
    0 will be put in img_buffer[11] lsb bit
    0 will be put in img_buffer[12] lsb bit
    0 will be put in img_buffer[13] lsb bit
    0 will be put in img_buffer[14] lsb bit
    0 will be put in img_buffer[15] lsb bit
    0 will be put in img_buffer[16] lsb bit
    0 will be put in img_buffer[17] lsb bit
    0 will be put in img_buffer[18] lsb bit
    0 will be put in img_buffer[19] lsb bit
    0 will be put in img_buffer[20] lsb bit
    0 will be put in img_buffer[21] lsb bit
    0 will be put in img_buffer[22] lsb bit
    0 will be put in img_buffer[23] lsb bit
    0 will be put in img_buffer[24] lsb bit
    0 will be put in img_buffer[25] lsb bit
    0 will be put in img_buffer[26] lsb bit
    0 will be put in img_buffer[27] lsb bit
    0 will be put in img_buffer[28] lsb bit
    1 will be put in img_buffer[29] lsb bit
    0 will be put in img_buffer[30] lsb bit
    0 will be put in img_buffer[31] lsb bit
    */
    char lsb_bit;
    for (int i = 0; i < 32; i++)
    {
        //getting (7 - i)th bit from data and storing in img_buffer[i] lsb bit
        lsb_bit = (size >> (31-i)) & 1;
        //clearing lsb bit in img_buffer[i]
        img_buffer[i] = img_buffer[i] & ~(1);
        //setting lsb_bit value at img_buffer[i] lsb
        img_buffer[i] |= lsb_bit; 
    }
    return e_success;
}