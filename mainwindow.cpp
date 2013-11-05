#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"

int MAXDEVNAMELENGTH = 100;

bool txtBox_text_toLPWSTR(QLineEdit *txtBox, LPWSTR outString)
{
    int strlength = txtBox->text().length();
    if (strlength > 0 && strlength < 100)
    {
        memset(outString,0,(strlength*2)+2);

        int charArrayLen = txtBox->text().toWCharArray(outString);

        if(strlength == charArrayLen)
        {
            return true;
        }
    }

    return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenClose_clicked()
{
    QPushButton *ptrbtnOpenClose = (QPushButton *)ui->btnOpenClose;
    if (ptrbtnOpenClose->text() == "Open")
    {
        QLineEdit *ptrtxtBoxDeviceFileName = (QLineEdit *)ui->txtBoxDeviceFileName;

        LPWSTR deviceName = new wchar_t[ptrtxtBoxDeviceFileName->text().length()*2];

        if(txtBox_text_toLPWSTR(ptrtxtBoxDeviceFileName,deviceName))
        {
            MessageBox(NULL,deviceName,L"OpenDevice",0);

            devHandle = CreateFile(deviceName,
                                   0,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   NULL,
                                   OPEN_EXISTING,
                                   0,
                                   NULL);

            if(devHandle != INVALID_HANDLE_VALUE)
            {
                ptrtxtBoxDeviceFileName->setEnabled(false);
                ptrbtnOpenClose->setText("Close");
                QLineEdit *ptrtxtBoxIOCTL = (QLineEdit *)ui->txtBoxIOCTL;
                ptrtxtBoxIOCTL->setEnabled(true);
            }
            else
            {
                DWORD errorCode = GetLastError();

                LPWSTR ErrorMessage = NULL;
                DWORD strLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, \
                                                NULL, errorCode, 0, \
                                                (LPWSTR)&ErrorMessage, 10,NULL);


                //LPWSTR strErrorCode = new wchar_t[20];
                //wsprintf(strErrorCode,L"%X",errorCode);
                //MessageBox(NULL,strErrorCode,L"ERROR",0);
                if (strLength > 0)
                    MessageBox(NULL,ErrorMessage,L"ERROR",0);

                LocalFree(ErrorMessage);

                //delete(strErrorCode);

            }

        }

        delete(deviceName);
    }
    else
    {
        if(devHandle != NULL)
            CloseHandle(devHandle);

        ptrbtnOpenClose->setText("Open");

        QLineEdit *ptrtxtBoxDeviceFileName = (QLineEdit *)ui->txtBoxDeviceFileName;
        ptrtxtBoxDeviceFileName->setEnabled(true);

        QWidget *ptrbtnSendIOCTL = (QWidget *) ui->btnSendIOCTL;
        ptrbtnSendIOCTL->setEnabled(false);

        QLineEdit *ptrtxtBoxIOCTL = (QLineEdit *)ui->txtBoxIOCTL;
        ptrtxtBoxIOCTL->setEnabled(false);

    }
}

void MainWindow::on_btnSendIOCTL_clicked()
{
    QLineEdit *ptrtxtBoxIOCTL = (QLineEdit *)ui->txtBoxIOCTL;

    LPWSTR strIOCTL = new wchar_t[ptrtxtBoxIOCTL->text().length()*2];

    if(txtBox_text_toLPWSTR(ptrtxtBoxIOCTL, strIOCTL))
    {
        MessageBox(NULL,strIOCTL,L"TEST",0);
    }

    delete(strIOCTL);


}

void MainWindow::on_txtBoxDeviceFileName_textChanged(const QString &arg1)
{
    QPushButton *ptrbtnOpenClose = (QPushButton *)ui->btnOpenClose;
    if (arg1.length() > 0 && arg1.length() < MAXDEVNAMELENGTH)
    {
        ptrbtnOpenClose->setEnabled(true);
    }
    else
    {
        ptrbtnOpenClose->setEnabled(false);
    }
}

void MainWindow::on_txtBoxIOCTL_textChanged(const QString &arg1)
{
    QPushButton *ptrbtnSendIOCTL = (QPushButton *)ui->btnSendIOCTL;
    if (arg1.length() > 0 && arg1.length() < MAXDEVNAMELENGTH)
    {
        ptrbtnSendIOCTL->setEnabled(true);
    }
    else
    {
        ptrbtnSendIOCTL->setEnabled(false);
    }
}
