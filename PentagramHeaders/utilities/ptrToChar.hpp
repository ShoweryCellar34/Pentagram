#pragma once

#include <includes.hpp>

namespace PNT
{
    // Fills the first string with the second string
    void ptrToChar(const char *str, char *newStr)
    {
        char *strCopy = strdup(str);
        strCopy = (char *)realloc(strCopy, strlen(newStr));
        strcpy(strCopy, newStr);
        free(strCopy);
    }
}
