#include <stdio.h>
#include <stdlib.h>

typedef int record_type;

int record_compare (record_type a, record_type b) {
	if (a < b) return -1;
	if (a > b) return +1;
	return 0;
}

int record_swap (record_type *a, record_type *b) {
	record_type t;
	t = *a;
	*a = *b;
	*b = t;
}

/* bubble sort (Knuth calls it exchange sort) */

void bubble_sort (record_type v[], int n) {
	int i;
	record_type t;

	/* loop forever */

	for (;;) {

		/* initially we haven't swapped anything */

		int swapped = 0;
		for (i=1; i<n; i++) {

			/* compare adjacent elements */

			int c = record_compare (v[i-1], v[i]);
			if (c > 0) {
				record_swap (&v[i-1], &v[i]);

				/* ok now we've swapped */

				swapped = 1;
			}
		}

		/* if we made it through all the elements without swapping then we're done */

		if (!swapped) break;
	}
}

/* some prototypes */

void merge (record_type [], int, int, int, record_type *, record_type *);
void merge_sort_recursive (record_type [], int, int, record_type *, record_type *);

/* merge sort invocation */

void merge_sort (record_type v[], int n) {

	/* make temporary arrays to hold the subarrays to be merged */

	record_type *v1 = malloc ((n/2+1) * sizeof (record_type));
	record_type *v2 = malloc ((n/2+1) * sizeof (record_type));

	/* invoke algorithm */

	merge_sort_recursive (v, 0, n, v1, v2);

	/* free up those arrays */

	free (v2);
	free (v1);
}

/* sort the (sub)array v from start to end */

void merge_sort_recursive (record_type v[], int start, int end, record_type *v1, record_type *v2) {
	int	middle;		/* the middle of the subarray */

	/* two base cases: */
	/* 1. no elements to sort */

	if (start == end) return;	

	/* 2. one element; already sorted! */

	if (start == end - 1) return;	

	/* find the middle of the array, splitting it into two subarrays */

	middle = (start + end) / 2;

	/* sort the subarray from start..middle */

	merge_sort_recursive (v, start, middle, v1, v2);

	/* sort the subarray from middle..end */

	merge_sort_recursive (v, middle, end, v1, v2);

	/* merge the two sorted halves */

	merge (v, start, middle, end, v1, v2);
}

/* merge the subarray v[start..middle] with v[middle..end], placing the
 * result back into v.
 */

void merge (record_type v[], int start, int middle, int end, record_type *v1, record_type *v2) {
	int	v1_n, v2_n, v1_index, v2_index, i;

	/* number of elements in first subarray */

	v1_n = middle - start;

	/* number of elements in second subarray */

	v2_n = end - middle;

	/* fill v1 and v2 with the elements of the first and second
	 * subarrays, respectively
	 */
	for (i=0; i<v1_n; i++) v1[i] = v[start + i];
	for (i=0; i<v2_n; i++) v2[i] = v[middle + i];

	/* v1_index and v2_index will index into v1 and v2, respectively... */

	v1_index = 0;
	v2_index = 0;

	/* ... as we pick elements from one or the other to place back
	 * into v
	 */
	for (i=0; (v1_index < v1_n) && (v2_index < v2_n); i++) {

		/* current v1 element less than current v2 element? */

		if (record_compare (v1[v1_index], v2[v2_index]) < 0)

			/* if so, this element belong as next in v */

			v[start + i] = v1[v1_index++];
		else

			/* otherwise, the element from v2 belongs there */

			v[start + i] = v2[v2_index++];
	}

	/* clean up; either v1 or v2 may have stuff left in it */

	for (; v1_index < v1_n; i++) v[start + i] = v1[v1_index++];
	for (; v2_index < v2_n; i++) v[start + i] = v2[v2_index++];
}

/* Quicksort: sort the array v from v[low]..v[high] */

void quicksort (record_type v[], int low, int high) {

	/* start off with indices to the extreme ends of the subarray */

	int i = low, j = high;

	/* choose an element whose value will divide the array into two partitions */

	record_type pivot = v[low + (high-low)/2];

	/* move i and j toward each other until they meet */

	while (i <= j) {

		/* find the first element i starting from the low end such that v[i] exceeds the pivot */

		while (record_compare (v[i], pivot) < 0) i++;

		/* find the first element j starting from the high end such that v[j] does not exceed the pivot */

		while (record_compare (v[j], pivot) > 0) j--;

		/* if we have found such elements, then they are out of order; swap them */

		if (i <= j) {
			record_swap (&v[i], &v[j]);
			i++;
			j--;
		}
	}

	/* now we have two subarrays: low..j and i..high such that
	 * everything in the first subarray is less than everything else in
	 * the second subarray. recursively sort them. no need to merge them
	 * with each other, since we know everything in one is less than everything 
	 * in the other
	 */

	if (low < j)
		quicksort (v, low, j);
	if (i < high) 
		quicksort (v, i, high);
}

