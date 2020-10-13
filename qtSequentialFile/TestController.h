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

        lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunAdd(string filename, string codigo, string nombre, string carrera, int ciclo) {
        vector<string> lines;
        AppController<Alumno> app;

        lines.push_back("INSERT INTO "+filename+" VALUES (\""+codigo+"\",\""+nombre+"\",\""+carrera+"\","+std::to_string(ciclo)+")");
        lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunDelete(string filename, string nombre) {
        vector<string> lines;
        AppController<Alumno> app;

        lines.push_back("DELETE FROM "+filename+" WHERE nombre = \""+nombre+"\"");
        lines.push_back("SELECT * FROM " + filename);

        app.run(lines);

    }

    static void testRunSearch(string filename, string nombre) {
        vector<string> lines;
        AppController<Alumno> app;

        lines.push_back("SELECT * FROM "+filename+" WHERE nombre = \""+nombre+"\"");

        app.run(lines);

    }

};

#endif // TESTCONTROLLER_H
