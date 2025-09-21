#include "bfcc.hpp"

void interpreter::interpret()
{
    unsigned long long mem = global_vars::memory_size;
    char tape[mem] = {0};
    char* ptr = tape;

    for(cursor = 0; bfcode[cursor] != 0; cursor++)
    {
        current = bfcode[cursor];
        switch (current)
        {
        case '+':
            (*ptr)++;
            break;
        case '-':
            (*ptr)--;
            break;
        case '>':
            ptr++;
            break;
        case '<':
            ptr--;
            break;
        case '[':
            if(*ptr == 0)
            {
                loop = 1;
                while(loop > 0)
                {
                    cursor++;
                    if (bfcode[cursor] == '[')
                        loop++;
                    else if (bfcode[cursor] == ']')
                        loop--;

                    if (bfcode[cursor] == 0) break;
                }
            }
            break;
        case ']':
            if (*ptr)
				{
					loop = 1;
					while (loop > 0)
					{
                        cursor--;
                        if (bfcode[cursor] == '[')
                                loop--;
                        else if (bfcode[cursor] == ']')
                                loop++;

                        if (bfcode[cursor] == 0) break;
					}
				}
            break;
        case '.':
            putchar(*ptr);
            break;
        case ',':
            *ptr = getchar();
            break;
        default:
            break;
        }
    }
}