#include "termcolor.h"


std::string term::colored(const std::string &msg, term::Color col, bool bold)
{
    
    const char s = 27;
    std::string spec = std::string(&s);

    if (bold)
        return spec + "[1m" + spec + "[" + col + "m" + msg + spec + "[0m";
    else
        return spec + "[" + col + "m" + msg + spec + "[0m";
}


std::string term::emphasise(const std::string &msg)
{
    
    const char s = 27;
    std::string spec = std::string(&s);
    // spec + "[2m" + 
    return spec + "[4m" + msg + spec + "[0m";
}