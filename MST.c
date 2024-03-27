#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>

#define MAX 30
bool visited[MAX] = {false};
bool stack[MAX] = {false};
bool visit[MAX] = {false};

bool isVisitedFull(bool visit[],int n)
{
    for(int i = 0; i<n; i++)
    {
        if(visit[i] == false)
        {
            return false;
        }
    }
    return true;
}

void reset()
{
    for(int i = 0; i<MAX; i++)
    {
        visited[i] = false;
        stack[i] = false;
    }
}

struct graph
{
    int vertices;
    int adjm[MAX][MAX];
    int clone[MAX][MAX];
};

struct graph* createGraphUnweighted(int verticeCount)
{
    struct graph* newGraph = (struct graph*)malloc(sizeof(struct graph));
    newGraph->vertices = verticeCount;
    for(int i = 0; i<verticeCount; i++)
    {
        for(int j = 0; j<verticeCount; j++)
        {
            newGraph->adjm[i][j] = 0;
            newGraph->clone[i][j] = 0;
        }
    }
    return newGraph;
}

struct graph* createGraphWeighted(int verticeCount)
{
    struct graph* newGraph = (struct graph*)malloc(sizeof(struct graph));
    newGraph->vertices = verticeCount;
    for(int i = 0; i<verticeCount; i++)
    {
        for(int j = 0; j<verticeCount; j++)
        {
            newGraph->adjm[i][j] = INT_MAX;
            newGraph->clone[i][j] = INT_MAX;
        }
    }
    return newGraph;
}

struct graph* addEdgeUndirected(struct graph *map,int src, int dest)
{
    map->adjm[src][dest] = 1;
    map->adjm[dest][src] = 1;
    map->clone[src][dest] = 1;
    map->clone[dest][src] = 1;
    return map;
}

struct graph* addEdgeUndirectedWeighted(struct graph *map,int src, int dest, int weight)
{
    map->adjm[src][dest] = weight;
    map->adjm[dest][src] = weight;
    map->clone[src][dest] = weight;
    map->clone[dest][src] = weight;
    return map;
}

void addEdge(struct graph* graph, int source, int destination) {
    graph->adjm[source][destination] = 1;
    graph->clone[source][destination] = 1;
}

void addEdgeWeighted(struct graph* graph, int source, int destination, int weight) {
    graph->adjm[source][destination] = weight;
    graph->clone[source][destination] = weight;
}

void BFS(struct graph* map, int start)
{
    int q[map->vertices];
    printf("\nBFS starts from vertice: %d\n", start);

    int front = -1, rear = -1;
    q[++rear] = start;
    visited[start] = true;

    while(front != rear)
    {
        int current = q[++front];
        printf("%d\t", current);
        for(int i = 0; i<map->vertices;i++)
        {
            if(map->adjm[current][i] >0 && map->adjm[current][i]<INT_MAX && !visited[i])
            {
                q[++rear] = i;
                visited[i] = true;
            }
        }
    }
}

void DFS(struct graph* map, int start)
{
    visited[start] = true;
    printf("%d\t", start);

    for(int i = 0; i<map->vertices; i++)
    {
        if(map->adjm[start][i] >0 && map->adjm[start][i]<INT_MAX && !visited[i])
        {
            DFS(map,i);
        }
    }
}

bool hasCycleDFS(struct graph* graph, int vertex, int parent) {
    visited[vertex] = true;

    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjm[vertex][i]>0 && graph->adjm[vertex][i]<INT_MAX) {
            if (!visited[i]) {
                if (hasCycleDFS(graph, i, vertex)) {
                    return true;
                }
            } else if (i != parent) {
                return true;
            }
        }
    }

    return false;
}

bool hasCycle(struct graph* graph) {
    reset();

    for (int i = 0; i < graph->vertices; i++) {
        if (!visited[i] && hasCycleDFS(graph, i, -1)) {
            return true;
        }
    }

    return false;
}

bool hasCycleDFSD(struct graph* graph, int vertex) {
    if (!visited[vertex]) {
        visited[vertex] = true;
        stack[vertex] = true;

        for (int i = 0; i < graph->vertices; i++) {
            if (graph->adjm[vertex][i]>0&& graph->adjm[vertex][i]<INT_MAX) {
                if (!visited[i] && hasCycleDFSD(graph, i)) {
                    return true;
                } else if (stack[i]) {
                    return true;
                }
            }
        }
    }

    stack[vertex] = false;
    return false;
}

bool hasCycleDirectedGraph(struct graph* graph) {
    reset();

    for (int i = 0; i < graph->vertices; i++) {
        if (!visited[i] && hasCycleDFSD(graph, i)) {
            return true;
        }
    }

    return false;
}

struct graph* MSTKrusakUD(struct graph* map)
{
    for(int i = 0; i<map->vertices; i++)
    {
        visit[i] = false;
    }
    struct graph* MST = createGraphWeighted(map->vertices);
    MST->vertices = map->vertices;
    
    int min;
    int x;
    int y;
    while(!isVisitedFull(visit, map->vertices))
    {
        min = INT_MAX;
        for(int i = 0; i<map->vertices;i++)
        {
            for(int j = 0; j<map->vertices;j++)
            {
                if(map->clone[i][j] <= min)
                {
                    min = map->clone[i][j];
                    x=i;
                    y=j;
                }
            }
        }
        addEdgeUndirectedWeighted(MST,x,y,min);
        map->clone[x][y] = INT_MAX;
        map->clone[y][x] = INT_MAX;
        visit[x] = true;
        visit[y] = true;
        if(hasCycle(MST))
        {
            MST->adjm[x][y] = INT_MAX;
        }
    }
    return MST;
}

void printGraph(struct graph *map) 
{
    for(int i = 0; i<map->vertices; i++)
    {
        for(int j = 0; j<map->vertices; j++)
        {
            if(map->adjm[i][j] > 0 && map->adjm[i][j]<INT_MAX)
            {
                printf("\nVertex %d is connected to vertex %d and the weight is %d" , i, j, map->adjm[i][j]);
            }
        }
    }
}

void totalWeightUndirected(struct graph* map)
{
    int sum  = 0;
    for(int i = 0; i<map->vertices;i++)
    {
        for(int j = 0; j<map->vertices; j++)
        {
            if(map->adjm[i][j]>0 && map->adjm[i][j] < INT_MAX)
            {
                sum += map->adjm[i][j]; 
            } 
        }
    }
    printf("\nWeight of it = %d", sum/2);
}


int main()
{
    struct graph* map =  createGraphWeighted(6);
    addEdgeUndirectedWeighted(map,0,1,3);
    addEdgeUndirectedWeighted(map,1,2,5);
    addEdgeUndirectedWeighted(map,2,3,4);
    addEdgeUndirectedWeighted(map,3,4,2);
    addEdgeUndirectedWeighted(map,4,5,5);
    addEdgeUndirectedWeighted(map,4,1,4);
    addEdgeUndirectedWeighted(map,0,5,7);
    addEdgeUndirectedWeighted(map,1,5,8);
    addEdgeUndirectedWeighted(map,2,5,6);
    addEdgeUndirectedWeighted(map,3,5,8);

    BFS(map,0);
    
    struct graph* map2 = MSTKrusakUD(map);
    reset();
    BFS(map2,0);
    totalWeightUndirected(map2);

    printGraph(map2);
    
    return 0;
}