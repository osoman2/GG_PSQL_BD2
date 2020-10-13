#ifndef BUCKET_H
#define BUCKET_H
#include "GeneralHeader.h"
#define iter(i,a,b) for(int i=a ; i<b ; i++)
using namespace std;
const int TAM_MAX = 3;

class Bucket{
    public:
        int tam_act=0;
        int next_del=-1;
        Juego juegos[TAM_MAX];
        //vector<Juego> juegos;
       
        vector<Juego> return_vect_juegos_lleno(){
            vector<Juego>res;
            iter(i,0,TAM_MAX){
                res.push_back(juegos[i]);
            }
            return res;
        }
        void clear_bucket_lleno(){
            tam_act = 0;next_del=-1;
            iter(i,0,TAM_MAX){
                Juego aux;
                aux = juegos[i];
                aux.next_del = -2;
            }
        }


        void show_back_content(){
            if(tam_act==0){
                cout<<"\nBuck is empty\n";
            }
            else{
                cout<<"\n---------------------------------------------------";
                cout<<"\ntamanho: "<<tam_act<<" next_del: "<<next_del;
                iter(i,0,TAM_MAX){
                    if(juegos[i].next_del>=-1){
                        juegos[i].show_value();
                    }
                }
                cout<<"---------------------------------------------------";
            }    
        }

        Juego find(char* key){           
            iter(i,0,TAM_MAX){
                if(juegos[i].next_del==0){
                    if(!strcmp(juegos[i].name,key)){   
                        return juegos[i];
                    }
                }
            }
        }

        bool eliminate(char* key){
            int aux_tam = tam_act-1;
            if(next_del==-1&&aux_tam>=0){
                iter(i,0,TAM_MAX){
                    if(juegos[i].next_del==0){
                        if(!strcmp(juegos[i].name,key)){
                        next_del = i+1;
                        juegos[i].next_del=-1;
                        tam_act--;
                        return true;
                        }    
                    }
                }
                cout<<"No se encuentra el registro para eliminar";
                return true;
            }
          
            else if(aux_tam>=0){
                iter(i,0,TAM_MAX){
                    if(juegos[i].next_del==0){
                        if(!strcmp(juegos[i].name,key)){
                            juegos[i].next_del = next_del;
                            next_del = i+1;
                            tam_act--;
                            return true;
                        }
                    }    
                }
                return true;
            }
            return false; //en caso de quedar en 0;
        }

        bool add_toBucket(Juego juego){//agregar un registro si este tiene espacio
            int aux_tam = tam_act;juego.next_del=0;
            if(next_del==-1&&aux_tam+1<=TAM_MAX){
                juegos[tam_act]=juego;
                tam_act++;
                return true;
            }
            else if(aux_tam+1<=TAM_MAX){
                int aux2 = juegos[next_del-1].next_del;
                juegos[next_del-1]=juego;
                next_del = aux2;
                tam_act++;
                return true;
            }
            return false;
        }
        
        
};

ofstream& operator<< (ofstream& stream, Bucket& bucket){
	stream.write((char*)&bucket.tam_act,sizeof(int));
	stream.write((char*)&bucket.next_del,sizeof(int));
	
    if(bucket.tam_act == TAM_MAX){
        iter(i,0,bucket.tam_act){
            stream.write((char*)&bucket.juegos[i],sizeof(Juego));
        }    
    }
    else{
        iter(i,0,TAM_MAX){
                stream.write((char*)&bucket.juegos[i],sizeof(Juego));
        }
       /* iter(i,0,TAM_MAX - bucket.tam_act){
            Juego defaultgame;
            stream.write((char*)&defaultgame,sizeof(defaultgame));
        }*/
    }

	stream<< flush;
	return stream;
}

ifstream& operator>> (ifstream& stream, Bucket& bucket)
{
	stream.read((char*)&bucket.tam_act,sizeof(int));
	stream.read((char*)&bucket.next_del,sizeof(int));
	//stream.get(); 
	
    iter(i,0,TAM_MAX){
        Juego aux;
        stream.read((char*)&aux,sizeof(Juego));
        bucket.juegos[i]= aux;
    }

	return stream;
}

#endif