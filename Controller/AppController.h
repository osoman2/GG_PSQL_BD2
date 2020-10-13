//
// Created by Raul.Mosquera on 6/10/2020.
//

#ifndef GG_PSQL_BD2_APPCONTROLLER_H
#define GG_PSQL_BD2_APPCONTROLLER_H

#include <iostream>
#include <vector>
#include <VariableRecord.h>
#include "../Components/Sql.h"
#include "../Components/SequentialFile.h"

using namespace std;

template<typename T>
class AppController {
public:
    bool run(vector<string> lines) {
        vector<Sql> sqls;
        vector<T> result;

        for(auto line: lines) {
            auto tmp = Sql(line);
            sqls.push_back(tmp);

            if(!tmp.valid) {
                cout << "Error!" <<endl;
                return false;
            }
        }

        bool rc;
        for(auto sqlsentece: sqls) {
            cout << sqlsentece.sentence << endl;
            result = execute_sentence(sqlsentece, &rc);

            for (auto res: result)
                res.showData();
        }

        if(result.size() > 0)
            this->saveOutput("../data/output.txt", result);
    }

private:

    void saveOutput(string filename, vector<T> data) {
        auto fileOutput = VariableRecord<T>(filename);
        const char * file1 = filename.c_str();

        remove(file1);

        for(auto record: data)
            fileOutput.add(record);
    }

    vector<T> execute_sentence(Sql sqlsentence, bool rc) {
        vector<T> result;

        result.clear();
        switch (sqlsentence.operation) {
            case 'a': {
                vector<string> tmp;
                auto file = SequentialFile<T>(sqlsentence.table);

                for(auto val: sqlsentence.val_cond)
                    tmp.push_back(remove_special(val));

                auto recordObj = T(tmp);
                file.add(recordObj);

                rc = true;
            } break;
            case 'r': {
                auto file = SequentialFile<T>(sqlsentence.table);

                auto key = sqlsentence.val_cond.at(0);
                key = remove_special(key);
                auto singleRes = file.search(key);
                result.push_back(singleRes);

                rc = true;
            } break;
            case 'l': {
                auto file = SequentialFile<T>(sqlsentence.table);

                result = file.load('d');
                auto resultAux = file.load('a');

                result.insert(end(result), begin(resultAux), end(resultAux));

                rc = true;
            } break;
            case 'd': {
                auto file = SequentialFile<T>(sqlsentence.table);

                auto key = sqlsentence.val_cond.at(0);
                key = remove_special(key);
                auto rc_del = file.delete_key(key);
                rc = true;
            } break;
            default: rc = false;
        }

        return result;
    }

    string remove_special(string str) {
        string tmp=str;

        auto found = string(tmp).find("\"");
        if (found!=string::npos)
            tmp.replace(found, found + 1, "");

        found = string(tmp).find("\"", found + 1);
        if (found!=string::npos)
            tmp.replace(found, found + 1, "");

        return tmp;
    }
};


#endif //GG_PSQL_BD2_APPCONTROLLER_H
