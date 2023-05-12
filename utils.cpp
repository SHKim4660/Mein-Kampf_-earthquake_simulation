#define __gl_h_
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include "utils.hpp"

void terminate(int status)
{
    glfwTerminate();

    exit(status);
}

void panik(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    terminate(1);
}

std::string load_file(std::string filename)
{
    if (!std::filesystem::exists(std::filesystem::path(filename)))
        panik("Fatal: The file does not exist: %s\n", filename.c_str());
    
    std::ifstream ifs(filename);

    return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}
