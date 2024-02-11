//
//  CreateVideo.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 02.07.2023.
//

import Foundation

import AVFoundation
import AppKit

import ImageIO


func getArrayOfNSImages(directoryPath: String, imageNamesSequence: [String]) -> [NSImage] {
    
    var nsImages: [NSImage] = []
    
    for name in imageNamesSequence {
        let imageURL = URL(filePath: directoryPath + name)
        nsImages.append(NSImage(contentsOf: imageURL)!)
    }
    
    return nsImages
}


func createGIFByImageSequence(gifName: String , directoryPath: String, imageNamesSequence: [String], fps: Double) {
    
    let nsImages = getArrayOfNSImages(directoryPath: directoryPath, imageNamesSequence: imageNamesSequence)
    
    createGif(from: nsImages, withDelay: 1 / fps, saveToURL: URL(filePath: directoryPath + gifName + ".gif"))
   
    
}


func createGif(from images: [NSImage], withDelay delay: TimeInterval, saveToURL url: URL) {
    guard let destination = CGImageDestinationCreateWithURL(url as CFURL, kUTTypeGIF, images.count, nil) else {
        print("Failed to create CGImageDestination")
        return
    }

    let frameProperties = [kCGImagePropertyGIFDictionary as String: [kCGImagePropertyGIFDelayTime as String: delay]]
    let gifProperties = [kCGImagePropertyGIFDictionary as String: [kCGImagePropertyGIFLoopCount as String: 0]]

    for image in images {
        if let imageData = image.tiffRepresentation,
           let bitmapImage = NSBitmapImageRep(data: imageData),
           let cgImage = bitmapImage.cgImage {
            CGImageDestinationAddImage(destination, cgImage, frameProperties as CFDictionary)
        }
    }

    CGImageDestinationSetProperties(destination, gifProperties as CFDictionary)

    if CGImageDestinationFinalize(destination) {
        print("GIF created successfully at \(url.path)")
    } else {
        print("Failed to create GIF at \(url.path)")
    }
}



