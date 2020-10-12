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
       
        void show_back_content(){
            if(tam_act==0){
                cout<<"\nBuck is empty\n";
            }
            else{
                cout<<"\ntamanho: "<<tam_act<<" next_del: "<<next_del;
                iter(i,0,tam_act){
                    juegos[i].show_value();
                }
            }    
        }

        Juego find(char* key){           
            iter(i,0,tam_act){

                if(!strcmp(juegos[i].name,key)){
                    if(juegos[i].next_del!=-2){   
                        return juegos[i];
                    }
                }
        
            }
        }

        bool eliminate(char* key){
            int aux_tam = tam_act-1;
            if(next_del==-1&&aux_tam>=0){
                iter(i,0,tam_act){
                    if(juegos[i].name==key){
                        next_del = i+1;
                        juegos[i].next_del=-1;
                        tam_act--;
                        return true;
                    }    
                }
            }
          
            else if(aux_tam>=0){
                iter(i,0,tam_act){
                    if(juegos[i].name==key){
                        if(juegos[i].next_del==0){
                            juegos[i].next_del = next_del;
                            next_del = i+1;
                        }
                        return true;
                    }    
                }
            }
            return false;
        }

        bool add_toBucket(Juego juego){//agregar un registro si este tiene espacio
            int aux_tam = tam_act;
            if(next_del==-1&&aux_tam+1<=TAM_MAX){
                juegos[tam_act]=juego;
                tam_act++;
                return true;
            }
            else if(aux_tam+1<=TAM_MAX){
                int aux2 = juegos[next_del-1].next_del;
                juegos[aux2-1].next_del = -1;
                juegos[next_del-1]=juego;
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
        iter(i,0,bucket.tam_act){
            stream.write((char*)&bucket.juegos[i],sizeof(Juego));
        }
        iter(i,0,TAM_MAX - bucket.tam_act){
            Juego defaultgame;
            defaultgame.next_del = -2;
            stream.write((char*)&defaultgame,sizeof(defaultgame));
        }
    }

	stream<< flush;
	return stream;
}

ifstream& operator>> (ifstream& stream, Bucket& bucket)
{
	stream.read((char*)&bucket.tam_act,sizeof(int));
	stream.read((char*)&bucket.next_del,sizeof(int));
	//stream.get(); 
	
    iter(i,0,bucket.tam_act){
        Juego aux;
        stream.read((char*)&aux,sizeof(Juego));
        bucket.juegos[i]= aux;
    }

	return stream;
}

#endif