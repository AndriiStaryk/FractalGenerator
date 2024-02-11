//  generateFractal.cpp
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 27.06.2023.




#include "generateFractal.h"
#include "FractalGenerator-Bridging-header.h"
#include "Complex.h"

#include <iostream>
#include <fstream>
#include <mach/mach_time.h>
#include <vector>
//#include <cmath>


//#define CROSS


#pragma pack(push, 1) // Ensure structure is packed tightly

// Bitmap file header
struct BitmapFileHeader {
    uint16_t fileType{ 0x4D42 };     // File type ("BM" in little-endian)
    uint32_t fileSize{ 0 };           // Size of the file in bytes
    uint16_t reserved1{ 0 };          // Reserved, must be set to 0
    uint16_t reserved2{ 0 };          // Reserved, must be set to 0
    uint32_t dataOffset{ 0 };         // Offset to the start of pixel data
};

// Bitmap info header
struct BitmapInfoHeader {
    uint32_t headerSize{ 40 };        // Size of the header in bytes
    int32_t width{ 0 };               // Width of the image
    int32_t height{ 0 };              // Height of the image
    uint16_t planes{ 1 };             // Number of color planes, must be set to 1
    uint16_t bitsPerPixel{ 24 };      // Number of bits per pixel (24 for RGB)
    uint32_t compression{ 0 };        // Compression method (0 for uncompressed)
    uint32_t imageSize{ 0 };          // Size of the image data in bytes
    int32_t xPixelsPerMeter{ 0 };     // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter{ 0 };     // Vertical resolution (pixels per meter)
    uint32_t totalColors{ 0 };        // Number of colors in the color palette (0 for full color)
    uint32_t importantColors{ 0 };    // Number of important colors (0 when every color is important)
};

#pragma pack(pop)



void GenerateBMP(const std::string& fileName, const std::string& directoryPath, int width, int height, const uint8_t* imageData) {
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    // Set file header properties
    fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + width * height * 3;// + imageData.size();
    fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    // Set info header properties
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.imageSize = width * height * 3;

    //std::string directoryPath = "/Users/andreystarik/Desktop/Temp Swift files/RendredImages/";
//    std::string directoryPath = "/Users/andreystarik/Downloads/SwiftAppRenderedImages/";

    std::string filePath = directoryPath + fileName;

    std::ofstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Failed to create file: " << fileName << std::endl;
        return;
    }

    // Write headers to the file
    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));

    // Write pixel data to the file
   // file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
    file.write(reinterpret_cast<const char*>(imageData), width * height * 3);

    file.close();

    std::cout << "BMP image generated successfully: " <<  std::endl;

}


uint16_t isPointInMandelbrotSet(Complex& c, const uint16_t maxIterationsCount) {

    auto z = Complex(0, 0);

    for (uint16_t i = 0; i < maxIterationsCount; ++i) {

        z = z * z + c;

        if (z.moduleSquared() > 4) {
            return i;
        }

    }
    return 0;
}

uint16_t isPointInJuliaSet(Complex& z, const uint16_t maxIterationsCount, const double real, const double imaginary) {
    auto c = Complex(real, imaginary);

        for (uint16_t i = 0; i < maxIterationsCount; ++i) {

            z = z * z + c;

            if (z.moduleSquared() > 4) {
                return i;
            }

        }
        return 0;

}


int map (int in_val, int in_min, int in_max, int out_min, int out_max) {
    return (in_val - in_min)*(out_max - out_min) / (in_max - in_min) + out_min;
}




uint64_t GetTickCount64() {
    static mach_timebase_info_data_t timebase;
    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    uint64_t timestamp = mach_absolute_time();
    uint64_t nanoseconds = timestamp * timebase.numer / timebase.denom;
    return nanoseconds / 1000000; // Convert to milliseconds
}


uint64_t CalculateFractalData(uint8_t* imageData,  const uint32_t width, const uint32_t  height, const uint16_t maxIterationsCount, double mathX, double mathY, double zoom, uint8_t redImpact, uint8_t greenImpact, uint8_t blueImpact, uint8_t isMandelbrotSet, const double juliaSetC_X, const double juliaSetC_Y) {

    const double SCALE = 1.0 / zoom;

    const double displayOriginRealOffset = double(width / 2);
    const double displayOriginImaginaryOffset = double(height / 2);

    
    uint64_t iterationsPerFrame = 0;

    for (uint32_t y = 0; y < height; ++y) {

        for (uint32_t x = 0; x < width; ++x) {

            const double mX = ((double)x - displayOriginRealOffset) * SCALE + mathX;
            const double mY = ((double)y - displayOriginImaginaryOffset) * SCALE + mathY;

            uint32_t index = (y * width + x) * 3;

#ifdef CROSS
                                        if (x == width / 2) {
                                        imageData[index + 0] = 0;
                                        imageData[index + 1] = 0;
                                        imageData[index + 2] = 255;

                                    } else if (y == height / 2) {
                                        imageData[index + 0] = 0;
                                        imageData[index + 1] = 0;
                                        imageData[index + 2] = 255;

                                    }

                                    else {
#endif
                                        Complex C  = Complex(mX, mY);
                                        uint16_t iterationsPerPoint;
                                        
                                        if (isMandelbrotSet == 1) {
                                            iterationsPerPoint = isPointInMandelbrotSet(C, maxIterationsCount);
                                        } else {
                                            iterationsPerPoint = isPointInJuliaSet(C, maxIterationsCount, juliaSetC_X, juliaSetC_Y);
                                        }
                                        
                                        double colorRelation = (double) iterationsPerPoint / (double)maxIterationsCount;
                                        
                                        iterationsPerFrame += iterationsPerPoint;
                                        
                                        int len = std::to_string(int(zoom)).length();
                                        double step = std::pow(10, -len + 2);


                                        imageData[index + 0] = (uint8_t)(colorRelation * blueImpact * zoom * step);
                                        imageData[index + 1] = (uint8_t)(colorRelation * greenImpact * zoom * step);
                                        imageData[index + 2] = (uint8_t)(colorRelation * redImpact * zoom * step);

                                    }
#ifdef CROSS
          }
#endif
    }

    return iterationsPerFrame;
}



void GetFractalImage(const char* imageName, const char* directoryPath ,const int width, const int height, const int maxIterationsCount, double mathX, double mathY, double zoom, unsigned char redImpact, unsigned char greenImpact, unsigned char blueImpact, unsigned char isMandelbrotSet, const double juliaSetC_X, const double juliaSetC_Y) {

    uint64_t startOfCalculations = GetTickCount64();

    uint8_t* imageData = new uint8_t[width * height * 3];
    
    uint64_t iterationsPerFrame = CalculateFractalData(imageData, width, height, maxIterationsCount, mathX, mathY, zoom, redImpact, greenImpact, blueImpact, isMandelbrotSet, juliaSetC_X, juliaSetC_Y);
    
    
    std::string bmpName = std::string(imageName) + ".bmp";

    uint64_t endOfCalculations= GetTickCount64();
    std::cout << "Image: " << bmpName << " was rendering for " << (double)(endOfCalculations - startOfCalculations) / 1000 << " seconds and there was " << (double)iterationsPerFrame / 1000000.0 << " millions of iterations" << std::endl;

    GenerateBMP(bmpName, directoryPath, width, height, imageData);
    
    delete [] imageData;

}







































