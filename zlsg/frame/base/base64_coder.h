#ifndef __BASE64_CODER_H__
#define __BASE64_CODER_H__

#include <stdio.h>
#include <stdarg.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/syscall.h>
#endif // _WIN32
#include <sys/stat.h>
#include <string>

std::string GetBasePath();
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s );
unsigned int base64_decode(std::string const& encoded_string, char* bytes_for_decode);

#endif
