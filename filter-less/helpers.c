#include "helpers.h"
#include "math.h"

// Math.min implementation
float min(float a, float b)
{
    return a > b ? b : a;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = roundf((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0f);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            float sepiaRed = .393f * originalRed + .769f * originalGreen + .189f * originalBlue;
            float sepiaGreen = .349f * originalRed + .686f * originalGreen + .168f * originalBlue;
            float sepiaBlue = .272f * originalRed + .534f * originalGreen + .131f * originalBlue;

            image[i][j].rgbtRed = roundf(min(255l, sepiaRed));
            image[i][j].rgbtGreen = roundf(min(255l, sepiaGreen));
            image[i][j].rgbtBlue = roundf(min(255l, sepiaBlue));
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int isOdd = width % 2 == 1;
    int calculatedWidth = width;

    if (isOdd)
    {
        calculatedWidth++;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < calculatedWidth / 2; j++)
        {
            BYTE tempRed = image[i][j].rgbtRed;
            BYTE tempGreen = image[i][j].rgbtGreen;
            BYTE tempBlue = image[i][j].rgbtBlue;

            int rightIndex = width - j - 1;

            image[i][j].rgbtRed = image[i][rightIndex].rgbtRed;
            image[i][j].rgbtGreen = image[i][rightIndex].rgbtGreen;
            image[i][j].rgbtBlue = image[i][rightIndex].rgbtBlue;

            image[i][rightIndex].rgbtRed = tempRed;
            image[i][rightIndex].rgbtGreen = tempGreen;
            image[i][rightIndex].rgbtBlue = tempBlue;
        }
    }

    return;
}

// Helper to calculate color values of a range
RGBTRIPLE calculateColorValues(RGBTRIPLE *array, float width)
{
    int red = 0, green = 0, blue = 0;

    for (int i = 0; i < width; i++)
    {
        red += array[i].rgbtRed;
        green += array[i].rgbtGreen;
        blue += array[i].rgbtBlue;
    }

    RGBTRIPLE result;
    result.rgbtRed = round(red / width);
    result.rgbtGreen = round(green / width);
    result.rgbtBlue = round(blue / width);

    return result;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the array
    RGBTRIPLE tempArr[height][width];
    for (int i = 0; i < height; i++)
    {
        // Start looping through each rows
        for (int j = 0; j < width; j++)
        {
            tempArr[i][j] = image[i][j];
        }
    }

    // Start looping through each columns
    for (int i = 0; i < height; i++)
    {
        // Start looping through each rows
        for (int j = 0; j < width; j++)
        {
            // Check if we are at the first column
            if (i == 0)
            {
                // Check if we are at the first row
                if (j == 0)
                {
                    RGBTRIPLE temp[4] =
                    {
                        tempArr[i][j],
                        tempArr[i][j + 1],
                        tempArr[i + 1][j],
                        tempArr[i + 1][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 4.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last row
                else if (j == width - 1)
                {
                    RGBTRIPLE temp[4] =
                    {
                        tempArr[i][j - 1],
                        tempArr[i][j],
                        tempArr[i + 1][j - 1],
                        tempArr[i + 1][j],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 4.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE temp[6] =
                    {
                        tempArr[i][j - 1],
                        tempArr[i][j],
                        tempArr[i][j + 1],
                        tempArr[i + 1][j - 1],
                        tempArr[i + 1][j],
                        tempArr[i + 1][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 6.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            // Check if we are at the last column
            else if (i == height - 1)
            {
                // Check if we are at the first row
                if (j == 0)
                {
                    RGBTRIPLE temp[4] =
                    {
                        tempArr[i - 1][j],
                        tempArr[i - 1][j + 1],
                        tempArr[i][j],
                        tempArr[i][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 4.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last row
                else if (j == width - 1)
                {
                    RGBTRIPLE temp[4] =
                    {
                        tempArr[i - 1][j - 1],
                        tempArr[i - 1][j],
                        tempArr[i][j - 1],
                        tempArr[i][j],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 4.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE temp[6] =
                    {
                        tempArr[i - 1][j - 1],
                        tempArr[i - 1][j],
                        tempArr[i - 1][j + 1],
                        tempArr[i][j - 1],
                        tempArr[i][j],
                        tempArr[i][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 6.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            // At this point, we don't need to consider corners only the edges
            else
            {
                // Check if we are at the first column
                if (j == 0)
                {
                    RGBTRIPLE temp[6] =
                    {
                        tempArr[i - 1][j],
                        tempArr[i - 1][j + 1],
                        tempArr[i][j],
                        tempArr[i][j + 1],
                        tempArr[i + 1][j],
                        tempArr[i + 1][j + 1],

                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 6.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last column
                else if (j == width - 1)
                {
                    RGBTRIPLE temp[6] =
                    {
                        tempArr[i - 1][j - 1],
                        tempArr[i - 1][j],
                        tempArr[i][j - 1],
                        tempArr[i][j],
                        tempArr[i + 1][j - 1],
                        tempArr[i + 1][j],

                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 6.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE temp[9] =
                    {
                        tempArr[i - 1][j - 1],
                        tempArr[i - 1][j],
                        tempArr[i - 1][j + 1],
                        tempArr[i][j - 1],
                        tempArr[i][j],
                        tempArr[i][j + 1],
                        tempArr[i + 1][j - 1],
                        tempArr[i + 1][j],
                        tempArr[i + 1][j + 1],

                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp, 9.0);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
        }
    }

    return;
}
