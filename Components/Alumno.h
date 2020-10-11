#ifndef GG_PSQL_BD2_ALUMNO_H
#define GG_PSQL_BD2_ALUMNO_H

//#include <iostream>

//using namespace std;

#include "FileEntity.h"

const char* sep = "|";

class Alumno : public FileEntity {
public:
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;

    Alumno() {
      strcpy(this->codigo, "");
      strcpy(this->nombre, "");
      this->next = NOT_FOUND;
    }

    Alumno(char* codigo, char* nombre, char* carrera, int ciclo,
          char destiny) {
            strcpy(this->codigo, codigo);
            strcpy(this->nombre, nombre);
            strcpy(this->carrera, carrera);
            this->ciclo = ciclo;
            this->next = -1;
            this->destiny = destiny;
    }

    Alumno(string recordLine) {
        char str[recordLine.length() + 1];
        strcpy(str, recordLine.c_str());

        char * pch;
        pch = strtok (str, sep);

        int index=0;
        while (pch != NULL) {
            switch( index ) {
                case 0:
                    strcpy(this->codigo, pch);
                    break;
                case 1:
                    strcpy(this->nombre, pch);
                    break;
                case 2:
                    strcpy(this->carrera, pch);
                    break;
                case 3:
                    this->ciclo = atoi(pch);
                    break;
            }
            pch = strtok(NULL, sep);
            index++;
        }
    }

    Alumno(vector<string> values) {
        for(int i=0; i<values.size(); i++) {
            switch (i) {
                case 0: {
                    strcpy(this->codigo, values.at(i).c_str());
                } break;
                case 1: {
                    strcpy(this->nombre, values.at(i).c_str());
                } break;
                case 2: {
                    strcpy(this->carrera, values.at(i).c_str());
                } break;
                case 3: {
                    this->ciclo = stoi(values.at(i));
                } break;
            }
        }

    }

    void showData() {
      cout << setw(7) << left << this->codigo << setw(21) << this->nombre << setw(16) << this->carrera << setw(5) << this->ciclo << this->next << "-"<<this->destiny << endl;
    }

    void readData() {
      cout <<"Código: ";
      cin >> this->codigo;
      cout <<"Nombre: ";
      cin >> this->nombre;
      cout <<"Carrera: ";
      cin >> this->carrera;
      cout <<"Ciclo: ";
      cin >> this->ciclo;
      this->next = -1;
    }

    string toLine() {
      return string(this->codigo) + '\t' + string(this->nombre) + '\t' + string(this->carrera) + '\t' + to_string(this->ciclo)
      + '\t' + to_string(this->next) + '\t' + this->destiny;
    }

    string toLineWithSep() {
        return string(this->codigo) + sep + string(this->nombre) + sep + string(this->carrera) + sep +
                to_string(this->ciclo);
    }

    string getHeader() {
        string sep_str = string(sep);
        return "codigo" + sep_str + "nombre" + sep_str + "carrera" + sep_str + "ciclo";
    }

    bool operator <(const Alumno& rhs) {
      auto nombreLHS = string(this->nombre);
      auto nombreRHS = string(rhs.nombre);

      transform(nombreLHS.begin(), nombreLHS.end(), nombreLHS.begin(), ::tolower);
      transform(nombreRHS.begin(), nombreRHS.end(), nombreRHS.begin(), ::tolower);

      return nombreLHS < nombreRHS;      
    }

    bool operator >(const Alumno& rhs) {
      auto nombreLHS = string(this->nombre);
      auto nombreRHS = string(rhs.nombre);

      transform(nombreLHS.begin(), nombreLHS.end(), nombreLHS.begin(), ::tolower);
      transform(nombreRHS.begin(), nombreRHS.end(), nombreRHS.begin(), ::tolower);

      return nombreLHS > nombreRHS;      
    }

    bool operator == (const Alumno& rhs) {
      auto nombreLHS = string(this->nombre);
      auto nombreRHS = string(rhs.nombre);

      transform(nombreLHS.begin(), nombreLHS.end(), nombreLHS.begin(), ::tolower);
      transform(nombreRHS.begin(), nombreRHS.end(), nombreRHS.begin(), ::tolower);

      return nombreLHS == nombreRHS;      
    }            
};

#endif