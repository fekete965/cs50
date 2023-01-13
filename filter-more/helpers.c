#include "helpers.h"
#include "math.h"

// Constant matrix-x for edge calculation
const int GX[3][3] =
{
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1},
};

// Constant matrix-y for edge calculation
const int GY[3][3] =
{
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1},
};

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the colors
            int avg = roundf((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0f);
            // Assign correct values
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
            // Get the color of the current pixel
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            // Parform the calculation on each color
            float sepiaRed = .393f * originalRed + .769f * originalGreen + .189f * originalBlue;
            float sepiaGreen = .349f * originalRed + .686f * originalGreen + .168f * originalBlue;
            float sepiaBlue = .272f * originalRed + .534f * originalGreen + .131f * originalBlue;

            // Round and cap then assign the values
            image[i][j].rgbtRed = roundf(fmin(255l, sepiaRed));
            image[i][j].rgbtGreen = roundf(fmin(255l, sepiaGreen));
            image[i][j].rgbtBlue = roundf(fmin(255l, sepiaBlue));
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Check if the width is odd or even
    int isOdd = width % 2 == 1;
    int calculatedWidth = width;

    // If it is odd, increase the calculatedWidth by 1
    if (isOdd)
    {
        calculatedWidth++;
    }
    
    // Loop on the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < calculatedWidth / 2; j++)
        {
            // Save the color in a temp value
            BYTE tempRed = image[i][j].rgbtRed;
            BYTE tempGreen = image[i][j].rgbtGreen;
            BYTE tempBlue = image[i][j].rgbtBlue;

            // Calculcate the correct right index
            int rightIndex = width - j - 1;
            
            // Swap the pixel on the left with the pixel on the right
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

    // Add up the each color value
    for (int i = 0; i < width; i++)
    {
        red += array[i].rgbtRed;
        green += array[i].rgbtGreen;
        blue += array[i].rgbtBlue;
    }

    // Average the colors then round the result
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

// Calculate the edge value for each pixels
// Calculate the edge value for each pixels
RGBTRIPLE calculateColorEdgeValues(RGBTRIPLE **array)
{
    // Initialise variable
    int redX = 0, redY = 0, greenX = 0, greenY = 0, blueX = 0, blueY = 0;
    // Start the loop on our color matrix
    for (int i = 0; i < 3; i++)
    {
        // Get the correct row
        RGBTRIPLE *currentRow = array[i];

        for (int j = 0; j < 3; j++)
        {
            // Get the current elements
            RGBTRIPLE element = currentRow[j];

            // Perform the calculation based on the matrix at the top of the file
            redX += element.rgbtRed * GX[i][j];
            redY += element.rgbtRed * GY[i][j];

            greenX += element.rgbtGreen * GX[i][j];
            greenY += element.rgbtGreen * GY[i][j];

            blueX += element.rgbtBlue * GX[i][j];
            blueY += element.rgbtBlue * GY[i][j];
        }
    }

    // Power the values to 2 then take the square root and finnally round it
    double redResult = round(sqrt(pow(redX, 2) + pow(redY, 2)));
    double greenResult = round(sqrt(pow(greenX, 2) + pow(greenY, 2)));
    double blueResult = round(sqrt(pow(blueX, 2) + pow(blueY, 2)));

    // Save the values by capping them between 0 and 255
    RGBTRIPLE result;
    result.rgbtRed = fmax(fmin((redResult), 255), 0);
    result.rgbtGreen = fmax(fmin(greenResult, 255), 0);
    result.rgbtBlue = fmax(fmin(blueResult, 255), 0);

    return result;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // We copy the original image so we won't interact with the original data directly
    RGBTRIPLE imgBackup[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imgBackup[i][j].rgbtRed = image[i][j].rgbtRed;
            imgBackup[i][j].rgbtGreen = image[i][j].rgbtGreen;
            imgBackup[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    // Start looping through the image height
    for (int i = 0; i < height; i++)
    {
        // Start looping through the image width
        for (int j = 0; j < width; j++)
        {
            // Check if we are at the first row
            if (i == 0)
            {
                // Check if we are at the first column
                if (j == 0)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;
                    firstRow[1].rgbtRed = 0;
                    firstRow[1].rgbtGreen = 0;
                    firstRow[1].rgbtBlue = 0;
                    firstRow[2].rgbtRed = 0;
                    firstRow[2].rgbtGreen = 0;
                    firstRow[2].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0].rgbtRed = 0;
                    secondRow[0].rgbtGreen = 0;
                    secondRow[0].rgbtBlue = 0;
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0].rgbtRed = 0;
                    thirdRow[0].rgbtGreen = 0;
                    thirdRow[0].rgbtBlue = 0;
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2] = imgBackup[i + 1][j + 1];

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last column
                else if (j == width - 1)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;
                    firstRow[1].rgbtRed = 0;
                    firstRow[1].rgbtGreen = 0;
                    firstRow[1].rgbtBlue = 0;
                    firstRow[2].rgbtRed = 0;
                    firstRow[2].rgbtGreen = 0;
                    firstRow[2].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2].rgbtRed = 0;
                    secondRow[2].rgbtGreen = 0;
                    secondRow[2].rgbtBlue = 0;

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0] = imgBackup[i + 1][j - 1];
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2].rgbtRed = 0;
                    thirdRow[2].rgbtGreen = 0;
                    thirdRow[2].rgbtBlue = 0;

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;
                    firstRow[1].rgbtRed = 0;
                    firstRow[1].rgbtGreen = 0;
                    firstRow[1].rgbtBlue = 0;
                    firstRow[2].rgbtRed = 0;
                    firstRow[2].rgbtGreen = 0;
                    firstRow[2].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0] = imgBackup[i + 1][j - 1];
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2] = imgBackup[i + 1][j + 1];

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            // Check if we are at the last row
            else if (i == height - 1)
            {
                // Check if we are at the first column
                if (j == 0)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2] = imgBackup[i - 1][j + 1];

                    RGBTRIPLE secondRow[3];
                    secondRow[0].rgbtRed = 0;
                    secondRow[0].rgbtGreen = 0;
                    secondRow[0].rgbtBlue = 0;
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0].rgbtRed = 0;
                    thirdRow[0].rgbtGreen = 0;
                    thirdRow[0].rgbtBlue = 0;
                    thirdRow[1].rgbtRed = 0;
                    thirdRow[1].rgbtGreen = 0;
                    thirdRow[1].rgbtBlue = 0;
                    thirdRow[2].rgbtRed = 0;
                    thirdRow[2].rgbtGreen = 0;
                    thirdRow[2].rgbtBlue = 0;

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else if (j == width - 1)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0] = imgBackup[i - 1][j - 1];
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2].rgbtRed = 0;
                    firstRow[2].rgbtGreen = 0;
                    firstRow[2].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2].rgbtRed = 0;
                    secondRow[2].rgbtGreen = 0;
                    secondRow[2].rgbtBlue = 0;

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0].rgbtRed = 0;
                    thirdRow[0].rgbtGreen = 0;
                    thirdRow[0].rgbtBlue = 0;
                    thirdRow[1].rgbtRed = 0;
                    thirdRow[1].rgbtGreen = 0;
                    thirdRow[1].rgbtBlue = 0;
                    thirdRow[2].rgbtRed = 0;
                    thirdRow[2].rgbtGreen = 0;
                    thirdRow[2].rgbtBlue = 0;

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0] = imgBackup[i - 1][j - 1];
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2] = imgBackup[i - 1][j + 1];

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0].rgbtRed = 0;
                    thirdRow[0].rgbtGreen = 0;
                    thirdRow[0].rgbtBlue = 0;
                    thirdRow[1].rgbtRed = 0;
                    thirdRow[1].rgbtGreen = 0;
                    thirdRow[1].rgbtBlue = 0;
                    thirdRow[2].rgbtRed = 0;
                    thirdRow[2].rgbtGreen = 0;
                    thirdRow[2].rgbtBlue = 0;

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            // At this point we don't need to consider the corners only the edges
            else
            {
                // Check if we are at the first column
                if (j == 0)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2] = imgBackup[i - 1][j + 1];

                    RGBTRIPLE secondRow[3];
                    secondRow[0].rgbtRed = 0;
                    secondRow[0].rgbtGreen = 0;
                    secondRow[0].rgbtBlue = 0;
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0].rgbtRed = 0;
                    thirdRow[0].rgbtGreen = 0;
                    thirdRow[0].rgbtBlue = 0;
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2] = imgBackup[i + 1][j + 1];

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Check if we are at the last column
                else if (j == width - 1)
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0] = imgBackup[i - 1][j - 1];
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2].rgbtRed = 0;
                    firstRow[2].rgbtGreen = 0;
                    firstRow[2].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2].rgbtRed = 0;
                    secondRow[2].rgbtGreen = 0;
                    secondRow[2].rgbtBlue = 0;

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0] = imgBackup[i + 1][j - 1];
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2].rgbtRed = 0;
                    thirdRow[2].rgbtGreen = 0;
                    thirdRow[2].rgbtBlue = 0;

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                // Otherwise, we are in between 
                else
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0] = imgBackup[i - 1][j - 1];
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[2] = imgBackup[i - 1][j + 1];

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[2] = imgBackup[i][j + 1];

                    RGBTRIPLE thirdRow[3];
                    thirdRow[0] = imgBackup[i + 1][j - 1];
                    thirdRow[1] = imgBackup[i + 1][j];
                    thirdRow[2] = imgBackup[i + 1][j + 1];

                    RGBTRIPLE *temp[3];
                    temp[0] = firstRow;
                    temp[1] = secondRow;
                    temp[2] = thirdRow;

                    RGBTRIPLE result = calculateColorEdgeValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
        }
    }

    return;
}
