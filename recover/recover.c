#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Pre-define BYTE struct
typedef uint8_t BYTE;

// Create constant values
const int BLOCK_SIZE = 512;
const BYTE SIGNATURE_FIRST_BYTES[] = {0xff, 0xd8, 0xff};

// Define function prototype(s)
int isValidFourthSignature(BYTE data);

int main(int argc, char *argv[])
{
    // Check argument length
    if (argc != 2)
    {
        // Prompt the user
        printf("Usage: ./recover IMAGE\n");
        // Exit the application
        exit(1);
    }

    // Try to open the file using the second argument
    char *fileName = argv[1];
    FILE *rawFile = fopen(fileName, "r");

    // Check if the file couldn't be open
    if (rawFile == NULL)
    {
        // Prompt the user
        printf("Could not open file: %s\n", fileName);
        // Exit the application
        exit(1);
    }

    // Create a buffer
    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK_SIZE);
    // Create a file pointer for the output
    FILE *output = NULL;
    // Create buffer for the file's name
    char *filename = malloc(sizeof(char) * 8);
    // Create a file counter
    int fileCounter = 0;

    // Start reading the file in 512 blocks
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, rawFile) == BLOCK_SIZE)
    {
        // Gather JPG signature flags
        int isValidByte1 = buffer[0] == SIGNATURE_FIRST_BYTES[0];
        int isValidByte2 = buffer[1] == SIGNATURE_FIRST_BYTES[1];
        int isValidByte3 = buffer[2] == SIGNATURE_FIRST_BYTES[2];
        int isValidByte4 = (buffer[3] & 0xf0) == 0xe0;
        // Check if the first 4 bytes maches the JPG's signature
        if (isValidByte1 && isValidByte2 && isValidByte3 && isValidByte4)
        {
            // Check if we already write into a file
            if (output != NULL)
            {
                // Close the opened file
                fclose(output);
                // Increase file counter
                fileCounter++;
            }
            // Assign correctly padded value to fileName
            sprintf(filename, "%03d.jpg", fileCounter);
            // Open a new file
            output = fopen(filename, "w");
            // Check for NULL pointer
            if (output == NULL)
            {
                // Prompt the user
                printf("Something went wrong during file opening");
                // Exit the applicaton
                exit(1);
            }
        }
        // Check if we have a file open to write
        if (output != NULL)
        {
            // Write the bytes into the file
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }

    // Check if the output is still open
    if (output != NULL)
    {
        // Close the file
        fclose(output);
    }
    fclose(rawFile);
    // Free allocated memories
    free(buffer);
    free(filename);
    // Exit the application
    exit(0);
}
