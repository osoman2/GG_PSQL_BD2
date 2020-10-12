#include "GeneralHeader.h"
#define iter(i,a,b) for(int i=a ; i<b ; i++)
using namespace std;


int main(){

       Juego juego1, juego2,juego3;
       juego1.set_value();
       juego2.set_value();
       juego3.set_value();
    //string dataname = "data.dat";
    //string indename = "ind.dat";
    ExtendibleHash extend_table("data.dat","ind.dat");

    extend_table.add(juego1);
    extend_table.add(juego2);
   extend_table.add(juego3);
    extend_table.search("hola").show_value();
 
    return 0;
}
