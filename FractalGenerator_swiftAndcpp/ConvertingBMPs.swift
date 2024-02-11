//
//  ConvertingBMPs.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 30.06.2023.
//

import Foundation
import Cocoa

//let directoryPath = "/Users/andreystarik/Downloads/SwiftAppRenderedImages/"
//mathX = -1.258506814;
//mathY = 0.38266398842


   
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













//func convertCharToData(charPointer: UnsafePointer<CChar>) -> Data {
//    let length = strlen(charPointer)
//    let bufferPointer = UnsafeBufferPointer(start: charPointer, count: length)
//    return Data(buffer: bufferPointer)
//}




//func convertCharDataToPNG(charData: UnsafePointer<CChar>, length: Int, outputPath: String) async {
//    let data = Data(convertCharToData(charPointer: charData) as Data)
//
//    guard let image = NSImage(data: data) else {
//        print("Failed to create NSImage from char data.")
//        return
//    }
//
//    guard let bitmapRepresentation = NSBitmapImageRep(data: image.tiffRepresentation!) else {
//        print("Failed to create bitmap representation from NSImage.")
//        return
//    }
//
//    guard let pngData = bitmapRepresentation.representation(using: .png, properties: [:]) else {
//        print("Failed to create PNG data.")
//        return
//    }
//
//    do {
//        try pngData.write(to: URL(fileURLWithPath: outputPath))
//        print("Conversion to PNG successful.")
//    } catch {
//        print("Failed to write PNG data to file: \(error.localizedDescription)")
//    }
//}





//func saveImage(_ image: NSImage, to directoryURL: URL, withName imageName: String) {
//    // Create a file URL for the image
//    let imageURL = directoryURL.appendingPathComponent("\(imageName).png")
//
//    // Convert NSImage to NSBitmapImageRep
//    guard let imageData = image.tiffRepresentation,
//        let bitmap = NSBitmapImageRep(data: imageData) else {
//            print("Failed to convert image to bitmap representation")
//            return
//    }
//
//    // Set the file type as PNG
//    let fileType = NSBitmapImageRep.FileType.png
//
//    // Set the compression factor (0.0 - 1.0, 1.0 being the highest quality)
//    let properties = [NSBitmapImageRep.PropertyKey.compressionFactor: NSNumber(value: 1.0)]
//
//    // Get the data representation of the image
//    guard let imageData = bitmap.representation(using: fileType, properties: properties) else {
//        print("Failed to get image data representation")
//        return
//    }
//
//    // Write the image data to the file URL
//    do {
//        try imageData.write(to: imageURL)
//        print("Image saved successfully!")
//    } catch {
//        print("Failed to save image: \(error)")
//    }
//}
