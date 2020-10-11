//
// Created by Raul.Mosquera on 10/10/2020.
//

#ifndef GG_PSQL_BD2_TESTPLANNER_H
#define GG_PSQL_BD2_TESTPLANNER_H

#include <Planner.h>
#include <iostream>
#include <thread>

using namespace std;

class TestPlanner {
    Planner plan;

public:
    TestPlanner() {
        plan = Planner();
        plan.addTransaction(1);
        plan.addTransaction(2);
        plan.addTransaction(3);
    }

    void *transaction1() {
        plan.read(1, "X");
        plan.write(1, "X");
        plan.read(1, "Y");
        plan.write(1, "Y");
    }

    void *transaction2() {
        plan.read(2, "Z");
        plan.read(2, "Y");
        plan.write(2, "Y");
        plan.read(2, "X");
        plan.write(2, "X");
    }

    void *transaction3() {
        plan.read(3, "Y");
        plan.read(3, "Z");
        plan.write(3, "Y");
        plan.write(3, "Z");
    }

    void runThreads() {

    }

    void serializableTest() {
        auto plan = Planner();

        plan.print = true;
        plan.addTransaction(1);
        plan.addTransaction(2);
        plan.addTransaction(3);
        cout<<"EXAMPLE 1"<<endl;
        plan.read(2, "Z");
        plan.read(2, "Y");
        plan.write(2, "Y");
        plan.read(3, "Y");
        plan.read(3, "Z");
        plan.read(1, "X");
        plan.write(1, "X");
        plan.write(3, "Y");
        plan.write(3, "Z");
        plan.read(2, "X");
        plan.read(1, "Y");
        plan.write(1, "Y");
        plan.write(2, "X");

        plan.showPrecedenceGraph();

        auto res = (plan.isSerializable())? "Yes" : "No";
        cout << "Serializable " << res <<endl;

        auto plan2 = Planner();

        plan2.print = true;
        plan2.addTransaction(1);
        plan2.addTransaction(2);
        plan2.addTransaction(3);
        cout<<"EXAMPLE 2"<<endl;
        plan2.read(2, "Z");
        plan2.read(2, "Y");
        plan2.write(2, "Y");
        plan2.read(3, "Y");
        plan2.read(3, "Z");
        plan2.write(3, "Y");
        plan2.read(1, "Y");
        plan2.write(1, "Y");
        plan2.write(3, "Z");
        plan2.read(2, "X");
        plan2.write(2, "X");
        plan2.read(1, "X");
        plan2.write(1, "X");

        plan2.showPrecedenceGraph();

        auto res2 = (plan2.isSerializable())? "Yes" : "No";
        cout << "Serializable " << res2 <<endl;
    }
};

void foo() {

}

class TestConcurrency {
public:
    static void run() {
        TestPlanner *test1 = new TestPlanner();
        //thread test(foo);

        delete test1;
    }
};
#endif //GG_PSQL_BD2_TESTPLANNER_H
