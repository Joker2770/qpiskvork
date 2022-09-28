#include "mainwindow.h"
#include <QDebug>

void MainWindow::readyRead()
{
    //qDebug()<<"readyRead-readAll:";
    //qDebug()<<QString::fromLocal8Bit(myProcess->readAll());
    qDebug()<<"readyRead-readAllStandardOutput:";
    qDebug()<<QString::fromLocal8Bit(myProcess->readAllStandardOutput());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    myProcess = new QProcess(this);
    connect(myProcess,SIGNAL(readyRead()),this,SLOT(readyRead()));
    myProcess->setProgram("D:\\open_source\\tools\\piskvork\\pbrain-pela.exe");
    myProcess->start();
    if(!myProcess->waitForStarted())
    {
        qDebug()<<"failure!";
    }
    else
    {
        qDebug()<<"succ!";
    }
    myProcess->waitForFinished();
    myProcess->write("START 15\n");
    myProcess->waitForBytesWritten();
    myProcess->closeWriteChannel();
}

MainWindow::~MainWindow()
{
    myProcess->close();
    if (nullptr != myProcess)
    {
        delete myProcess;
        myProcess = nullptr;
    }
}

