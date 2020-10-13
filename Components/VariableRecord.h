//
// Created by Raul.Mosquera on 11/10/2020.
//

#ifndef GG_PSQL_BD2_VARIABLERECORD_H
#define GG_PSQL_BD2_VARIABLERECORD_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

template<typename T>
class VariableRecord {
public:
    string fileName;

    VariableRecord(string fileName) {
        this->fileName = fileName;
    }

    vector<T> load() {
        vector<T> tmp;
        fstream file;
        string linea;
        bool firstLine;
        file = fstream();

        file.open(this->fileName, ios::in);

        firstLine = true;
        while(getline(file, linea)) {
            if (!firstLine) {
                tmp.push_back(T(linea));
            } else
                firstLine = false;
        }

        if(file.is_open())
            file.close();

        return tmp;
    }

    void add(T record) {
        fstream out;
        string header;
        bool existFile;
        //check if is first time
        ifstream filetmp (this->fileName);

        if (filetmp.is_open()) {
            filetmp.close();
            existFile = true;
        } else {
            header = record.getHeader();
            existFile = false;
        }

        if (!existFile) {
            out.open(this->fileName, ios::in | ios::out | ios::app );
            out << header << endl;
        } else
            out.open(this->fileName, ios::in | ios::out);

        if (existFile)
            out.seekg(0, ios::end);

        auto recordLine = record.toLineWithSep();

        out << recordLine << endl;

        out.close();
    }

    T readRecord(int pos) {
        fstream file;
        string linea;
        int index=0;

        file = fstream();

        file.open(this->fileName, ios::in);
        while(getline(file, linea)) {
            if (index == pos + 1) {
                file.close();
                return T(linea);
            }
            index++;
        }

        if(file.is_open()) {
            file.close();
            cout << "No position found" << endl;
        } else
            cout << " File not found" << endl;

        return T();
    }
};


#endif //GG_PSQL_BD2_VARIABLERECORD_H
