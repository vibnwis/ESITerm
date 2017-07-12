#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "esitimer.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDevFile(QString);
    QString getDevFile();
    void setFileName(QString);
    QString getFileName();
    void qMesgBox(QString);
    bool setupSerialPort(QString port);

signals:
    void sendDevFile_M2D(QString devFile);

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_triggered();

    void on_actionComm_Mode_triggered();

    void on_actionNew_triggered();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_actionSettings_triggered();

    void receiveDevName(QString data);

    void on_actionUpload_file_triggered();

    void on_lineEdit_returnPressed();

    void on_lineEdit_textEdited(const QString &arg1);

    void displayTemperature(float val);

private:
    Ui::MainWindow *ui;
    QString devFile = "untitled";
    QString fileName = "~/untitled";
    //Console *console;
    //SettingsDialog *settings;
    QSerialPort *serial;
    EsiTimer *timer;
};

#endif // MAINWINDOW_H
