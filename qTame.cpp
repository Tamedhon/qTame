#include "qTame.h"
#include "ui_qTame.h"


qTame::qTame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTame),
    telnet(new QTelnet(this))
{
    ui->setupUi(this);

    crypt = new Crypto::Crypto();
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    DefaultSettings();
    SetMode();

    settings->beginGroup("User");

    ui->txtAddr->setText(settings->value("address").toString());
    ui->sbPort->setValue(settings->value("port").toInt());

    if(settings->value("tls").toInt() == 1)
    {
        ui->cbTLS->setChecked(true);
    }
    else if(settings->value("tls").toInt() == 0)
    {
        ui->cbTLS->setChecked(false);
    }

    settings->endGroup();

    Connects();
}

qTame::~qTame()
{
    telnet->disconnectFromHost();
    delete settings;
    delete crypt;
    delete telnet;
    delete ui;
}

void qTame::Connects()
{
    connect(telnet, SIGNAL(newData(const char*,int)), this, SLOT(addText(const char*,int)));
    connect(telnet, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(ui->cbCmd, SIGNAL(command(QString)), this, SLOT(onCommand(QString)));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose()));
    connect(ui->btnFrei1, SIGNAL(clicked()), this, SLOT(btnFrei1()));
    connect(ui->btnFrei2, SIGNAL(clicked()), this, SLOT(btnFrei2()));
    connect(ui->btnFrei3, SIGNAL(clicked()), this, SLOT(btnFrei3()));
    connect(ui->btnFrei4, SIGNAL(clicked()), this, SLOT(btnFrei4()));
    connect(ui->btnFrei5, SIGNAL(clicked()), this, SLOT(btnFrei5()));
    connect(ui->txtAddr, SIGNAL(textChanged(QString)), this, SLOT(addressChanged(QString)));
    connect(ui->sbPort, SIGNAL(valueChanged(int)), this, SLOT(portChanged(int)));
    connect(ui->cbTLS, SIGNAL(stateChanged(int)), this, SLOT(tlsChanged(int)));
}

void qTame::DefaultSettings()
{
    settings->beginGroup("User");
    if(settings->value("mode").toString() == "")
        settings->setValue("mode", "1");
    if(settings->value("address").toString() == "")
        settings->setValue("address", "mud.tamedhon.de");
    if(settings->value("port").toString() == "")
        settings->setValue("port", "4711");
    if(settings->value("tls").toString() == "")
        settings->setValue("tls", "1");
    if(settings->value("login").toString() == "")
        settings->setValue("login", "0");
    if(settings->value("pass").toString() == "")
        settings->setValue("pass", "0");
    if(settings->value("key").toString() == "")
        settings->setValue("key", QUuid::createUuid().toString().replace("{","").replace("}","").replace("-",""));
    settings->endGroup();

    settings->beginGroup("Buttons");
    if(settings->value("button1text").toString() == "")
        settings->setValue("button1text", "freier Button 1");
    if(settings->value("button1cmd").toString() == "")
        settings->setValue("button1cmd", "");
    if(settings->value("button2text").toString() == "")
        settings->setValue("button2text", "freier Button 2");
    if(settings->value("button2cmd").toString() == "")
        settings->setValue("button2cmd", "");
    if(settings->value("button3text").toString() == "")
        settings->setValue("button3text", "freier Button 3");
    if(settings->value("button3cmd").toString() == "")
        settings->setValue("button3cmd", "");
    if(settings->value("button4text").toString() == "")
        settings->setValue("button4text", "freier Button 4");
    if(settings->value("button4cmd").toString() == "")
        settings->setValue("button4cmd", "");
    if(settings->value("button5text").toString() == "")
        settings->setValue("button5text", "freier Button 5");
    if(settings->value("button5cmd").toString() == "")
        settings->setValue("button5cmd", "");
    settings->endGroup();
}

void qTame::SetMode()
{
    settings->beginGroup("User");

    switch(static_cast<Themes>(settings->value("mode").toInt()))
    {
    case Themes::dark:
    {
        break;
    }
    case Themes::light:
    {
        break;
    }
    case Themes::green:
    {
        break;
    }
    case Themes::bernstein:
    {
        break;
    }
    }

    settings->endGroup();
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
        setStatusText( tr("<qTame> DNS Auflösung %1").arg(telnet->peerName()), true );
        ui->btConnect->setText( tr("Beenden") );
        break;
    case QAbstractSocket::ConnectingState:
        //        setStatusText( tr("Verbinde zu %1").arg(telnet->peerInfo()), true );
        //        ui->btConnect->setText( tr("Trennen") );
        break;
    case QAbstractSocket::ConnectedState:
        setStatusText( tr("<qTame> Verbunden zu %1").arg(telnet->peerInfo()), true );
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
    if( telnet->isConnected() )
    {
        telnet->sendData(cmd.toUtf8());
        telnet->sendData("\n");
    }
}

void qTame::on_btConnect_clicked()
{
    if( telnet->isConnected() )
        telnet->disconnectFromHost();
    else
        telnet->connectToHost(ui->txtAddr->text(), ui->sbPort->value());
}

void qTame::addText(const char *msg, int count)
{
    ui->txtConsole->insertPlainText( QByteArray(msg, count) );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);
}

void qTame::btnClose()
{
    QApplication::exit();
}

void qTame::btnFrei1()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button1cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btnFrei2()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button2cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btnFrei3()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button3cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btnFrei4()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button4cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btnFrei5()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button5cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::onCursorUp()
{

}

void qTame::onCursorDown()
{

}

void qTame::addressChanged(QString txt)
{
    settings->beginGroup("User");
    settings->setValue("address", txt);
    settings->endGroup();
}

void qTame::portChanged(int val)
{
    settings->beginGroup("User");
    settings->setValue("port", val);
    settings->endGroup();
}

void qTame::tlsChanged(int val)
{
    settings->beginGroup("User");
    if(val == 2)
        settings->setValue("tls", "1");
    else if(val == 0)
        settings->setValue("tls", "0");
    settings->endGroup();
}
