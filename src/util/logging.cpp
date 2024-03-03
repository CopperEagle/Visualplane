
#include "logging.h"
#include "termcolor.h"

void logging::debug(std::string module, std::string msg)
{
    std::cout << "DEBUG: [" << module << "] " << msg << std::endl;
}


void logging::error(std::string module, std::string msg)
{
    std::cout << term::colored("ERROR: [" + module + "] ", term::RED, true)  << term::emphasise(msg) << std::endl;
}

void logging::info(std::string module, std::string msg)
{
    std::cout << term::colored("INFO:  [" + module + "] ", term::GREEN, true) << msg << std::endl;
}