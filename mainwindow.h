#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>

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
    void on_btnOpenClose_clicked();

    void on_btnSendIOCTL_clicked();

    void on_txtBoxDeviceFileName_textChanged(const QString &arg1);

    void on_txtBoxIOCTL_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    HANDLE devHandle;
};

#endif // MAINWINDOW_H
