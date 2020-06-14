#include <iostream>
#include <limits.h> 
using namespace std;

struct Node
{
    int dest;
    float weight;
    Node* next;
};

struct List
{
    Node *head;
};

struct Graph
{
    int V;
    List* array;
};

Node* createNode(int dest, float weight)
{
    Node* new_node = new Node;
    new_node->dest = dest;
    new_node->weight = weight*10; // hardcode
    new_node->next = NULL;
    return new_node;
}

Graph* createGraph(int V)
{
    Graph* graph = new Graph;
    graph->V = V;
    graph->array = new List[V];

    int i;
    for (i=0; i<V; ++i) graph->array[i].head = NULL;

    return graph;
}

void addEdge(Graph* graph, int src, int dest, float weight){
    Node* new_node = createNode(dest, weight);
    new_node->next = graph->array[src].head;
    graph->array[src].head = new_node;

    // uncomment for undirect
    // new_node = createNode(src, weight);
    // new_node->next = graph->array[dest].head;
    // graph->array[dest].head = new_node;
}

void printGraph(Graph* graph){
    int v;
    for (v=0; v<graph->V; ++v) {
        Node* pHelp = graph->array[v].head;
        cout<<"Adjacenct list untuk vertex "<<v<<endl;
        cout<<"head ";
        while (pHelp)
        {
            cout<<"-> ("<<pHelp->weight<<")"<<pHelp->dest;
            pHelp = pHelp->next;
        }
        cout<<endl<<endl;
    }
}

// For Dijkstra

struct MinHeapNode
{
    int v;
    int dist;
};

struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
};

MinHeapNode* createMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = new MinHeapNode;
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap* createMinHeap(int capacity){
    MinHeap* minHeap = new MinHeap;
    minHeap->pos = new int[capacity];
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**) new MinHeapNode[capacity];
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) 
{ 
    int smallest, left, right; 
    smallest = idx; 
    left = 2 * idx + 1; 
    right = 2 * idx + 2; 
  
    if (left < minHeap->size && 
        minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
      smallest = left; 
  
    if (right < minHeap->size && 
        minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
      smallest = right; 
  
    if (smallest != idx) 
    { 
        // The nodes to be swapped in min heap 
        MinHeapNode *smallestNode = minHeap->array[smallest]; 
        MinHeapNode *idxNode = minHeap->array[idx]; 
  
        // Swap positions 
        minHeap->pos[smallestNode->v] = idx; 
        minHeap->pos[idxNode->v] = smallest; 
  
        // Swap nodes 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
  
        minHeapify(minHeap, smallest); 
    } 
}

// A utility function to check if the given minHeap is ampty or not 
int isEmpty(struct MinHeap* minHeap) 
{ 
    return minHeap->size == 0; 
} 
  
// Standard function to extract minimum node from heap 
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
    if (isEmpty(minHeap)) 
        return NULL; 
  
    // Store the root node 
    struct MinHeapNode* root = minHeap->array[0]; 
  
    // Replace root node with last node 
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
    minHeap->array[0] = lastNode; 
  
    // Update position of last node 
    minHeap->pos[root->v] = minHeap->size-1; 
    minHeap->pos[lastNode->v] = 0; 
  
    // Reduce heap size and heapify root 
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return root; 
} 
  
// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, int v, int dist) 
{ 
    // Get the index of v in  heap array 
    int i = minHeap->pos[v]; 
  
    // Get the node and update its dist value 
    minHeap->array[i]->dist = dist; 
  
    // Travel up while the complete tree is not hepified. 
    // This is a O(Logn) loop 
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
    { 
        // Swap this node with its parent 
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]); 
  
        // move to parent index 
        i = (i - 1) / 2; 
    } 
} 
  
// A utility function to check if a given vertex 
// 'v' is in min heap or not 
bool isInMinHeap(struct MinHeap *minHeap, int v) 
{ 
   if (minHeap->pos[v] < minHeap->size) 
     return true; 
   return false; 
} 
  
// A utility function used to print the solution 
void printArr(int dist[], int n) 
{ 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i) 
        printf("%d \t\t %f\n", i, float(dist[i])/10); 
} 
  
