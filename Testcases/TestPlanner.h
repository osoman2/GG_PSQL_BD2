//
// Created by Raul.Mosquera on 10/10/2020.
//

#ifndef GG_PSQL_BD2_TESTPLANNER_H
#define GG_PSQL_BD2_TESTPLANNER_H

#include <Planner.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

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

    static void serializableTest() {
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

class TestPlanner2
{
public:
    Planner plan;

    TestPlanner2() {
        this->plan = Planner();
        this->plan.print = true;
    }

    void * t1_before_plan() {
//    cout << "TestPlanner2 :: execute from Transaction 1 " << pthread_self()
        cout << "Executed from Transaction 1 " << endl;
        this->plan.addTransaction(1);
        this->plan.read(1, "Y");
        //sleep(2); //do something with Y
        this->plan.write(1, "Y");
        this->plan.read(1, "X");
        //sleep(2); //do something with X
        this->plan.write(1, "X");

        return NULL;
    }

    void * t2_before_plan(){
        cout << "Executed from Transaction 2 " << endl;
        this->plan.addTransaction(2);
        this->plan.read(2, "Z");
        this->plan.read(2, "Y");
        //sleep(1); //do something with Y
        this->plan.write(2, "Y");
        this->plan.read(2, "X");
        //sleep(2); //do something with X
        this->plan.write(2, "X");

        return NULL;
    }

    void * t3_before_plan() {
        cout << "Executed from Transaction 3 " << endl;
        this->plan.addTransaction(3);
        this->plan.read(3, "Y");
        //sleep(2); //do something with Y
        this->plan.read(3, "Z");
        this->plan.write(3, "Y");
        //sleep(1); //do something with Z
        this->plan.write(3, "Z");

        return NULL;
    }

    void * t1_exe_plan() {
        //this->plan.show_operations(1);
        this->plan.exe_plan(1);
    }

    void * t2_exe_plan() {
        //this->plan.show_operations(2);
        this->plan.exe_plan(2);
    }

    void * t3_exe_plan() {
        //this->plan.show_operations(3);
        this->plan.exe_plan(3);
    }

    bool isSerializable() {
        return this->plan.isSerializable();
    }

    void show_graph() {
        this->plan.showPrecedenceGraph();
    }

    void show_resources() {
        this->plan.showResource();
    }

};

typedef void * (*THREADFUNCPTR)(void *);

class TestConcurrency {
public:
    static int run() {
        TestPlanner2 * testPlan = new TestPlanner2();
        // Thread id
        pthread_t threadId1;
        pthread_t threadId2;
        pthread_t threadId3;

        int err = pthread_create(&threadId1, NULL, (THREADFUNCPTR) &TestPlanner2::t1_before_plan, testPlan);
        if (err) {
            std::cout << "Thread creation failed : " << strerror(err);
            return err;
        }

        err = pthread_create(&threadId2, NULL, (THREADFUNCPTR) &TestPlanner2::t2_before_plan, testPlan);
        if (err) {
            std::cout << "Thread creation failed : " << strerror(err);
            return err;
        }

        err = pthread_create(&threadId3, NULL, (THREADFUNCPTR) &TestPlanner2::t3_before_plan, testPlan);
        if (err) {
            std::cout << "Thread creation failed : " << strerror(err);
            return err;
        }

        // Do some stuff in Main Thread
        //std::cout << "Waiting for thread " << threadId1 << " to exit" << std::endl;
        err = pthread_join(threadId1, NULL);
        if (err)
            return err;

        //std::cout << "Waiting for thread " << threadId2 << " to exit" << std::endl;
        err = pthread_join(threadId2, NULL);
        if (err)
            return err;

        //cout << "Waiting for thread " << threadId3 << " to exit" << std::endl;
        err = pthread_join(threadId3, NULL);
        if (err)
            return err;

        cout << "Precedence graph " << endl;
        testPlan->show_graph();
        //testPlan->show_resources();

        auto res = (testPlan->isSerializable()) ? "Yes" : "No";
        cout << "Is serializable? " << res <<endl;

        pthread_t threadExe1;
        pthread_t threadExe2;
        pthread_t threadExe3;
        err = pthread_create(&threadExe1, NULL, (THREADFUNCPTR) &TestPlanner2::t1_exe_plan, testPlan);
        err = pthread_create(&threadExe2, NULL, (THREADFUNCPTR) &TestPlanner2::t2_exe_plan, testPlan);
        err = pthread_create(&threadExe3, NULL, (THREADFUNCPTR) &TestPlanner2::t3_exe_plan, testPlan);

        cout <<endl;
        cout << "Equivalent plan" <<endl;
        pthread_join(threadExe2, NULL);
        pthread_join(threadExe1, NULL);
        pthread_join(threadExe3, NULL);

        delete testPlan;

        std::cout << "Finish" << std::endl;
    }
};
#endif //GG_PSQL_BD2_TESTPLANNER_H
