#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "esisettings.h"
#include "esitimer.h"
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>


using namespace std;
QSerialPort *serial;

void writeFile(QString filename);
QString readFile(QString filename);

void writeFile(QString filename, QString fileStr)
{
    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    // To write text, we use operator<<(),
    // which is overloaded to take
    // a QTextStream on the left
    // and data types (including QString) on the right

    QTextStream out(&file);
    out<< fileStr;
    file.flush();
    file.close();
}

QString readFile(QString fileName)
{

    QString myText= "";
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return myText;
    }

    QTextStream in(&file);
    myText = in.readAll();
    qDebug() << myText;

    file.flush();
    file.close();

    return myText;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);

    //initActionsConnections();

    //connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
    //           this, &MainWindow::handleError);

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    //connect(console, &Console::getData, this, &MainWindow::writeData);
    ui->lineEdit->setDisabled(1);
    //ui->commandLineEdit->clear();
    timer = new EsiTimer();
    connect(timer,SIGNAL(temperatureReading(float)),this,SLOT(displayTemperature(float)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if (serial->isOpen()) {
        serial->close();
    }
    delete timer;
}

void MainWindow::setDevFile(QString devFile) {
    this->devFile = devFile;
}


QString MainWindow::getDevFile() {
    return this->devFile;
}

void MainWindow::setFileName(QString fileName) {
    this->fileName = fileName;
}


QString MainWindow::getFileName() {
    return this->fileName;
}

void MainWindow::qMesgBox(QString msg) {
    QMessageBox msgBox;
    msgBox.tr("ESI Term");
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::on_actionOpen_triggered()
{

    if (getFileName() == "")
         setFileName("~/untitled.txt");

    QFileInfo fInfo(getFileName());

    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), "/home/myPython/untitled.txt", tr("Files (*.txt)"));

    if (!fileName.isNull()) {
        this->setFileName(fileName);
        ui->plainTextEdit_Term->insertPlainText(getFileName());
        ui->plainTextEdit_Term->insertPlainText("\n");

        QString absFilePath = fInfo.absolutePath();
        ui->plainTextEdit_Term->insertPlainText(absFilePath);
        ui->plainTextEdit_Term->insertPlainText("\n");

        QString qfileName = fInfo.fileName();
        ui->plainTextEdit_Term->insertPlainText(qfileName);
        ui->plainTextEdit_Term->insertPlainText("\n");

        QString baseName = fInfo.baseName();
        ui->plainTextEdit_Term->insertPlainText(baseName);
        ui->plainTextEdit_Term->insertPlainText("\n");

        QString fileStr = readFile(getFileName());
        ui->plainTextEdit->clear();
        ui->plainTextEdit->insertPlainText(fileStr);
    }
    else {
        qDebug() << "Null filename - nil operation";
        ui->plainTextEdit_Term->insertPlainText("Null filename - Open operation abort\n");
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = getFileName();

    if (fileName.isNull())
    {        
        // handle ops when fileName is NULL
        ui->plainTextEdit_Term->insertPlainText("Null filename");
        fileName = QFileDialog::getOpenFileName(this, tr("save file"), "/home/myPython/untitled.txt", tr("Files (*.txt)"));
    }

    if (!fileName.isNull()) {
        QFileInfo fInfo(fileName);

        if (fInfo.suffix()!="txt")
            fileName = fInfo.baseName() + ".txt";

        ui->plainTextEdit_Term->insertPlainText(ui->plainTextEdit->toPlainText());

        this->setFileName(fileName);
        ui->plainTextEdit_Term->insertPlainText(fileName);

        writeFile(this->getFileName(), ui->plainTextEdit->toPlainText());
    }
    else {
        qDebug() << "Null filename - nil operation";
        ui->plainTextEdit_Term->insertPlainText("Null filename - Save operation abort\n");
        qMesgBox("Null filaname");
    }

}

void MainWindow::on_actionSave_As_triggered()
{

    QFileInfo fInfo(getFileName());

    QString absFilePath = fInfo.absolutePath();

    QString tempFileName = QFileDialog::getSaveFileName(this, tr("save as file"), "/home/myPython/untitled.txt", tr("Files (*.txt)"));

    ui->plainTextEdit_Term->insertPlainText(tempFileName);
    ui->plainTextEdit_Term->insertPlainText("\n");

    if (!tempFileName.isNull()) {
        ui->plainTextEdit_Term->insertPlainText("Save As " + tempFileName + "\n");
        ui->plainTextEdit_Term->insertPlainText(ui->plainTextEdit->toPlainText());

        this->setFileName(tempFileName);
        writeFile(tempFileName, ui->plainTextEdit->toPlainText());
    }
    else {
        qDebug() << "Null filename - nil operation";
        ui->plainTextEdit_Term->insertPlainText("Null filename - Save As operation abort\n");
        qMesgBox("Null filaname");
    }
}

void MainWindow::on_actionClose_triggered()
{

    ui->plainTextEdit_Term->insertPlainText(this->getFileName() + " closed\n");
    ui->plainTextEdit->clear();

}

void MainWindow::on_actionComm_Mode_triggered()
{
     ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_actionNew_triggered()
{

    QString fileName=getFileName();
    if (!fileName.isNull()) {
        QFile newFile(fileName);
        newFile.open( QIODevice::WriteOnly);
        newFile.close();
        ui->plainTextEdit_Term->insertPlainText("New file " + fileName + "\n");
    }
    else {
        qDebug() << "Null filename - nil operation";
        ui->plainTextEdit_Term->insertPlainText("Null filename - New operation abort\n");
        qMesgBox("Null filaname");
    }
}

void MainWindow::on_actionConnect_triggered()
{
       if (ui->actionConnect->isVisible() && !serial->isOpen()) {

              serial->setPortName(getDevFile());

              if(serial->open(QIODevice::ReadWrite)){
                  //Now the serial port is open try to set configuration
                  if(!serial->setBaudRate(QSerialPort::Baud9600))
                      qDebug()<<serial->errorString();

                  if(!serial->setDataBits(QSerialPort::Data8))
                      qDebug()<<serial->errorString();

                  if(!serial->setParity(QSerialPort::NoParity))
                      qDebug()<<serial->errorString();

                  if(!serial->setStopBits(QSerialPort::OneStop))
                      qDebug()<<serial->errorString();

                  if(!serial->setFlowControl(QSerialPort::NoFlowControl))
                      qDebug()<<serial->errorString();

                  //If any error was returned the serial will corrctly configured

                  //serial->write("M114 \n");
                  //the serial must remain opened

                  if(serial->waitForReadyRead(100)){
                      //Data was returned
                      qDebug()<<"Response: "<<serial->readAll();
                  }else{
                      //No data
                      qDebug()<<"Time out";
                  }

                  ui->actionConnect->setVisible(false);
                  ui->actionDisconnect->setVisible(true);
                  ui->lineEdit->setEnabled(1);
                  //ui->commandLineEdit->clear();

              }else{

                  qMesgBox("Serial port setting failed");
                  qDebug()<<"Serial inf " + getDevFile() + " not opened. Error: "<<serial->errorString();
              }

       }
}


void MainWindow::on_actionDisconnect_triggered()
{
    if (ui->actionDisconnect->isVisible()) {

         if (serial->isOpen()) {
             serial->flush();
             serial->close();           
             qDebug() << "Serial port " + devFile + " is closed";
         }
         else {
             qMesgBox("No connection.");
         }
         //ui->commandLineEdit->clear();
         ui->lineEdit->setDisabled(1);
         ui->actionDisconnect->setVisible(false);
         ui->actionConnect->setVisible(true);
    }
}


void MainWindow::on_actionSettings_triggered()
{
      ESISettings *dialogSettings = new ESISettings(this);
      emit sendDevFile_M2D(getDevFile());

      dialogSettings->setModal(true);
      connect(dialogSettings, SIGNAL(sendDevName(QString)), this, SLOT(receiveDevName(QString)));
      dialogSettings->exec();
}

void MainWindow::receiveDevName(QString devFile)
{
    this->setDevFile(devFile);
    qDebug() << "validate " << devFile << " from Setting dialog ";
}


void MainWindow::on_actionUpload_file_triggered()
{
    if (ui->actionDisconnect->isVisible() && serial->isOpen())
    {
        QString fn = QFileDialog::getOpenFileName(this, tr("open file"), "/home/myPython/untitled.txt", tr("Files (*.txt)"));
        if (!fn.isNull()) {
            QString fileStr = readFile(fn);

            QByteArray asc_array = fileStr.toLocal8Bit();
            ui->plainTextEdit_Term->insertPlainText(asc_array);

            serial->clear(QSerialPort::AllDirections);
            serial->write("\r\n");
            serial->write(asc_array);
            serial->write("\r\n");
            serial->flush();   // flush all data in the sending buffer

            if(serial->waitForReadyRead(1000)){
                //Data was returned
                QString serial_response = serial->readAll();
                qDebug()<<"Response: "<< serial_response;
                ui->plainTextEdit_Term->insertPlainText(serial_response);
            }else{
                //No data
                ui->plainTextEdit_Term->insertPlainText("Time out");
                qDebug()<<"Time out";
            }
        }
        else {
            qMesgBox("Null filename");
        }
    }
    else {
        qMesgBox("No connection.");
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    if (ui->actionDisconnect->isVisible() && serial->isOpen())
    {
        QByteArray asc_array = "\r\n"+ ui->lineEdit->text().toLocal8Bit() + "\r\n";
        ui->plainTextEdit_Term->insertPlainText(asc_array);

        serial->clear(QSerialPort::AllDirections);
        serial->write(asc_array);
        serial->flush();   // flush all data in the sending buffer
        ui->lineEdit->clear();
        if(serial->waitForReadyRead(1000)){
            //Data was returned
            QString serial_response = serial->readAll() + "\r\n";
            qDebug()<<"Response: "<< serial_response;
            ui->plainTextEdit_Term->insertPlainText(serial_response);
        }

    }
    else
    {
        qMesgBox("No connection.");
    }


}

// directive __attribute__((unused)) causes red underline, but no harms
void MainWindow::on_lineEdit_textEdited(__attribute__((unused)) const QString &arg1)
{
    if (!ui->actionDisconnect->isVisible() || !serial->isOpen())
    {
        qMesgBox("No connection.");

        ui->lineEdit->setDisabled(1);
    }
}

// Display Temperature of printing head periodically - To be completed
void MainWindow::displayTemperature(float val)
{
    qDebug()<<"temperature = "<< val;
}
