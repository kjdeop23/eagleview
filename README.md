# eagleview

Eagleview is a computer vision project that processes a set of images using OpenCV. It reads multiple images in parallel, creates binary masks based on pixel values, and counts the number of pixels where the mask condition holds true.

The given problem statement:
Given a set of jpg photo files on disk, write code using OpenCV that would, in parallel, read each in, 
create a binary mask image where max means all 3 channels are above 200 (considering 8 bit as max), and write out 
the mask images as any lossless files. Also, sum the number of pixels where the mask is max in all images 
and log that count.

## Features
- Parallel processing of images for faster execution.
- Creates a binary mask where all RGB channels are greater than 200.
- Supports various image formats like `.jpg`, `.jpeg`, `.png`, etc.
- Saves the masks as lossless PNG files.
- Logs the total count of pixels satisfying the mask condition across all images.

## Requirements

- **C++17** or later
- **OpenCV** 4.9 or later
- **CMake** for building the project
- **Threading Support** (std::async for parallelism)

## Usage
# Install Dependencies (If not already present)
sudo apt-get install libopencv-dev

# build project
git clone https://github.com/your-username/Eagleview.git
cd eagleview/src
g++ -std=c++17 -o process_images process.cpp $(pkg-config --cflags --libs opencv4) -lstdc++fs -lpthread

# Run
./process_images


