//
// Created by Raul.Mosquera on 9/10/2020.
//

#ifndef GG_PSQL_BD2_FILEENTITY_H
#define GG_PSQL_BD2_FILEENTITY_H

#include <iostream>
#include <iomanip>
const char* sep = "|";

static int NOT_FOUND = -7;

using namespace std;

class FileEntity {
public:
    int next;
    char destiny;

    virtual void showData() = 0;

    virtual void readData()=0;

    virtual string getHeader()=0;

    virtual string toLine()=0;

    virtual string toLineWithSep()=0;
};


#endif //GG_PSQL_BD2_FILEENTITY_H
