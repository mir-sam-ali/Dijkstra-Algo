#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <climits>
using namespace std;

class binomial_node
{
public:
    int key;
    int vertex;
    int degree;
    binomial_node *left_sibling;
    binomial_node *right_sibling;
    binomial_node *parent;
    binomial_node *first_child;
    binomial_node *last_child;

    binomial_node(int data)
    {

        key = data;
        first_child = NULL;
        last_child = NULL;
        parent = NULL;
        left_sibling = NULL;
        right_sibling = NULL;
        degree = 0;
    }

    binomial_node(int data, int v)
    {
        vertex = v;
        key = data;
        first_child = NULL;
        last_child = NULL;
        parent = NULL;
        left_sibling = NULL;
        right_sibling = NULL;
        degree = 0;
    }
};

binomial_node *merge_two_trees(binomial_node *tree_1, binomial_node *tree_2)
{
    if (tree_1->key >= tree_2->key) //tree_2 is smaller and it will be the parent
    {
        binomial_node *temp = tree_2->last_child;
        tree_2->last_child = tree_1;
        tree_1->parent = tree_2;
        tree_1->right_sibling = temp;
        if (temp != NULL)
            temp->left_sibling = tree_1;

        //when two tree of degree 0 are attached then first_child and last_child are same.
        if (tree_1->degree == 0)
        {
            tree_2->first_child = tree_1;
        }
        (tree_2->degree)++;

        return tree_2;
    }
    else
    {
        //tree_1 is smaller
        binomial_node *temp = tree_1->last_child;
        tree_1->last_child = tree_2;
        tree_2->parent = tree_1;
        tree_2->right_sibling = temp;
        if (temp != NULL)
            temp->left_sibling = tree_2;
        if (tree_1->degree == 0)
        {
            tree_1->first_child = tree_2;
        }
        (tree_1->degree)++;
        return tree_1;
    }
}

list<binomial_node *> union_heap(list<binomial_node *> h1, list<binomial_node *> h2)
{

    //code for merging of linked list according to the degree
    //cout<<"h1: "<<h1.size()<<" h2: "<<h2.size()<<endl;
    list<binomial_node *> newList;
    auto l1 = h1.begin();
    auto l2 = h2.begin();

    while (l1 != h1.end() && l2 != h2.end())
    {
        if ((*l1)->degree <= (*l2)->degree)
        {
            newList.push_back(*l1);
            l1++;
        }
        else
        {
            newList.push_back(*l2);
            l2++;
        }
    }
    while (l1 != h1.end())
    {
        //cout<<(*l1)->key<<endl;
        newList.push_back(*l1);
        l1++;
    }
    while (l2 != h2.end())
    {
        newList.push_back(*l2);
        l2++;
    }
    return newList;
}

list<binomial_node *> modify_heap(list<binomial_node *> heap)
{
    if (heap.size() <= 1)
    {
        return heap;
    }

    auto prev = heap.begin();
    auto next = prev;
    next++;
    auto next_next = next;
    next_next++;

    while (true)
    {

        if (next_next == heap.end())
        {
            if ((*prev)->degree == (*next)->degree)
            {
                binomial_node *newTree = merge_two_trees((*prev), (*next));
                (*prev) = newTree;
                heap.erase(next);
                return heap;
            }
            return heap;
        }

        if ((*prev)->degree == (*next)->degree && (*next)->degree != (*next_next)->degree)
        {
            binomial_node *newTree = merge_two_trees((*prev), (*next));
            (*prev) = newTree;
            next = heap.erase(next);
            next_next++;
        }
        else if ((*prev)->degree == (*next)->degree && (*next)->degree == (*next_next)->degree)
        {
            prev++;
            next++;
            next_next++;
        }
        else
        {
            prev++;
            next++;
            next_next++;
        }
    }
}

list<binomial_node *> insert(list<binomial_node *> heap, int key, int vertex, vector<binomial_node *> &ref)
{
    binomial_node *temp = new binomial_node(key, vertex);
    ref[vertex] = temp;
    list<binomial_node *> heap_2;
    heap_2.push_back(temp);
    heap_2 = union_heap(heap, heap_2);
    heap_2 = modify_heap(heap_2);
    return heap_2;
}

void printTree(binomial_node *h)
{
    while (h)
    {
        cout << h->key << " ";
        printTree(h->last_child);
        h = h->right_sibling;
    }
}

