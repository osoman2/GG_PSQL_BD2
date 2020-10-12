#ifndef TESTSEQUENTIAL_H
#define TESTSEQUENTIAL_H

#include "SequentialFile.h"
#include "Alumno.h"

void searchTest(string key, string filename) {
  auto myfile = SequentialFile<Alumno>(filename);
  auto searchRes = myfile.search(key);

  cout << "Search: " << key << endl;
  if (searchRes.next != -7)
    cout << searchRes.toLine() << endl;
  else
    cout << "Not found" << endl;
}

void searchRangeTest(string begin, string end, string filename) {
  auto myfile = SequentialFile<Alumno>(filename);

  auto searchRes = myfile.search(begin, end);
  cout << "From "<<begin << " to " << end <<endl;
  if (searchRes.size() == 0) {
    cout << "Nothing found" << endl;
  } else {
    for(auto res: searchRes) {
      cout << res.toLine() <<endl;
    }
  }
}

void testAdd(char* codigo, char* nombre, char* carrera, int ciclo, string filename) {
  auto myfile = SequentialFile<Alumno>(filename);
  Alumno alumno = Alumno(codigo, nombre, carrera, ciclo, 'd' );

  cout << endl;
  cout << "Adding "<<endl;
  alumno.showData();

  myfile.add(alumno);
  auto result = myfile.load('d');
  cout << endl;
  cout << "File after add func" << endl;
  for(auto line: result)
    line.showData();

  cout << endl;

  cout << "Aux file after add func" << endl;
  auto resultAux = myfile.load('a');
  for(auto line: resultAux)
    line.showData();
}

void testAddandReorg(string filename) {
  cout << ">>>Testing add and reorganization"<< endl;
  testAdd("9767", "Jacqueline", "Psicologia", 6, filename);
  testAdd("1207", "Roberto", "Ing.Electro", 4, filename);
  testAdd("9641", "Julia", "Contabilidad", 10, filename);
  testAdd("1133", "Angela", "Disenio", 5, filename);
  testAdd("9917", "Paola", "Ing.Electrica", 10, filename);
  testAdd("1777", "Julio", "Economia", 10, filename);
}

void testDel(string key, string filename) {
  auto myfile = SequentialFile<Alumno>(filename);
  cout << "Deleting... "<< key << endl;
  auto searchRes = myfile.delete_key(key);

  if (searchRes)
    cout << "deleted "<<endl;
  else
    cout << "not deleted "<<endl;

  cout << endl;
  auto result = myfile.load('d');
  for(auto line: result)
    line.showData();

  cout << endl;
  result = myfile.load('a');
  for(auto line: result)
    line.showData();

  cout << endl;

}

void testLoad_add_Alumno(string filename) {
    auto myfile = SequentialFile<Alumno>(filename);

    vector<Alumno> tmp;

    tmp.push_back(Alumno("8777", "Osman", "CS", 7,'d'));
    tmp.push_back(Alumno("3142", "Jonathan", "CS", 7,'d'));
    tmp.push_back(Alumno("1253", "Jose", "CS", 7,'d'));
    tmp.push_back(Alumno("9761", "Luis", "CS", 6,'d'));
    tmp.push_back(Alumno("1283", "David", "CS", 6,'d'));
    tmp.push_back(Alumno("1873", "Christian", "CS", 7,'d'));
    tmp.push_back(Alumno("6312", "Ivan", "CS", 5,'d'));
    tmp.push_back(Alumno("3129", "Piero", "CS", 7,'d'));
    tmp.push_back(Alumno("9812", "Raul", "CS", 6,'d'));
    tmp.push_back(Alumno("1312", "Jorge", "CS", 7,'d'));

    myfile.insertAll(tmp);

    auto result = myfile.load('d');

    for(auto line: result)
        line.showData();

    cout << endl;
    result = myfile.load('a');
    for(auto line: result)
        line.showData();

    testAdd("9767", "Jacqueline", "Psicologia", 6, filename);
    testAdd("1207", "Roberto", "Ing.Electro", 4, filename);
    testAdd("9641", "Julia", "Contabilidad", 10, filename);
    testAdd("1133", "Angela", "Disenio", 5, filename);
    testAdd("9917", "Paola", "Ing.Electrica", 10, filename);
    testAdd("1777", "Julio", "Economia", 10, filename);
    cout << endl;
    result = myfile.load('d');

    for(auto line: result)
        line.showData();

    cout << endl;
    result = myfile.load('a');
    for(auto line: result)
        line.showData();
}

void testP1(string filename) {
  auto myfile = SequentialFile<Alumno>(filename);

  vector<Alumno> tmp;

  /*tmp.push_back(Alumno("8777", "Osman", "CS", 7,'d'));
  tmp.push_back(Alumno("3142", "Jonathan", "CS", 7,'d'));
  tmp.push_back(Alumno("1253", "Jose", "CS", 7,'d'));
  tmp.push_back(Alumno("9761", "Luis", "CS", 6,'d'));
  tmp.push_back(Alumno("1283", "David", "CS", 6,'d'));
  tmp.push_back(Alumno("1873", "Christian", "CS", 7,'d'));
  tmp.push_back(Alumno("6312", "Ivan", "CS", 5,'d'));
  tmp.push_back(Alumno("3129", "Piero", "CS", 7,'d'));
  tmp.push_back(Alumno("9812", "Raul", "CS", 6,'d'));
  tmp.push_back(Alumno("1312", "Jorge", "CS", 7,'d'));

  myfile.insertAll(tmp);*/

  auto result = myfile.load('d');

  for(auto line: result)
    line.showData();

  cout << endl;
  result = myfile.load('a');
  for(auto line: result)
    line.showData();

  cout << endl;
  cout << "File size: " << myfile.length() << endl;
  //Test search Ivan
  //searchTest("Julia");

  //testDel("Roberto");
  /*//Test search Mosquera
  searchTest("Mosquera");

  //Test search Alvaro
  searchTest("Alvaro");

  //Test search Christian
  searchTest("Christian");

  //Test search Christian
  searchTest("Raul");

  //
  searchRangeTest("Dave", "Luisa");

  //
  searchRangeTest("Alvaro", "Xiomara");

  //
  searchRangeTest("Ernesto", "Esteban"); */
  //auto res=myfile.getIndexForInsert("Ivan");
  //cout << " res= " << res.first << " "<<res.second << endl;

  //testAddandReorg();
}

#endif // TESTSEQUENTIAL_H
