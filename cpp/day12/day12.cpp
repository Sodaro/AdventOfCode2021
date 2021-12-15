#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

bool isUpper(std::string &word)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (word[i] > 'Z')
            return false;
    }
    return true;
}

struct Node
{
    Node(std::string p_name, bool p_canBeReused, int p_index)
    {
        name = p_name;
        canBeReused = p_canBeReused;
        index = p_index;
    }
    void addConnectedNodes(Node* node)
    {
        neighbors.push_back(node);
    }
    std::vector<Node*> neighbors;
    std::string name = "";
    int index = 0;
    //bool visited = false;
    bool canBeReused = false;
    bool canBeUsedTwice = false;
    int timesVisited = 0;
};
struct Edge
{
    Edge(Node* n1, Node* n2)
    {
        nodes[0] = n1;
        nodes[1] = n2;
    }
    Node* nodes[2];
    std::string name = "";
};
struct Graph
{
    Node* getNode(std::string name)
    {
        for (Node* node : nodes)
        {
            if (node->name == name)
                return node;
        }
        return nullptr;
    }
    void setStartNode(Node* node)
    {
        startNode = node;
    }
    void setEndNode(Node* node)
    {
        endNode = node;
    }
    void addNode(Node* node)
    {
        bool isUnique = true;
        for (Node* n : nodes)
        {
            if (node->name == n->name)
            {
                isUnique = false;
                break;
            }
        }
        if (isUnique)
            nodes.push_back(node);
    }
    void addEdge(Edge* edge)
    {
        std::vector<int> indices;
        indices.push_back(edge->nodes[0]->index);
        indices.push_back(edge->nodes[1]->index);
        std::sort(indices.begin(), indices.end());
        std::string edgeName = nodes[indices[0]]->name + nodes[indices[1]]->name;

        if (edges.count(edgeName) > 0)
            return;

        edges[edgeName] = edge;
        edge->name = edgeName;
    }
    void removeEdge(Node* n1, Node* n2)
    {
        
        std::vector<int> indices;
        indices.push_back(n1->index);
        indices.push_back(n2->index);
        std::sort(indices.begin(), indices.end());
        std::string edgeName = nodes[indices[0]]->name + nodes[indices[1]]->name;
        if (edges.count(edgeName) == 0)
            return;
        delete(edges[edgeName]);
        edges.erase(edgeName);
    }
    void removeNeighbor(Node* neighbor)
    {
        for (Node* node : nodes)
        {
            for (int i = node->neighbors.size() - 1; i >= 0; i--)
            {
                if (node->neighbors[i]->name == neighbor->name)
                    node->neighbors.erase(node->neighbors.begin() + i);
            }
        }
    }

    void pruneUnusableNodes()
    {
        for (int i = nodes.size() - 1; i >= 0; i--)
        {
            if (nodes[i]->neighbors.size() == 1 && nodes[i]->neighbors[0]->canBeReused == false)
            {
                removeEdge(nodes[i], nodes[i]->neighbors[0]);
                removeNeighbor(nodes[i]);
                delete(nodes[i]);
                nodes.erase(nodes.begin() + i);
            }
        }
        updateIndices();
    }
    void updateIndices()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            nodes[i]->index = i;
        }
    }
    Node* startNode = nullptr;
    Node* endNode = nullptr;
    std::vector<Node*> nodes;
    std::map<std::string, Edge*> edges;
};

void traverseV1(int u, int d, std::vector<bool> &visited,
    std::vector<std::string> &path, int& path_index, Graph* graph, int &count)
{
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = graph->nodes[u]->name;
    path_index++;
    graph->nodes[u]->timesVisited++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d) {
        count++;
        for (int i = 0; i < path_index; i++)
            std::cout << path[i] << " ";
        std::cout << std::endl;
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        for (Node* node : graph->nodes[u]->neighbors)
            if (!visited[node->index] && node->canBeReused == false || node->canBeReused == true)
                traverseV1(node->index, d, visited, path, path_index, graph, count);
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
}