binomial_node *getMin(list<binomial_node *> heap)
{

    auto it = heap.begin();
    auto min = *it;
    while (it != heap.end())
    {
        if ((*it)->key < (min)->key)
        {
            min = *it;
        }
        it++;
    }
    return min;
}
// print function for binomial heap
void printHeap(list<binomial_node *> heap)
{
    //cout << heap.size() << endl;
    auto it = heap.begin();
    while (it != heap.end())
    {
        cout << "Degree: " << (*it)->degree << endl;
        printTree(*it);
        cout << endl;
        it++;
    }
}

list<binomial_node *> deleteMin(list<binomial_node *> heap)
{
    binomial_node *min = getMin(heap);

    list<binomial_node *> new_heap;
    auto it = heap.begin();
    while (it != heap.end())
    {
        if (*it != min)
        {
            new_heap.push_back((*it));
        }
        it++;
    }

    list<binomial_node *> heap_2;

    binomial_node *child = min->first_child;
    // min->first_child = NULL;

    while (child != NULL)
    {

        binomial_node *temp = child->left_sibling;
        //cout << child->key << endl;
        child->parent = NULL;

        child->right_sibling = NULL;
        child->left_sibling = NULL;

        heap_2.push_back(child);

        child = temp;
    }

    //printHeap(new_heap);
    if (heap_2.size() != 0)
    {
        new_heap = union_heap(heap_2, new_heap);

        new_heap = modify_heap(new_heap);
    }
    return new_heap;
}

void decreaseKey(binomial_node *node, int newKey, vector<binomial_node *> &ref)
{
    node->key = newKey;
    binomial_node *parent = node->parent;
    //cout<<"DECREASING..."<<endl;

    while (parent != NULL && parent->key > node->key)
    {
        //cout<<parent->key<<endl;
        //swap data and change the ref of the node for the vertices
        int parentVertex = parent->vertex;
        int parentKey = parent->key;

        parent->vertex = node->vertex;
        parent->key = node->key;

        ref[node->vertex] = parent;

        node->vertex = parentVertex;
        node->key = parentKey;
        ref[parentVertex] = node;

        //go up the tree
        node = parent;
        parent = node->parent;
    }
}

//Graph code
vector<vector<pair<int, int>>> create_adjList(vector<vector<int>> adj)
{
    int size = adj.size();
    vector<vector<pair<int, int>>> adjList;

    for (int i = 0; i < size; i++)
    {
        vector<pair<int, int>> list;
        for (int j = 0; j < size; j++)
        {
            if (adj[i][j] > 0)
            {
                list.push_back(make_pair(j, adj[i][j])); //vertex and distance
            }
        }
        adjList.push_back(list);
    }

    cout << "Adjacency List:" << endl;

    for (int i = 0; i < size; i++)
    {
        cout << i << "->";
        for (auto x : adjList[i])
        {
            cout << "(" << x.first << "," << x.second << ")"
                 << " ";
        }
        cout << endl;
    }

    return adjList;
}

bool dfsUndirected(vector<vector<int>> adj, int s, bool vis[], int parent)
{
    vis[s] = true;
    for (int i = 0; i < adj[s].size(); i++)
    {

        if ((vis[adj[s][i]] && adj[s][i] != parent) || s == adj[s][i])
            return true;
        else
        {
            if (!vis[adj[s][i]] && dfsUndirected(adj, adj[s][i], vis, s))
            {
                // cout<<s<<" "<<adj[s][i]<<endl;
                return true;
            }
        }
    }

    return false;
}

bool isCyclicUndirected(int V, vector<vector<int>> adj)
{

    bool vis[V] = {false};

    for (int i = 0; i < V; i++)
    {
        if (!vis[i])
        {
            if (dfsUndirected(adj, i, vis, -1))
            {
                return true;
            }
        }
    }
    return false;
}

bool dfsDirected(vector<vector<int>> adj, int s, bool vis[], bool rec[], int parent)
{
    vis[s] = true;
    rec[s] = true;

    for (int i = 0; i < adj[s].size(); i++)
    {

        if ((rec[adj[s][i]]) || s == adj[s][i])
            return true;
        else
        {
            if (!vis[adj[s][i]] && dfsDirected(adj, adj[s][i], vis, rec, s))
            {
                // cout<<s<<" "<<adj[s][i]<<endl;
                return true;
            }
        }
    }
    rec[s] = false;
    return false;
}

