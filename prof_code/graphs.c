#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int vertex;

/* adjacency matrix representation of graphs */

struct graph {
	int n;
	int adj_matrix[];
};

struct graph *new_graph (int n) {
	int i;

	/* allocate enough space for n and an n x n adjacency matrix */
	/* (how much space do we really need here?) */

	struct graph *g = malloc (sizeof (struct graph) + sizeof (int) * n * n);

	/* initialize all elements of matrix to 0 */

	memset (g->adj_matrix, 0, sizeof (int) * n * n);

	/* set n */

	g->n = n;

	/* done */

	return g;
}

/* this macro makes sure the smaller vertex comes first in an edge */

#define ORDER_EDGE(a,b) { if ((a)>(b)) { int t = (a); (a)=(b); (b)=t; }}

/* insert an edge into a graph */

void insert_edge (struct graph *g, vertex a, vertex b) {
	ORDER_EDGE (a, b);
	g->adj_matrix[a * g->n + b] = 1;
}

/* test to see if an edge exists in a graph */

int edge_exists (struct graph *g, vertex a, vertex b) {
	ORDER_EDGE (a, b);
	return g->adj_matrix[a * g->n + b];
}

/* depth first search of a graph */

void DFS (struct graph *g, vertex v, int discovered[]) {
	vertex w;

	if (v > g->n) return;
	discovered[v] = 1;
	for (w=0; w<g->n; w++) 
		if (!discovered[w] && edge_exists (g, v, w))
			DFS (g, w, discovered);
}

/* returns 1 if the graph is connected, 0 otherwise */

int is_connected (struct graph *g) {

	/* an array of Boolean values; discovered[i] == 1 means vertex i has been discovered */

	int discovered[g->n];
	vertex i;

	/* initially all vertices are undiscovered */

	memset (discovered, 0, sizeof (int) * g->n);

	/* call the recursive component */

	DFS (g, 0, discovered);

	/* if all vertices are discovered then the graph is connected */

	for (i=0; i<g->n; i++)
		if (!discovered[i]) return 0;
	return 1;
}

struct graph *Warshall (struct graph *g) {
	vertex i, j, k;

	/* make a new graph for g' */

	struct graph *tc_g = new_graph (g->n);

	/* copy all the edges over to the new matrix */

	memcpy (tc_g->adj_matrix, g->adj_matrix, sizeof (int) * g->n * g->n);

	/* make sure there is an edge from v to v for all v (reflexive property) */

	for (i=0; i<g->n; i++) insert_edge (tc_g, i, i);

	/* Warshall's algorithm */

	for (i=0; i<g->n; i++)
		for (j=0; j<g->n; j++)
			for (k=0; k<g->n; k++)
				if (edge_exists (tc_g, i, j) && edge_exists (tc_g, j, k))
					insert_edge (tc_g, i, k);
	return tc_g;
}

void print_adj_matrix (struct graph *g) {
	int i, j;

	for (j=0; j<g->n; j++) {
		printf ("[ ");
		for (i=0; i<g->n; i++) {
			if (i >= j) {
				if (edge_exists (g, j, i)) printf ("1 "); else printf ("0 ");
			} else {
				printf ("  ");
			}
		}
		printf ("]\n");
	}
}

int main () {
	struct graph *G = new_graph (5);
	struct graph *tc_G;

	/* make a funny graph */

	insert_edge (G, 0, 1);
	insert_edge (G, 1, 2);
	insert_edge (G, 2, 3);
	insert_edge (G, 3, 4);
	insert_edge (G, 4, 0);

	/* is the graph connected? */

	if (is_connected (G)) printf ("connected!\n"); else printf ("not connected!\n");

	/* print the adjacency matrix */

	printf ("adjacency matrix for G:\n");
	print_adj_matrix (G);

	/* find the transitive closure */

	tc_G = Warshall (G);

	/* print the adjacency matrix for the transitive closure */

	printf ("adjacency matrix for transitive closure of G:\n");
	print_adj_matrix (tc_G);
	return 0;
}
