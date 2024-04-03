#pragma once

#include <includes.hpp>

namespace PNT
{
    // Replaces the contents of the first string with the second string
    void ptrToChar(const char *str, const char *newStr)
    {
        char *strCopy = strdup(str);
        strCopy = (char *)realloc(strCopy, strlen(newStr));
        strcpy(strCopy, newStr);
        free(strCopy);
    }
}
