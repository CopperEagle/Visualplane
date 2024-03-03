#ifndef TERMCOLOR_H
#define TERMCOLOR_H

#include <string>

namespace term{

typedef std::string Color;

const Color BLACK     = "30";
const Color RED       = "31";
const Color GREEN     = "32";
const Color YELLOW    = "33";
const Color BLUE      = "34";
const Color MAGENTA   = "35";
const Color CYAN      = "36";
const Color WHITE     = "37";

std::string colored(const std::string& msg, Color col, bool bold = false);

std::string emphasise(const std::string& msg);

};

#endif // TERMCOLOR_H