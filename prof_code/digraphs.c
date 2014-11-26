#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef int vertex;

/* adjacency matrix representation of digraphs */

struct digraph {
	int n;
	int adj_matrix[];
};

struct digraph *new_digraph (int n) {
	int i;

	/* allocate enough space for n and an n x n adjacency matrix */
	/* (how much space do we really need here?) */

	struct digraph *g = malloc (sizeof (struct digraph) + sizeof (int) * n * n);

	/* initialize all elements of matrix to 0 */

	memset (g->adj_matrix, 0, sizeof (int) * n * n);

	/* set n */

	g->n = n;

	/* done */

	return g;
}

/* insert an edge into a digraph */

void insert_edge (struct digraph *g, vertex a, vertex b) {
	int index = a * g->n + b;
	assert (index >= 0 && index < g->n * g->n);
	g->adj_matrix[index] = 1;
}

/* test to see if an edge exists in a digraph */

int edge_exists (struct digraph *g, vertex a, vertex b) {
	int index = a * g->n + b;
	assert (index >= 0 && index < g->n * g->n);
	return g->adj_matrix[index];
}

/* depth first search of a digraph */

void DFS (struct digraph *g, vertex v, int discovered[]) {
	vertex w;

	if (v > g->n) return;
	discovered[v] = 1;
	for (w=0; w<g->n; w++) 
		if (!discovered[w] && edge_exists (g, v, w))
			DFS (g, w, discovered);
}

/* returns 1 if the digraph is connected, 0 otherwise */

int is_connected (struct digraph *g) {

	/* an array of Boolean values; discovered[i] == 1 means vertex i has been discovered */

	int discovered[g->n];
	vertex i;

	/* initially all vertices are undiscovered */

	memset (discovered, 0, sizeof (int) * g->n);

	/* call the recursive component */

	DFS (g, 0, discovered);

	/* if all vertices are discovered then the digraph is connected */

	for (i=0; i<g->n; i++)
		if (!discovered[i]) return 0;
	return 1;
}

void print_adj_matrix (struct digraph *g) {
	int i, j;

	for (j=0; j<g->n; j++) {
		printf ("[ ");
		for (i=0; i<g->n; i++) {
			if (edge_exists (g, j, i)) printf ("1 "); else printf ("0 ");
		}
		printf ("]\n");
	}
}

struct digraph *Warshall (struct digraph *g) {
	vertex i, j, k;

	/* make a new digraph for g' */

	struct digraph *tc_g = new_digraph (g->n);
	assert (tc_g->n == g->n);

	/* copy all the edges over to the new matrix */

	for (j=0; j<g->n; j++)
		for (i=0; i<g->n; i++) 
			if (edge_exists (g, i, j)) insert_edge (tc_g, i, j);

	/* make sure there is an edge from v to v for all v (reflexive property) */

	for (i=0; i<g->n; i++) insert_edge (tc_g, i, i);

	/* Warshall's algorithm */

	for (k=0; k<g->n; k++)
		for (i=0; i<g->n; i++)
			for (j=0; j<g->n; j++)
				if (edge_exists (tc_g, i, k) && edge_exists (tc_g, k, j))
					insert_edge (tc_g, i, j);
	return tc_g;
}

struct list_node {
	int x;
	struct list_node *next;
};

/* another depth first search of a digraph, pushing vertices on a list in
 * reverse order of finishing time 
 */

void DFS2 (struct digraph *g, vertex v, int discovered[], struct list_node **L) {
	vertex w;

	if (v > g->n) return;
	discovered[v] = 1;
	//for (w=0; w<g->n; w++) {
	for (w=g->n-1; w>=0; w--) {
		if (!discovered[w] && edge_exists (g, v, w))
			DFS2 (g, w, discovered, L);
	}

	/* we are finished with node v; push it onto the list */

	struct list_node *e = malloc (sizeof (struct list_node));
	e->next = *L;
	e->x = v;
	*L = e;
}

/* print a topological sort of the graph (assuming we know the root node v) */

void topological_sort (struct digraph *g, vertex v) {

	/* discovered, as before */

	int discovered[g->n];

	/* make a list of nodes to be output as the topological sort */

	struct list_node *L = NULL;

	/* all zeros */

	memset (discovered, 0, g->n * sizeof (int));

	/* depth first search, pushing nodes onto the list */

	DFS2 (g, v, discovered, &L);

	/* print the list, freeing it at the same time */

	printf ("{ ");
	while (L) {
		struct list_node *p = L->next;
		printf ("%d ", L->x);
		free (L);
		L = p;
	}
	printf ("}\n");
}

int main () {
	struct digraph *G = new_digraph (5);
	struct digraph *tc_G;

	/* make a funny digraph */

	insert_edge (G, 0, 1);
	insert_edge (G, 1, 2);
	//insert_edge (G, 2, 3);
	insert_edge (G, 3, 4);
	insert_edge (G, 4, 0);

	/* is the digraph connected? */

	if (is_connected (G)) printf ("connected!\n"); else printf ("not connected!\n");

	/* print the adjacency matrix */

	printf ("adjacency matrix for G:\n");
	print_adj_matrix (G);

	/* find the transitive closure */

	tc_G = Warshall (G);

	/* print the adjacency matrix for the transitive closure */

	printf ("adjacency matrix for transitive closure of G:\n");
	print_adj_matrix (tc_G);
	//return 0;

	/* make a funny DAG */

	struct digraph *H = new_digraph (7);
	insert_edge (H, 3, 4);
	insert_edge (H, 4, 2);
	insert_edge (H, 3, 5);
	insert_edge (H, 5, 2);
	insert_edge (H, 2, 6);
	insert_edge (H, 5, 3);
	insert_edge (H, 6, 1);
	insert_edge (H, 6, 0);
	printf ("adjacency matrix for H:\n");
	print_adj_matrix (H);
	if (is_connected (H)) printf ("connected!\n"); else printf ("not connected!\n");
	topological_sort (H, 3);
	//return 0;
	struct digraph *tc_H = Warshall (H);
	printf ("transitive closure of H is:\n");
	print_adj_matrix (tc_H);
	return 0;
}
