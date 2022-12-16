// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
const int SAMPLE_SIZE = 16;

int main(int argc, char *argv[])
{
  // Check command-line arguments
  if (argc != 4)
  {
    printf("Usage: ./volume input.wav output.wav factor\n");
    return 1;
  }

  // Open files and determine scaling factor
  FILE *input = fopen(argv[1], "r");
  if (input == NULL)
  {
    printf("Could not open file.\n");
    return 1;
  }

  FILE *output = fopen(argv[2], "w");
  if (output == NULL)
  {
    printf("Could not open file.\n");
    return 1;
  }

  float factor = atof(argv[3]);

  // Copy header from input file to output file
  uint8_t headerBuffer[HEADER_SIZE];
  fread(headerBuffer, sizeof(headerBuffer), 1, input);
  fwrite(headerBuffer, sizeof(headerBuffer), 1, output);

  // Read samples from input file and write updated data to output file
  fseek(input, HEADER_SIZE, SEEK_SET);

  int16_t sampleBuffer[SAMPLE_SIZE];

  while (1)
  {
    fread(sampleBuffer, sizeof(sampleBuffer), 1, input);

    if (feof(input))
    {
      break;
    }

    for (int i = 0; i < sizeof(sampleBuffer); i++)
    {
      sampleBuffer[i] = sampleBuffer[i] * factor;
    }

    fwrite(sampleBuffer, sizeof(sampleBuffer), 1, output);
  }

  // Close files
  fclose(input);
  fclose(output);
}
