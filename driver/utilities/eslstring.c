#include "eslstring.h"
#include "stm32f2xx_esl_uart.h"
#include "uart.h"

static void reverse_string(char* str)
{
    UInt32 len = stringlen(str);
    for (UInt32 j = 0; j < len / 2; j++) 
    {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}

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
    for (UInt32 i = 0; i <= src_length; i++)
    {
        dst[dst_length + i] = src[i];
    }

    dst[dst_length + src_length] = '\0';
}

void stringset(char* string, char c, UInt32 len)
{
    for (UInt32 i = 0; i < len; i++)
        string[i] = c;
}

void uint_to_string(UInt32 num, char* str)
{
    UInt32 i = 0;

    // Convert the number to its ASCII representation
    do 
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the null terminator
    str[i] = '\0';

    // Reverse the string
    reverse_string(str);
}

void int_to_string(Int32 num, char* str)
{
    UInt32 i = 0;
    UInt8 is_negative = 0;

    // Handle negative numbers
    if (num < 0) 
    {
        is_negative = 1;
        num = -num;
    }

    // Convert the number to its ASCII representation
    do 
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (is_negative) 
    {
        str[i++] = '-';
    }

    // Add the null terminator
    str[i] = '\0';

    // Reverse the string
    reverse_string(str);
}

void print(const char* string)
{
    ESL_UARTx_Write(&uart2, (UInt8*)string, stringlen(string), 5000);
}

void println(char* string)
{
    stringcat(string, "\n\r");
    ESL_UARTx_Write(&uart2, (UInt8*)string, stringlen(string), 5000);
}
