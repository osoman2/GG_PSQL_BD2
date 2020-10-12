#ifndef GENERAL_HEADER_H
#define GENERAL_HEADER_H

#include "GeneralHeader.h"
#define iter(i,a,b) for(int i=a ; i<b ; i++)
using namespace std;


class ExtendibleHash{
    private:
        int GlobalDensity = 1;
        int MaxBucketSize = TAM_MAX;
        string index_name;
        string data_name;
        int buckets_actual =2;
        vector<pair<int,pair<int,int>>>llave_posi; //<llave-posición de bucket>

    public:
        ExtendibleHash(string _data,string _index){
            data_name = _data;
            index_name = _index;

            ifstream indi;indi.open(_index,ifstream::in|ifstream::binary);
            if(is_empty_f(indi)){
                indi.close();
                ofstream indi;indi.open(index_name,ofstream::out|ofstream::binary);
                
                indi.seekp(0,ios::beg);
                //Headers of file GD, TAM_MAX,buckets_actual
                indi.write((char*)&GlobalDensity,sizeof(int));
                indi.write((char*)&TAM_MAX,sizeof(int));
                indi.write((char*)&buckets_actual,sizeof(int));
                
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


                fstream datos;datos.open(data_name,fstream::out|fstream::binary);                               
                Bucket buc1,buc2;
                datos.seekp(0,ios::beg);
                datos.write((char*)&buc1,sizeof(Bucket));
                datos.write((char*)&buc2,sizeof(Bucket));        
               
                datos.close();  
            }
            else{
                read_hash();
            }
            
   
        }

        void read_hash(){
            ifstream indi;indi.open(index_name,ifstream::in|ifstream::binary);
           /* ifstream data; data.open(data_name,ifstream::out|ifstream::binary);
            Bucket buck1;
            Bucket buck2;
            data.read((char*)&buck1,sizeof(Bucket));
            data.read((char*)&buck2,sizeof(Bucket));
            buck1.show_back_content();
            buck2.show_back_content();
            data.close();*/
            indi.seekg(0,ios::beg);
                //Reading headers
                indi.read((char*)&GlobalDensity,sizeof(int));
                indi.read((char*)&MaxBucketSize,sizeof(int));
                indi.read((char*)&buckets_actual,sizeof(int));
                for(int i = 0;i< (int)pow(GlobalDensity,2);i++){
                    int aux_key,aux_posi,aux_locald;
                    indi.read((char*)&aux_key,sizeof(int));
                    indi.read((char*)&aux_posi,sizeof(int));
                    indi.read((char*)&aux_locald,sizeof(int));
                    llave_posi.push_back({aux_key,{aux_posi,aux_locald}});
                }     
            indi.close();          
        }

        int hashf(char* key){
            int key_int;
            sscanf(key, "%d", &key_int);
            int value = key_int % (int)pow(GlobalDensity,2);
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
            
       /*     int dens_local  = llave_posi[posi].second.second;
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
                    //llave_posi[apuntador_prima].second.second = ;
                }
            }
            datos.close();
            update_ind();*/

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
                int key_int;
                sscanf(juego.name, "%d", &key_int);              
                int key = key_int % (int)pow(GlobalDensity,2);

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
                int key_int;
                sscanf(key, "%d", &key_int);              
                int key_aux = key_int % (int)pow(GlobalDensity,2);
                lack(key_aux);
                delete_element(key);
            }
        }
        ~ExtendibleHash(){}

};
#endif