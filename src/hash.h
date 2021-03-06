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


#ifndef HASH_H
#define HASH_H


// collision resolution types
#define SEPARATE_CHAINING 1
#define OPEN_ADDRESSING   2


// common data structures
typedef struct {
	void *key;
	size_t len;
	void *val;
} entry;


typedef struct hash_struct hash;
struct hash_struct {
	int type;
	size_t (*func)(void *, size_t);
	void *slots;
	size_t size;
	size_t load;

	hash  *(*create)(hash *, size_t (*func)(void *, size_t), size_t);
	int    (*insert)(hash *, void *, size_t, void *);
	void  *(*lookup)(hash *, void *, size_t);
	int    (*rehash)(hash *, size_t);
	int    (*delete)(hash *, void *, size_t);
	void   (*print)(hash *);
	void   (*destroy)(hash *);
};


// virtual hash handling functions
hash  *hash_create(int, size_t (*func)(void *, size_t), size_t);
int    hash_insert(hash *, void *, size_t, void *);
void  *hash_lookup(hash *, void *, size_t);
int    hash_rehash(hash *, size_t);
int    hash_delete(hash *, void *, size_t);
void   hash_print(hash *);
void   hash_destroy(hash *);


// common hash handling functions
size_t hash_size(hash *h);
size_t hash_load(hash *h);
double hash_loadfactor(hash *);


// hash function collection
size_t hash_djb(void *, size_t);


#endif  /* ! HASH_H */
