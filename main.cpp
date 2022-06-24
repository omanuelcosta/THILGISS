#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <tuple>
#include <windows.h>
#include <cstring>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/version.hpp>
#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>

#include "functions.h"
#include "except.h"
#include "classes.h"
#include "gnuplot-iostream.h"
//#include <boost/iostreams/device/file_descriptor.hpp>

/** main.cpp
* @brief Main program -> reads input file or outputs helper
* @return plot of GIXRD and compositional gradient given input file
**/

int main(int argc, char* argv[]) {

    std::string gnupath, outpath;
    int i;

    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << std::endl;
        if (argc < 1) {
            gnupath == " ";
        }
        else {
            if (i + 1 != argc) // check that we haven't finished parsing already
                if (argv[i] == "-gp") {
                    gnupath = std::string(argv[i + 1]);
                }
                else if (argv[i] == "-o") {
                    outpath = std::string(argv[i + 1]);
                }
        }
    }

    //std::string input_name = "input.inp";
    std::string input_name;

    std::cout << "input file:" << std::endl;

    std::getline(std::cin, input_name);

    //std::string gnupath;

    if (input_name == "help") { std::cout << "Input structure (.ginp):" << std::endl; }
    else {

        InputReader input(input_name);

        CompositionVectorizer comp = input;

        //comp.plot(gnupath);

        //Element N(input.elist[0]);
        Element Ti(input.elist[1]);
    };

    std::cout << "press Enter to continue" << std::endl;
    std::cin.get();
}