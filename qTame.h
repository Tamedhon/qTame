#ifndef QTAME_H
#define QTAME_H

#include <QMainWindow>
#include <QSettings>
#include "QTelnet.h"

class Colors
{
public:
    enum ANSI_COLORS
    {

    };
    int r, g, b;
};

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
    explicit qTame(QWidget *parent = 0);
    ~qTame();

private:
    QSettings settings{"qtame.conf", QSettings::IniFormat};
    QList<Colors> colors;

private slots:
	void onStateChanged(QAbstractSocket::SocketState s);

	void on_btConnect_clicked();
	void onCommand(const QString &cmd);

public slots:
	void setStatusText(const QString &msg, bool onMainWindow = false);
	void addText(const char *msg, int count);
};

#endif // QTAME_H
