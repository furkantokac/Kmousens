#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sliderValue, SIGNAL(valueChanged(int)),
            ui->lblValue, SLOT(setNum(int)) );

    QStringList allDeviceIds = runCommand("xinput --list --id-only").split("\n");

    foreach( QString id, allDeviceIds)
    {
        qint8 result = runCommand("xinput --list-props "+ id).split("Constant Deceleration").size();

        if(result==2)
        {
            ui->comboBoxDevices->addItem( runCommand("xinput --list --name-only "+id).split("\n").first() );

            QString rval = runCommand("xinput list-props "+id).split("Constant Deceleration")[1];
            rval = rval.split("\t")[1];
            rval = rval.split("\n").first(); // rval will be terminal value of corresponding id

            qint16 sval = rvalToSval(rval.toFloat());

            ui->sliderValue->setValue(sval);

            deviceIds.append(id);
            deviceValues.append( QString::number(sval) );
        }
    }

    ui->comboBoxDevices->setCurrentIndex( deviceIds.size()-1 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::runCommand(QString cmd)
{
    QProcess process;

    process.start(cmd);
    process.waitForFinished();
    QString output(process.readAllStandardOutput());

    return output;
}

void MainWindow::on_btnApply_clicked()
{
    applySettings();
}

void MainWindow::applySettings()
{
    qint8 index = ui->comboBoxDevices->currentIndex();
    qint8 sval = ui->sliderValue->value();

    deviceValues[index] = QString::number(sval);

    setIdSpeed(index, sval);
}

// sval = slider value
float MainWindow::svalToRval(qint8 sval)
{
    return 10-sval/10.0;
}

// rval = real value
qint8 MainWindow::rvalToSval(float rval)
{
    return 100-rval*10;
}

void MainWindow::setIdSpeed(qint8 index, qint8 sval)
{
    float rval = svalToRval(sval);
    QString id = deviceIds[index];

    runCommand("xinput --set-prop "+ id +" \"Device Accel Constant Deceleration\" "+QString::number(rval));
    qDebug() << id << rval << endl;
}

void MainWindow::on_comboBoxDevices_currentIndexChanged(int index)
{
    if( deviceValues.size()<=0 )
        return;

    QString value = deviceValues[index];
    ui->sliderValue->setValue( value.toInt() );
}

void MainWindow::on_btnReset_clicked()
{
    ui->sliderValue->setValue(90);
    applySettings();
}
