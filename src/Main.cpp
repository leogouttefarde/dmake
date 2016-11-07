#include "../include/Parser.hpp"
#include <iostream>
#include <fstream>
  
using namespace std;



int main( int argc, char* argv[] )
{
    Parser main_parser = Parser(argc, argv);

    return main_parser.readMakefile(); // return EXIT_FAILURE if failure

}
