#ifndef FILEENTITY_H
#define FILEENTITY_H

#include <iostream>

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

#endif // FILEENTITY_H
