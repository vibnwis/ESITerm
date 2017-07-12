#include "mainwindow.h"
#include "esisettings.h"
#include "ui_esisettings.h"
#include <QObject>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>


ESISettings::ESISettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ESISettings)
{
    ui->setupUi(this);
    connect(this->parent(), SIGNAL(sendDevFile_M2D(QString)), this, SLOT(receiveDevFile_M2D(QString)));

    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;

    foreach(port, com_ports)
    {
        ui->comboBox->addItem(port.portName());
    }

    ui->lineEditDevFile->setText(ui->comboBox->currentText());
}

ESISettings::~ESISettings()
{
    delete ui;
}


void ESISettings::on_File_clicked()
{

    qDebug() << " ESISettings::on_File_clicked devFile (1) = " + this->getDevFile();
    QString devFile = QFileDialog::getOpenFileName(this, tr("Serial Interface file"), this->getDevFile(), tr("Files (*.*)"));
    if (!devFile.isNull()){
        ui->lineEditDevFile->setText(devFile);
        this->setDevFile(devFile);
        qDebug() << " ESISettings::on_File_clicked devFile (2) = " + devFile;
    }
    else {
        qDebug() << "Null Devfile or devfile operation aborted ";
    }
}


void ESISettings::setDevFile(QString devFile){
    qDebug() << " ESISettings::setDevFile = " + devFile;
    this->devFile = devFile;
}

QString ESISettings::getDevFile()
{
    qDebug() << " ESISettings::getDevFile = " + this->devFile;
    return this->devFile;
}

void ESISettings::on_buttonBox_accepted()
{
    emit sendDevName(ui->lineEditDevFile->text());
}

void ESISettings::receiveDevFile_M2D(QString devFile){
    this->setDevFile(devFile);
    ui->lineEditDevFile->setText(devFile);
    qDebug() << "ESISetting SLOT devFile = <<" + devFile;

    int index = ui->comboBox->findText(this->getDevFile());
    if (index != -1) {
        ui->comboBox->setCurrentIndex(index);
    }
}

void ESISettings::on_comboBox_currentTextChanged(const QString &arg1)
{
  ui->lineEditDevFile->setText(arg1);
  //this->setDevFile(arg1);
  //qDebug() << "ESTSetting Devfile = " + arg1;
}
