/*
   hmac_sha256.c
   Originally written by https://github.com/h5p9sl
 */

//modified to use sha1
#include "hmacsha1.h"
#include "teeny-sha1.h"
extern "C"
{
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


 /* LOCAL FUNCTIONS */

 // Concatenate X & Y, return hash.
static void* H(const void* x, const size_t xlen, const void* y, const size_t ylen, void* out, const size_t outlen);

// Wrapper for teeny-sha1
static void* sha1(const void* data,  const size_t datalen,  void* out,  const size_t outlen);

// Declared in hmac_sha1.h
size_t hmac_sha1(const void* key, const size_t keylen, const void* data, const size_t datalen, void* out, const size_t outlen) 
{
    uint8_t k[SHA1_BLOCK_SIZE];
    uint8_t k_ipad[SHA1_BLOCK_SIZE];
    uint8_t k_opad[SHA1_BLOCK_SIZE];
    uint8_t ihash[SHA1_HASH_SIZE];
    uint8_t ohash[SHA1_HASH_SIZE];
    size_t sz;
    int i;

    memset(k, 0, sizeof(k));
    memset(k_ipad, 0x36, SHA1_BLOCK_SIZE);
    memset(k_opad, 0x5c, SHA1_BLOCK_SIZE);

    if (keylen > SHA1_BLOCK_SIZE) {
        // If the key is larger than the hash algorithm's
        // block size, we must digest it first.
        sha1(key, keylen, k, sizeof(k));
    }
    else {
        memcpy(k, key, keylen);
    }

    for (i = 0; i < SHA1_BLOCK_SIZE; i++) {
        k_ipad[i] ^= k[i];
        k_opad[i] ^= k[i];
    }

    // Perform HMAC algorithm: ( https://tools.ietf.org/html/rfc2104 )
    //      `H(K XOR opad, H(K XOR ipad, data))`
    H(k_ipad, sizeof(k_ipad), data, datalen, ihash, sizeof(ihash));
    H(k_opad, sizeof(k_opad), ihash, sizeof(ihash), ohash, sizeof(ohash));

    sz = (outlen > SHA1_HASH_SIZE) ? SHA1_HASH_SIZE : outlen;
    memcpy(out, ohash, sz);
    return sz;
}

static void* H(const void* x, const size_t xlen, const void* y, const size_t ylen, void* out, const size_t outlen) 
{
    void* result = NULL;
    size_t buflen = (xlen + ylen);
    uint8_t* buf = (uint8_t*)malloc(buflen);
    if (buf)
    {
        memcpy(buf, x, xlen);
        memcpy(buf + xlen, y, ylen);
        result = sha1(buf, buflen, out, outlen);

        free(buf);
    }
    return result;
}

static void* sha1(const void* data, const size_t datalen, void* out, const size_t outlen) 
{
    size_t sz;
    uint8_t hash[SHA1_HASH_SIZE];
    sha1digest(hash, NULL, (uint8_t*)data, datalen);

    sz = (outlen > SHA1_HASH_SIZE) ? SHA1_HASH_SIZE : outlen;
    return memcpy(out, hash, sz);
}
}