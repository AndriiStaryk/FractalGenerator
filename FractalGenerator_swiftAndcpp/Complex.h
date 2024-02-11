//
//  Complex.hpp
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 27.06.2023.
//

//#ifndef Complex_h
//#define Complex_h
//
//#include <stdio.h>
//
//#endif /* Complex_hpp */

#pragma once


class Complex
{
    double m_real;
    double m_imaginary;

public:

    Complex(const double realPart, const double imaginaryPart);

    
    double getReal() const;

    double getImaginary() const;


    Complex operator * (const Complex& rhs);


    Complex operator + (const Complex& rhs);


    Complex operator - (const Complex& rhs);

    double moduleSquared() const;



};
