#include <stdio.h>
#include <stdlib.h>

int ceil(double x)
{
    int x_int = (int)x;
    if ((double)x_int == x)
    {
        return x_int;
    }
    return x_int + 1;
}

double sqrt(int x)
{
    int first = 0, last = x;
    int mid;
    double res;

    while (first <= last)
    {
        mid = (first + last) / 2;

        if ((mid * mid) == x)
        {
            res = mid;
            break;
        }
        if ((mid * mid) > x)
        {
            last = mid - 1;
        }
        else
        {
            res = first;
            first = mid + 1;
        }
    }

    double increment = 0.1;
    for (int i = 0; i < 6; i++)
    {
        while (res * res <= x)
        {
            res += increment;
        }
        res = res - increment;
        increment = increment / 10;
    }
    return res;
}

int floor(double x)
{
    int x_int = (int)x;
    if ((double)x_int == x || x_int > 0)
    {
        return x_int;
    }
    return x_int - 1;
}

int main(void){
    int v = floor(3/4.0);
    double s = sqrt(7);
    printf("%d\n", v); 
    printf("%f\n", s); 
}