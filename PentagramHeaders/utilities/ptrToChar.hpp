#pragma once

namespace PNT
{
    char *ptrToChar(const char *str)
    {
        int size = 0;
        while(str[size] != '\0')
        {
            size++;
        }
        char charArray[size + 1];
        for (int i = 0; i < size; i++) {
            charArray[i] = str[i];
        }
        charArray[size] = '\0';
        return charArray;
    }
}
