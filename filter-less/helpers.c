#include "helpers.h"

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
            int avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
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

            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            image[i][j].rgbtRed = (int)min(255l, sepiaRed);
            image[i][j].rgbtGreen = (int)min(255l, sepiaGreen);
            image[i][j].rgbtBlue = (int)min(255l, sepiaBlue);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            BYTE tempRed = image[i][j].rgbtRed;
            BYTE tempGreen = image[i][j].rgbtGreen;
            BYTE tempBlue = image[i][j].rgbtBlue;

            int leftIndex = width - j;
            image[i][j].rgbtRed = image[i][leftIndex].rgbtRed;
            image[i][j].rgbtGreen = image[i][leftIndex].rgbtGreen;
            image[i][j].rgbtBlue = image[i][leftIndex].rgbtBlue;

            image[i][leftIndex].rgbtRed = tempRed;
            image[i][leftIndex].rgbtGreen = tempGreen;
            image[i][leftIndex].rgbtBlue = tempBlue;
        }
    }

    return;
}

// Little helper to check if we are at the beginning of the row or line
int isFirstRowOrLine(int a)
{
    return a == 0;
}

// Little helper to check if we are at the end of the row or line
int isLastRowOrLine(int a, int b)
{
    return a == b;
}

// Helper to calculate color values of a range
RGBTRIPLE calculateColorValues(RGBTRIPLE *array)
{
    int length = sizeof(array);
    float red = 0, green = 0, blue = 0;

    for (int i = 0; i < length; i++)
    {
        red += array[i].rgbtRed;
        green += array[i].rgbtGreen;
        blue += array[i].rgbtBlue;
    }

    RGBTRIPLE result;
    result.rgbtRed = (int)(red / length);
    result.rgbtGreen = (int)(green / length);
    result.rgbtBlue = (int)(blue / length);

    return result;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Start looping through each columns
    for (int i = 0; i < height; i++)
    {
        // Start looping through each rows
        for (int j = 0; j < width; j++)
        {
            // Check if we are at the first column
            if (isFirstRowOrLine(i))
            {
                // Check if we are at the first row
                if (isFirstRowOrLine(j))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i][j],
                        image[i][j + 1],
                        image[i + 1][j],
                        image[i + 1][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last row
                else if (isLastRowOrLine(j, width))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i - 1][j],
                        image[i - 1][j + 1],
                        image[i][j],
                        image[i][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE temp[6] =
                    {
                        image[i - 1][j - 1],
                        image[i - 1][j + 1],
                        image[i][j],
                        image[i][j + 1],
                        image[i + 1][j],
                        image[i + 1][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            // Check if we are at the last column
            else if (isLastRowOrLine(i, height))
            {
                // Check if we are at the first row
                if (isFirstRowOrLine(j))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i][j - 1],
                        image[i][j],
                        image[i + 1][j - 1],
                        image[i + 1][j],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last row
                else if (isLastRowOrLine(j, width))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i - 1][j - 1],
                        image[i - 1][j],
                        image[i][j - 1],
                        image[i][j],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE temp[6] =
                    {
                        image[i - 1][j - 1],
                        image[i - 1][j],
                        image[i][j - 1],
                        image[i][j],
                        image[i - 1][j + 1],
                        image[i][j + 1],
                    };
                    // Calculate the average of the range
                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            else
            {
                RGBTRIPLE temp[9] =
                {
                    image[i - 1][j - 1],
                    image[i - 1][j],
                    image[i - 1][j + 1],
                    image[i - 1][j],
                    image[i][j],
                    image[i + 1][j],
                    image[i + 1][j - 1],
                    image[i + 1][j],
                    image[i + 1][j + 1],

                };
                // Calculate the average of the range
                RGBTRIPLE result = calculateColorValues(temp);
                image[i][j].rgbtRed = result.rgbtRed;
                image[i][j].rgbtGreen = result.rgbtGreen;
                image[i][j].rgbtBlue = result.rgbtBlue;
            }
        }
    }

    return;
}
