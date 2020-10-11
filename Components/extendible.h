#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>

#define iter(i,a,b) for(int i=a ; i<b ; i++)


using namespace std;


int TAM_MAX = 3;

bool is_empty_f(ifstream& pFile){
	    return pFile.peek() == std::ifstream::traits_type::eof();
}



class Juego{
  public:
    char id[10];
    char name[15];
    char description[100];
    int next_del;
    int HorasJugadas; 
};

ostream& operator<< (ostream& stream, Juego & record){
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

istream& operator>> (istream& stream, Juego& record)
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




class Bucket{
    public:
        int tam_act;
        int next_del;
        vector<Juego> juegos;



        Juego find(char* key){
            for(auto it:juegos){
                if(it.name==key){
                    if(it.next_del!=0)return it;
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
            int aux_tam = tam_act-1;
            if(next_del==-1&&aux_tam<=TAM_MAX){
                juegos.push_back(juego);
                tam_act++;
                return true;
            }
            else if(aux_tam<=TAM_MAX){
                int aux2 = juegos[next_del-1].next_del;
                juegos[aux2-1].next_del = -1;
                juegos[next_del-1]=juego;
                tam_act++;
                return true;
            }
            return false;
        }
        
};

ostream& operator<< (ostream& stream, Bucket& bucket){
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
        iter(i,0,TAM_MAX-bucket.tam_act){
            char aux[sizeof(Juego)];
            stream.write((char*)&aux,sizeof(Juego));
        }
    }

	stream<< flush;
	return stream;
}

istream& operator>> (istream& stream, Bucket& bucket)
{
	stream.read((char*)&bucket.tam_act,sizeof(int));
	stream.read((char*)&bucket.next_del,sizeof(int));
	//stream.get(); 
	
    iter(i,0,bucket.tam_act){
        Juego aux;
        stream.read((char*)&aux,sizeof(Juego));
        bucket.juegos.push_back(aux);
    }

	return stream;
}



class ExtendibleHash{
    private:
        int GlobalDensity = 1;
        int MaxBucketSize = TAM_MAX;
        string index_name;
        string data_name;
        bool lleno_dat = false;
        int buckets_actual;
        vector<pair<int,pair<int,int>>>llave_posi; //<llave-posición de bucket>

    public:
        ExtendibleHash(string _data,string _index){
            data_name = _data;
            index_name = _index;
            ifstream datos;datos.open(data_name,ifstream::in|ifstream::binary);
            ifstream indi;indi.open(index_name,ifstream::in|ifstream::binary);
            
            if(!is_empty_f(datos))lleno_dat = true;
            
            if(!is_empty_f(indi)){
                indi.seekg(0,ios::beg);
                indi.read((char*)&GlobalDensity,sizeof(int));
                indi.read((char*)&MaxBucketSize,sizeof(int));
                for(int i = 0;i< 1<<GlobalDensity;i++){
                    int aux_key,aux_posi,aux_locald;
                    indi.read((char*)&aux_key,sizeof(int));
                    indi.read((char*)&aux_posi,sizeof(int));
                    indi.read((char*)&aux_locald,sizeof(int));
                    llave_posi.push_back({aux_key,{aux_posi,aux_locald}});
                }
                indi.close();
            }
            else{
                indi.close();
                ofstream indi;indi.open(index_name,ofstream::out|ofstream::binary);
                indi.seekp(0,ios::beg);
                indi.write((char*)&GlobalDensity,sizeof(int));
                indi.write((char*)&TAM_MAX,sizeof(int));
                
                int aux_key,aux_posi=0;
                int aux_locald = 1;
                indi.write((char*)&aux_key,sizeof(int));
                indi.write((char*)&aux_posi,sizeof(int));
                indi.write((char*)&aux_locald,sizeof(int));

                llave_posi.push_back({aux_key,{aux_posi,aux_locald}});
                aux_posi++;aux_key++;
                indi.write((char*)&aux_key,sizeof(int));
                indi.write((char*)&aux_posi,sizeof(int));
                indi.write((char*)&aux_locald,sizeof(int));
                llave_posi.push_back({aux_key,{aux_posi,aux_locald}});
                indi.close();
            }
            datos.close();
             
        }
        int hashf(char* key){
            int value = (int)key % (1<<GlobalDensity);
            int par = llave_posi[value].second.first;
            return par*sizeof(Bucket);
        }   


        Juego search(char* key){
            int posiaux = hashf(key);
            ifstream archi;
            archi.open(data_name,ifstream::in|ifstream::binary);
            archi.seekg(posiaux);
            Bucket buck;
            archi.read((char*)&buck,sizeof(Bucket));
            archi.close();
            return buck.find(key);
        }

        void split(int posi){
            
            int dens_local  = llave_posi[posi].second.second;
            int abuscar = llave_posi[posi].second.first;
            vector<Juego> juegos_to_reposicionar;
            fstream datos; datos.open(data_name,fstream::in|fstream::out|fstream::binary);

            if(dens_local<=GlobalDensity){
                 vector<int> similar_posi;
                 iter(i,0,(int)llave_posi.size()){
                     if(llave_posi[i].second.first==abuscar){
                         similar_posi.push_back(i);
                     }
                 }
                 
                Bucket buck;
                datos.seekg(abuscar*sizeof(Bucket));
                datos.read((char*)&buck,sizeof(Bucket));
                juegos_to_reposicionar = buck.juegos;                
                int cont_superficie = 0;//Posiciones en el techo
                iter(i,0,(int)similar_posi.size()){
                    if(llave_posi[i].first != abuscar){
                        llave_posi[i].second.first = cont_superficie+buckets_actual-1;
                        cont_superficie++;                        
                    }    
                }
                buckets_actual = (int)similar_posi.size() + buckets_actual-1;
            }
            else{
                int tam_aux =(int)llave_posi.size();
                for(int i = 0;i<tam_aux;i++){
                    int apuntador = llave_posi[i].second.first;
                    int apuntador_prima = i+ pow(2,(GlobalDensity-1));
                    llave_posi[apuntador_prima].second.first = apuntador;
                }
            }
            datos.close();
            update_ind();

        }

        void  lack(int key){

        }

        void update_ind(){
            fstream ind_file;
            ind_file.open(index_name,fstream::out|fstream::in|fstream::binary);
            ind_file.seekp(0,ios::beg);
            ind_file.write((char*)&GlobalDensity,sizeof(int));vector<int> same_key;
            ind_file.write((char*)&MaxBucketSize,sizeof(int)); 
            for(auto it:llave_posi){  
                ind_file.write((char*)&it.first,sizeof(int));
                ind_file.write((char*)&it.second.first,sizeof(int));
                ind_file.write((char*)&it.second.second,sizeof(int));
            }
            ind_file.close();
        }


        void add(Juego juego){
            int posiaux = hashf(juego.name);
            fstream archi;
            archi.open(data_name,fstream::out|fstream::in|fstream::binary);
            archi.seekg(posiaux);
            Bucket buck;
            archi.read((char*)&buck,sizeof(Bucket));
            if(buck.add_toBucket(juego)){
                archi.seekp(posiaux);
                archi.write((char*)&buck,sizeof(Bucket));
                archi.close();
            }
            else{
                archi.close();
                int key = (int)juego.name % (1<<GlobalDensity);
                split(key);
                add(juego);
            }
        }

        void delete_element(char* key){
            int posiaux = hashf(key);
            fstream archi;
            archi.open(data_name,fstream::out|fstream::in|fstream::binary);
            archi.seekg(posiaux);
            Bucket buck;
            archi.read((char*)&buck,sizeof(Bucket));
            if(buck.eliminate(key)){
                archi.seekp(posiaux);
                archi.write((char*)&buck,sizeof(Bucket));
                archi.close();
            }
            else{
                archi.close();
                int key_aux = (int)key % (1<<GlobalDensity);
                lack(key_aux);
                delete_element(key);
            }
        }

};


int main(){

    Juego juego1,juego2,juego3;
    


    string dataname = "dat.bin";
    string indename = "ind.bin";
    ExtendibleHash extend_table(dataname,indename);
    extend_table.add(juego1);
    extend_table.add(juego2);
    extend_table.add(juego3);
 
    return 0;
}
