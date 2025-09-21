#pragma once

class interpreter
{
    const char* bfcode;
    char current;
    unsigned int cursor;
    unsigned int loop;
public:
    interpreter(const char* input)
    {
        bfcode = input;
        cursor = 0;
        loop = 0;
        current = input[cursor];
    }
    void interpret();
};