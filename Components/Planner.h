//
// Created by Raul.Mosquera on 10/10/2020.
//

#ifndef GG_PSQL_BD2_PLANNER_H
#define GG_PSQL_BD2_PLANNER_H

#include <iostream>
#include <vector>
#include "Graph.h"
#include <semaphore.h>
#define N 4

using namespace std;

sem_t db_elements[N]; //resources from data base

class Planner {
private:

    typedef vector<pair<string, string>> operation; //Operation, resource

    unordered_map<string, operation> transactions;
    unordered_map<string, int> resources;

public:
    string prefix;
    graph precedence;
    bool print;
    int num;

    Planner() {
        this->prefix = "T";
        this->num = 0;
        precedence = graph(true);
        print = false;

        for (int i = 0; i < N; i++)
            sem_init(&db_elements[i], 0, 0);
    }

    void* execute(void *){
        cout << "TestPlanner2 :: execute from Thread ID : " << std::endl;
        return NULL;
    }

    void addTransaction(int id) {
        auto str_id = this->prefix + to_string(id);
        transactions[str_id];

        precedence.addVertex(str_id, 0, 0);
    }

    void read(int id, string resource) {
        find_conflicts(id, "R", resource);

        if (this->resources.find(resource) == this->resources.end()) {
            this->resources[resource] = this->num;
            this->num++;
        }

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("R", resource));

        if (this->print)
            cout << str_id << " READ " << resource << endl;
    }

    void write(int id, string resource) {
        find_conflicts(id, "W", resource);

        if (this->resources.find(resource) == this->resources.end()) {
            this->resources[resource] = this->num;
            this->num++;
        }

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("W", resource));

        if (this->print)
            cout << str_id << " WRITE " << resource << endl;
    }

    void exe_plan(int id) {
        auto str_id = this->prefix + to_string(id);

        auto operations = this->transactions[str_id];

        for (auto operation: operations) {
            if (operation.first == "R" )
                this->xread(id, operation.second);
            else
                this->xwrite(id, operation.second);
        }
    }

    void xread(int id, string resource) {
        auto sem_no = this->resources[resource];

        auto str_id = this->prefix + to_string(id);
        //cout << str_id << " r semaphore " << sem_no <<endl;
        sem_post(&db_elements[sem_no]);
        if (this->print)
            cout << str_id << " xREAD " << resource << endl;
    }

    void xwrite(int id, string resource) {
        auto sem_no = this->resources[resource];

        auto str_id = this->prefix + to_string(id);
        //cout << str_id << " w semaphore " << sem_no <<endl;
        sem_wait(&db_elements[sem_no]);
        if (this->print)
            cout << str_id << " xWRITE " << resource << endl;
    }

    bool isSerializable() {
        return !precedence.is_cyclic();
    }

    void showPrecedenceGraph() {
        precedence.printGraph();
    }

    void showResource() {
        for(auto res: this->resources)
            cout << res.first << " " << res.second <<endl;
    }

    void show_operations(int id) {
        auto str_id = this->prefix + to_string(id);

        auto operations = this->transactions[str_id];

        for(auto operation: operations)
            cout << str_id << ": " << operation.first << " " <<operation.second << endl;
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
                        if (!precedence.findEdge(to_id, from_id, resource))
                            precedence.addEdge(to_id, from_id, 0, resource);
                    }
            }
        }
    }

public:
    ~Planner() {
        //delete precedence;
    }
};


#endif //GG_PSQL_BD2_PLANNER_H
