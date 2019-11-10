// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// prototype to find the length of a path.
static int pathLength (int *, int *, Vertex);

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);

	if (src == dest) {
		path[0] = src;
		return 1;
	}

	int visited[g->nV];
	int pred[g->nV];

	for (int i = 0; i < g->nV; i++) {
		visited[i] = 0; // haven't visited.
		pred[i] = -1; // no preds.
	}

	visited[src] = 1; // src location was seen.

	// init queue.
	Queue routes = newQueue ();
	QueueJoin (routes, src);

	int path_length;

	while (!QueueIsEmpty (routes)) {
		Vertex cur = QueueLeave (routes); // dequeue routes.
		// loop through each vertex and find edge weights > 0.
		for (int i = 0; i < g->nV; i++) {
			if (g->edges[cur][i] != 0 && (int)(g->edges[cur][i]) < max) {
				// adjacent edge within distance. Check if we have visited it before.
				if (visited[i] == 1) continue;
				visited[i] = 1;
				pred[i] = cur;

				QueueJoin (routes, i);
			}
		}
	}

	path_length = pathLength (path, pred, dest);
	if (path_length > 0) {
		path[0] = src;
		path_length++; // for src node.
	}
	
	// prevent leaky bois.
	dropQueue (routes);
	return path_length;
}

// helper function to find the length of a path given the path, preds and the dest.
static int pathLength (int *path, int *pred, Vertex dest) {
	
	// if no path exists, return 0;
	if (pred[dest] == -1) return 0;

	// recursively call pathLength.
	int len = pathLength (path, pred, pred[dest]);
	len++;
	path[len] = dest;

	return len;
}
