#pragma once

extern "C"
{

#include <stddef.h>

#define SHA1_BLOCK_SIZE 64
#define SHA1_HASH_SIZE 20

size_t hmac_sha1(const void* key, const size_t keylen, const void* data, const size_t datalen, void* out, const size_t outlen);
}
