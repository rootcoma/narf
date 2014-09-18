#include "stdlib.h"

size_t
digits10(uint32_t value)
{
    size_t length = 1;
    uint32_t tmp = value;
    while ((tmp /=10) > 0)
        length++;
    return length;
}

size_t
facebook_uint32_to_str(uint32_t value, char *dst)
{
    const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";
    const size_t length = digits10(value);
    size_t next = length - 1;
    while (value >= 100) {
        int i = (value % 100) * 2;
        value /= 100;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
        next -= 2;
    }
    // Handle last 1-2 digits
    if (value < 10) {
        dst[next] = '0' + value;
    } else {
        int i = value * 2;
        dst[next] = digits[i + 1];
        dst[next - 1] = digits[i];
    }
    return length;
}
