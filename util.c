#include "main.h"

int ftoi(float x)
{
    int int_part = (int)x;
    float decimal_part = x - int_part;

    if (decimal_part >= 0.5)
        return ceil(x);
    else
        return floor(x);
}
