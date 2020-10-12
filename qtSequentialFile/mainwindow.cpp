#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QInputDialog>
#include <QWidget>
//#include "TestController.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(4);
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputCarrera->setText("");
    ui->OuputCiclo->setText("");

    // Open the file from the resources. Instead of the file
    // Need to specify the path to your desired file
    QFile file("/home/theflilux/qtSequentialFile/output.txt");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        // Create a thread to retrieve data from a file
        QTextStream xin(&file);
        //Reads the data up to the end of file
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

void MainWindow::setValueAt(int ix, int jx, const QString &value)
{
    if(!csvModel->item(ix, jx)){
        csvModel->setItem(ix, jx, new QStandardItem(value));
    } else{
        csvModel->item(ix, jx)->setText(value);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::on_pushButton_clicked()
{
    QString N = QInputDialog::getText(this,"Texto","Ingrese su codigo: ");
    ui->outputCodigo->setText(N);
    return N;
}

QString MainWindow::on_pushButton_2_clicked()
{
    QString N = QInputDialog::getText(this,"Texto","Ingrese su nombre: ");
    ui->outputNombre->setText(N);
    return N;
}

QString MainWindow::on_pushButton_3_clicked()
{
    QString N = QInputDialog::getText(this,"Texto","Ingrese su carrera: ");
    ui->outputCarrera->setText(N);
    return N;
}

int MainWindow::on_pushButton_4_clicked()
{
    int N = QInputDialog::getInt(this,"Texto","Ingrese su ciclo: ");
    ui->OuputCiclo->setText(QString::number(N));
    return N;
}

void MainWindow::on_pushButton_ADD_clicked()
{
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    ui->outputCodigo->setText(inputCodigo);
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    QString inputCarrera = QInputDialog::getText(this,"CARRERA","Ingrese su carrera: ");
    ui->outputCarrera->setText(inputCarrera);
    int inputCiclo = QInputDialog::getInt(this,"CICLO","Ingrese su ciclo: ");
    ui->OuputCiclo->setText(QString::number(inputCiclo));
    QString filename = "/home/theflilux/qtSequentialFile/Alumno.txt";

    // Cambiando QString to string
    std::string utf8_filename = filename.toUtf8().constData();
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    std::string utf8_inputCarrera = inputCarrera.toUtf8().constData();
    //TestController::testRun("/home/theflilux/qtSequentialFile/Alumno.txt");
    //TestController::testRunAdd(utf8_filename, utf8_inputCodigo, utf8_inputNombre, utf8_inputCarrera, inputCiclo);
}
