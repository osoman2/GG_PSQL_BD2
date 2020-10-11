//
// Created by Raul.Mosquera on 10/10/2020.
//

#ifndef GG_PSQL_BD2_PLANNER_H
#define GG_PSQL_BD2_PLANNER_H

#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;

class Planner {
public:
    string prefix;
    typedef vector<pair<string, string>> operation; //Operation, resource

    unordered_map<string, operation> transactions;

    graph *precedence;
    bool print;

    Planner() {
        this->prefix = "T";
        precedence = new graph(true);
        print = false;
    }

    void addTransaction(int id) {
        auto str_id = this->prefix + to_string(id);
        transactions[str_id];

        precedence->addVertex(str_id, 0, 0);
    }

    void read(int id, string resource) {
        find_conflicts(id, "R", resource);

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("R", resource));

        if (this->print)
            cout << str_id << " READ " << resource << endl;
    }

    void write(int id, string resource) {
        find_conflicts(id, "W", resource);

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("W", resource));

        if (this->print)
            cout << str_id << " WRITE " << resource << endl;
    }

    bool isSerializable() {
        return !precedence->is_cyclic();
    }
    void showPrecedenceGraph() {
        precedence->printGraph();
    }

private:
    void find_conflicts(int fromTx, string ope, string resource) {
        auto from_id = this->prefix + to_string(fromTx);
        string ope_str;

        if (ope == "R")
            ope_str = "W";
        else
            ope_str = "R";

        for(auto & transaction : transactions) {
            if(transaction.first != from_id) {
                auto to_id = transaction.first;
                auto operations = transaction.second;

                for(auto itOpe=operations.rbegin() ; itOpe != operations.rend() ; ++itOpe)
                    if(itOpe->first == ope_str && itOpe->second == resource) {
                        if (!precedence->findEdge(to_id, from_id, resource))
                            precedence->addEdge(to_id, from_id, 0, resource);
                    }
            }
        }
    }

public:
    ~Planner() {
        delete precedence;
    }
};


#endif //GG_PSQL_BD2_PLANNER_H
