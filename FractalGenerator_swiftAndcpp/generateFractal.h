//
//  generateFractal.hpp
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 27.06.2023.
//

//#ifndef generateFractal_h
//#define generateFractal_h
//
//#include <stdio.h>
//
//#endif /* generateFractal_hpp */


#ifdef __cplusplus
extern "C" {
#endif

void GetFractalImage(const char* imageName, const char* directoryPath, const int width, const int height, const int maxIterationsCount, double mathX, double mathY, double zoom, unsigned char redImpact, unsigned char greenImpact, unsigned char blueImpact, unsigned char isMandelbrotSet, const double juliaSetC_X, const double juliaSetC_Y);

char* CalculateFractalData_forSwift(const int width, const int  height, const int maxIterationsCount, double mathX, double mathY, double zoom);

//void GetArrayOfImages(const char* imageName, const char* directoryPath ,const int width, const int height, const int maxIterationsCount, double mathX, double mathY, double zoom);


#ifdef __cplusplus
} // extern "C"
#endif
