// Created by felix on 10/10/19.
#ifndef GRAPHS_NOMEQUIEROIRSENORGRAFO_GRAPH_H
#define GRAPHS_NOMEQUIEROIRSENORGRAFO_GRAPH_H

#include "../components/node.h"
#include "../components/edge.h"
#include <functional>

struct Traits {
    typedef string N;
    typedef float E;
};

template<typename Tr>
class Graph {
public:
    typedef typename Tr::N N;
    typedef typename Tr::E E;

    typedef Graph<Tr> self;
    typedef Node<self> node;
    typedef Edge<self> edge;

    typedef unordered_map<N, node *> NodeSeq;
    typedef vector<edge *> EdgeSeq;
    typedef typename NodeSeq::const_iterator NodeIte;
private:
    NodeSeq nodes;
    bool directed;

private:
    NodeIte findVertex(N tag) { return nodes.find(tag); }

public:
    Graph() : directed(false) {}
    Graph(bool directed) : directed(directed) {}

    Graph(const Graph &oldGraph) { //Copy constructor
        if ( this->nodes.size() == 0 ) {
            for( auto nodeIte = oldGraph.firstNode() ;
                 nodeIte != oldGraph.lastNode() ;
                 nodeIte++) {
                this->nodes.insert( *nodeIte );
            }
        }
    }

    bool isDirected(){ return directed; }

    node *addVertex(N tag, double x, double y) {
        auto newNode = new node(tag, x, y); //Node or vertex
        nodes.insert({tag, newNode});

        return newNode;
    }

    void addVertex(node *oldNode) {
        node *newNode = new node(oldNode);
        nodes[oldNode->data] = newNode;
    }

    bool addEdge(N from, N to, E weight, string tag) {
        if ((nodes.find(from) == nodes.end()) || (nodes.find(to) == nodes.end())) { return false; }
        E newWeight;

        if(!weight)
            newWeight = getDistance(nodes[from], nodes[to]);
        else
            newWeight = weight;

        node *nodeFrom = nodes[from];
        node *nodeTo = nodes[to];

        if (this->directed) {
            auto edgeFromTo = new edge(nodeFrom, nodeTo, newWeight, tag);
            nodeFrom->edges.emplace_back(edgeFromTo);
            return true;
        }

        auto edgeFromTo = new edge(nodeFrom, nodeTo, newWeight, tag);
        nodeFrom->edges.emplace_back(edgeFromTo);
        auto edgeToFrom = new edge(nodeTo, nodeFrom, newWeight, tag);
        nodeTo->edges.emplace_back(edgeToFrom);

        return true;
    }

    bool addDirectedEdge(N from, N to) {
        if ((nodes.find(from) == nodes.end()) || (nodes.find(to) == nodes.end())) { return false; }

        auto weight = getDistance(nodes[from], nodes[to]);

        node *nodeFrom = nodes[from];
        node *nodeTo = nodes[to];

        auto newEdge = new edge(nodeFrom, nodeTo, weight);
        nodeFrom->edges.emplace_back(newEdge);
        return true;

    }

    E getDistance(node *nodeFrom, node *nodeTo) {
        const int EARTH_DIAMETER = 12742;
        auto latFrom = nodeFrom->x; auto latTo = nodeTo->x;
        auto longFrom = nodeFrom->y; auto longTo = nodeTo->y;

        /*auto distance =  EARTH_DIAMETER*asin(sqrt(pow(sin((latTo - latFrom) * (M_PI / 180) / 2), 2) +
                                         pow(sin((longTo - longFrom) * (M_PI / 180) / 2), 2) *
                                         cos(latFrom * M_PI / 180) * cos(latTo * M_PI / 180)));

        return distance;*/

        auto distance =  pow(sin((latTo - latFrom) * (M_PI / 180) / 2), 2) +
                         pow(sin((longTo - longFrom) * (M_PI / 180) / 2), 2) *
                         cos(latFrom * M_PI / 180) * cos(latTo * M_PI / 180);

        distance = atan2(sqrt(distance), sqrt(1-distance));

        return EARTH_DIAMETER*distance;
    }

    bool deleteNode(N tag) {
        bool result = false;
        EdgeSeq *edges;
        auto nodeIte = findVertex(tag);

        if (nodeIte != nodes.end()) {
            result = true;
            nodes.erase(nodeIte);
        }
        // Deletes edges with Node(tag) as destination
        for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
            edges = &(*nodeIt).second->edges;
            for (auto edgeIt = edges->begin(); edgeIt != edges->end(); ++edgeIt) {
                auto nodeTo = (*edgeIt)->nodes[1]->data;
                if (nodeTo == tag) {
                    edges->erase(edgeIt);
                    break;
                }

            }
        }

