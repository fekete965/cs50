#include "helpers.h"
#include "stdio.h"
#include "math.h"

const int GX[3][3] =
{
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1},
};

const int GY[3][3] =
{
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1},
};

// Math.min implementation
float min(float a, float b)
{
    return a > b ? b : a;
}

// Math.max implementation
float max(float a, float b)
{
    return a < b ? b : a;
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

// Math.power
double power(int a, int pow)
{
    if (pow == 0)
    {
        return 1;
    }

    if (pow == 1)
    {
        return a;
    }

    return a * power(a, pow - 1);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (isFirstRowOrLine(i))
            {
                if (isFirstRowOrLine(j))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i][j],
                        image[i][j + 1],
                        image[i + 1][j],
                        image[i + 1][j + 1],
                    };

                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else if (isLastRowOrLine(j, width))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i - 1][j],
                        image[i - 1][j + 1],
                        image[i][j],
                        image[i][j + 1],
                    };

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

                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
            }
            else if (isLastRowOrLine(i, height))
            {
                if (isFirstRowOrLine(j))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i][j - 1],
                        image[i][j],
                        image[i + 1][j - 1],
                        image[i + 1][j],
                    };

                    RGBTRIPLE result = calculateColorValues(temp);
                    image[i][j].rgbtRed = result.rgbtRed;
                    image[i][j].rgbtGreen = result.rgbtGreen;
                    image[i][j].rgbtBlue = result.rgbtBlue;
                }
                else if (isLastRowOrLine(j, width))
                {
                    RGBTRIPLE temp[4] =
                    {
                        image[i - 1][j - 1],
                        image[i - 1][j],
                        image[i][j - 1],
                        image[i][j],
                    };

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

                RGBTRIPLE result = calculateColorValues(temp);
                image[i][j].rgbtRed = result.rgbtRed;
                image[i][j].rgbtGreen = result.rgbtGreen;
                image[i][j].rgbtBlue = result.rgbtBlue;
            }
        }
    }

    return;
}

// Calculate the edge value for each pixels
RGBTRIPLE calculateColorEdgeValues(RGBTRIPLE **array)
{
    // Initialise variable
    int redX = 0, redY = 0, greenX = 0, greenY = 0, blueX = 0, blueY = 0;
    // Start the loop on our color matrix
    for (int i = 0; i < 3; i++)
    {
        RGBTRIPLE *currentRow = array[i];

        for (int j = 0; j < 3; j++)
        {
            RGBTRIPLE element = currentRow[j];

            redX += (int)element.rgbtRed * GX[i][j];
            redY += (int)element.rgbtRed * GY[i][j];

            greenX += (int)element.rgbtGreen * GX[i][j];
            greenY += (int)element.rgbtGreen * GY[i][j];

            blueX += (int)element.rgbtBlue * GX[i][j];
            blueY += (int)element.rgbtBlue * GY[i][j];
        }
    }

    // Power the values to 2 then take the square root and finnally round it
    double redXResult = round(sqrt(power(redX, 2)));
    double redYResult = round(sqrt(power(redY, 2)));
    double greenXResult = round(sqrt(power(greenX, 2)));
    double greenYResult = round(sqrt(power(greenY, 2)));
    double blueXResult = round(sqrt(power(blueX, 2)));
    double blueYResult = round(sqrt(power(blueY, 2)));

    // Save the values by capping them between 0 and 255
    RGBTRIPLE result;
    result.rgbtRed = max(min((redXResult + redYResult), 255), 0);
    result.rgbtGreen = max(min((greenXResult + greenYResult), 255), 0);
    result.rgbtBlue = max(min((blueXResult + blueYResult), 255), 0);

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

    // Start looping through the dataset
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (isFirstRowOrLine(i))
            {
                if (isFirstRowOrLine(j))
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
                else if (isLastRowOrLine(j, width))
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
                else
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
            }
            else if (isLastRowOrLine(i, height))
            {
                if (isFirstRowOrLine(j))
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
                else if (isLastRowOrLine(j, width))
                {
                    RGBTRIPLE firstRow[3];
                    firstRow[0] = imgBackup[i - 1][j - 1];
                    firstRow[1] = imgBackup[i - 1][j];
                    firstRow[0].rgbtRed = 0;
                    firstRow[0].rgbtGreen = 0;
                    firstRow[0].rgbtBlue = 0;

                    RGBTRIPLE secondRow[3];
                    secondRow[0] = imgBackup[i][j - 1];
                    secondRow[1] = imgBackup[i][j];
                    secondRow[0].rgbtRed = 0;
                    secondRow[0].rgbtGreen = 0;
                    secondRow[0].rgbtBlue = 0;

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
                    thirdRow[0] = imgBackup[i - 1][j + 1];
                    thirdRow[1] = imgBackup[i][j + 1];
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
            else
            {
                RGBTRIPLE firstRow[3];
                firstRow[0] = imgBackup[i - 1][j - 1];
                firstRow[1] = imgBackup[i - 1][j];
                firstRow[2] = imgBackup[i - 1][j + 1];

                RGBTRIPLE secondRow[3];
                secondRow[0] = imgBackup[i - 1][j];
                secondRow[1] = imgBackup[i][j];
                secondRow[2] = imgBackup[i + 1][j];

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

    return;
}
