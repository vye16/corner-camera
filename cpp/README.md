C++ implementation of real-time around-the-corner imaging.

# Installation Instructions
This library requires OpenCV 3 and CMake

To make and run, create `build` directory and cmake from there:

```
mkdir build
cd build
cmake ..
make
```

(because it's better practice to have a build tree separate from
the actual source tree)

# Installing Dependencies 
To install OpenCV 3 and CMake on OSX, use Homebrew

```
brew install cmake
brew install opencv3 --c++11 --with-ffmpeg
```
To install Homebrew, run 
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
For more information, visit https://brew.sh/

To install OpenCV 3 (and CMake) on Ubuntu, please see this link: https://www.learnopencv.com/install-opencv3-on-ubuntu/
