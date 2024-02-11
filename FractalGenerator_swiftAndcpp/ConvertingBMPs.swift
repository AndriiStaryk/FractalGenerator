//
//  ConvertingBMPs.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 30.06.2023.
//

import Foundation
import Cocoa

func convertBMPtoPNG(imageName: String, directoryPath: String) {
    guard let bmpData = NSData(contentsOf: URL(filePath: directoryPath + imageName + ".bmp")) else {
        print("Failed to read BMP file.")
        return
    }
    
    guard let bitmap = NSBitmapImageRep(data: bmpData as Data) else {
        print("Failed to create bitmap image from BMP data.")
        return
    }
    
    guard let pngData = bitmap.representation(using: .png, properties: [:]) else {
        print("Failed to create PNG data.")
        return
    }
    
    do {
        try pngData.write(to: URL(filePath: directoryPath + imageName + ".png"))
        try FileManager().removeItem(at: URL(fileURLWithPath: directoryPath + imageName + ".bmp"))
        print("Conversion to PNG successful.")
    } catch {
        print("Failed to write PNG data to file: \(error.localizedDescription)")
    }
}


