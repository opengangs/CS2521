#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"

static double numPaths (ShortestPaths, Vertex, Vertex, double *);

NodeValues closenessCentrality (Graph g) {
    NodeValues perp;
    perp.numNodes = GraphNumVertices (g);
    perp.values = malloc (sizeof (double)*perp.numNodes);
    
    for (int v = 0; v < perp.numNodes; v++) {
        perp.values[v] = 0;
    }
    
    int nodesReached;
    double distance;
    ShortestPaths Dijkstra;
    
    for (int i = 0; i < perp.numNodes; i++) {
        // iterate through each node and find the number of nodes that is reachable.
        
        nodesReached = 0;
        Dijkstra = dijkstra (g, i);
        
        // finds all of the reachable nodes.
        for (int j = 0; j < perp.numNodes; j++) {
            PredNode *cur = Dijkstra.pred[j];
            if (cur != NULL) {
                nodesReached++;
            }
        }
        
        // src to src.
        nodesReached++;
        
        distance = 0;
        for (int k = 0; k < perp.numNodes; k++) {
            // sum of the shortest path; distance is the total length of the shortest distance.
            distance = distance + Dijkstra.dist[k];
        }
        
        if (distance == 0) {
            perp.values[i] = 0;
        }
        
        else {
        
            // n: nodesReached.
            // N: perp.numNodes.
            // sum: distance.
            
            // C_WF (u) = (n - 1)/(N - 1)*(n - 1)/sum
            perp.values[i] = ((nodesReached - 1)/(perp.numNodes - 1))*((nodesReached - 1)/(distance));
        }
        
    }
    return perp;
}

NodeValues betweennessCentrality(Graph g) {
    NodeValues perp;
    perp.numNodes = GraphNumVertices (g);
    perp.values = malloc (sizeof (double)*perp.numNodes);
    
    for (int vIndex = 0; vIndex < perp.numNodes; vIndex++) {
        perp.values[vIndex] = 0;
    }   
    
    // source node.
    for (Vertex s = 0; s < perp.numNodes; s++) {
        // shortest path from s.
        ShortestPaths sps = dijkstra (g, s);
        
        // dest node.
        for (Vertex t = 0; t < perp.numNodes; t++) {
            
            // count paths.
            for (Vertex vertex = 0; vertex < perp.numNodes; vertex++) {
            
                if ((vertex != s) && (vertex != t)) {
                
                    double paths, v;
                    paths = v = 0;
                
                /*
                
                numPaths calculates the number of paths from a vertex to a destination vertex.
                Keeps track of the number of times a vertex was passed.
                
                */
                    paths = numPaths (sps, vertex, t, &v);
                    if (paths != 0) {
                        perp.values[t] = perp.values[t] + (v / paths);
                    }
               }
            }
        }
        
        freeShortestPaths (sps);
         
    }
    return perp;
}

NodeValues betweennessCentralityNormalised(Graph g) {
    NodeValues perp;
    perp = betweennessCentrality (g);
    
    // update each node.
    for (int i = 0; i < perp.numNodes; i++) {
        // perp.values[i] = 1/((n - 1)(n - 2))*perp.values[i].
        // n: perp.numNodes.
        
        perp.values[i] = 1/((perp.numNodes - 1)*(perp.numNodes - 2))*perp.values[i];
    }
    
    return perp;
}



/**
 * This  function is for you to print out the NodeValues structure while
 * you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given NodeValues structure in whatever format you want.
 */
void showNodeValues(NodeValues nvs) {

}

/**
 * Frees all memory associated with the given NodeValues structure.
 */
void freeNodeValues(NodeValues nvs) {
    
}

/*
    Helper function: determines the number of paths from a vertex to a destination.
    Keeps track of the number of times a particular vertex was passed.
    
    Uses adjacency list (prednode) to back track from dest to v.
*/
double numPaths (ShortestPaths sps, Vertex v, Vertex dest, double *vertex) {
    
    if (v == sps.src) return 1;
    double paths = 0;
    PredNode *pred = sps.pred[v];
    
    while (pred != NULL) {
        // update paths.
        paths = paths + numPaths (sps, pred->v, dest, vertex);
        pred = pred->next;
    }
    
    if (v == dest) (*vertex) += paths;
    return paths;
} 
