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


struct ColorRGB {
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
    
    ColorRGB(uint8_t red, uint8_t green, uint8_t blue) : m_red(red), m_green(green), m_blue(blue) {}
    
};

struct ColorHSV {
    uint16_t m_hue;
    double m_saturation;
    double m_value;

    ColorHSV(uint16_t hue, double saturation, double value) : m_hue(hue), m_saturation(saturation), m_value(value) {}
    
};


ColorRGB convertRGB_2_HCV(ColorHSV hsv) {
    ColorRGB rgb = ColorRGB(0,0,0);
    
    uint8_t M = (uint8_t)(255.0 * hsv.m_value);
    uint8_t m = (uint8_t)((double)M * (1.0 - hsv.m_saturation));
    uint8_t z = (M - m) * (1 - std::abs(hsv.m_hue % 2 - 1));
    
    if (hsv.m_hue >= 0 && hsv.m_hue < 60) {
        rgb.m_red = M;
        rgb.m_green = z + m;
        rgb.m_blue = m;
    } else if (hsv.m_hue >= 60 && hsv.m_hue < 120) {
        rgb.m_red = z + m;
        rgb.m_green = M;
        rgb.m_blue = m;
    } else if (hsv.m_hue >= 120 && hsv.m_hue < 180) {
        rgb.m_red = m;
        rgb.m_green = M;
        rgb.m_blue = z + m;
    } else if (hsv.m_hue >= 180 && hsv.m_hue < 240) {
        rgb.m_red = m;
        rgb.m_green = z + m;
        rgb.m_blue = M;
    } else if (hsv.m_hue >= 240 && hsv.m_hue < 300) {
        rgb.m_red = z + m;
        rgb.m_green = m;
        rgb.m_blue = M;
    } else if (hsv.m_hue >= 300 && hsv.m_hue < 360) {
        rgb.m_red = M;
        rgb.m_green = m;
        rgb.m_blue = z + m;
    }
   
    return rgb;
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

    long testArray[95] = {0};

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
                                        
//                                        ColorHSV hsv = ColorHSV(0,0,0);
//                                        if (colorRelation == 0) { hsv = ColorHSV((uint16_t)(colorRelation * 360.0), 0, 0); }
//                                        else { hsv = ColorHSV((uint16_t)(colorRelation * 360.0), 0.8, 0.8); }
//
//                                        ColorRGB rgb = convertRGB_2_HCV(hsv);
//                                        imageData[index + 0] = rgb.m_blue;
//                                        imageData[index + 1] = rgb.m_green;
//                                        imageData[index + 2] = rgb.m_red;


                                        int len = std::to_string(int(zoom)).length();
                                        double step = std::pow(10, -len + 2);


                                        imageData[index + 0] = (uint8_t)(colorRelation * blueImpact * zoom * step);
                                        imageData[index + 1] = (uint8_t)(colorRelation * greenImpact * zoom * step);
                                        imageData[index + 2] = (uint8_t)(colorRelation * redImpact * zoom * step);


//
                                        
//
//                                        int red = map(iterationsPerPoint, 0, maxIterationsCount, 0, redImpact);
//                                        int blue = map(iterationsPerPoint, 0, maxIterationsCount, 0, blueImpact);
//                                        int green = map(iterationsPerPoint, 0, maxIterationsCount, 0, greenImpact);
//
//                                        imageData[index + 0] = (uint8_t)(blue);// * zoom * step);
//                                        imageData[index + 1] = (uint8_t)(green);// * zoom * step);
//                                        imageData[index + 2] = (uint8_t)(red);// * zoom * step);
                                        
                                      //  testArray[iterationsPerPoint]++;

                                        
                                        
                                        //       progress = (double)(index + 2) / (double)(width * height * 3);
                                        
                                    }
#ifdef CROSS
          }
#endif
    }

    return iterationsPerFrame;
}








void GetFractalImage(const char* imageName, const char* directoryPath ,const int width, const int height, const int maxIterationsCount, double mathX, double mathY, double zoom, unsigned char redImpact, unsigned char greenImpact, unsigned char blueImpact, unsigned char isMandelbrotSet, const double juliaSetC_X, const double juliaSetC_Y) {

    uint64_t startOfCalculations = GetTickCount64();

    //std::vector<uint8_t> imageData(3 * width * height, 0);
    uint8_t* imageData = new uint8_t[width * height * 3];
    
    uint64_t iterationsPerFrame = CalculateFractalData(imageData, width, height, maxIterationsCount, mathX, mathY, zoom, redImpact, greenImpact, blueImpact, isMandelbrotSet, juliaSetC_X, juliaSetC_Y);
    
    
    std::string bmpName = std::string(imageName) + ".bmp";

    uint64_t endOfCalculations= GetTickCount64();
    std::cout << "Image: " << bmpName << " was rendering for " << (double)(endOfCalculations - startOfCalculations) / 1000 << " seconds and there was " << (double)iterationsPerFrame / 1000000.0 << " millions of iterations" << std::endl;

    GenerateBMP(bmpName, directoryPath, width, height, imageData);
    
    delete [] imageData;

}







char* CalculateFractalData_forSwift(const int width, const int  height, const int maxIterationsCount, double mathX, double mathY, double zoom) {

    const double SCALE = 1.0 / zoom;

    const double displayOriginRealOffset = double(width / 2);
    const double displayOriginImaginaryOffset = double(height / 2);


    uint64_t iterationsPerFrame = 0;
    
    char* imageData = new char [width * height * 3];

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

            uint16_t iterationsPerPoint = isPointInMandelbrotSet(C, maxIterationsCount);

            double d = (double) iterationsPerPoint / (double)maxIterationsCount;

            iterationsPerFrame += iterationsPerPoint;

            imageData[index + 0] = (uint8_t)(d * 145);
            imageData[index + 1] = (uint8_t)(d * 255);
            imageData[index + 2] = (uint8_t)(d * 55);
        }
#ifdef CROSS
          }
#endif
    }

    return imageData;
}






































