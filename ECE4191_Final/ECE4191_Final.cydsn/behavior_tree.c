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
#include "project.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "behavior_tree.h"
#include "locomotion.h"
#include "ultrasonic.h"
#include "bluetooth.h"

int done;
int obstacle;
int moving;

// Functions to be associated with each node
void rootNode() {
    // printValue("Root Node Executed\n");
}

void readDistance() {
    // printValue("Check Distance\n");
    double FLU, FRU;
    
    FLU = UltrasonicSensor_ReadDistanceData(0);
    FRU = UltrasonicSensor_ReadDistanceData(1);
    
    if (FRU <= 25 || FRU <= 25) obstacle = 1;
    
    printValue("Distance: %lf %lf\n", FLU, FRU);
    
}

void moveForward() {
    // printValue("Moving Forward\n");
    if (!obstacle && !moving) {
        wheel_move(Forward, 250); 
        moving = 1;
    }
}

void avoidObs() {
    // printValue("Function executed for node 3\n");
    if (obstacle) {
        wheel_move(StopMotion, 250); 
        // printValue("Avoiding Obstacles\n");
        wheel_move_by_metrics(Left, 250, 90);
        moving = 0;
        done = 1;
    }
    
}

void END() {
    
    // printValue("Function executed for node 4\n");
    if (done)
    {
        printValue("END function executed\n");
        wheel_move(StopMotion, 250);   
    }
}

void function5() {
    printValue("Function executed for node 5\n");
}

void function6() {
    printValue("Function executed for node 6\n");
}

// Create a node
struct node* createNode(int v) {
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Create graph
struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc(vertices * sizeof(struct node*));
    graph->functionArray = malloc(vertices * sizeof(void (*)()));

    graph->visited = malloc(vertices * sizeof(int));

    int i;
    for (i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;

        // Set the function pointers for each node
        switch (i) {
            case 0:
                graph->functionArray[i] = &rootNode;
                break;
            case 1:
                graph->functionArray[i] = &END;
                break;
            case 2:
                graph->functionArray[i] = &readDistance;
                break;
            case 3:
                graph->functionArray[i] = &avoidObs;
                break;
            case 4:
                graph->functionArray[i] = &moveForward;
                break;
//            case 5:
//                graph->functionArray[i] = &function5;
//                break;
//            case 6:
//                graph->functionArray[i] = &function6;
//                break;
            default:
                graph->functionArray[i] = NULL;
                break;
        }
    }
    
    obstacle = 0;
    done = 0;
    moving = 0;
    
    return graph;
}

// Add edge
void addEdge(struct Graph* graph, int src, int dest) {
    // Add edge from src to dest
    struct node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Add edge from dest to src
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Helper function to sort the adjacency list in ascending order
void sortAdjList(struct Graph* graph, int vertex) {
    struct node* adjList = graph->adjLists[vertex];
    struct node* temp = adjList;
    int* sortedVertices = malloc(graph->numVertices * sizeof(int));

    // Store the connected vertices in an array
    int count = 0;
    while (temp != NULL) {
        sortedVertices[count++] = temp->vertex;
        temp = temp->next;
    }

    // Sort the vertices using bubble sort (simple for small arrays)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sortedVertices[j] > sortedVertices[j + 1]) {
                int temp = sortedVertices[j];
                sortedVertices[j] = sortedVertices[j + 1];
                sortedVertices[j + 1] = temp;
            }
        }
    }

    // Update the adjacency list in sorted order
    temp = adjList;
    for (int i = 0; i < count; i++) {
        temp->vertex = sortedVertices[i];
        temp = temp->next;
    }

    free(sortedVertices);
}

// DFS algo
void DFS(struct Graph* graph, int vertex) {
    sortAdjList(graph, vertex); // Sort the adjacency list

    struct node* adjList = graph->adjLists[vertex];
    struct node* temp = adjList;

    graph->visited[vertex] = 1;
    // printValue("Visited %d \n", vertex);

    if (graph->functionArray[vertex] != NULL) {
        // Execute the associated function for the node
        graph->functionArray[vertex]();
    }

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}


void printGraph(struct Graph* graph) {
    int v;
    for (v = 0; v < graph->numVertices; v++) {
    struct node* temp = graph->adjLists[v];
    // printValue("\n Adjacency list of vertex %d\n ", v);
    while (temp) {
        // printValue("%d -> ", temp->vertex);
        temp = temp->next;
    }
    // printValue("\n");
    }
}


/* [] END OF FILE */
