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
    enum class Themes
    {
        dark = 0,
        light = 1,
        green = 2,
        bernstein = 3
    };
    Q_ENUM(Themes);

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
    void moveCursorToEnd();
    void LoadButtonTexts();
    void LoadValues();
    void TelnetCommand(QString cmd);
    void WriteLog(QString txt);

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

    void onCursorUp();
    void onCursorDown();

    void addressChanged(QString txt);
    void portChanged(int val);
    void tlsChanged(int val);
    void autologinChanged(int val);
    void logChanged(int val);

public slots:
    void setStatusText(const QString &msg, bool onMainWindow = false);
    void addText(const char *msg, int count);

};

#endif // QTAME_H
