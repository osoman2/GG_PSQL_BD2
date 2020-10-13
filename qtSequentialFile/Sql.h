#ifndef SQL_H
#define SQL_H

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

class Sql {
private:
    int rc;
    char ope;

public:
    string table;
    vector<string> columns;
    vector<string> col_cond;
    vector<string> val_cond;
    string sentence;
    char operation; //string operation;
    bool valid;

    Sql(string sentence) {
        char* sep = " ";

        char str[sentence.length() + 1];
        strcpy(str, sentence.c_str());

        this->sentence = sentence;

        char * pch;
        pch = strtok (str, sep);

        valid = false;
        if (string(pch) == "INSERT") {
            operation = 'a'; // add
            ope = 'A';
            rc = insert_sentence(sentence);
        } else if (string(pch) == "DELETE") {
            operation = 'd'; //delete_key
            ope = 'D';
            rc = delete_sentence(sentence);
        } else if (string(pch) == "SELECT" ) {
            operation = 'r'; //read
            ope = 'R';
            rc = select_sentence(sentence);
        } else {
            this->valid = false;
            rc = 1;
        }
    }

    int insert_sentence(string pch) {
        auto tmp = string(pch);

        size_t found = string(tmp).find("VALUES");
        if (found==string::npos)
            return 12;

        found = string(tmp).find("INTO");
        if (found==string::npos)
            return 13;

        //remove INSERT INTO
        tmp = tmp.substr(found+4, (tmp.length()-found+4));
        tmp = trim(tmp);

        char str[tmp.length() + 1];
        strcpy(str, tmp.c_str());

        found = string(tmp).find("VALUES");

        this->table = tmp.substr(0, found-1);

        string values;

        values = tmp.substr(found+6, tmp.length());
        values = trim(values);

        if (values[0] != '(' )
            return 14;

        if (values[values.length()-1] != ')' )
            return 15;

        values = values.substr(1, values.length()-2);

        char str_tmp[values.length() + 1];
        strcpy(str_tmp, values.c_str());

        char * pch_tmp;
        pch_tmp = strtok(str_tmp, ",");

        while (pch_tmp != NULL) {
            this->val_cond.emplace_back(pch_tmp);

            pch_tmp = strtok(NULL, ",");
        }

        this->valid = true;
    }

    int select_sentence(string pch) {
        auto tmp = string(pch);
        bool foundWhere;

        foundWhere = false;

        size_t found = string(tmp).find("WHERE");
        if (found!=string::npos)
            foundWhere = true;

        found = string(tmp).find("FROM");
        if (found==string::npos)
            return 33;

        //remove SELECT
        found = string(tmp).find("SELECT");
        tmp = tmp.substr(found+6, (tmp.length()-found+6));
        tmp = trim(tmp);

        found = string(tmp).find("FROM");

        //Columns
        this->columns.clear();
        auto cols = tmp.substr(0, found-1);
        this->columns.push_back(cols);

        tmp = tmp.substr(found+4, tmp.length()-1);
        tmp = trim(tmp);

        this->table = tmp;

        if (foundWhere) {
            found = string(tmp).find("WHERE");

            this->table = tmp.substr(0, found-1);

            string condition = tmp.substr(found+5, tmp.length());
            condition = trim(condition);

            parse_conditions(condition);
        } else
            this->operation = 'l'; //load

        this->valid = true;
    }

    int delete_sentence(string pch) {
        auto tmp = string(pch);

        size_t found = string(tmp).find("WHERE");
        if (found==string::npos)
            return 22;


        found = string(tmp).find("FROM");
        if (found==string::npos)
            return 23;

        //remove DELETE FROM
        tmp = tmp.substr(found+4, (tmp.length()-found+4));
        tmp = trim(tmp);

        found = string(tmp).find("WHERE");

        this->table = tmp.substr(0, found-1);

        string condition = tmp.substr(found+6, tmp.length());
        condition = trim(condition);

        parse_conditions(condition);

        this->valid = true;
    }

    void parse_conditions(const string &condition) {
        auto found = condition.find("=");

        this->col_cond.clear();
        this->val_cond.clear();

        col_cond.push_back(trim(condition.substr(0, found - 1)));
        val_cond.push_back(trim(condition.substr(found + 1, condition.length() - 1)));
    }

    string ltrim(const string& s) {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == string::npos) ? "" : s.substr(start);
    }

    string rtrim(const string& s) {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == string::npos) ? "" : s.substr(0, end + 1);
    }

    string trim(const std::string& s) {
        return rtrim(ltrim(s));
    }
};

#endif // SQL_H