#define N	10000

#define LEFT(x) ((x)*2)
#define RIGHT(x) ((x)*2+1)
#define PARENT(x) ((x)/2)

/* heap property: all children of a node are less than or equal to that node */

/* heapify: assuming left and right trees of i are heaps, make i the root of a heap */
/* analysis of heapify: in the worst case, each call to heapify (i) takes O(log i) time
   because it has to go down the tree from i to a leaf
 */
void heapify (int A[], int i, int n) {
	int l = LEFT(i);
	int r = RIGHT(i);
	int largest;

	/* find the largest of i, left(i), and right(i) */

	if (l < n && A[l] > A[i])
		largest = l;
	else
		largest = i;
	if (r < n && A[r] > A[largest])
		largest = r;

	/* if i is not the largest, we need to swap i with the largest and heapify that
	 * subheap 
	 */
	if (largest != i) {
		int t = A[i];
		A[i] = A[largest];
		A[largest] = t;
		heapify (A, largest, n);
	}
}

/* make array A into a heap */
/* analysis of buildheap: it takes the sum as i goes from 1 to n/2 of O(log i),
 * since it is doing n/2 heapify's. this is bound from above by n log n, so it is O(n log n).
*/
void buildheap (int A[], int n) {
	int i;

	/* make every internal node into a heap (leaf nodes are trivially heaps) */

	for (i=n/2-1; i>=0; i--) 
		heapify (A, i, n);
}

/* heap sort array A */

/* analysis of heapsort: it does buildheap at O(n log n), plus n more calls to heapify(O(n)), which
 * is another O(n log n). So it is O(n log n).
 */

void heapsort (int A[], int n) {
	int i;

	/* make A into a heap */

	buildheap (A, n);

	/* for each element in the array ... */

	for (i=n-1; i>=1; i--) {

		/* A[0] is the maximum element in the heap from 0..n;
		 * swap it with the last element 
		 */

		int t = A[0];
		A[0] = A[i];
		A[i] = t;

		/* decrease n by one */

		n--;

		/* make sure A is still a heap */

		heapify (A, 0, n);
	}
}

/* binary search tree structure */

struct binary_search_tree_node {
	int x;
	struct binary_search_tree_node *llink, *rlink;
};

/* insert an item into a binary search tree */

void insert_binary_search_tree (struct binary_search_tree_node **r, int x) {
	if (*r == NULL) {
		*r = malloc (sizeof (struct binary_search_tree_node));
		(*r)->llink = NULL;
		(*r)->rlink = NULL;
		(*r)->x = x;
	} else if ((*r)->x < x) 
		insert_binary_search_tree (&(*r)->rlink, x);
	else if ((*r)->x >= x)
		insert_binary_search_tree (&(*r)->llink, x);
}

/* traverse binary search tree, placing items into an array in inorder */

void traverse_binary_search_tree (struct binary_search_tree_node *r, int v[], int *i) {
	if (r) {
		traverse_binary_search_tree (r->llink, v, i);
		v[*i] = r->x;
		(*i)++;
		traverse_binary_search_tree (r->rlink, v, i);
	}
}

/* recursively free heap storage for binary search tree */

void delete_binary_search_tree (struct binary_search_tree_node *r) {
	if (r) {
		delete_binary_search_tree (r->llink);
		delete_binary_search_tree (r->rlink);
		free (r);
	}
}

/* sort an array by inserting it into a binary search tree, then traversing
 * the tree in order placing elements back into the array
 */

void binarytreesort (int v[], int n) {
	int i;
	struct binary_search_tree_node *r = NULL;
	for (i=0; i<n; i++)
		insert_binary_search_tree (&r, v[i]);
	i = 0;
	traverse_binary_search_tree (r, v, &i);
	delete_binary_search_tree (r);
}

int main () {
	int i;
	int *v = malloc (N * sizeof (record_type));
	for (i=0; i<N; i++) v[i] = rand () % 1000000;
	//bubble_sort (v, N);
	//merge_sort (v, N);
	//heapsort (v, N);
	//quicksort (v, 0, N);
	binarytreesort (v, N);
	for (i=0; i<N; i++) printf ("%d\n", v[i]);
	return 0;
}