        return result;
    }

    bool deleteEdge(N from, N to) {
        auto delEdge = findEdge(from, to);

        if (!delEdge)
            return false;
        else {
            auto edges = &nodes[from]->edges;
            for (auto edgeIt = edges->begin(); edgeIt != edges->end(); ++edgeIt) {
                auto edge = *edgeIt;
                if (edge->nodes[1]->data == to) {
                    edges->erase(edgeIt);
                    break;
                }
            }
        }

        if (!this->directed) {
            delEdge = findEdge(to, from);
            if (!delEdge)
                return false;
            else {
                auto edges = &nodes[to]->edges;
                for (auto edgeIt = edges->begin(); edgeIt != edges->end(); ++edgeIt) {
                    auto edge = *edgeIt;
                    if (edge->nodes[1]->data == from) {
                        edges->erase(edgeIt);
                        break;
                    }
                }
            }
        }
        return true;
    }

    bool findvertex(N tag) {
        auto result = this->findVertex(tag);

        if (result == this->nodes.end())
            return false;
        else
            return true;
    }

    edge *findEdge(N from, N to) {
        auto nodeFrom = nodes[from];
        if (!nodeFrom) return nullptr;

        auto nodeTo = nodes[to];
        if (!nodeTo) return nullptr;

        for (auto &&edg: nodeFrom->edges)
            //if (edg->nodes[1]->data == nodeTo->data)
            if (edg->nodes[1] == nodeTo)
                return edg;

        return nullptr;
    }

    edge *findEdge(N from, N to, string tag) {
        auto nodeFrom = nodes[from];
        if (!nodeFrom) return nullptr;

        auto nodeTo = nodes[to];
        if (!nodeTo) return nullptr;

        for (auto &&edg: nodeFrom->edges)
            if (edg->nodes[1] == nodeTo && edg->tag == tag)
                return edg;

        return nullptr;
    }

    int getNumberOfNodes() const { return nodes.size(); }

    int getNumberOfEdges() const {
        int result = 0;
        for (auto itNods = nodes.begin(); itNods != nodes.end(); ++itNods) {
            node *nod = itNods->second;
            auto sizeOfEdgesForEachNode = nod->sizeEdges();
            result += sizeOfEdgesForEachNode;
        }
        if (!this->directed)
            result = result / 2;

        return result;
    }

    E getEdgesWeightSum() {
        E sum = 0;

        for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
            auto node = nodeIt->second;
            auto edges = node->edges;
            for (auto edgeIt = edges.begin(); edgeIt != edges.end(); ++edgeIt) {
                auto edge = *edgeIt;
                sum += edge->weight;
            }
        }

        if (this->directed) { return sum; }
        else { return sum / 2; }
    }

    void printGraph() {
        cout << "Imprimiendo GRAFOOOO" << endl;
        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            cout << it->first << " : { ";
            auto miniMap = it->second;
            for (auto i: miniMap->edges) {
                cout << "(" << (i->nodes[0])->data << " : " << (i->nodes[1])->data << " resource: " << i->tag << ") ";
            }
            cout << "}" << endl;
        }
        cout << endl;
    }

    bool is_cyclic() {
        unordered_map<N, bool> visited;
        unordered_map<N, bool> traversed;

        for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
            visited.insert({nodeIt->second->data, false});
            traversed.insert({nodeIt->second->data, false});
        }

        for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
            auto tag = nodeIt->first;
            if (!visited[tag])
                if (this->is_cyclic_util(tag, visited, traversed))
                    return true;
        }
        return false;
    }

    bool is_cyclic_util(N tag, unordered_map<N, bool> visited, unordered_map<N, bool> traversed) {
        visited[tag] = true;
        traversed[tag] = true;

        auto neighbours = this->nodes[tag]->edges;

        for(auto neighbour: neighbours) {
            auto tagNeighbour = neighbour->nodes[1]->data;
            if (!visited[tagNeighbour]) {
                if (this->is_cyclic_util(tagNeighbour, visited, traversed))
                    return true;
            } else if (traversed[tagNeighbour]) {
                return true;
            }
        }
        traversed[tag] = false;

        return false;
    }

//  Iterators
    NodeIte firstNode() const {
        return this->nodes.begin();
    }

    NodeIte lastNode() const {
        return this->nodes.end();
    }

    ~Graph() {
        /*for( auto nodeforDel: this->nodes) {
            auto oldNode = nodeforDel.second;

            for( auto edgeIte = oldNode->firstEdge() ;
                 edgeIte != oldNode->lastEdge() ;
                 edgeIte++) {
                auto oldEgde = *(edgeIte);
                this->deleteEdge( oldEgde->nodes[0]->data, oldEgde->nodes[1]->data );
            }

           this->deleteNode(oldNode->data);
        }*/
        this->nodes.clear();
    }
};

typedef Graph<Traits> graph;

#endif //GRAPHS_NOMEQUIEROIRSENORGRAFO_GRAPH_H