bool isCyclicDirected(int V, vector<vector<int>> adj)
{

    bool vis[V] = {false};
    bool rec[V] = {false};

    for (int i = 0; i < V; i++)
    {
        if (!vis[i])
        {
            if (dfsDirected(adj, i, vis, rec, -1))
            {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    list<binomial_node *> heap;

    //     heap = insert(heap, 10,0,vertex_references);
    //     heap = insert(heap, 20,1,vertex_references);
    //     heap = insert(heap, 10,2,vertex_references);
    //     heap = insert(heap, 20,3,vertex_references);
    //     // heap = insert(heap, 20,4,vertex_references);
    //     // heap = insert(heap, 10,5,vertex_references);
    //     //cout << (getMin(heap))->key << endl;
    //     printHeap(heap);
    // heap = deleteMin(heap);
    // printHeap(heap);
    // cout<<vertex_references[3]->key<<endl;
    // decreaseKey(vertex_references[3], 5, vertex_references);
    // printHeap(heap);

    //Dijkstra's Algo implementation
    int V;
    cin >> V;
    vector<vector<int>> adj(V, vector<int>(V, 0));
    vector<binomial_node *> vertex_references(V);
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            cin >> adj[i][j];
        }
    }
    vector<vector<pair<int, int>>> adjList = create_adjList(adj);

    int directed;
    cin >> directed;
    if (directed > 0)
    {
        if (isCyclicDirected(V, adj))
        {
            cout << "Your Directed graph is cyclic" << endl;
        }
        else
        {
            cout << "Your Directed graph is acyclic" << endl;
        }
    }
    else
    {
        if (isCyclicUndirected(V, adj))
        {
            cout << "Your Undirected graph is cyclic" << endl;
        }
        else
        {
            cout << "Your Undirected graph is acyclic" << endl;
        }
    }

    int source; //Source Vertex
    vector<int> distances(V, INT_MAX);

    cin >> source;
    distances[source] = 0;
    heap = insert(heap, 0, source, vertex_references);

    for (int i = 0; i < V; i++)
    {
        if (i != source)
            heap = insert(heap, INT_MAX, i, vertex_references);
    }

    bool vis[V] = {false};

    while (heap.size() > 0)
    {
        cout << "Current Distances:" << endl;
        for (int i = 0; i < V; i++)
        {
            cout << "vertex: " << i << " distance: " << distances[i] << endl;
        }

        //printHeap(heap);
        binomial_node *min_node = getMin(heap);
        heap = deleteMin(heap);

        cout << "MIN: "
             << "vertex: " << min_node->vertex << " value: " << min_node->key << endl;
        int current_source = min_node->vertex;
        vis[current_source] = true;
        int distance = min_node->key;

        for (int i = 0; i < adjList[current_source].size(); i++)
        {
            pair<int, int> neighbour = adjList[current_source][i];

            int vertex = neighbour.first;
            int weight = neighbour.second;

            cout << "Vertex: " << vertex << ", weight: " << weight << endl;

            int newDist = distance + weight;
            cout << "New dist: " << newDist << " , Old distance: " << distances[vertex] << endl;
            vis[vertex] = true;
            if (newDist < distances[vertex])
            {
                distances[vertex] = newDist;
                decreaseKey(vertex_references[vertex], newDist, vertex_references);
            }
        }
    }

    cout << "Minimum Distances" << endl;
    for (int i = 0; i < V; i++)
    {
        cout << "vertex: " << i << " distance: " << distances[i] << endl;
    }
}

/*
INPUT FORMAT:
Number of vertices
then the adjacency matrix

then 1 if graph is directed else 0

then source vertex

Example:
3
0 1 43
1 0 6
43 6 0
0
2

OUTPUT:
Your Undirected graph is cyclic

Minimum Distances
vertex: 0 distance: 7
vertex: 1 distance: 6
vertex: 2 distance: 0


TEST CASE-2:
INPUT:
5
0 3 2 0 0
3 0 5 3 0
2 5 0 0 20
0 3 0 0 4
0 0 20 4 0
0
0

OUTPUT:
Your Undirected graph is cyclic
Minimum Distances
vertex: 0 distance: 0
vertex: 1 distance: 3
vertex: 2 distance: 2
vertex: 3 distance: 6
vertex: 4 distance: 10

*/