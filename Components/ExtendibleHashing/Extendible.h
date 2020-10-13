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
        void show_hash(){
            for(auto it:llave_posi){
                cout<<"p_hash: "<<it.first<<" p_dat: "<<it.second.first<<" l_desnsity: "<<it.second.second<<"\n";
            }
        }
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
                
                int aux_key = 0;
                int aux_posi=0;
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
            ifstream data; data.open(data_name,ifstream::out|ifstream::binary);
            Bucket buck1;
            Bucket buck2;
            Bucket buck3;
            //show the 2 first buckets
            
            data.read((char*)&buck1,sizeof(Bucket));
            data.read((char*)&buck2,sizeof(Bucket));
            data.read((char*)&buck3,sizeof(Bucket));
            buck1.show_back_content();
            buck2.show_back_content();
            buck3.show_back_content();
            data.close();
            indi.seekg(0,ios::beg);
                //Reading headers
                indi.read((char*)&GlobalDensity,sizeof(int));
                indi.read((char*)&MaxBucketSize,sizeof(int));
                indi.read((char*)&buckets_actual,sizeof(int));
                for(int i = 0;i< (int)pow(2,GlobalDensity);i++){
                    int aux_key,aux_posi,aux_locald;
                    indi.read((char*)&aux_key,sizeof(int));
                    indi.read((char*)&aux_posi,sizeof(int));
                    indi.read((char*)&aux_locald,sizeof(int));
                    llave_posi.push_back({aux_key,{aux_posi,aux_locald}});
                }     
            indi.close();          
        }

        int hashf(char* key){           
            int val_key = val_string(key);
            int value = val_key % (int)pow(2,GlobalDensity);
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

        void split(char* key,Juego juego){
            int val_key = val_string(key);            
            cout<<"valkey "<<val_key<<key;
            int llave_posi_hash = val_key % (int)pow(2,GlobalDensity);
            cout<<" posi: "<<llave_posi_hash;
            int local_dens = llave_posi[llave_posi_hash].second.second;
             cout<<" ldens: "<<local_dens<<"\n";
            int posi_buxk_dat = hashf(key);
            llave_posi[llave_posi_hash].second.second++;//aumentando densidad local del buck principal
            Bucket bucket_lleno;
            fstream datos; datos.open(data_name,fstream::in|fstream::out|fstream::binary);
            datos.seekg(posi_buxk_dat);
            datos.read((char*)&bucket_lleno,sizeof(Bucket));

            if(local_dens<GlobalDensity){
                //Buscando las entradas que coinciden en el bucket
                vector<int> posi_currents_buck;
                for(auto it:llave_posi){
                    if( (it.first!=llave_posi_hash) && (it.second.first==llave_posi[llave_posi_hash].second.first)){
                        posi_currents_buck.push_back(it.first);
                    }
                }
                
                //Reasignando las posiciones de los overflow y cambiando el tamaño de buckets actuales
                for(auto it:posi_currents_buck){
                    llave_posi[it].second.first = buckets_actual;
                    llave_posi[it].second.second++;
                    Bucket buckaux;
                    buckaux.next_del = -1;
                    datos.seekp((buckets_actual)*sizeof(Bucket));
                    datos.write((char*)&buckaux,sizeof(Bucket));
                    buckets_actual++;
                }
                
                vector<Juego>copia_vec;
                for(auto it:bucket_lleno.return_vect_juegos_lleno()){
                    copia_vec.push_back(it);
                }
                //vacío el bucket en el archivo data.dat
                bucket_lleno.clear_bucket_lleno();
                datos.seekp(posi_buxk_dat);
                datos.write((char*)&bucket_lleno,sizeof(Bucket));
                
                datos.close();

                //actualizo los indices del archivo indi.dat
                update_ind();
                //Inserto todo en los nuevos buckets
                add(juego);
                insert_all(copia_vec);

               

            }
            else{
                vector<int> posi_currents_buck;
                for(auto it:llave_posi){
                    if( (it.first!=llave_posi_hash) && (it.second.first==llave_posi[llave_posi_hash].second.first)){
                        cout<<"posi hash original: "<<llave_posi_hash;
                        cout<<" posi en ind: "<<it.first<<" posi en dat: "<<it.second.first;
                        posi_currents_buck.push_back(it.first);
                    }
                }                
                //Reasignando las posiciones de los overflow y cambiando el tamaño de buckets actuales
                for(auto it:posi_currents_buck){
                    llave_posi[it].second.first = buckets_actual;
                    llave_posi[it].second.second++;
                    Bucket buckaux;
                    buckaux.next_del = -1;
                    datos.seekp((buckets_actual)*sizeof(Bucket));
                    datos.write((char*)&buckaux,sizeof(Bucket));
                    buckets_actual++;
                }

                //Creando nuevas entradas para los nuevos niveles
                iter(i,pow(2,GlobalDensity),pow(2,GlobalDensity+1)){
                    int posi_de_hash = i;  
                    int pos_de_dat = llave_posi[ i-pow(2,GlobalDensity)].second.first;
                    int densi_nuevo = llave_posi[i-pow(2,GlobalDensity)].second.second;     
                    //nuevos buckets en dat
                    if((i-pow(2,GlobalDensity))==llave_posi_hash){
                        Bucket buckaux;
                        buckaux.next_del = -1;
                        cout<<"posi_nuevo_buck"<<buckets_actual<<"\n";
                        pos_de_dat = buckets_actual; 
                        datos.seekp((buckets_actual)*sizeof(Bucket));
                        datos.write((char*)&buckaux,sizeof(Bucket));
                        buckets_actual++; 
                    }
                    llave_posi.push_back({posi_de_hash,{pos_de_dat,densi_nuevo}});
                }
                GlobalDensity++;

                //Copi el vector en overflow
                vector<Juego>copia_vec;
                for(auto it:bucket_lleno.return_vect_juegos_lleno()){
                    copia_vec.push_back(it);
                }
                //vacío el bucket en el archivo data.dat
                bucket_lleno.clear_bucket_lleno();
                datos.seekp(posi_buxk_dat);
                datos.write((char*)&bucket_lleno,sizeof(Bucket));
                datos.close();
                
                update_ind();
                //Inserto todo en los nuevos buckets
                add(juego);
                insert_all(copia_vec);
                //actualizo los indices del archivo indi.dat
            }
        }

        void  lack(int key){

        }

        void update_ind(){
            fstream ind_file;
            ind_file.open(index_name,fstream::out|fstream::in|fstream::binary);
            ind_file.seekp(0,ios::beg);
            ind_file.write((char*)&GlobalDensity,sizeof(int));
            ind_file.write((char*)&MaxBucketSize,sizeof(int)); 
            ind_file.write((char*)&buckets_actual,sizeof(int));
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
                split(juego.name,juego);
            }
        }

        void insert_all(vector<Juego>vec){
            for(auto it:vec){
                add(it);
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
                cout<<"El bucket ya está vacío\n";
            }
        }
        ~ExtendibleHash(){}

};
#endif