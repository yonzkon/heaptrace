#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#define __USE_GNU
#include <dlfcn.h>

static int initialized;

static void *(*libc_malloc)(size_t bytes);
static void (*libc_free)(void *ptr);
static void *(*libc_realloc)(void *oldmem, size_t bytes);
static void *(*libc_calloc)(size_t n, size_t elem_size);

__attribute__((constructor))
static void heaptrace_initialize(void)
{
	initialized = -1;
	libc_malloc = dlsym(RTLD_NEXT, "malloc");
	libc_free = dlsym(RTLD_NEXT, "free");
	libc_realloc = dlsym(RTLD_NEXT, "realloc");
	libc_calloc = dlsym(RTLD_NEXT, "calloc");

	if (!libc_malloc || !libc_free || !libc_realloc || !libc_calloc) {
		fprintf(stderr, "heaptrace initialized failed!\n");
		exit(EXIT_FAILURE);
	}

	initialized = 1;
	fprintf(stderr, "heaptrace initialized success!\n");
	fprintf(stderr, "libc_malloc is %p\n", libc_malloc);
	fprintf(stderr, "libc_free is %p\n", libc_free);
	fprintf(stderr, "libc_realloc is %p\n", libc_realloc);
	fprintf(stderr, "libc_calloc is %p\n", libc_calloc);
}

void *malloc(size_t bytes)
{
	void *rc;

	if (initialized <= 0) {
		if (initialized == -1)
			return NULL;
		heaptrace_initialize();
	}

	if (!libc_malloc) {
		fprintf(stderr, "heaptrace initialized failed!\n");
		exit(EXIT_FAILURE);
	}

	rc = libc_malloc(bytes);
	if (rc) {
		fprintf(stderr, "malloc %ld bytes => %p\n", bytes, rc);
	}

	return rc;
}

void free(void *ptr)
{
	if (!ptr) return;

	if (initialized <= 0) {
		if (initialized == -1)
			return;
		heaptrace_initialize();
	}


	fprintf(stderr, "free %p\n", ptr);
	libc_free(ptr);
}

void *realloc(void *oldmem, size_t bytes)
{
	void *rc;

	if (initialized <= 0) {
		if (initialized == -1)
			return NULL;
		heaptrace_initialize();
	}

	rc = libc_realloc(oldmem, bytes);
	if (rc) {
		fprintf(stderr, "realloc %ld bytes => %p, oldmem => %p\n",
		        bytes, rc, oldmem);
	}

	return rc;
}

void *calloc(size_t n, size_t elem_size)
{
	void *rc;

	if (initialized <= 0) {
		if (initialized == -1)
			return NULL;
		heaptrace_initialize();
	}

	rc = libc_calloc(n, elem_size);
	if (rc) {
		fprintf(stderr, "malloc %ld bytes => %p\n", n * elem_size, rc);
	}

	return rc;
}
