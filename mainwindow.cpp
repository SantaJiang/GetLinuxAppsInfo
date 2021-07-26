#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonValue>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_linuxAppInfo = new LinuxAppInfo();
    connect(m_linuxAppInfo,SIGNAL(findOneApp(QJsonObject)),this,SLOT(showAppInfo(QJsonObject)),Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_action_clicked()
{
    m_linuxAppInfo->start();
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::showAppInfo(QJsonObject obj)
{
    ui->textEdit->append(QString("name:%1").arg(obj["name"].toString()));
    ui->textEdit->append(QString("enName:%1").arg(obj["enName"].toString()));
    ui->textEdit->append(QString("appName:%1").arg(obj["appName"].toString()));
    ui->textEdit->append(QString("appPath:%1").arg(obj["appPath"].toString()));
    ui->textEdit->append(QString("image:%1").arg(obj["image"].toString()));
    ui->textEdit->append("\n");
}
