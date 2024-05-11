#include "qTame.h"
#include "ui_qTame.h"
#include <QScrollBar>

qTame::qTame(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::qTame),
	telnet(this)
{
	ui->setupUi(this);
    ui->leAddr->setText("mud.tamedhon.de");
	connect( &telnet, SIGNAL(newData(const char*,int)), this, SLOT(addText(const char*,int)) );
	connect( &telnet, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)) );
	connect( ui->cbCmd, SIGNAL(command(QString)), this, SLOT(onCommand(QString)));
}

qTame::~qTame()
{
	telnet.disconnectFromHost();
	delete ui;
	ui = Q_NULLPTR;
}

void qTame::onStateChanged(QAbstractSocket::SocketState s)
{
	switch( s )
	{
	case QAbstractSocket::UnconnectedState:
        setStatusText( tr("<qTame> Getrennt"), true );
        ui->btConnect->setText( tr("Verbinde") );
		break;
	case QAbstractSocket::HostLookupState:
        setStatusText( tr("<qTame> DNS Auflösung %1").arg(telnet.peerName()), true );
        ui->btConnect->setText( tr("Beenden") );
		break;
//	case QAbstractSocket::ConnectingState:
//        setStatusText( tr("Verbinde zu %1").arg(telnet.peerInfo()), true );
//        ui->btConnect->setText( tr("Trennen") );
//		break;
	case QAbstractSocket::ConnectedState:
        setStatusText( tr("<qTame> Verbunden zu %1").arg(telnet.peerInfo()), true );
        ui->btConnect->setText( tr("Trennen") );
		break;
	case QAbstractSocket::BoundState:
        setStatusText( tr("<qTame> Verlinkt"), true );
		ui->btConnect->setText( tr("- - - -") );
		break;
	case QAbstractSocket::ListeningState:
        setStatusText( tr("<qTame> Lausche"), true );
		ui->btConnect->setText( tr("- - - -") );
		break;
	case QAbstractSocket::ClosingState:
        setStatusText( tr("\n<qTame> Trenne Verbindung"), true );
        ui->btConnect->setText( tr("Schließen erzwingen") );
		break;

	}
}

void qTame::setStatusText(const QString &msg, bool onQTelnetTester)
{
	if( onQTelnetTester )
	{
        addText(msg.toUtf8(), msg.length()+1);
		if( !msg.endsWith("\n") )
			addText("\n", 1);
	}
    ui->statusBar->showMessage(QString(msg).remove("<qTame> "));
}

void qTame::onCommand(const QString &cmd)
{
	if( telnet.isConnected() )
	{
        telnet.sendData(cmd.toUtf8());
		telnet.sendData("\n");
	}
}

void qTame::on_btConnect_clicked()
{
	if( telnet.isConnected() )
		telnet.disconnectFromHost();
	else
		telnet.connectToHost(ui->leAddr->text(), ui->sbPort->value());
}

void qTame::addText(const char *msg, int count)
{
	ui->teOutput->insertPlainText( QByteArray(msg, count) );
	ui->teOutput->verticalScrollBar()->setValue(0xFFFFFFF);
}
