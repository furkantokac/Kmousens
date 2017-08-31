#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include <QProcess>
#include <QString>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnApply_clicked();

    void on_comboBoxDevices_currentIndexChanged(int index);

    void on_btnReset_clicked();

private:
    QString runCommand(QString cmd);
    float svalToRval(qint8 sval);
    qint8 rvalToSval(float rval);
    void setIdSpeed(qint8 id, qint8 sval);
    void applySettings();

    QStringList deviceIds;
    QStringList deviceValues;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
