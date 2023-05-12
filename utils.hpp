#ifndef __utils_hpp_
#define __utils_hpp_

#include <string>

void terminate(int status);
void panik(const char *fmt, ...);

std::string load_file(std::string filename);

#endif
