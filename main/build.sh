cd ./src
g++ -pedantic-errors -std=c++20 -c main.cpp
g++ -pedantic-errors -std=c++20 -c ConvertUTF.c
g++ -std=c++20 -c OptionBinary.cpp
g++ -pedantic-errors -std=c++20 -c OptionIni.cpp
g++ -pedantic-errors -std=c++20 -c Program.cpp

g++ -std=c++20 -pedantic-errors main.o ConvertUTF.o OptionBinary.o OptionIni.o Program.o -o ../MeltyConfig-linux

rm *.o
