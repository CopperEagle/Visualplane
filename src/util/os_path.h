#ifndef OS_PATH_H
#define OS_PATH_H

#include <string>

/*!
* @brief Very lazily joins two strings as path.
**/
inline std::string join_paths(const std::string& s, const std::string& t)
{
    
    if (s.at(s.length() - 1) == '/')
    {
        return s + t;
    }
    return s + '/' + t;
}

#endif // OS_PATH_H