// FlogramCompiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\CompilerLib\Headers\CompilerLib.h"

int main()
{
    CompileObj compiler;

    /* Rememeber to adjust path according to local directory structure */
    compiler.compile_file("D:\\Flogram\\FloRepository\\Flogram-Core-Compiler\\FloCode\\Sample_MainLoops.flo");
}