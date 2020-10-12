#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TestController.h"
#include "TestSequential.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QInputDialog>
#include <QWidget>

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


void MainWindow::on_pushButton_clicked()
{
    TestController::testRun("/home/theflilux/qtSequentialFile/Alumno.txt");
    MainWindow::on_pushButton_5_clicked();
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
    TestController::testRunAdd("/home/theflilux/qtSequentialFile/Alumno.txt", utf8_inputCodigo, utf8_inputNombre, utf8_inputCarrera, inputCiclo);
    MainWindow::on_pushButton_5_clicked();
}

void MainWindow::on_pushButton_5_clicked()
{
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

void MainWindow::on_pushButton_6_clicked()
{
    testLoad_add_Alumno("/home/theflilux/qtSequentialFile/Alumno.txt");
}

void MainWindow::on_pushButton_7_clicked()
{
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    TestController::testRunDelete("/home/theflilux/qtSequentialFile/Alumno.txt", utf8_inputNombre);
    MainWindow::on_pushButton_5_clicked();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    TestController::testRunSearch("/home/theflilux/qtSequentialFile/Alumno.txt", utf8_inputNombre);
    MainWindow::on_pushButton_5_clicked();
}
