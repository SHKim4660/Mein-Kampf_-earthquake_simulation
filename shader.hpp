#ifndef __shader_hpp_
#define __shader_hpp_

#include <string>
#include <glbinding/gl/gl.h>
#include "utils.hpp"

class Shader
{
private:
    gl::GLuint program;

public:
    Shader(std::string vfilename, std::string ffilename);
    Shader(std::string vfilename, std::string gfilename, std::string ffilename);

    ~Shader();

    void bind();
}; 

#endif
