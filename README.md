README file for LSB Image Steganography Project
================================================

Implementation Guidelines:-

Step 1: Implement Encoding Part
Step 2: Implement Decoding Part
Step 3: Implement main function

Implementation details:-

Step 1: Implement Encoding Part
--------------------------------
Files:
------
encode.h: Header file containing EncodeInfo structure and encode function prototypes. Avoid modifying this file.
encode.c: Source file containing encode function definitions. You need to implement functions declared in encode.h
test_encode.c: Contains main() function to test encoding part. Do not modify this file.

Steps to compile & test encoding part:
--------------------------------------
Compile:
gcc encode.c test_encode.c -o encode

Run:
./encode
This will encode secret.txt into beautiful.bmp and generate output file stego_img.bmp

Validation:
You need to validate whether encoding is successful by:
1. stego_img.bmp should be same as beautiful.bmp
2. Size of stego_img.bmp and beautiful.bmp should be same (ls -l)
3. Use tools like hexdump to dump source and stego image pixels in hex format. Then compare the binary values.

Step 2: Implement Decoding Part
--------------------------------
Files:
------
decode.h: Header file containing DecodeInfo structure and decode function prototypes. Its empty now. You need to start from scrath.
decode.c: Source file containing decide function definitions. You need to implement functions declared in decode.h
test_decode.c: Contains main() function to test decoding part. 

Steps to compile & test decoding part:
--------------------------------------
Compile:
gcc decode.c test_decode.c -o decode

Run:
./decode
This should decode retrieved_secret.txt from stego_img.bmp

Validation:
You need to validate whether decoding is successful by comparing retrieved_secret.txt with original file secret.txt


Step 3: Implement main function
--------------------------------
Files:
------
main.c: Contain main function. Should parse the cmdline arguments and invoke encode or decode function.

Compile:
gcc encode.c decode.c main.c -o steg

Run:
1. Encode secret.txt into beautiful.bmp and generate stego_img.bmp
./steg -e -s secret.txt -i beautiful.bmp -o stego_img.bmp

This should encode secret.txt into beautiful.bmp and generate output file stego_img.bmp

2. Decode secret from stego_img.bmp and save as retrieved_secret.txt
./steg -d -i stego_img.bmp -o retrieved_secret.txt

This should decode retrieved_secret.txt from stego_img.bmp
