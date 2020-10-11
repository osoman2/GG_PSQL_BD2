#include "Testcases/TestSequential.h"
#include "Testcases/TestController.h"
#include "Testcases/TestPlanner.h"

int main() {
    string filename = "../data/Alumno.txt";
    //testP1();
    TestController::testRun(filename);
    //testLoad_add_Alumno(filename);
    //TestPlanner::serializableTest();
}