# Base de Datos 2: Proyecto 1

Integrantes:
* Mosquera, Raúl
* Nicho, Jorge
* Vilchez, Osman

## Introducción
La emergencia sanitaria en la que nos encontramos ha traído varios desafíos en múltiples ámbitos, siendo la tecnología una parte crucial en la solución de estos. En consecuencia, uno de los ámbitos que han incrementado exponencialmente su impacto y su promoción en la sociedad, considerado como una "alternativa segura" de diversión, son los videojuegos. Tanto es así, que gran cantidad de jugadores casuales y targets imprevistos se han unido a esta forma de entretenimiento. Prueba de ello, es que a mediados del presente año 2020, 2 juegos casuales "FallGuys" y "AmongUs" (desarrollado el 2016 para plataformas móviles) han dado mantenimiento a sus plataformas por fallas en los servidores por la alta concurrencia de jugadores no contemplada. Esto ha promovido que plataformas de distribución de juegos como STEAM presenten catálogos extendidos y requieran cuantificar a los jugadores por juego para exigir la robustez de los servidores tanto propios como de los juegos de su catálogo.

El objetivo de este proyecto es realizar un gestor de base de datos simple con interfaz gráfica de usuario, limitado a las operaciones de búsqueda, inserción y eliminación, usando dos técnicas de organización de archivos. Además, se va a simular un planificador de transacciones concurrentes con las operaciones de lectura y escritura.

El dominio de datos esta basado en la plataforma digital de videojuegos, STEAM. Los campos son: Id, Nombre, Descripción y Horas Jugadas.

Los resultados esperados para este proyecto son poder realizar satisfactoriamente un gestor de base de datos eficiente con una interfaz amigable para el usuario. Este será puesto a prueba para medir su desempeño en las diferentes operaciones, y posteriormente ser analizado.

## Técnicas utilizadas

### Sequential FIle
Lo definimos como un archivo que mantiene los registros ordenados fisicamente en base al valor de alguno de sus campos. Este nos permite tener búsquedas eficientes. Sin embargo, son dificiles de mantener y generan un costo extra a la hora de reorganizar el archivo.

  * Inserción:
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
  
  * Eliminación:
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
  
  * Búsqueda:
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
  * Inserción:
  Lozalizamos el bucket usando la secuencia D-peso, que utiliza la equivalencia de la palabra en ASCII y la profundidad. Si no encuentra el bucket, procede a buscar uno con la con la profundidad local mínima. Si no lo encuentra, procede a crear el bucket, y si lo encuentra y no esta lleno, procedemos a insertar. Por el contrario, si el bucket se encuentra lleno, lo dividimos y reinsertamos todos los registros. Entonces se crean nuevo buckets con una nueva profundidad local y el índice es modificado.
