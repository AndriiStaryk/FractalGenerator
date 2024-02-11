//
//  Complex.cpp
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 27.06.2023.
//

#include "Complex.h"
   
    Complex::Complex(const double realPart, const double imaginaryPart) : m_real(realPart), m_imaginary(imaginaryPart) {}


    double Complex::getReal() const {
        return m_real;
    }

    double Complex::getImaginary() const {
        return m_imaginary;
    }


    Complex Complex::operator * (const Complex& rhs) {
        double realPart = m_real * rhs.m_real - m_imaginary * rhs.m_imaginary;
        double imaginaryPart = m_real * rhs.m_imaginary + rhs.m_real * m_imaginary;
        return Complex(realPart, imaginaryPart);
    }


    Complex Complex::operator + (const Complex& rhs) {
        double realPart = m_real + rhs.m_real;
        double imaginaryPart = m_imaginary + rhs.m_imaginary;
        return Complex(realPart, imaginaryPart);
    }


    Complex Complex::operator - (const Complex& rhs) {
        double realPart = m_real - rhs.m_real;
        double imaginaryPart = m_imaginary - rhs.m_imaginary;
        return Complex(realPart, imaginaryPart);
    }


    double Complex::moduleSquared() const {
        return m_real * m_real + m_imaginary * m_imaginary;
    }
