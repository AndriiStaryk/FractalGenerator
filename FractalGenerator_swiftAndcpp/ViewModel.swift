//
//  ViewModel.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrey Starik on 30.06.2023.
//

import Foundation


class ViewModel: ObservableObject {
    
    @Published var status: UUID = UUID()
    @Published var frameName = "fractal"
    @Published var resolution: Resolution = ._FullHD
    @Published var renderMode: RenderMode = .SingleImage
    @Published var selectedSet: Set = .MandelbrotSet { didSet { status = UUID() } }
    @Published var width: Int32 = 1000
    @Published var height: Int32 = 1000
    @Published var mathX: Double = -1.258506814 { didSet { status = UUID() } }
    @Published var mathY: Double = 0.38266398842 { didSet { status = UUID() } }
    @Published var zoom: Double = 200 { didSet { status = UUID() } }
    @Published var maxIterationsCount: Int32 = 95 { didSet { status = UUID() } }
    @Published var renderedImageNames: [String] = []
    @Published var maxZoom: Double = 50_000
    
    @Published var redImpact: Double = 0 //{ didSet { status = UUID() } }
    @Published var greenImpact: Double = 0 //{ didSet { status = UUID() } }
    @Published var blueImpact: Double = 0 //{ didSet { status = UUID() } }
    

    @Published var mouseLocation: CGPoint = .zero
    @Published var hoveredMathX: Double = 0
    @Published var hoveredMathY: Double = 0
    
    @Published var juliaSetC_X: Double = -0.8 { didSet { status = UUID() } }
    @Published var juliaSetC_Y: Double = 0.156 { didSet { status = UUID() } }
    
    
    let resolutions: [Resolution] = [._HD, ._FullHD, ._2K, ._4K, ._Custom, ._Preview]
    let renderModes: [RenderMode] = [.SingleImage]//, .SequenceOfImages]
    let sets: [Set] = [.MandelbrotSet, .JuliaSet]
    //let directoryPath = "/Users/andreystarik/Downloads/SwiftAppRenderedImages/"
    let directoryPath  = FileManager.default.urls(for: .downloadsDirectory, in: .userDomainMask).first!.appendingPathComponent("FractalGenerator_Output").path + "/"
     
    let previewName = "Fractal4Preview"
    let previewWidth: Int32 = 800
    let previewHeight: Int32 = 450
    
    func Resolution2Values() -> (width: Int32, height: Int32) {
        switch resolution {
        case ._HD: return (1280, 720)
        case ._FullHD: return (1920, 1080)
        case ._2K: return (2560, 1440)
        case ._4K: return (3840, 2160)
        case ._Custom: return (width, height)
        case ._Preview: return (previewWidth, previewHeight)
        }
    }
    
    
}
