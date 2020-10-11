cmake_minimum_required(VERSION 3.17)
project(GG_PSQL_BD2)

set(CMAKE_CXX_STANDARD 14)

include_directories(Components)
include_directories(Controller)
include_directories(Testcases)
include_directories(data)

add_executable(GG_PSQL_BD2
        Components/SequentialFile.h
        Components/Alumno.h
        Testcases/TestSequential.h
        main.cpp
        Components/Sql.h
        Controller/AppController.h
        Testcases/TestController.h
        Components/FileEntity.h
        Components/Planner.h
        Components/node.h
        Components/edge.h
        Components/Graph.h
        Testcases/TestPlanner.h)
