#pragma once

extern "C"
{
#include <stdint.h>

int sha1digest(uint8_t* digest, char* hexdigest, const uint8_t* data, size_t databytes);
}
