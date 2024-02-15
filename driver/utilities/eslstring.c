#include "eslstring.h"
#include "stm32f2xx_esl_uart.h"

UInt32 stringlen(const char* string)
{
    UInt32 len = 0;
    while (string[len] != '\0')
    {
        len++;
    }
    return len;
}

void stringcopy(char* dst, const char* src)
{
    UInt32 dst_len = stringlen(dst);
    UInt32 src_len = stringlen(src);

    stringset(dst, '\0', dst_len);

    for (UInt32 i = 0; i < src_len; i++)
    {
        dst[i] = src[i];
    }
}

void stringcat(char* dst, const char* src)
{
    UInt32 dst_length = stringlen(dst);
    UInt32 src_length = stringlen(src);
    for (UInt32 i = 0; i < src_length; i++)
    {
        dst[dst_length - 1 + i] = src[i];
    }
}

void stringset(char* string, char c, UInt32 len)
{
    for (UInt32 i = 0; i < len; i++)
        string[i] = c;
}

void print(const char* string)
{
    ESL_UARTx_Write(UART2, (UInt8*)string, stringlen(string), 5000);
}