/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef BEHAVIOR_TREE_H
    #define BEHAVIOR_TREE_H
    
    extern int done;
    extern int obstacle;
    
    struct node {
      int vertex;
      struct node* next;
    };

    struct Graph {
      int numVertices;
      int* visited;
      struct node** adjLists;
      void (**functionArray)();
    };
    
    struct node* createNode(int v);
    struct Graph* createGraph(int vertices);
    void addEdge(struct Graph* graph, int src, int dest);
    void sortAdjList(struct Graph* graph, int vertex);
    void DFS(struct Graph* graph, int vertex);
    void printGraph(struct Graph* graph);
    
#endif
/* [] END OF FILE */
