#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "wav.h"

// Pre-define BYTE struct
typedef uint8_t BYTE;

int check_format(WAVHEADER *header);
int get_block_size(WAVHEADER *header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        // Prompt the user
        printf("Usage: ./reverse input.wav output.wav\n");
        // Exit the application
        exit(1);
    }

    // Open input file for reading
    char *inputFileName = argv[1];
    FILE *inputPtr = fopen(inputFileName, "r");
    if (inputPtr == NULL)
    {
        // Prompt the user
        printf("Could not open the file: %s", inputFileName);
        // Exit the application
        exit(1);
    }

    // Read header into an array
    WAVHEADER *header = malloc(sizeof(WAVHEADER));
    if (header == NULL)
    {
        // Prompt the user
        printf("Could not allocate enough memory for the header");
        // Close the application
        exit(1);
    }
    fread(header, sizeof(WAVHEADER), 1, inputPtr);

    // Use check_format to ensure WAV format
    if (!check_format(header))
    {
        // Prompt the user
        printf("Input is not a WAV file.");
        // Close the application
        exit(1);
    }

    // Open output file for writing
    char *outputFileName = argv[2];
    FILE *outputPtr = fopen(outputFileName, "w");
    if (outputPtr == NULL)
    {
        // Prompt the user
        printf("Something went wrong during file opening: %s\n", outputFileName);
        // Exit the application
        exit(1);
    }

    // Write header to file
    fwrite(header, sizeof(WAVHEADER), 1, outputPtr);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(header);

    // Write reversed audio to file
    // Allocate memory for a buffer
    BYTE *dataBuffer = malloc(blockSize);
    // Check for null pointer
    if (dataBuffer == NULL)
    {
        // Prompt the user
        printf("Could not allocate enough memor for: dataBuffer");
        // Exit the application
        exit(1);
    }
    // Read audio data from the input file
    // Get the current position
    int endOfHeader = ftell(inputPtr);
    // Setup our multiplier
    int multiplier = 1;
    // Get to the end of the file
    fseek(inputPtr, -blockSize * multiplier, SEEK_END);
    // Save current Position
    int currentPosition = ftell(inputPtr);
    // Start a loop to get throught the file from the end until we get to the header
    while (currentPosition >= endOfHeader)
    {
        fread(dataBuffer, 1, blockSize, inputPtr);
        fwrite(dataBuffer, blockSize, 1, outputPtr);
        multiplier++;
        fseek(inputPtr, -blockSize * multiplier, SEEK_END);
        currentPosition = ftell(inputPtr);
    }

    // Free allocated memory
    free(header);
    free(dataBuffer);
    // Close the files
    fclose(inputPtr);
    fclose(outputPtr);
    // Close the application
    exit(0);
}

int check_format(WAVHEADER *header)
{
    if (header->format[0] == 'W' && header->format[1] == 'A' && header->format[2] == 'V' && header->format[3] == 'E')
    {
        return 1;
    }

    return 0;
}

int get_block_size(WAVHEADER *header)
{
    return header->numChannels * header->bitsPerSample / 8;
}
