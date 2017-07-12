#ifndef ESISETTINGS_H
#define ESISETTINGS_H

#include <QDialog>

namespace Ui {
class ESISettings;
}

class ESISettings : public QDialog
{
    Q_OBJECT

public:
    explicit ESISettings(QWidget *parent = 0);

    ~ESISettings();

signals:
    void sendDevName(QString devName);

private slots:
    void on_File_clicked();
    void on_buttonBox_accepted();
    void receiveDevFile_M2D(QString);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    void setDevFile(QString devFile);
    QString getDevFile();
    Ui::ESISettings *ui;
    QString devFile;

};

#endif // ESISETTINGS_H
