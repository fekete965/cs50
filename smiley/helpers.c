#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through the whole image matrix
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the first pixel
            RGBTRIPLE pixel = image[i][j];
            // If the pixel is black update its color
            if (pixel.rgbtBlue == 0 && pixel.rgbtGreen == 0 && pixel.rgbtRed == 0)
            {
                image[i][j].rgbtRed = 153;
                image[i][j].rgbtGreen = 50;
                image[i][j].rgbtBlue = 204;
            }
        }
    }
}
