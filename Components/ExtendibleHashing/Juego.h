#ifndef JUEGO_H
#define JUEGO_H
#include "GeneralHeader.h"
using namespace std;

class Juego{
  public:
    char id[10]="";
    char name[15]="";
    char description[100]="";
    int next_del=-2;
    int HorasJugadas=0;

    Juego(){}

   Juego (char* _id,char* _name,char* _description,int h_j){
      strcpy(id,_id);
      strcpy(name,_name);
      strcpy(description,_description);
      HorasJugadas = h_j;
   }

    void set_value(){
        cout<<"Ingrese el id:\n";
        cin.getline(id,10);
        cout<<"\nIngrese el nombre:\n";
        cin.getline(name,15);
        cout<<"\nIngrese la descripcion:\n";
        cin.getline(description,100);
        cout<<"\nIngrese las horas jugadas:\n";
        cin>>HorasJugadas;
        cin.ignore(1);
    }
    void show_value(){
        cout<<"\nId: "<<id<<"\n";
        cout<<"Nombre: "<<name<<"\n";
        cout<<"Descripción: "<<description<<"\n";
        cout<<"Horas jugadas: "<<HorasJugadas<<"\n";
        cout<<"Next del: "<<next_del<<"\n";
    }
  //  ~Juego(){};


};


ofstream& operator<< (ofstream& stream, Juego & record){
	stream.write((char*)&record.next_del,sizeof(int));
	stream.write((char*)&record.id, 10);
	//stream.write((char*)&" ", 1); 
	stream.write((char*)&record.name, 15); 
	stream.write((char*)&record.description,100); 
	stream.write((char*)&record.HorasJugadas,sizeof(int)); 
    stream.write("\n", 1); 
	//stream<< "\n";
	stream<< flush;
	return stream;
}

ifstream& operator>> (ifstream& stream, Juego& record)
{
	stream.read((char*)&record.next_del,sizeof(int));
	stream.read((char*)&record.id, 10);
	//stream.get(); 
	stream.read((char*)&record.name, 15); 
	stream.read((char*)&record.description,100);
	stream.read((char*)&record.HorasJugadas,sizeof(int));
    stream.get(); 
	return stream;
}


#endif