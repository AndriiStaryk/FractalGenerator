//
//  ContentView.swift
//  FractalGenerator_swiftAndcpp
//
//  Created by Andrii Staryk on 27.06.2023.
//


import SwiftUI

//mathX = -1,258506814;
//mathY = 0,38266398842;
struct ContentView: View {
    
    @StateObject var vM = ViewModel()
    
    @State var renderedImagePath: String = ""
    @State var name: String = "fractal"
    
    @State var uuid4TrackingChanges = UUID()
    @State var progress: Double = 0.0

    var body: some View {
        
        VStack {
            
            HStack {
                
                Group {
                    
                    VStack {
                        
                        Group {
                            
                            Text("Set point to zoom")
                            
                            HStack {
                                TextField("X", value: $vM.mathX, format: .number)
                                TextField("Y", value: $vM.mathY, format: .number)
                            }
                            
                            HStack {
                                Text("Set resolution:")
                                Picker("", selection: $vM.resolution) {
                                    ForEach(vM.resolutions, id: \.self) { res in
                                        Text("\(String(describing: res))")
                                    }
                                }
                                .frame(width: 100)
                            }
                            
                            if vM.resolution == ._Custom {
                                
                                HStack {
                                    
                                    Text("Width")
                                    TextField("", value: $vM.width, format: .number)
                                        .onChange(of: vM.width) { newValue in
                                            if newValue < 0 {
                                                vM.width = 1000
                                            } else if newValue > 25_000 {
                                                vM.width = 25_000
                                            }
                                        }
                                        .frame(width: 60)
                                    
                                    Text("Height")
                                    TextField("", value: $vM.height, format: .number)
                                        .onChange(of: vM.height) { newValue in
                                            if newValue < 0 {
                                                vM.height = 1000
                                            } else if newValue > 25_000 {
                                                vM.height = 25_000
                                            }
                                        }
                                        .frame(width: 60)
                                }
                            }
                            
                            HStack {
                                Text("Set zoom")
                                HStack {
                                    TextField("", value: $vM.zoom, format: .number)
                                        .onChange(of: vM.zoom) { newValue in
                                            if newValue < 0 {
                                                vM.zoom = 100
                                            }
                                        }
                                }
                            }
                            
                            HStack {
                                Text("Set iterations")
                                
                                TextField("", value: $vM.maxIterationsCount, format: .number)
                                    .onChange(of: vM.maxIterationsCount) { newValue in
                                        if newValue < 0 {
                                            vM.maxIterationsCount = 5
                                        } else if newValue > 500 {
                                            vM.maxIterationsCount = 500
                                        }
                                    }
                                
                            }
                            
                            
                            Picker("Render mode", selection: $vM.renderMode) {
                                ForEach(vM.renderModes, id: \.self) { renderMode in
                                    Text(String(describing: renderMode))
                                }
                            }
                            
                            TextField("filename", text: $name)
                            
                        }
                        
                        Group {
                            
                            ColorSlider(value: $vM.redImpact, status: $vM.status, header: "R")
                            ColorSlider(value: $vM.greenImpact, status: $vM.status, header: "G")
                            ColorSlider(value: $vM.blueImpact, status: $vM.status, header: "B")
                            
                        }
                        
                        
                        Group {
                            
                            Picker("Set", selection: $vM.selectedSet) {
                                ForEach(vM.sets, id: \.self) { choosenSet in
                                    Text(String(describing: choosenSet))
                                }
                            }
                            //    .pickerStyle(.radioGroup)
                            
                            
                            if vM.selectedSet == .JuliaSet {
                                Text("Set constant point C")
                                HStack {
                                    TextField("X", value: $vM.juliaSetC_X, format: .number)
                                    TextField("Y", value: $vM.juliaSetC_Y, format: .number)
                                }
                            }
                            
                        }
                        
                        Button {
                            Task {
                                
                                vM.frameName = name + "_" + String(vM.zoom) + "_" + String(Int(vM.redImpact * 255)) + "_" + String(Int(vM.greenImpact * 255)) + "_" + String(Int(vM.blueImpact * 255)) + "_" + String(vM.maxIterationsCount) + "_" + String(describing: vM.resolution)
                                
                                vM.zoom *= sqrt(Double((vM.Resolution2Values().width * vM.Resolution2Values().height)) / Double((vM.previewWidth * vM.previewHeight)))
                                
                                await RenderFractalFrame(
                                    imageName: vM.frameName,
                                    directoryPath: vM.directoryPath,
                                    width: vM.Resolution2Values().width,
                                    height: vM.Resolution2Values().height,
                                    maxIterationsCount: vM.maxIterationsCount,
                                    mathX: vM.mathX,
                                    mathY: vM.mathY,
                                    zoom: vM.zoom,
                                    redImpact: vM.redImpact,
                                    greenImpact: vM.greenImpact,
                                    blueImpact: vM.blueImpact,
                                    isMandelbrotSet: vM.selectedSet == .MandelbrotSet ?  1 : 0,
                                    juliaSetC_X: vM.juliaSetC_X,
                                    juliaSetC_Y: vM.juliaSetC_Y
                                )
                                
                                
                                renderedImagePath = vM.directoryPath + vM.frameName + ".png"
                                vM.zoom /= sqrt(Double((vM.Resolution2Values().width * vM.Resolution2Values().height)) / Double((vM.previewWidth * vM.previewHeight)))
                                vM.renderedImageNames = []
                            }
                            
                        } label: {
                            Text("Render")
                        }
                        
                    }
                }
                .padding()
                
                Group {
                    VStack {
                        
                        AsyncImage(url: URL(filePath: renderedImagePath)) { image in
                            
                            image
                                .resizable()
                                .scaledToFit()
                        } placeholder: {
                            Color.black
                        }
                        
                        
                        .id(uuid4TrackingChanges)
                        .frame(width: Double(vM.previewWidth), height: Double(vM.previewHeight))
                        .gesture(
                            DragGesture(minimumDistance: 0)
                                .onChanged { value in
                                    
                                    if value.location.x < 0 || value.location.x > Double(vM.previewWidth) || value.location.y < 0 || value.location.y > Double(vM.previewHeight) {
                                        vM.mouseLocation = CGPoint(x: Double(vM.previewWidth/2), y: Double(vM.previewHeight) - Double(vM.previewHeight/2))
                                    } else {
                                        vM.mouseLocation = value.location
                                    }
                                    
                                    vM.hoveredMathX = (vM.mouseLocation.x - Double(vM.previewWidth/2)) / vM.zoom + vM.mathX
                                    vM.hoveredMathY = (Double(vM.previewHeight) - vM.mouseLocation.y - Double(vM.previewHeight/2)) / vM.zoom + vM.mathY
                                    
                                }
                            
                                .onEnded({ value in
                                    
                                    let xDisplayOffset = value.location.x - value.startLocation.x
                                    let yDisplayOffset = value.location.y - value.startLocation.y
                                    
                                    if xDisplayOffset != 0  && yDisplayOffset != 0 {
                                        
                                        vM.hoveredMathX = xDisplayOffset / vM.zoom
                                        vM.hoveredMathY = yDisplayOffset / vM.zoom
                                        
                                        vM.mathX -= vM.hoveredMathX
                                        vM.mathY += vM.hoveredMathY
                                        
                                    } else {
                                        vM.mathX = vM.hoveredMathX
                                        vM.mathY = vM.hoveredMathY
                                    }
                                    
                                })
                        )
                        
                        .onAppear {
                            NSEvent.addLocalMonitorForEvents(matching: .scrollWheel) { event in
                                if abs(event.scrollingDeltaY) >= 0.1 {
                                    vM.zoom -= event.scrollingDeltaY * vM.zoom * 0.1
                                }
                                return event
                            }
                        }
                        
                        
                    }
                }
            }
            Group {
                Text("Copyright © 2023 by Andrii Staryk Inc.")
                    .font(.headline)
            }
            
        }
        .onChange(of: vM.status, perform: { newValue in
            print("something has changed")
            Task {
                await previewFrame()
            }
        })
        .padding()
        .onAppear {
            checkOutputDirectory()
        }
        
    }
    
