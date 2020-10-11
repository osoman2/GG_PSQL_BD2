#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(3);
    csvModel->setHorizontalHeaderLabels(QStringList() << "Марка" << "Модель" << "Цена");
    ui->tableView->setModel(csvModel);

    // Open the file from the resources. Instead of the file
    // Need to specify the path to your desired file
    QFile file("/home/theflilux/Escritorio/register.txt");
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
//            QString line = in.readLine();
//            // Adding to the model in line with the elements
//            QList<QStandardItem *> standardItemsList;
//            // consider that the line separated by semicolons into columns
//            for (QString item : line.split("|")) {
//                standardItemsList.append(new QStandardItem(item));
//            }
//            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
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

