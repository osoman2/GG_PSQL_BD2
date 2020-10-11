# Base de Datos 2: Proyecto 1

Integrantes:
* Mosquera, Raúl
* Nicho, Jorge
* Vilchez, Osman

## Introducción
La emergencia sanitaria en la que nos encontramos ha traído varios desafíos en múltiples ámbitos, siendo la tecnología una parte crucial en la solución de estos. En consecuencia, uno de los ámbitos que han incrementado exponencialmente su impacto y su promoción en la sociedad, considerado como una "alternativa segura" de diversión, son los videojuegos. Tanto es así, que gran cantidad de jugadores casuales y targets imprevistos se han unido a esta forma de entretenimiento. Prueba de ello, es que a mediados del presente año 2020, 2 juegos casuales "FallGuys" y "AmongUs" (desarrollado el 2016 para plataformas móviles) han dado mantenimiento a sus plataformas por fallas en los servidores por la alta concurrencia de jugadores no contemplada. Esto ha promovido que plataformas de distribución de juegos como STEAM presenten catálogos extendidos y requieran cuantificar a los jugadores por juego para exigir la robustez de los servidores tanto propios como de los juegos de su catálogo.

El objetivo de este proyecto es realizar un gestor de base de datos simple con interfaz gráfica de usuario, limitado a las operaciones de búsqueda, inserción y eliminación, usando dos técnicas de organización de archivos. Además, se va a simular un planificador de transacciones concurrentes con las operaciones de lectura y escritura.

El dominio de datos esta basado en la plataforma digital de videojuegos, STEAM. Los campos son: Código, Nombre, Descripción, Precio y valoración.

Los resultados esperados para este proyecto es poder realizar satisfactoriamente un gestor de base de datos eficiente con una interfaz amigable para el usuario. Este será puesto a prueba para medir su desempeño en las diferentes operaciones, y posteriormente ser analizado.

## Técnicas utilizadas

### Sequential FIle
Lo definimos como un archivo que mantiene los registros ordenados fisicamente en base al valor de alguno de sus campos. Este nos permite tener búsquedas eficientes. Sin embargo, son dificiles de mantener y generan un costo extra a la hora de reorganizar el archivo.

  * Inserción
  Localizamos la posición en donde será insertado el nuevo registro. Si el espacio esta libre, lo insertamos. De lo contrario, insertamos un registro en un espacio auxiliar y los actualizamos los punteros.
  
  ```
  void add(T record) {
        int auxFileSize;
        //Alumno alumnotmp;
        T recordPrev;
        fstream auxFile;
        fstream inFile;
        bool existFile;
        int offset;
        //logical position to insert
        auto indexInsert = this->getIndex(record.nombre);

        ifstream filetmp(this->auxfileName);

        if (filetmp.is_open()) {
            filetmp.read((char*) &auxFileSize, sizeof(int));
            filetmp.close();
            existFile = true;
            auxFileSize++;
        } else {
            existFile = false;
            auxFileSize = 1;
        }

        if (!existFile)
            auxFile.open(this->auxfileName, ios::in | ios::out | ios::binary | ios::app );
        else
            auxFile.open(this->auxfileName, ios::in | ios::out | ios::binary);

        auxFile.write((char *) &auxFileSize, sizeof(int));

        if ( indexInsert.second == 'd') {
            inFile.open(this->fileName, ios::in | ios::out | ios::binary);

            if (inFile.is_open()) {
                inFile.read((char*) &this->size, sizeof(int));

                //offset = sizeof(int)*2 + sizeof(char);
                offset = this->headerLength;
                inFile.seekg(indexInsert.first * sizeof(T) + offset);
                inFile.read((char*) &recordPrev, sizeof(T));

                record.destiny = recordPrev.destiny;
                if (indexInsert.first != 0)
                    record.next = recordPrev.next;
                else
                    record.next = 0;

                recordPrev.destiny = 'a';
                recordPrev.next = auxFileSize - 1;

                if (indexInsert.first != 0) {
                    inFile.seekg(indexInsert.first * sizeof(T)+offset);
                    inFile.write((char*) &recordPrev, sizeof(T));
                } else {
                    //update first record pointer
                    inFile.seekg(sizeof(int) * 2); // 1
                    inFile.write((char*) &recordPrev.next, sizeof(int));
                    inFile.write((char*) &recordPrev.destiny, sizeof(char));
                }

                inFile.close();
            }
        } else { //aux
            offset = sizeof(int);
            auxFile.seekg(indexInsert.first * sizeof(T)+offset);
            auxFile.read((char*) &recordPrev, sizeof(T));

            record.destiny = recordPrev.destiny;
            record.next = recordPrev.next;

            recordPrev.destiny = 'a';
            recordPrev.next = auxFileSize - 1;

            auxFile.seekg(indexInsert.first * sizeof(T)+offset);
            auxFile.write((char*) &recordPrev, sizeof(T));
        }

        auxFile.seekg(0, ios::end);
        auxFile.write((char *) &record, sizeof(record));
        auxFile.close();

        if ( auxFileSize == this->reorgThreshold )
            this->reorg();
    }
  ```
  
  * Eliminación
  Utilizamos los punteros para saltar las tuplas eliminadas hasta llegar al registro que queremos eliminar.
  
  ```
  bool delete_key(string key) {
        auto result = exists(key);
        auto actualNode = (result.first).first;
        auto prevNode = (result.first).second;

        if (result.second) {

            auto prevNodetmp = getPrevious(actualNode.index, key, actualNode.source, prevNode);
            auto actualReg = read_from_node(actualNode);
            auto prevReg = read_from_node(prevNodetmp);

            prevReg.next = actualReg.next;
            prevReg.destiny = actualReg.destiny;

            auto oldReg = actualReg;

            actualReg.next = DELETED_REG;
            actualReg.destiny = actualNode.source;

            auto resPrev = write_from_node(prevNodetmp, prevReg, oldReg);
            auto resAct = write_from_node(actualNode, actualReg, oldReg);

            return true;
        } else {
            return false;
        }
    }
  ```
  
  * Búsqueda
  Usamos un algoritmo de busqueda binaria para llegar al registro que queremos buscar.
  
  ```
  T search(string key) {
        ifstream inFile;
        fstream auxFile;
        T record;

        auto result = exists(key);

        if (result.second) {
            auto tmp = result.first;
            auto index = (tmp.first).index;
            auto source = (tmp.first).source;

            if (source == 'd') { //principal

                inFile.open(this->fileName, ios::binary);

                if (inFile.is_open()) {
                    if ( index == 0)
                        inFile.seekg(this->headerLength);
                    else
                        inFile.seekg(index * sizeof(T) + this->headerLength);

                    inFile.read((char*) &record, sizeof(T));
                    inFile.close();

                    return record;
                } else {
                    return T(); //Alumno();
                }

            } else if (source == 'a') { //auxiliar

                auxFile.open(this->auxfileName, ios::in);

                if (auxFile.is_open()) {
                    auxFile.seekg(index*sizeof(T) + sizeof(int));
                    auxFile.read((char*) &record, sizeof(T));

                    auxFile.close();

                    return record;
                } else {
                    return T(); //Alumno();
                }
            }
        } //else {
        return T(); //Alumno();
        //}
    }
  ```
  
