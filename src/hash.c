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

