//
// Created by Raul.Mosquera on 11/10/2020.
//

#ifndef GG_PSQL_BD2_GAME_H
#define GG_PSQL_BD2_GAME_H

#include <cstring>
#include <vector>
#include <algorithm>
#include "FileEntity.h"


class Game : public FileEntity{
public:
    char id[10] = "";
    char name[15] = "";
    char description[100] = "";
    int horasJugadas;

    Game() {
        strcpy(this->id, "");
        strcpy(this->name, "");
        strcpy(this->description, "");
        this->next = NOT_FOUND;
    }

    Game(const char* id, const char* name,const char* description, int horasJugadas, char destiny) {
        //strcpy(this->description, "");

        strcpy(this->id, id);
        strcpy(this->name, name);
        strcpy(this->description, description);
        this->horasJugadas = horasJugadas;
        this->next = -1;
        this->destiny = destiny;
    }

    Game(string recordLine) {
        char str[recordLine.length() + 1];
        strcpy(str, recordLine.c_str());

        char * pch;
        pch = strtok (str, sep);

        int index=0;
        while (pch != NULL) {
            switch( index ) {
                case 0:
                    strcpy(this->id, pch);
                    break;
                case 1:
                    strcpy(this->name, pch);
                    break;
                case 2:
                    strcpy(this->description, pch);
                    break;
                case 3:
                    this->horasJugadas = atoi(pch);
                    break;
            }
            pch = strtok(NULL, sep);
            index++;
        }
    }

    Game(vector<string> values) {
        for(int i=0; i<values.size(); i++) {
            switch (i) {
                case 0: {
                    strcpy(this->id, values.at(i).c_str());
                } break;
                case 1: {
                    strcpy(this->name, values.at(i).c_str());
                } break;
                case 2: {
                    strcpy(this->description, values.at(i).c_str());
                } break;
                case 3: {
                    this->horasJugadas = stoi(values.at(i));
                } break;
            }
        }
    }

    void showData() {
        cout << setw(7) << left << this->id << setw(21) << this->name << setw(16) << this->description
        << setw(5) << this->horasJugadas << this->next << "-" << this->destiny << endl;
    }

    void readData() {
        cout <<"Id: ";
        cin >> this->id;
        cout <<"Name: ";
        cin >> this->name;
        cout <<"Description: ";
        cin >> this->description;
        cout <<"Horas Jugadas: ";
        cin >> this->horasJugadas;
        this->next = -1;
    }

    string toLine() {
        return string(this->id) + '\t' + string(this->name) + '\t' + string(this->description) + '\t' +
                to_string(this->horasJugadas) + '\t' + to_string(this->next) + '\t' + this->destiny;
    }

    string toLineWithSep() {
        return string(this->id) + sep + string(this->name) + sep + string(this->description) + sep +
               to_string(this->horasJugadas);
    }

    string getHeader() {
        string sep_str = string(sep);
        return "id" + sep_str + "name" + sep_str + "description" + sep_str + "horasJugadas";
    }

    bool operator <(const Game& rhs) {
        auto nombreLHS = string(this->name);
        auto nombreRHS = string(rhs.name);

        transform(nombreLHS.begin(), nombreLHS.end(), nombreLHS.begin(), ::tolower);
        transform(nombreRHS.begin(), nombreRHS.end(), nombreRHS.begin(), ::tolower);

        //cout << nombreLHS << " " << nombreRHS <<endl;

        return nombreLHS < nombreRHS;
    }

    bool operator >(const Game& rhs) {
        auto nameLHS = string(this->name);
        auto nameRHS = string(rhs.name);

        //cout << nameLHS << " " << nameRHS <<endl;
        transform(nameLHS.begin(), nameLHS.end(), nameLHS.begin(), ::tolower);
        transform(nameRHS.begin(), nameRHS.end(), nameRHS.begin(), ::tolower);

        return nameLHS > nameRHS;
    }

    bool operator == (const Game& rhs) {
        auto nameLHS = string(this->name);
        auto nameRHS = string(rhs.name);

        transform(nameLHS.begin(), nameLHS.end(), nameLHS.begin(), ::tolower);
        transform(nameRHS.begin(), nameRHS.end(), nameRHS.begin(), ::tolower);
        //cout << nameLHS << " " << nameRHS <<endl;

        return nameLHS == nameRHS;
    }
};


#endif //GG_PSQL_BD2_GAME_H
