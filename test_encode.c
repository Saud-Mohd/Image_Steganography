#include <stdio.h>
#include "encode.h"
#include "types.h"

int main()
{
    Status status;
    OperationType type;
    EncodeInfo encInfo;
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


}
