/*
 * MurmurHash64B.cpp
 *
 *  Created on: 2013-5-10
 *      Author: liuyi
 */

#include "MurmurHash64.h"

const unsigned int SALT_CONSTANT = 0x97c29b3a;
// 64-bit hash for 64-bit platforms

static uint64_t MurmurHash64A(const void * key, uint16_t len, unsigned int seed) {
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *) key;
	const uint64_t * end = data + (len / 8);

	while (data != end) {
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*) data;

	switch (len & 7) {
	case 7:
		h ^= uint64_t(data2[6]) << 48;
	case 6:
		h ^= uint64_t(data2[5]) << 40;
	case 5:
		h ^= uint64_t(data2[4]) << 32;
	case 4:
		h ^= uint64_t(data2[3]) << 24;
	case 3:
		h ^= uint64_t(data2[2]) << 16;
	case 2:
		h ^= uint64_t(data2[1]) << 8;
	case 1:
		h ^= uint64_t(data2[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

// 64-bit hash for 32-bit platforms

static uint64_t MurmurHash64B(const void * key, uint16_t len, unsigned int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int * data = (const unsigned int *) key;

	while (len >= 8) {
		unsigned int k1 = *data++;
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m;
		k2 ^= k2 >> r;
		k2 *= m;
		h2 *= m;
		h2 ^= k2;
		len -= 4;
	}

	if (len >= 4) {
		unsigned int k1 = *data++;
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;
	}

	switch (len) {
	case 3:
		h2 ^= ((unsigned char*) data)[2] << 16;
	case 2:
		h2 ^= ((unsigned char*) data)[1] << 8;
	case 1:
		h2 ^= ((unsigned char*) data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18;
	h1 *= m;
	h2 ^= h1 >> 22;
	h2 *= m;
	h1 ^= h2 >> 17;
	h1 *= m;
	h2 ^= h1 >> 19;
	h2 *= m;

	uint64_t h = h1;

	h = (h << 32) | h2;

	return h;
}
extern uint64_t MurmurHash64 (const void * key, uint16_t len)
{
	uint64_t result;
#ifdef BUILD_64
	result = MurmurHash64A(key,len,SALT_CONSTANT);
#else
	result = MurmurHash64B(key,len,SALT_CONSTANT);
#endif
	return result;
}
