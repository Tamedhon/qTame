#ifndef QTAME_H
#define QTAME_H

#include <QMainWindow>
#include <QSettings>
#include "QTelnet.h"
#include <QUuid>
#include <QScrollBar>
#include <QCoreApplication>
#include "crypto.h"
#include "EnumHelper.h"


namespace Ui
{
class qTame;
}


class qTame : public QMainWindow
{
    Q_OBJECT

    Ui::qTame *ui;
    QTelnet telnet;


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
    QSettings *settings;
    Crypto::Crypto *crypt;

    void Connects();
    void SetMode();
    void DefaultSettings();

private slots:
    void onStateChanged(QAbstractSocket::SocketState s);

    void on_btConnect_clicked();
    void onCommand(const QString &cmd);

    void btnClose();
    void btnFrei1();
    void btnFrei2();
    void btnFrei3();
    void btnFrei4();
    void btnFrei5();

    void onCursorUp();
    void onCursorDown();

    void addressChanged(QString txt);
    void portChanged(int val);
    void tlsChanged(int val);

public slots:
    void setStatusText(const QString &msg, bool onMainWindow = false);
    void addText(const char *msg, int count);
};

#endif // QTAME_H
