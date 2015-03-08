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


static int hash_oa_insert_entry(entry *e, size_t size, size_t pos, void *key, size_t len, void *val);

hash *hash_oa_create(hash *h, size_t (*func)(void *, size_t), size_t size)
{
	// malloc buckets
	h->slots = calloc(size, sizeof (entry));
	if (h->slots == NULL) {
		free(h);
		return NULL;
	}
	
	// initialize hash
	h->func = func;
	h->size = size;
	h->load = 0;
	
	return h;
}

static int hash_oa_insert_entry(entry *e, size_t size, size_t pos, void *key, size_t len, void *val)
{
	size_t i;
	
	// find first free entry starting from pos
	for (i = pos; i < pos + size; i++) {
		if (e[i % size].len == 0) {
			e[i % size].key = key;
			e[i % size].len = len;
			e[i % size].val = val;
			
			return 0;
		}
		
		// key already inserted?
		if (e[i % size].len == len)
			if (!memcmp(e[i % size].key, key, len))
				return -1;
	}
	
	return -1;
}

int hash_oa_insert(hash *h, void *key, size_t len, void *val)
{
	size_t pos;
	
	// compute slot number
	pos = h->func(key, len) % h->size;
	
	if (!hash_oa_insert_entry(h->slots, h->size, pos, key, len, val)) {
		h->load++;
		return 0;
	} else {
		return -1;
	}
}

void *hash_oa_lookup(hash *h, void *key, size_t len)
{
	size_t i, pos;
	entry *slots = (entry *)h->slots;
	
	// compute slot number
	pos = h->func(key, len) % h->size;
	
	// find first matching entry starting from pos
	for (i = pos; i < pos + h->size; i++)
		if (slots[i % h->size].len == len)
			if (!memcmp(slots[i % h->size].key, key, len))
				return slots[i % h->size].val;
	
	return NULL;
}

int hash_oa_rehash(hash *h, size_t size) {
	size_t i, pos, len;
	void *key, *val;
	entry *slots = (entry *)h->slots;
	entry *new_slots;
	
	// nothing to rehash
	if (size == h->size)
		return 0;
	
	new_slots = calloc(size, sizeof *new_slots);
	if (new_slots == NULL)
		return -1;

	for (i = 0; i < h->size; i++) {
		if (slots[i].len) {
			key = slots[i].key;
			len = slots[i].len;
			val = slots[i].val;
			
			pos = h->func(key, len) % size;
			if (hash_oa_insert_entry(new_slots, size, pos, key, len, val)) {
				free(new_slots);
				return -1;
			}
			    
		}
	}
	
	free(h->slots);
	h->slots = new_slots;
	h->size = size;
	
	return 0;
}

void hash_oa_print(hash *h)
{
	size_t i;
	entry *slots = (entry *)h->slots;

	for (i = 0; i < h->size; i++) {
		printf("%lu", i);
		
		if (slots[i].len) {
			printf(" <- %lu: (%s) -> (%s)", h->func(slots[i].key, slots[i].len) % h->size, slots[i].key, slots[i].val);

		}
		
		printf("\n");
	}
}

void hash_oa_destroy(hash *h)
{
	free(h->slots);
	free(h);
}


