#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

namespace logging {
    void info(std::string module, std::string msg);
    void debug(std::string module, std::string msg);
    void error(std::string module, std::string msg);
}

#endif // LOGGING_H