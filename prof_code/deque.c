/* 
 * partial implementation of deque data structure with linked allocation
 * code by Daniel A. Jimenez
 */
#include <stdio.h>
#include <stdlib.h>

struct deque_node {
	int X;
	struct deque_node *next;
};

struct deque_node *new_deque_node (void) {
	return malloc (sizeof (struct deque_node));
}

/* this function inserts a new node at the beginning of the list */

void insert_front (struct deque_node **L, int X) {
	struct deque_node *p = new_deque_node ();
	p->next = *L;
	p->X = X;
	*L = p;
} 

/* this function inserts a new node at the end of the list (using recursion) */

void insert_rear_recursive (struct deque_node **L, int X) {
	if (*L == NULL)
		insert_front (L, X);
	else 
		insert_rear_recursive (&(*L)->next, X);
} 

/* this function inserts a new node at the end of the list */

void insert_rear (struct deque_node **L, int X) {
	/* find the pointer to the next field of the last element of the list,
	 * or just use L if the list is empty
	 */

	while (*L) L = &(*L)->next;

	/* put the element here */

	insert_front (L, X);
}

/* this function deletes the node at the beginning of the list */

int delete_front (struct deque_node **L) {
	if (*L == NULL) {
		printf ("underflow!\n");
		exit (1);
	} else {
		int X = (*L)->X;
		struct deque_node *p = (*L)->next;
		free (*L);
		*L = p;
		return X;
	}
}

/* this function deletes the entire list */

void delete_list (struct deque_node **L) {
	struct deque_node *p = *L;
	*L = NULL;
	while (p) {
		struct deque_node *q = p->next;
		free (p);
		p=q;
	}
}

/* go through each item in the list in order, printing it */

void traverse_list (struct deque_node *L) {
	for (; L; L=L->next) printf ("%d\n", L->X);
}

/* use recursion to print the list in reverse order */

void traverse_list_backwards (struct deque_node *L) {
	if (L) {
		traverse_list_backwards (L->next);
		printf ("%d\n", L->X);
	}
}

/* use a stack to print the list in reverse order */

void print_as_bignum (struct deque_node *L) {
	struct deque_node *S = NULL;

	for (; L; L=L->next) insert_front (&S, L->X);
	while (S) printf ("%d", delete_front (&S));
}

/* print the list as a decimal integer from left to right */

void print_as_bignum_recursive (struct deque_node *L) {
	if (L) {
		print_as_bignum_recursive (L->next);
		printf ("%d", L->X);
	}
}

/* treat lists as arbitrary precision integers, add two together to produce a third */

struct deque_node *add (struct deque_node *A, struct deque_node *B) {
	struct deque_node *C = NULL;

	int carry = 0;
	for (; A && B; (A=A->next),(B=B->next)) {
		int sum = A->X + B->X + carry;
		insert_rear (&C, sum % 10);
		carry = sum >= 10;
	}
	if (B) A = B;
	for (; A; A=A->next) {
		int sum = A->X + carry;
		insert_rear (&C, sum % 10);
		carry = sum >= 10;
	}
	if (carry) insert_rear (&C, 1);
	return C;
}

/* do some sums */

void do_sums (void) {
	struct deque_node *A = NULL, *B = NULL, *C;
	int i;

	insert_front (&A, 0);
	insert_front (&B, 1);
	for (i=2; i<=200; i++) {
		C = add (A, B);
		printf ("F(%d) = ", i);
		print_as_bignum (C);
		printf ("\n");
		delete_list (&A);
		A = B;
		B = C;
	}
}

int main () {
	do_sums ();
	return 0;
}
