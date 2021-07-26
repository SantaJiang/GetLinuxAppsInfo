#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "linuxappinfo.h"
#include <QMainWindow>
#include <QJsonObject>

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
    void on_pushButton_action_clicked();
    void on_pushButton_clear_clicked();
    void showAppInfo(QJsonObject obj);

private:
    Ui::MainWindow *ui;
    LinuxAppInfo* m_linuxAppInfo;
};

#endif // MAINWINDOW_H
