#include <stdio.h>

int Function(int Arg)
{
    printf("Hello, world!\n");
    
    
    printf("Hello, world!\n");
    
    
    printf("Hello, world!\n");
    
    
    printf("Hello, world!\n");
    
    
    printf("Hello, world!\n");
    
    
    printf("Hello, world!\n");
    
    int k = 0;//加算用
    int l = 0;//現状用途無し
    for(int i=0;i<10;i++)
    {
        i = (k++) + l;
    }
    
    return Arg;
}
