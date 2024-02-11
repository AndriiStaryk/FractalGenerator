//
//  Custom enums.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 30.06.2023.
//

import Foundation
import AppKit
import SwiftUI

enum Resolution: CustomStringConvertible {
    case _4K, _2K, _FullHD, _HD, _Custom, _Preview
    
    var description: String {
        switch self {
        case._4K: return "4K"
        case ._2K: return "2K"
        case ._FullHD: return "FullHD"
        case ._HD: return "HD"
        case ._Custom: return "Custom"
        case ._Preview: return "Preview"
            
        }
    }
    
}

enum RenderMode: CustomStringConvertible {
    case SingleImage, SequenceOfImages
    
    var description: String {
        switch self {
        case .SingleImage: return "Single Image"
        case .SequenceOfImages: return "Sequence of images"
        }
    }
    
}

enum Set: CustomStringConvertible {
    case MandelbrotSet, JuliaSet
    
    var description: String {
        switch self {
        case .MandelbrotSet: return "Mandelbrot Set"
        case .JuliaSet: return "Julia Set"
        }
    }
}