    func checkOutputDirectory() {
            if let downloadsPath = FileManager.default.urls(for: .downloadsDirectory, in: .userDomainMask).first {
                let directoryPath = downloadsPath.appendingPathComponent("FractalGenerator_Output")
                
                if !FileManager.default.fileExists(atPath: directoryPath.path) {
                    do {
                        try FileManager.default.createDirectory(at: directoryPath, withIntermediateDirectories: true, attributes: nil)
                    } catch {
                        print("Failed to create directory: \(error)")
                    }
                }
            } else {
                print("Could not find the Downloads directory")
            }
        }
    
    func RenderFractalFrame(imageName: String, directoryPath: String, width: Int32 , height: Int32, maxIterationsCount: Int32, mathX: Double, mathY: Double, zoom: Double, redImpact: Double, greenImpact: Double, blueImpact: Double, isMandelbrotSet: UInt8, juliaSetC_X: Double, juliaSetC_Y: Double) async {
        checkOutputDirectory()
        
        GetFractalImage(imageName.cString(using: .utf8), directoryPath.cString(using: .utf8), width, height, maxIterationsCount, mathX, mathY, zoom, UInt8(redImpact * 255), UInt8(greenImpact * 255), UInt8(blueImpact * 255), isMandelbrotSet, juliaSetC_X, juliaSetC_Y)

        convertBMPtoPNG(imageName: imageName, directoryPath: directoryPath)
    }
    

    
    func previewFrame() async {
        
        uuid4TrackingChanges = UUID()
        vM.resolution = ._Preview
        
        await RenderFractalFrame(
            imageName: vM.previewName,
            directoryPath: vM.directoryPath,
            width: vM.Resolution2Values().width,
            height: vM.Resolution2Values().height,
            maxIterationsCount: vM.maxIterationsCount,
            mathX: vM.mathX,
            mathY: vM.mathY,
            zoom: vM.zoom,
            redImpact: vM.redImpact,
            greenImpact: vM.greenImpact,
            blueImpact: vM.blueImpact,
            isMandelbrotSet: vM.selectedSet == .MandelbrotSet ?  1 : 0,
            juliaSetC_X: vM.juliaSetC_X,
            juliaSetC_Y: vM.juliaSetC_Y
        )
        
        renderedImagePath = vM.directoryPath + vM.previewName + ".png"
        
    }
    
}

//struct ContentView_Previews: PreviewProvider {
//    static var previews: some View {
//        ContentView()
//    }
//}



struct ColorSlider: View {
    
    @Binding var value: Double
    @Binding var status: UUID
    let header: String
    
    var body: some View {
        
        HStack {
            
            Text("\(header): ")
            
            Slider(value: $value, in: 0...1) {
                Text("\(UInt8(value * 255))")
            } minimumValueLabel: {
                Text("0")
            } maximumValueLabel: {
                Text("255")
            } onEditingChanged: { editing in
                if !editing {
                    status = UUID()
                }
            }

        }
    }
}
