/*************************************************************************
> File Name: printf_memaddr.c
> Author: luckxiang 
> Created Time: 2015年12月21日 星期一 13时40分14秒
************************************************************************/

//非常标准的两个打印函数，稍作修改可以用在任何地方，掌握这两个函数调试问题事半功倍。

#include<stdio.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

void dump_log(uint8_t *buf,  uint32_t phy, int size)
{
    int len, i, j, c;
    #define PRINT(...) do { printf(__VA_ARGS__);  } while(0)

    for(i = 0; i < size; i += 16)
    {
        len = size - i;
        if(len > 16)
        len = 16;
        PRINT("%08x ", i+phy);
        for(j = 0; j < 16; j++)
        {
            if(j < len)
                PRINT(" %02x", buf[i+j]);
            else
                PRINT("   ");
        }
        PRINT(" ");
        for(j = 0; j < len; j++)
        {
            c = buf[i+j];
            if (c < ' ' || c > '~')
            c = '.';
            PRINT("%c", c);
        }
        PRINT("\n");
    }
    #undef PRINT
}

void dump_reg(uint32_t *ioremap, uint32_t phy, int size)
{
    int len, i, j;
    #define PRINT(...) do { printf(__VA_ARGS__);  } while(0)

    PRINT("\n");
    for(i = 0; i < size; i += 4)
    {
        len = size - i;
        if(len > 4)
        len = 4;
        PRINT("%08x: ", (uint32_t)((i << 2) + phy));
        for(j = 0; j < 4; j++)
        {
            if(j < len)
                PRINT(" %08x", ioremap[i+j]);
            else
                PRINT("   ");
        }
        PRINT("\n");
    }
    PRINT("\n");
    #undef PRINT
}

int main(int argc, char *argv[])
{
    int p = 10;
    unsigned char d = 3;
    dump_reg(&p, (unsigned int)&p, 24);
    dump_log(&d, (unsigned int)&d, 2400);
    return 0;
}
