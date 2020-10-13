#include "Testcases/TestSequential.h"
#include "Testcases/TestController.h"
#include "Testcases/TestPlanner.h"

int main() {
    string filename = "../data/Alumno.txt";
    //TestController::testRun(filename);
    //testLoad_add_Alumno(filename);
    //TestPlanner::serializableTest();
    TestConcurrency::run();
    string filenameGames = "../data/gamesfullv2.dat";
    //testLoad_add_Gamesfull(filenameGames);
    //testLoad_add_Games2(filenameGames);
    //TestController::testRunGames(filenameGames);
}