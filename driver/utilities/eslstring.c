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

    memset(dst, '\0', dst_len);

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

void* memset(void* s, UInt32 c, UInt32 len)
{
    UInt8* p = s;
    while (len-- > 0) 
    {
        *p++ = (UInt8)c;
    }
    return s;
}

void uint_to_string(const UInt32 num, char* str)
{
    UInt32 i = 0;
    UInt32 num_buf = num;

    // Convert the number to its ASCII representation
    do 
    {
        str[i++] = num_buf % 10 + '0';
        num_buf /= 10;
    } while (num_buf > 0);

    // Add the null terminator
    str[i] = '\0';

    // Reverse the string
    reverse_string(str);
}

void int_to_string(const Int32 num, char* str)
{
    UInt32 i = 0;
    UInt8 is_negative = 0;
    Int32 num_buf = num;

    // Handle negative numbers
    if (num_buf < 0) 
    {
        is_negative = 1;
        num_buf = -num_buf;
    }

    // Convert the number to its ASCII representation
    do 
    {
        str[i++] = num_buf % 10 + '0';
        num_buf /= 10;
    } while (num_buf > 0);

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
    ESL_UARTx_Transmit(&uart2, (UInt8*)string, stringlen(string), 5000);
}

void println(char* string)
{
    stringcat(string, "\n\r");
    ESL_UARTx_Transmit(&uart2, (UInt8*)string, stringlen(string), 5000);
}