```
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

```
  
  * Eliminación:
  El algoritmo de eliminación Localizar el bucket respectivo mediante el índice y remueve utilizando la técnica de FreeList. Cuando el tamaño del Bucket sea 0, los indices serán alterados.
  ```
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
  ```
  
  * Búsqueda:
  Hacemos coincidir la secuencia D-peso con una entrada del directorio y nos dirigimos al bucket correspondiente para encontrar el registro.
  ```
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
  
  ```
  
  * Añadidos-FreeList:
      En cada bucket, se utiliza una técnica de FreeList, asegurando el proceso de inserción y eliminación en complejidad    O(1) asegurando un buen desempeño a nivel lógico, a cambio se creará un campo en los registros que contenga el atributo next_del,y un header que apunte a la posición disponible. 

  
  * Límites
      * En la eliminación, dado el caso con 2 o más buckets con un tamaño menor al permitido, no se realiza el merge.
      * La búsqueda se presenta bajo un solo parámetro, la generalización está fuera del scope pensado.
      * El desbordamiento no esta mapeado.
 
 * Demostration:
  ![Visualization](https://github.com/osoman2/GG_PSQL_BD2/blob/master/docu/Struct_page-0001.jpg)
 * Complejidad Teórica:**(en memoria secundaria)**
      * O(1) => Inserción
      * O(1) => Delete
      * O(k) => Search, k es la cantidad de registros en un bucket
 * Complejidad Espacial: **(RAM)**
     * O(mayor de O(lg(n)) o O(k)) => Mayor de almacenamiento de indices o tamaño de un Bucket.
     
| Registros | Search | Insertion | Delete  |
|-----------|--------|-----------|---------|
| 277       | 0.02ms | 0.298ms   | 0.097ms |


### Transacciones
Lo definimos como un conjunto de operaciones de acceso a base de datos que conforman una unidad lógica de trabajo.

```
sem_t db_elements[N]; //resources from data base

class Planner {
private:

    typedef vector<pair<string, string>> operation; //Operation, resource

    unordered_map<string, operation> transactions;
    unordered_map<string, int> resources;

public:
    string prefix;
    graph precedence;
    bool print;
    int num;

    Planner() {
        this->prefix = "T";
        this->num = 0;
        precedence = graph(true);
        print = false;

        for (int i = 0; i < N; i++)
            sem_init(&db_elements[i], 0, 0);
    }

    void* execute(void *){
        cout << "TestPlanner2 :: execute from Thread ID : " << std::endl;
        return NULL;
    }

    void addTransaction(int id) {
        auto str_id = this->prefix + to_string(id);
        transactions[str_id];

        precedence.addVertex(str_id, 0, 0);
    }

    void read(int id, string resource) {
        find_conflicts(id, "R", resource);

        if (this->resources.find(resource) == this->resources.end()) {
            this->resources[resource] = this->num;
            this->num++;
        }

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("R", resource));

        if (this->print)
            cout << str_id << " READ " << resource << endl;
    }

    void write(int id, string resource) {
        find_conflicts(id, "W", resource);

        if (this->resources.find(resource) == this->resources.end()) {
            this->resources[resource] = this->num;
            this->num++;
        }

        auto str_id = this->prefix + to_string(id);

        this->transactions[str_id].push_back(make_pair("W", resource));

        if (this->print)
            cout << str_id << " WRITE " << resource << endl;
    }

    void exe_plan(int id) {
        auto str_id = this->prefix + to_string(id);

        auto operations = this->transactions[str_id];

        for (auto operation: operations) {
            if (operation.first == "R" )
                this->xread(id, operation.second);
            else
                this->xwrite(id, operation.second);
        }
    }

    void xread(int id, string resource) {
        auto sem_no = this->resources[resource];

        auto str_id = this->prefix + to_string(id);
        //cout << str_id << " r semaphore " << sem_no <<endl;
        sem_post(&db_elements[sem_no]);
        if (this->print)
            cout << str_id << " xREAD " << resource << endl;
    }

    void xwrite(int id, string resource) {
        auto sem_no = this->resources[resource];

        auto str_id = this->prefix + to_string(id);
        //cout << str_id << " w semaphore " << sem_no <<endl;
        sem_wait(&db_elements[sem_no]);
        if (this->print)
            cout << str_id << " xWRITE " << resource << endl;
    }

    bool isSerializable() {
        return !precedence.is_cyclic();
    }

    void showPrecedenceGraph() {
        precedence.printGraph();
    }

    void showResource() {
        for(auto res: this->resources)
            cout << res.first << " " << res.second <<endl;
    }

    void show_operations(int id) {
        auto str_id = this->prefix + to_string(id);

        auto operations = this->transactions[str_id];

        for(auto operation: operations)
            cout << str_id << ": " << operation.first << " " <<operation.second << endl;
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
                        if (!precedence.findEdge(to_id, from_id, resource))
                            precedence.addEdge(to_id, from_id, 0, resource);
                    }
            }
        }
    }

public:
    ~Planner() {
        //delete precedence;
    }
};
```

## Resultados experimentales

### Comparación entre técnicas de indexación
  * Inserción
  * Búsqueda

### Análisis
  * Sequential file
  
    Los tiempos para la búsqueda registrados son:
  
  | Registros | Tiempo Promedio (μs) |
  |:---------:|:--------------------:|
  |    150    |        5.4667        |
  |    500    |        5.8667        |
  |    1500   |        6.2000        |
  |    5000   |        5.8667        |

Y graficamente se puede apreciar una curva logarítmica.
![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/01seqfile_search.png)

  Los tiempos para la inserción registrados son:

| Registros | Tiempo Promedio (μs) |
|:---------:|:--------------------:|
|    150    |        7.6000        |
|    500    |        6.0000        |
|    1500   |        5.0000        |
|    5000   |        5.2000        |

   Y graficamente se puede apreciar.
  ![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/01seqfile_insert.png)
  
   Los tiempos para la eliminacion registrados son:  
   
| Registros | Tiempo Promedio (μs) |
|:---------:|:--------------------:|
|    150    |        5.0000        |
|    500    |        4.8000        |
|    1500   |        5.2000        |
|    5000   |        7.4000        |

   Y graficamente se puede apreciar que el tiempo tiende a incrementar.
  ![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/01seqfile_delete.png)
     
### Simulador del planificación de concurrencia
Para simular las transacciones recurrimos al uso de threads, 1 transacción para cada thread con los recursos compartidos X, Y y Z. Una vez finalizada la ejecución de los threads se procede a armar el grafo de precedencia:

![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/TestPlan_01.png)
Graficamente se puede apreciar que no se contiene ciclos, por lo cual es serializable.
![enter image description here](https://github.com/osoman2/GG_PSQL_BD2/blob/master/docu/Grafo.png?raw=true)
Finalmente realizamos la planificacion equivalente usando los semáforos como mecanismo de bloqueo exclusivo del recurso Z, Y y X con ellos aseguramos que dos transacciones no puedan tomar un mismo recurso al mismo tiempo.
![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/TestPlan_02.png)     
     
## Pruebas de uso
Para realizar las pruebas y mostrar los datos decidimos hacerlo tanto en consola como en QT.
### Consola
### QT
Realizamos una interface para presentar la tabla de datos usando ambas técnicas y con una actualización de la tabla en tiempo real para cada una de las operaciones.

Tenemos el MainWindow, que es la ventana principal del SGBD, en donde se va a elegir entre las técnicas de Sequential File y Extendible Hashing.

```
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    sequentialwindow seqWindow;
    seqWindow.setModal(true);
    seqWindow.exec();
}
```

![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/qtSGBD.png)

Hacemos uso de una clase TestController para hacer las operaciones mediante consultas parseadas que se asemejen a un SGBD real. En cada una el usuario puede ingresar los datos del registro que quiere añadir, borrar o buscar y la tabla se actualizará apenas se completa la operación.

```
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
```

Por otro lado, tenemos el SequentialWindow, la ventana encargada de hacer las operaciones del Sequential File. Lo que hace es cargar una data inicial a alumnos.txt y leer un archivo output.txt creado a partir de otros archivos (Alumno.txt y auxfile.txt) en donde se realizan las operaciones. Al modificar los archivos alumno y auxfile, se copiarán los datos al output en un formato con divisiones '|' y este es leído para ser colocado en un TableView.


![enter image description here](https://raw.githubusercontent.com/osoman2/GG_PSQL_BD2/master/docu/qtSeq.png)

```
sequentialwindow::sequentialwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sequentialwindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(4);
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputCarrera->setText("");
    ui->OuputCiclo->setText("");
}

void sequentialwindow::setValueAt(int ix, int jx, const QString &value)
{
    if(!csvModel->item(ix, jx)){
        csvModel->setItem(ix, jx, new QStandardItem(value));
    } else{
        csvModel->item(ix, jx)->setText(value);
    }
}

sequentialwindow::~sequentialwindow()
{
    delete ui;
}

void sequentialwindow::on_pushButton_clicked()
{
    TestController::testRun("/home/theflilux/BD-P1-intent2/Alumno.txt");
    sequentialwindow::on_pushButton_5_clicked();
}

void sequentialwindow::on_pushButton_ADD_clicked()
{
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    ui->outputCodigo->setText(inputCodigo);
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    QString inputCarrera = QInputDialog::getText(this,"CARRERA","Ingrese su carrera: ");
    ui->outputCarrera->setText(inputCarrera);
    int inputCiclo = QInputDialog::getInt(this,"CICLO","Ingrese su ciclo: ");
    ui->OuputCiclo->setText(QString::number(inputCiclo));

    // Cambiando QString to string
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    std::string utf8_inputCarrera = inputCarrera.toUtf8().constData();
    TestController::testRunAdd("/home/theflilux/BD-P1-intent2/Alumno.txt", utf8_inputCodigo, utf8_inputNombre, utf8_inputCarrera, inputCiclo);
    sequentialwindow::on_pushButton_5_clicked();
}

void sequentialwindow::on_pushButton_5_clicked()
{
    QFile file("/home/theflilux/BD-P1-intent2/output.txt");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "El archivo no existe";
    } else {
        QTextStream xin(&file);
        int ix=-1;
        while (!xin.atEnd())
        {
            csvModel->setRowCount(ix);
            auto line = xin.readLine();
            auto values = line.split("|");
            const int colCount = values.size();
            csvModel->setColumnCount(colCount);
            for(int jx=0; jx < colCount;++jx){
                if(ix==-1){
                    csvModel->setHeaderData(jx, Qt::Horizontal, values.at(jx));
                }
                setValueAt(ix, jx, values.at(jx));
            }
            ++ix;
        }
        file.close();
    }
}

void sequentialwindow::on_pushButton_6_clicked()
{
    testLoad_add_Alumno("/home/theflilux/BD-P1-intent2/Alumno.txt");
    sequentialwindow::on_pushButton_clicked();
}

void sequentialwindow::on_pushButton_7_clicked()
{
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    TestController::testRunDelete("/home/theflilux/BD-P1-intent2/Alumno.txt", utf8_inputNombre);
    sequentialwindow::on_pushButton_5_clicked();
}

void sequentialwindow::on_pushButton_8_clicked()
{
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    TestController::testRunSearch("/home/theflilux/BD-P1-intent2/Alumno.txt", utf8_inputNombre);
    sequentialwindow::on_pushButton_5_clicked();
}
```

## Video
