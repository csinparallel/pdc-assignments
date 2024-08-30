/** md5.c
 * Simple MD5 implementation
 * https://gist.github.com/creationix/4710780#file-md5-c
 * "This code is licensed MIT and free to use."
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University.
 * Modified for use on GPU (CUDA device)
 */

#include "helperFunctions.hpp" // for PIN_DIGITS

// declare that these functions should be run on the device, not the host
#pragma acc routine(LEFTROTATE)
#pragma acc routine(uint8ToHexStr)
#pragma acc routine(hashUsingMD5)

uint32_t LEFTROTATE(uint32_t x, int c)
{
    return ((x) << (c)) | ((x) >> (32 - (c)));
}

/** used to manually convert from uint8_t to hex char
 * Done manually since built in functions cannot be used on a CUDA device.
 * Code received from ChatGPT on 5/8/24.
 */
void uint8ToHexStr(uint8_t *uinthash, char *currentHash, int inSize)
{
    const char *hexChars = "0123456789abcdef"; // Array of hexadecimal characters
    for (int i = 0; i < inSize; ++i)
    {
        currentHash[i * 2] = (hexChars[uinthash[i] >> 4]);         // first nibble
        currentHash[(i * 2) + 1] = (hexChars[uinthash[i] & 0x0F]); // second nibble
    }
}

void hashUsingMD5(const char *initial_msgStr, char *outputHash)
{
    // md5 uses uint8_t*, helperFunctions and pinFinder use char*
    // Conversion from char* to uint8*:
    uint8_t initial_msg[PIN_BUF_SIZE] = {0};
    uint8_t uintHash[16]; // used to store each part of the hash (2 digit hex)
    for (int i = 0; i < PIN_BUF_SIZE; ++i)
    {
        initial_msg[i] = (uint8_t)initial_msgStr[i];
    }

    // These vars will contain the hash
    uint32_t h0, h1, h2, h3;

    // Message (to prepare)
    // used to hold message information local to this device thread.
    // to declare the array locally on a device, it must have a constant length.
    // msg_len == ((((((PIN_DIGITS - 1) + 8) / 64) + 1) * 64) - 8) + 64; == 135 when PIN_DIGITS = 8
    uint8_t msg[((((((PIN_DIGITS - 1) + 8) / 64) + 1) * 64) - 8) + 64] = {0}; // also appends "0" bits // (we alloc also 64 extra bytes...)

    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating

    // r specifies the per-round shift amounts

    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as constants// Initialize variables:
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;

    // Pre-processing: adding a single 1 bit
    // append "1" bit to message
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */

    // Pre-processing: padding with zeros
    // append "0" bit until message length in bit ≡ 448 (mod 512)
    // append length mod (2 pow 64) to message

    int new_len = ((((PIN_DIGITS + 8) / 64) + 1) * 64) - 8;

    memcpy(msg, initial_msg, PIN_DIGITS);
    msg[PIN_DIGITS] = 128; // write the "1" bit

    uint32_t bits_len = 8 * PIN_DIGITS; // note, we append the len
    memcpy(msg + new_len, &bits_len, 4); // in bits at the end of the buffer

    // Process the message in successive 512-bit chunks:
    // for each 512-bit chunk of message:
    int offset;
    for (offset = 0; offset < new_len; offset += (512 / 8))
    {

        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *)(msg + offset);

        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;

        // Main loop:
        uint32_t i;
        for (i = 0; i < 64; ++i)
        {
            uint32_t f, g;

            if (i < 16)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            }
            else
            {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }

        // Add this chunk's hash to result so far:

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

    // Store result in "result"
    uint8_t *result[4];
    result[0] = (uint8_t *)&h0;
    result[1] = (uint8_t *)&h1;
    result[2] = (uint8_t *)&h2;
    result[3] = (uint8_t *)&h3;

// results -> hash
#pragma acc loop seq independent
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            uintHash[(i * 4) + j] = result[i][j];
        }
    }

    uint8ToHexStr(uintHash, outputHash, 16); // go from len 16 hex representation to len 32 string
}