// The main function that calulates distances of shortest paths from src to all 
// vertices. It is a O(ELogV) function 
void dijkstra(struct Graph* graph, int src, int path[], int dist[]) 
{ 
    int V = graph->V;// Get the number of vertices in graph 
    // int dist[V];      // dist values used to pick minimum weight edge in cut 

    // minHeap represents set E 
    struct MinHeap* minHeap = createMinHeap(V); 
  
    // Initialize min heap with all vertices. dist value of all vertices  
    for (int v = 0; v < V; ++v) 
    { 
        dist[v] = INT_MAX; 
        minHeap->array[v] = createMinHeapNode(v, dist[v]); 
        minHeap->pos[v] = v; 
    } 
  
    // Make dist value of src vertex as 0 so that it is extracted first 
    minHeap->array[src] = createMinHeapNode(src, dist[src]); 
    minHeap->pos[src]   = src; 
    dist[src] = 0; 
    decreaseKey(minHeap, src, dist[src]); 
  
    // Initially size of min heap is equal to V 
    minHeap->size = V; 
  
    // In the followin loop, min heap contains all nodes 
    // whose shortest distance is not yet finalized. 
    while (!isEmpty(minHeap)) 
    { 
        // Extract the vertex with minimum distance value 
        struct MinHeapNode* minHeapNode = extractMin(minHeap); 
        int u = minHeapNode->v; // Store the extracted vertex number 
  
        // Traverse through all adjacent vertices of u (the extracted 
        // vertex) and update their distance values 
        struct Node* pCrawl = graph->array[u].head; 

        while (pCrawl != NULL) 
        { 
            int v = pCrawl->dest; 
            
            // If shortest distance to v is not finalized yet, and distance to v 
            // through u is less than its previously calculated distance 
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&  
                                          pCrawl->weight + dist[u] < dist[v]) 
            { 
                dist[v] = dist[u] + pCrawl->weight;
                path[v] = u;

                // update distance value in min heap also 
                decreaseKey(minHeap, v, dist[v]); 
            } 
            pCrawl = pCrawl->next; 
        } 
    } 
  
    // print the calculated shortest distances 
    printArr(dist, V); 

} 

void printPath(int V, int path[], int asal, int tujuan)
{
    int stack[V], top = -1;

    int st = tujuan;
    while(st!=asal){
        top++;
        stack[top] = st;
        st = path[st];
    }
    
    cout<<"\nJalur yang dilintasi: \n";
    cout<<asal;
    while(top!=-1){
        cout<<" ->> "<<stack[top];
        top--;
    }
}

int main()
{
    int V = 11;
    int path[V], dist[V];

    // for path
    int asal, tujuan;

    Graph* graph = createGraph(V);
    // A = 0
    // B = 1
    // C = 2
    // D = 3
    // E = 4
    // F = 5
    // G = 6
    // H = 7
    // I = 8
    // J = 9
    // K = 10
    addEdge(graph, 0, 1, 2*0.1);
    addEdge(graph, 0, 2, 2*0.3);
    addEdge(graph, 0, 6, 4*0.2);
    addEdge(graph, 1, 3, 3*0.3);
    addEdge(graph, 1, 6, 5*0.3);
    addEdge(graph, 2, 3, 1*0.8);
    addEdge(graph, 3, 0, 3*0.3);
    addEdge(graph, 3, 4, 4*0.5);
    addEdge(graph, 3, 9, 6*0.3);
    addEdge(graph, 4, 8, 3*0.7);
    addEdge(graph, 4, 10, 7*0.3);
    addEdge(graph, 5, 3, 1*0.9);
    addEdge(graph, 5, 4, 1*0.3);
    addEdge(graph, 6, 7, 10*0.5);
    addEdge(graph, 7, 0, 6*0.4);
    addEdge(graph, 7, 1, 8*0.1);
    addEdge(graph, 7, 8, 4*0.7);
    addEdge(graph, 8, 1, 10*0.9);
    addEdge(graph, 9, 5, 7*0.2);
    addEdge(graph, 10, 5, 9*0.4);
    addEdge(graph, 10, 7, 9*0.2);
    
    printGraph(graph);

    cout<<"Masukan Asal: ";cin>>asal;
    cout<<"Masukan Tujuan: ";cin>>tujuan;

    dijkstra(graph, asal, path, dist); 

    printPath(V, path, asal, tujuan);
    cout<<endl<<"Bobot: "<<float(dist[tujuan])/10;

    return 0;
}