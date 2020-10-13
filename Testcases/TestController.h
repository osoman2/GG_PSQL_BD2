//
// Created by Raul.Mosquera on 6/10/2020.
//

#ifndef GG_PSQL_BD2_TESTCONTROLLER_H
#define GG_PSQL_BD2_TESTCONTROLLER_H

#include "../Controller/AppController.h"
#include "../Components/Sql.h"

class TestController {
public:
    static void testRun(string filename) {
        vector<string> lines;
        AppController<Alumno> app;

        //lines.push_back("INSERT INTO "+filename+" VALUES (9991,\"Heider\",\"CS\",11)");
        //lines.push_back("INSERT INTO "+filename+" VALUES (1111,\"Ernesto\",\"CS\",2)");
        //lines.push_back("SELECT * FROM " + filename);
        //lines.push_back("DELETE FROM "+filename+" WHERE name = \"Jacqueline\"");
        //lines.push_back("SELECT * FROM " + filename);
        lines.push_back("SELECT * FROM "+filename+" WHERE name = \"Osman\"");
        //lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunGames(string filename) {
        vector<string> lines;
        AppController<Game> app;

        lines.push_back("SELECT * FROM " + filename + " WHERE name = \"Dota 2\"");
        //lines.push_back("DELETE FROM "+filename+" WHERE name = \"Dota 2\"");
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


#endif //GG_PSQL_BD2_TESTCONTROLLER_H