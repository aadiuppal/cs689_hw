#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* fixed heap size; if this had been a real allocator we would grow the heap as needed */

#define HEAP_SIZE	(1<<28)

struct block {
	size_t	n_bytes;
	struct block *next;
	unsigned long long int bytes[];
};

static unsigned char 	
	*my_heap = NULL, 
	*my_bump_ptr,
	*my_brk;

static int malloc_calls = 0;

static struct block *freelist = NULL;

/* here are some functions to print stuff. why can't we just use
 * printf? because printf uses malloc, but we need to be able to print within
 * malloc. 
 */

/* print a message to standard output */

void message (const char *s) {
	write (2, s, strlen (s));
}

/* print out a decimal integer */

void write_int (long long int x) {
	int digits[30];
	int i;
	for (i=0; i<30; i++) {
		digits[i] = x % 10;
		x /= 10;
	}
	int leading_zero = 1;
	for (i=29; i>=0; i--) {
		if (digits[i] || !leading_zero) {
			char buf[1];
			buf[0] = "0123456789abcdef"[digits[i]];
			write (2, buf, 1);
			leading_zero = 0;
		}
	}
	if (leading_zero) write (2, "0", 1);
}

void *malloc (size_t n_bytes) {
	/* pointer to the return value */

	struct block *r;

	/* if the heap is not initialized, initialize it */

	if (!my_heap) {

		/* extend the data segment by HEAP_SIZE bytes (should use mmap but sbrk is easier) */

		my_heap = sbrk (HEAP_SIZE);

		/* this should work */

		if (my_heap == (void *) -1) {
			message ("not enough memory!\n");
			exit (1);
		}

		/* compute the new break; we need this so we can tell if we are going over */

		my_brk = HEAP_SIZE + my_heap;

		/* initialize the bump pointer to the start of the heap */

		my_bump_ptr = my_heap;
	}

	/* measure */

	malloc_calls++;
	if (malloc_calls % 1000 == 0) {
		if (malloc_calls >= 400000) assert (0);
		write_int (malloc_calls);
		message (" calls to malloc; ");
		write_int (my_bump_ptr - my_heap);
		message (" bytes allocated; ");
		struct block *p;
		int bytes = 0;
		for (p=freelist; p; p=p->next) bytes += p->n_bytes;
		write_int (bytes);
		message (" bytes on the free list\n");
	}


	/* see if there is a free node we can return */

	if (freelist) {
		/* search the free list for the first fit */

		struct block *p, *q = NULL, *r = NULL;

		for (p=freelist; p; p=p->next) {

			/* for the first block that fits... */

			if (p->n_bytes >= n_bytes) {

				/* delete this node from the freelist, saving a pointer to it in r */

				r = p;
				if (!q) {
					/* first element of free list; pop it */
					freelist = freelist->next;
					break;
				} else {
					/* link over this node */
					q->next = p->next;
					break;
				}
			}

			/* q becomes the predecessor of p when we continue the loop */

			q = p;
		}
		if (r) return &r->bytes[0];
	} 

	/* make sure size of allocation is a multiple of 8 */

	while (n_bytes & 7) n_bytes++;

	/* r is the start of a new block of memory */

	r = (struct block *) my_bump_ptr;

	my_bump_ptr += n_bytes + sizeof (struct block);
	r->n_bytes = n_bytes;

	/* are we out of heap space? */

	if (my_bump_ptr > my_brk) return NULL;

	/* return pointer to allocate block without metadata */

	return &r->bytes[0];
}

/* free a block */

void free (void *p) {
	if (!p) {
		/* free semantics on NULL: no operation */

		return;
	}

	/* find the beginning of this block */
	struct block *b = p;
	b--;

	/* walk the free list to see if this is a double free */

	struct block *q;
	for (q=freelist; q; q=q->next) {
		if (q == b) {
			message ("double free!\n");
			return;
		}
	}
	b->next = freelist;
	freelist = b;
}

void *calloc (size_t count, size_t size) {
	int n = count * size;
	void *p = malloc (n);
	if (p) memset (p, 0, n);
	return p;
}

void *realloc (void *ptr, size_t size) {
	if (!ptr) return malloc (size);
	void *p = malloc (size);
	struct block *b = ptr;
	b--;
	memcpy (p, ptr, b->n_bytes);
	free (ptr);
	return p;
}
