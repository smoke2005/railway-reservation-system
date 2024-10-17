#ifndef PATH_V1_H_INCLUDED
#define PATH_V1_H_INCLUDED

#define BRIGHT_YELLOW "\033[93m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 10000
#define MAX_NAME_LENGTH 50

// Structure to represent a station
typedef struct {
    char name[MAX_NAME_LENGTH];
} Station;

// Structure to represent an edge between two stations
typedef struct {
    int source;
    int destination;
    int weight;
} Edge;

// Structure to represent a graph
typedef struct {
    Station stations[MAX_STATIONS];
    Edge edges[MAX_STATIONS];
    int numStations;
    int numEdges;
} Graph;

// Function to add a station to the graph if it's not already present
void AddStation(Graph *graph, char name[MAX_NAME_LENGTH]) {
  for (int i = 0; i < graph->numStations; i++) {
    if (strcmp(graph->stations[i].name, name) == 0) {
      // Station already exists, no need to add
      return;
    }
  }
  strcpy(graph->stations[graph->numStations].name, name);
  graph->numStations++;
}

// Function to add an edge to the graph if it's not already present
void AddEdge(Graph *graph, int source, int destination, int weight) {
  for (int i = 0; i < graph->numEdges; i++) {
    if (graph->edges[i].source == source && graph->edges[i].destination == destination) {
      // Edge already exists, no need to add
      return;
    }
  }
  graph->edges[graph->numEdges].source = source;
  graph->edges[graph->numEdges].destination = destination;
  graph->edges[graph->numEdges].weight = weight;
  graph->numEdges++;
}

// Function to load graph data from CSV file
void LoadGraphFromCSV(Graph *graph, char filename[1000]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    fscanf(file, "%*[^\n]\n");

    while (!feof(file)) {
        int TrainNo, Distance;
        char TrainName[MAX_NAME_LENGTH], ArrivalTime[9], Duration[MAX_NAME_LENGTH];
        char SourceStationCode[4], DestinationStationCode[4];  // Include code fields if needed

        if (fscanf(file, "%d,%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,\n]", &TrainNo, TrainName, ArrivalTime, &Distance, SourceStationCode, graph->stations[graph->numStations].name, DestinationStationCode, graph->stations[graph->numStations + 1].name, Duration) != 9) {
            break; // Exit loop if EOF or error
        }

        // Find or add source and destination stations
        int sourceIndex = -1, destinationIndex = -1;
        for (int i = 0; i < graph->numStations; i++) {
            if (strcmp(graph->stations[i].name, graph->stations[graph->numStations].name) == 0) {
                sourceIndex = i;
            }
            if (strcmp(graph->stations[i].name, graph->stations[graph->numStations + 1].name) == 0) {
                destinationIndex = i;
            }
        }
        if (sourceIndex == -1) {
            AddStation(graph, graph->stations[graph->numStations].name);
            sourceIndex = graph->numStations - 1;
        }
        if (destinationIndex == -1) {
            AddStation(graph, graph->stations[graph->numStations + 1].name);
            destinationIndex = graph->numStations - 1;
        }

        // Add edge with weight as distance (assuming distance represents travel time)
        AddEdge(graph, sourceIndex, destinationIndex, Distance);
    }

    fclose(file);
}


// Function to print the graph
void PrintGraph(Graph *graph) {
    printf("Graph:\n");
    for (int i = 0; i < graph->numStations; i++) {
        printf("%s", graph->stations[i].name);
        for (int j = 0; j < graph->numEdges; j++) {
            if (graph->edges[j].source == i) {
                printf(" -> %s (%d)", graph->stations[graph->edges[j].destination].name, graph->edges[j].weight);
            }
        }
        printf("\n");
    }
}
// Function to print the stack of stations
void printStationStack(Graph *graph, int *stationStack, int top) {
    printf("\n");
    for (int i = top; i >= 0; i--) {
        printf("%s", graph->stations[stationStack[i]].name);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void FindConnectingTrains(Graph *graph, int sourceIndex, int destinationIndex) {
    int visited[MAX_STATIONS] = {0};
    int parent[MAX_STATIONS];
    int queue[MAX_STATIONS];
    int stationStack[MAX_STATIONS]; // Stack to store stations
    int top = -1; // Top of the stack

    // Initialize parent array with -1
    memset(parent, -1, sizeof(parent));

    int front = 0, rear = 0;
    queue[rear++] = sourceIndex;
    visited[sourceIndex] = 1;

    while (front < rear) {
        int current = queue[front++];

        if (current == destinationIndex)
            break;

        for (int i = 0; i < graph->numEdges; i++) {
            if (graph->edges[i].source == current) {
                int nextStation = graph->edges[i].destination;
                if (!visited[nextStation]) {
                    queue[rear++] = nextStation;
                    parent[nextStation] = current;
                    visited[nextStation] = 1;
                }
            }
        }
    }

    if (parent[destinationIndex] == -1) {
        printf("No connecting trains found between %s and %s.\n", graph->stations[sourceIndex].name, graph->stations[destinationIndex].name);
        return;
    }

    int current = destinationIndex;
    while (current != -1) {
        stationStack[++top] = current;
        current = parent[current];
    }

    //printf("Connecting trains between %s and %s:\n", graph->stations[sourceIndex].name, graph->stations[destinationIndex].name);
    for (int i = top; i > 0; i--) {
        printf(BRIGHT_YELLOW"%s -> "RESET, graph->stations[stationStack[i]].name);
    }
    printf(BRIGHT_YELLOW"%s\n"RESET, graph->stations[stationStack[0]].name);
}

void strToUpperCase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}


#endif // PATH_V1_H_INCLUDED