void traverseV2(int u, int d, std::vector<bool>& visited,
    std::vector<std::string>& path, int& path_index, Graph* graph, int& count, std::string reusableNode, int& timesReused)
{
    // Mark the current node and store it in path[]
    visited[u] = true;
    Node* currentNode = graph->nodes[u];
    std::string name = currentNode->name;
    path[path_index] = name;

    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d) {
        count++;
        for (int i = 0; i < path_index; i++)
            std::cout << path[i] << " ";
        std::cout << std::endl;
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        for (Node* &neighbor : currentNode->neighbors)
        {
            if ((!visited[neighbor->index] && neighbor->canBeReused == false) || (neighbor->canBeReused == true && visited[neighbor->index]) || (!visited[neighbor->index]))
            {
                if (neighbor->name == reusableNode)
                    timesReused++;
                traverseV2(neighbor->index, d, visited, path, path_index, graph, count, reusableNode, timesReused);
            }
            else if ((neighbor->name == reusableNode && timesReused < 2))
            {
                timesReused++;
                traverseV2(neighbor->index, d, visited, path, path_index, graph, count, reusableNode, timesReused);
            }
        }
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
}


void traverseGraphV1(Graph* graph)
{
    std::vector<std::string> path(graph->nodes.size()*8, "");
    std::vector<bool> visited(graph->nodes.size()*8, false);
    int pathIndex = 0;
    int s = graph->startNode->index;
    int d = graph->endNode->index;
    int count = 0;
    traverseV1(s, d, visited, path, pathIndex, graph, count);
    std::cout << "PATH COUNT: " << count << std::endl;
}

void traverseGraphV2(Graph* graph)
{
    std::vector<std::string> path(graph->nodes.size() * 8, "");
    std::vector<bool> visited(graph->nodes.size() * 8, false);
    std::vector<std::vector<std::string>> paths;
    int pathIndex = 0;
    int s = graph->startNode->index;
    int d = graph->endNode->index;
    int count = 0;
    std::vector<Node*> smallCaves;
    for (int i = 0; i < graph->nodes.size(); i++)
    {
        Node* node = graph->nodes[i];
        if (node->canBeReused == false && node->name != graph->startNode->name && node->name != graph->endNode->name)
            smallCaves.push_back(node);
    }
    for (Node* node : smallCaves)
    {
        int timesReused = 0;
        traverseV2(s, d, visited, path, pathIndex, graph, count, node->name, timesReused);
        paths.push_back(path);
        std::cout << timesReused << std::endl;
    }
    //int uniquePaths = 0;
    //for (int i = paths.size() - 1; i >= 0; i--)
    //{
    //    for 
    //}
    std::cout << "PATH COUNT: " << count << std::endl;
}

int main()
{
    //find all paths that:
    //begin in start (do not visit start multiple times)
    //end in end (do not visit end multiple times)
    //only visit small caves once, if at all

    Graph graph;
    std::ifstream input("test.txt");
    std::string line;
    char delimiter = '-';
    int nodeIndex = 0;
    while (std::getline(input, line))
    {
        int delimPos = line.find(delimiter);
        std::string val1, val2;
        val1 = line.substr(0, delimPos);
        val2 = line.substr(delimPos + 1, line.length());
        Node* node1 = graph.getNode(val1);
        Node* node2 = graph.getNode(val2);
        if (node1 == nullptr)
        {
            node1 = new Node(val1, isUpper(val1), nodeIndex);
            graph.addNode(node1);
            nodeIndex++;
        }
        if (node2 == nullptr)
        {
            node2 = new Node(val2, isUpper(val2), nodeIndex);
            graph.addNode(node2);
            nodeIndex++;
        }
        
        node1->addConnectedNodes(node2);
        node2->addConnectedNodes(node1);
        Edge* edge = new Edge(node1, node2);
        graph.addEdge(edge);

        if (val1 == "start")
        {
            graph.setStartNode(node1);
        }
        else if (val2 == "start")
        {
            graph.setStartNode(node2);
        }
        else if (val1 == "end")
        {
            graph.setEndNode(node1);
        }
        else if (val2 == "end")
        {
            graph.setEndNode(node2);
        }

    }
    //graph.pruneUnusableNodes();
    traverseGraphV2(&graph);
    std::cout << "Hello World!\n";
}
