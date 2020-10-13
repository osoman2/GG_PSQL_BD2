#include "GeneralHeader.h"
#define iter(i,a,b) for(int i=a ; i<b ; i++)
using namespace std;


int main(){

  ExtendibleHash extend_table("data.dat","ind.dat");
  //Juego juego1,juego2,juego3,juego4,juego5,juego6,juego7,juego8,juego9,juego10;  
  //juego1.set_value();
  //juego2.set_value();
  //juego3.set_value();
  //juego4.set_value();
  //juego5.set_value();
  //juego6.set_value();
  //juego7.set_value();
  //juego8.set_value();
  //juego9.set_value();
  //juego10.set_value();  
  
  //extend_table.add(juego1);
  //extend_table.add(juego2);
  //extend_table.add(juego3);
  //extend_table.add(juego4);
  //extend_table.add(juego5);
  //extend_table.add(juego6);
  //extend_table.add(juego7);
  //extend_table.add(juego8);
  //extend_table.add(juego9);
  //extend_table.add(juego10);
  //extend_table.show_hash();
     
 //extend_table.delete_element("oso");
     extend_table.search("oso").show_value();
    //extend_table.add(juego1);   
    return 0;
}
