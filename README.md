# MeltyConfig 0.5
###### A program which allows you to edit Melty Blood Actress Again Current Code's config data files without the need to open up MBAA.exe to do so.

## Getting Started
1. Download the latest MeltyConfig release from https://github.com/dor3k/MeltyConfig/releases depending on your system or compile the program yourself
- If you are using Windows download MeltyConfig-windows.exe. If you are using Linux download the MeltyConfig-linux build
2. Place the MeltyConfig.exe into the folder containing MBAA.exe or run it wherever you want and provide appropriate paths in MeltyConfig.ini
- The program will create the MetlyConfig.ini configuration file on the first run
3. Run the program and press the appropriate button to edit whatever option you want
4. Insert the desired value and hit enter
5. After you're done close the program or choose Exit in the options menu for the program to close

**MAKE SURE THE PROGRAM IS NOT RUNNING WHEN YOU OPEN THE GAME AS THE GAME WON'T HAVE ACCESS TO THOSE FILES, INSTEAD LOADING DEFAULT VALUES**

MAKE SURE THE GAME IS NOT OPEN WHEN YOU ARE EDITING THE CONFIG FILES AS THE CHANGES MAY NOT SAVE

## Build instructions
The program was built using G++.
Use whatever toolchain you want to compile and link all the files in ./main/src directory

The files inside of the ./main/System directory are used for testing, and can be ignored.

As an example there are two build scripts provided in the ./main directory which use G++ to build the program, one for Linux and the other for Windows
```
cd ./src

g++ -pedantic-errors -std=c++20 -c main.cpp
g++ -pedantic-errors -std=c++20 -c ConvertUTF.c
g++ -std=c++20 -c OptionBinary.cpp
g++ -pedantic-errors -std=c++20 -c OptionIni.cpp
g++ -pedantic-errors -std=c++20 -c Program.cpp

g++ -std=c++20 -pedantic-errors main.o ConvertUTF.o OptionBinary.o OptionIni.o Program.o -o ../MeltyConfig
```
1. Make sure you have GCC/G++ installed on your system
On Windows this can be done by installing MinGW toolchain using MSYS2
https://code.visualstudio.com/docs/cpp/config-mingw

On Linux you will need to download the packages provided by your distribution
2. Clone the repository using git
```
git clone https://github.com/dor3k/MeltyConfig.git
```
3. Navigate to the path where build.sh and build.bat are stored
```
cd ./MeltyConfig/main/
```
4. Run the provided build scripts depending on your system to build the application using g++
On Windows, run build.bat
```
./build.bat
```
On Linux, run build.sh
```
./build.sh
```

The program will be built in the ./MeltyConfig/main/ directory. From there either place it in the Melty Blood folder or place it somewhere else and configure the MeltyConfig.ini file to point to the proper directories.

## Changelog 0.5
- Unix compatibility added, the program will now either use the system provided function or use an alternative function depending on the OS
- Bugfixes
- Project cleanup

# LICENSE
MIT License

Copyright (c) 2022-2025 dor3k

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**
