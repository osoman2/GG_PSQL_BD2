#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "AppController.h"
#include "Sql.h"
#include "Alumno.h"
#include <string>

class TestController {
public:
    static void testRun(string filename) {
        vector<string> lines;
        AppController<Alumno> app;

        //lines.push_back("INSERT INTO "+filename+" VALUES (\"9991\",\"Chambilla\",\"CS\",1)");
        //lines.push_back("INSERT INTO "+filename+" VALUES (\"1112\",\"andy\",\"finanzas\",2)");
        lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("DELETE FROM "+filename+" WHERE nombre = \"Jacqueline\"");
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunAdd(string filename, string codigo, string nombre, string carrera, int ciclo) {
        vector<string> lines;
        AppController<Alumno> app;

        //lines.push_back("INSERT INTO "+filename+" VALUES (9991,\"Heider\",\"CS\",11)");
        lines.push_back("INSERT INTO "+filename+" VALUES (\""+codigo+"\",\""+nombre+"\",\""+carrera+"\","+std::to_string(ciclo)+")");
        lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("DELETE FROM "+filename+" WHERE nombre = \"Jacqueline\"");
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunDelete(string filename, string nombre) {
        vector<string> lines;
        AppController<Alumno> app;

        //lines.push_back("INSERT INTO "+filename+" VALUES (9991,\"Heider\",\"CS\",11)");
        //lines.push_back("INSERT INTO "+filename+" VALUES (\""+codigo+"\",\""+nombre+"\",\""+carrera+"\","+std::to_string(ciclo)+")");
        lines.push_back("DELETE FROM "+filename+" WHERE nombre = \""+nombre+"\"");
        lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("DELETE FROM "+filename+" WHERE nombre = \"Jacqueline\"");
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunSearch(string filename, string nombre) {
        vector<string> lines;
        AppController<Alumno> app;

        lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \""+nombre+"\"");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("DELETE FROM "+filename+" WHERE nombre = \"Jacqueline\"");
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

//    static void testSQL(string string1) {
//        Sql sql1 = Sql(string1);
//        cout << "SQL " << string1 << " translated into: " <<endl;
//        if ( sql1.valid) {
//            cout << sql1.operation << endl;
//            cout << "table: "<<sql1.table << endl;
//            if ( sql1.operation == "add") {
//
//                for(auto value: sql1.val_cond)
//                    cout << value << endl;
//            } else if ( sql1.operation == "delete_key") {
//                for(int i=0; i < sql1.col_cond.size(); i++)
//                    cout << sql1.col_cond[i] << "\t" << sql1.val_cond[i] << endl;
//            } else if (sql1.operation == "read") {
//                cout << "Columns:" << endl;
//                for(auto col: sql1.columns)
//                    cout << col << endl;
//                cout << "Condition:" <<endl;
//                for(int i=0; i < sql1.col_cond.size(); i++)
//                    cout << sql1.col_cond[i] << "\t" << sql1.val_cond[i] << endl;
//            }
//            cout << endl;
//
//        } else
//            cout << "Not valid"<< endl;
//
//    }
};

#endif // TESTCONTROLLER_H