### Extendible Hashing
Lo definimos como un hash dinámico para gestionar grandes base de datos y que reducen su tamaño en el tiempo (transaccionales). Estos indexan los registros en una tabla de direcciones de buckets usando un prefijo/sufijo.
  * Inserción
  Lozalizamos el bucket usando la secuencia D-bit. Si no encuentra el bucket, procede a buscar uno con la con la profundidad local mínima. Si no lo encuentra, procede a crear el bucket, y si lo encuentra y no esta lleno, procedemos a insertar. Por el contrario, si el bucket se encuentra lleno, lo dividimos y reinsertamos todos los registros. Entonces se crean nuevo buckets con una nueva profundidad local y el índice es modificado.
  En caso no se pueda incrementar la profundidad, ocurre un desbordamiento.
  ```
  ```
  
  * Eliminación
  Localizar el bucket respectivo mediante el índice y remover el registro. Si el bucket queda vacío, puede sr liberado, lo que implica actualizar el índice. Por otro lado, si dos buckets quedan con pocos elementos y tienen el mismo prefijo en la profundida local anterior, procedemos a mezclarlos, lo que implica actualizar el índice nuevamente.
  ```
  ```
  
  * Búsqueda
  Hacemos coincidir la secuencia D-bit con una entrada del directorio y nos dirigimos al bucket correspondiente para encontrar el registro.
  ```
  ```
  
### Transacciones
Lo definimos como un conjunto de operaciones de acceso a base de datos que conforman una unidad lógica de trabajo.

```
class Planner {
public:
    string prefix;
    typedef vector<pair<string, string>> operation; //Operation, resource

    unordered_map<string, operation> transactions;

    graph *precedence;
    bool print;

    Planner() {
        this->prefix = "T";
        precedence = new graph(true);
        print = false;
    }

    void addTransaction(int id) {
        auto str_id = this->prefix + to_string(id);
        transactions[str_id];

        precedence->addVertex(str_id, 0, 0);
    }

    void read(int id, string resource) {
        find_conflicts(id, "R", resource);

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("R", resource));

        if (this->print)
            cout << str_id << " READ " << resource << endl;
    }

    void write(int id, string resource) {
        find_conflicts(id, "W", resource);

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("W", resource));

        if (this->print)
            cout << str_id << " WRITE " << resource << endl;
    }

    bool isSerializable() {
        return !precedence->is_cyclic();
    }
    void showPrecedenceGraph() {
        precedence->printGraph();
    }

private:
    void find_conflicts(int fromTx, string ope, string resource) {
        auto from_id = this->prefix + to_string(fromTx);
        string ope_str;

        if (ope == "R")
            ope_str = "W";
        else
            ope_str = "R";

        for(auto & transaction : transactions) {
            if(transaction.first != from_id) {
                auto to_id = transaction.first;
                auto operations = transaction.second;

                for(auto itOpe=operations.rbegin() ; itOpe != operations.rend() ; ++itOpe)
                    if(itOpe->first == ope_str && itOpe->second == resource) {
                        if (!precedence->findEdge(to_id, from_id, resource))
                            precedence->addEdge(to_id, from_id, 0, resource);
                    }
            }
        }
    }

public:
    ~Planner() {
        delete precedence;
    }
};
```

## Resultados experimentales

### Comparación entre técnicas de indexación
  * Inserción
  * Búsqueda

### Análisis

## Pruebas de uso

## Video


