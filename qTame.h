#ifndef QTAME_H
#define QTAME_H

#include <QMainWindow>
#include <QSettings>
#include <QUuid>
#include <QScrollBar>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScreen>
#include <QTextBrowser>
#include <QDateTime>
#include <QDir>
#include "QTelnet.h"
#include "crypto.h"
#include "info.h"
#include "settings.h"
#include "enum.h"

using namespace Enums;


namespace Ui
{
class qTame;
}

class qTame : public QMainWindow
{
    Q_OBJECT

    Ui::qTame *ui;
    QTelnet *telnet;
    Info *info;
    Settings *set;

public:
    explicit qTame(QWidget *parent = 0);
    ~qTame();   

private:
    bool user = false;
    bool password = false;
    bool writeLog = false;

    QSettings *settings;
    Crypto::Crypto *crypt;
    QFile *log = nullptr;

    void Connects();
    void SetMode();
    void SetColors(QString foreground, QString background);
    void moveCursorToEnd();
    void LoadButtonTexts();
    void LoadValues();
    void TelnetCommand(QString cmd);
    void WriteLog(QString txt);
    QString VT100_ANSI_2_HTML(QString input);

    void CursorUp();
    void CursorDown();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onStateChanged(QAbstractSocket::SocketState s);

    void on_btConnect_clicked();
    void onCommand(const QString &cmd);

    void btnClose();
    void btn1();
    void btn2();
    void btn3();
    void btn4();
    void btn5();
    void btn6();
    void btn7();
    void btn8();
    void btn9();
    void btn10();

    void openInfo();
    void openFunktionen();
    void openNavigation();
    void openSettings();

    void addressChanged(QString txt);
    void portChanged(int val);
    void tlsChanged(int val);
    void autologinChanged(int val);
    void logChanged(int val);

public slots:
    void setStatusText(const QString &msg, bool onMainWindow = false);
    void addText(const char *msg, int count);
    void handleNumPadEvent(QKeyEvent *event);

};

#endif // QTAME_H
