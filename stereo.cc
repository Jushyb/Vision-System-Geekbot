#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imageUtils.h"
#include "matrixUtils.h"
#include "utils.h"

int main()
{
    const int imageWidth = 640;
    const int imageHeight = 480;
    const int blockWidth = 5;
    const int blockHeight = 5;

    const char* leftImagePath = "leftBW.ppm";
    const char* rightImagePath = "rightBW.ppm";
    const char* disparityImagePath = "disparity.ppm";
    const char* depthImagePath = "depth.ppm";

    unsigned char* disparityImage = (unsigned char*) malloc(imageHeight * imageWidth * sizeof(unsigned char));
    unsigned char* depthImage = (unsigned char*) malloc(imageHeight * imageWidth * sizeof(unsigned char));

    PPMImage* leftImage = readPPM(leftImagePath, 0);
    PPMImage* rightImage = readPPM(rightImagePath, 0);

//******************************************************************************************************************************************************

    // Step 1: Compute disparity image based on block average comparison
    for (int blockRow = 0; blockRow < imageHeight; blockRow += blockHeight)
    {
        for (int blockCol = 0; blockCol < imageWidth; blockCol += blockWidth)
        {
            double leftBlockSum = 0.0;
            double rightBlockSum = 0.0;
            int numPixelsInBlock = 0;

            for (int localRow = 0; localRow < blockHeight; localRow++)
            {
                for (int localCol = 0; localCol < blockWidth; localCol++)
                {
                    int pixelX = blockCol + localCol;
                    int pixelY = blockRow + localRow;
                    if (pixelX < imageWidth && pixelY < imageHeight)
                    {
                        int pixelIndex = pixelY * imageWidth + pixelX;
                        leftBlockSum += leftImage->data[pixelIndex];
                        rightBlockSum += rightImage->data[pixelIndex];
                        numPixelsInBlock++;
                    }
                }
            }

            double leftBlockAvg = leftBlockSum / numPixelsInBlock;
            double rightBlockAvg = rightBlockSum / numPixelsInBlock;

            double blendedValue = (leftBlockAvg + rightBlockAvg) / 2.0;
            double blockDifference = fabs(leftBlockAvg - rightBlockAvg);

            // Apply soft confidence weighting
            double matchConfidence = fmax(0.0, 1.0 - blockDifference / 50.0); // range [0,1], higher when pixels are closer
            unsigned char disparityPixelValue = (unsigned char)(matchConfidence * blendedValue + (1.0 - matchConfidence) * 255.0);

            //Computing the disparity value

            if (disparityPixelValue > 255) disparityPixelValue = 255;

            for (int localRow = 0; localRow < blockHeight; localRow++)
            {
                for (int localCol = 0; localCol < blockWidth; localCol++)
                {
                    int pixelX = blockCol + localCol;
                    int pixelY = blockRow + localRow;
                    if (pixelX < imageWidth && pixelY < imageHeight)
                        //Writing the result to ever pixel block
                        disparityImage[pixelY * imageWidth + pixelX] = disparityPixelValue;
                }
            }
        }
    }

//******************************************************************************************************************************************************

    // Step 2: Slight blur with center-weighting to improve depth smoothness without over-smoothing
    for (int blockRow = 0; blockRow < imageHeight; blockRow += blockHeight)
    {
        for (int blockCol = 0; blockCol < imageWidth; blockCol += blockWidth)
        {
            double weightedSum = 0.0;
            double totalWeight = 0.0;

            for (int localRow = 0; localRow < blockHeight; localRow++)
            {
                for (int localCol = 0; localCol < blockWidth; localCol++)
                {
                    int pixelX = blockCol + localCol;
                    int pixelY = blockRow + localRow;
                    if (pixelX < imageWidth && pixelY < imageHeight)
                    {
                        //center weighted averaging process
                        int pixelIndex = pixelY * imageWidth + pixelX;
                        double disparityValue = disparityImage[pixelIndex];
                        double centerWeight = 1.0 - (fabs(localCol - blockWidth / 2) + fabs(localRow - blockHeight / 2)) * 0.1;
                        if (centerWeight < 0.1) centerWeight = 0.1; // avoid zero weight
                        weightedSum += disparityValue * centerWeight;
                        totalWeight += centerWeight;
                    }
                }
            }

            unsigned char smoothedDepthValue = (unsigned char)(weightedSum / totalWeight);

            for (int localRow = 0; localRow < blockHeight; localRow++)
            {
                for (int localCol = 0; localCol < blockWidth; localCol++)
                {
                    int pixelX = blockCol + localCol;
                    int pixelY = blockRow + localRow;
                    if (pixelX < imageWidth && pixelY < imageHeight)
                        depthImage[pixelY * imageWidth + pixelX] = smoothedDepthValue;
                }
            }
        }
    }

    writePPM(disparityImagePath, imageWidth, imageHeight, 255, 0, disparityImage);
    writePPM(depthImagePath, imageWidth, imageHeight, 255, 0, depthImage);

    freePPM(leftImage);
    freePPM(rightImage);
    free(disparityImage);
    free(depthImage);

    return 0;
}
