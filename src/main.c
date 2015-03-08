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
