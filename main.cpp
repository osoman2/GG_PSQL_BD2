#include "Testcases/TestSequential.h"
#include "Testcases/TestController.h"
#include "Testcases/TestPlanner.h"

int main() {
    string filename = "../data/Alumno.txt";
    testLoad_add_Alumno(filename);
    TestController::testRun(filename);
    //TestPlanner::serializableTest(); //Test Transacciones
}