#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

// Do not use this code initially.
// Implement this function only after individually testing encode and decode part
int main(int argc, char *argv[])
{
    Status status;
    OperationType type;
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    char *secret_file, *src_image, *stego_image;

    // Fill with sample filenames
    secret_file = "secret.txt";
    src_image = "beautiful.bmp";
    stego_image = "stego_img.bmp";

 //    // Perform encoding
 //    status = do_encoding(secret_file, src_image, stego_image);

 //    // Test open_files
 //    if (status == e_failure)
 //    {
 //    	fprintf(stderr, "ERROR: Unable to encode %s file into %s image\n", secret_file, src_image);
 //    	return 1;
 //    }
 //    else
 //    {
 //    	printf("SUCCESS: %s file is encoded in %s image\n", secret_file, stego_image);
	// return 0;
 //    }

    type = check_operation_type(argv);
    if ( type == e_encode)
    {
        status = read_and_validate_encode_args (argv, &encInfo);
        if ( status == e_success)
        {
            printf("INFO: Validation is a success\n");
            status = do_encoding (&encInfo);
            if ( status == e_success)
            {
                printf("INFO: Encoding done successfully\n");
            }
            else
            {
                printf("Error: Encoding failure\n");
            } 
        }
        // if status is e_failure
        //print error message and return main
        else 
        {
        	printf("Error: pass valid arguments\n./lsb_seg -e beautiful.bmp secret.txt stego.bmp\n");
        	return -1;
        }    
    }
    else if ( type == e_decode)
    {
    	status = read_and_validate_decode_args (argv, &decInfo);
        if ( status == e_success)
        {
            printf("INFO: Validation is a success\n");
            status = do_decoding (&decInfo);
            if ( status == e_success)
            {
                printf("INFO: Decoding done successfully\n");
            }
            else
            {
                printf("Error: Decoding failure\n");
            } 
        }
        // if status is e_failure
        //print error message and return main
        else 
        {
        	printf("Error: pass valid arguments\n./lsb_seg -e stego.bmp output.txt\n");
        	return -1;
        }  
    }

    else
    {
    	printf("Unsupported\n./lsb_seg -e beautiful.bmp secret.txt stego.bmp\n./lsb_seg -d stego.bmp decode.txt\n");
    }
    return 0;
}
