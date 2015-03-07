/*  Copyright (c) 2015, Philip Busch <vzxwco@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "hash_open_addressing.h"


int main()
{
	hash *h;
	
	h = hash_create(OPEN_ADDRESSING, hash_djb, 10);
	if (h == NULL)
		printf("creating hash failed\n");
	
	puts("adding 1");
	if (hash_insert(h, "foo", 3, "bar"))
		printf("1 failed\n");
	
	puts("adding 2");
	if (hash_insert(h, "111klsdjfslk", 3, "123"))
		printf("2 failed\n");

	puts("adding 3");
	if (hash_insert(h, "sldkfj lksdj", 3, "456789"))
		printf("3 failed\n");

	puts("adding 4");
	if (hash_insert(h, "sdkjf lsdkfjslkdjf slkdjf lskdjflskdjf", 3, "bsdsdfsdfsdfsdfar"))
		printf("4 failed\n");

	puts("adding 5");
	if (hash_insert(h, "s", 3, "y"))
		printf("5 failed\n");

	puts("adding 6");
	if (hash_insert(h, "yxcv", 3, "yxcv"))
		printf("6 failed\n");
	
	puts("adding 7");
	if (hash_insert(h, "1yxcv", 3, "yxcv"))
		printf("6 failed\n");
	
	puts("adding 8");
	if (hash_insert(h, "2yxcv", 3, "yxcv"))
		printf("6 failed\n");
	
	puts("adding 9");
	if (hash_insert(h, "3yxcv", 3, "yxcv"))
		printf("6 failed\n");
	
	puts("adding 10");
	if (hash_insert(h, "4yxcv", 3, "yxcv"))
		printf("6 failed\n");
	
	printf("added 10 items\n");
	
	hash_print(h);
	
	printf("%s\n", (char *)hash_lookup(h, "foo", 3));
	
	printf("load: %f\n", hash_loadfactor(h));
	
	puts("rehashing...\n");
	
	if (hash_rehash(h, 25)) {
		printf("error rehashing!\n");
	}
	
	hash_print(h);
	printf("%s\n", (char *)hash_lookup(h, "foo", 3));
	
	printf("load: %f\n", hash_loadfactor(h));
	
	
	hash_destroy(h);
	
	// your kung-fu is old and now you must die!!
	return EXIT_SUCCESS;
}

hash *hash_create(int type, size_t (*func)(void *, size_t), size_t size)
{
	// malloc hash
	hash *h = malloc(sizeof *h);
	if (h == NULL)
		return NULL;
	
	switch (type) {
		case OPEN_ADDRESSING:
			h->create  = hash_oa_create;
			h->insert  = hash_oa_insert;
			h->lookup  = hash_oa_lookup;
			h->rehash  = hash_oa_rehash;
			h->print   = hash_oa_print;
			h->destroy = hash_oa_destroy;
			break;
		case SEPARATE_CHAINING:
			break;
		default:
			free(h);
			return NULL;
	}

	h->create(h, func, size);
	
	// initialize hash
	h->func = func;
	h->size = size;
	h->load = 0;
	
	return h;
}

int hash_insert(hash *h, void *key, size_t len, void *val)
{
	return h->insert(h, key, len, val);
}

void *hash_lookup(hash *h, void *key, size_t len)
{
	return h->lookup(h, key, len);
}

double hash_loadfactor(hash *h)
{
	return (double) h->load / h->size;
}

size_t hash_load(hash *h)
{
	return h->load;
}

size_t hash_size(hash *h)
{
	return h->size;
}

int hash_rehash(hash *h, size_t size) {
	return h->rehash(h, size);
}

void hash_print(hash *h)
{
	return h->print(h);
}

void hash_destroy(hash *h)
{
	return h->destroy(h);
}



/*
 *  Hash function collection starts here
 */

size_t hash_djb(void *key, size_t len)
{
	size_t i, hash = 5381;
	int c;
	
	for (i = 0; i < len; i++) {
		c = ((char *)key)[i];
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	
	return hash;
}

