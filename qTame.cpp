#include "qTame.h"
#include "ui_qTame.h"

#include <QThread>

qTame::qTame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTame),
    telnet(new QTelnet(this))
{
    ui->setupUi(this);

    //move(pos() + (QGuiApplication::primaryScreen()->geometry().center() - geometry().center()));
    move(QGuiApplication::primaryScreen()->geometry().center() - rect().center());

    info = new Info();
    set = new Settings();
    crypt = new Crypto::Crypto();
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    QFont font = QFont("Consolas", 13, 1);
    ui->txtConsole->setFont(font);

    LoadValues();
    LoadButtonTexts();
    Connects();
}

qTame::~qTame()
{
    telnet->disconnectFromHost();
    delete settings;
    delete crypt;
    delete telnet;
    delete info;
    delete set;
    delete ui;
}

void qTame::Connects()
{
    connect(telnet, SIGNAL(newData(const char*,int)), this, SLOT(addText(const char*,int)));
    connect(telnet, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));

    connect(ui->cbCmd, SIGNAL(command(QString)), this, SLOT(onCommand(QString)));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose()));
    connect(ui->btn1, SIGNAL(clicked()), this, SLOT(btn1()));
    connect(ui->btn2, SIGNAL(clicked()), this, SLOT(btn2()));
    connect(ui->btn3, SIGNAL(clicked()), this, SLOT(btn3()));
    connect(ui->btn4, SIGNAL(clicked()), this, SLOT(btn4()));
    connect(ui->btn5, SIGNAL(clicked()), this, SLOT(btn5()));
    connect(ui->btn6, SIGNAL(clicked()), this, SLOT(btn6()));
    connect(ui->btn7, SIGNAL(clicked()), this, SLOT(btn7()));
    connect(ui->btn8, SIGNAL(clicked()), this, SLOT(btn8()));
    connect(ui->btn9, SIGNAL(clicked()), this, SLOT(btn9()));
    connect(ui->btn10, SIGNAL(clicked()), this, SLOT(btn10()));

    connect(ui->txtAddr, SIGNAL(textChanged(QString)), this, SLOT(addressChanged(QString)));
    connect(ui->sbPort, SIGNAL(valueChanged(int)), this, SLOT(portChanged(int)));
    connect(ui->cbTLS, SIGNAL(stateChanged(int)), this, SLOT(tlsChanged(int)));
    connect(ui->cbAutologin, SIGNAL(stateChanged(int)), this, SLOT(autologinChanged(int)));

    connect(ui->actionBeenden, SIGNAL(triggered(bool)), this, SLOT(btnClose()));
    connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(openInfo()));
    connect(ui->actionFunktionen, SIGNAL(triggered(bool)), this, SLOT(openFunktionen()));
    connect(ui->actionNavigation, SIGNAL(triggered(bool)), this, SLOT(openNavigation()));
    connect(ui->actionEinstellungen, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
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
        setStatusText( tr("\n<qTame> Getrennt"), true );
        ui->btConnect->setText( tr("Verbinde") );
        break;
    case QAbstractSocket::HostLookupState:
        setStatusText( tr("\n<qTame> DNS Auflösung %1").arg(telnet->peerName()), true );
        ui->btConnect->setText( tr("Abbrechen") );
        break;
    case QAbstractSocket::ConnectingState:
        //        setStatusText( tr("Verbinde zu %1").arg(telnet->peerInfo()), true );
        //        ui->btConnect->setText( tr("Trennen") );
        break;
    case QAbstractSocket::ConnectedState:
        setStatusText( tr("\n<qTame> Verbunden zu %1").arg(telnet->peerInfo()), true );
        ui->btConnect->setText( tr("Trennen") );
        break;
    case QAbstractSocket::BoundState:
        setStatusText( tr("\n<qTame> Verlinkt"), true );
        ui->btConnect->setText( tr("- - - -") );
        break;
    case QAbstractSocket::ListeningState:
        setStatusText( tr("\n<qTame> Lausche"), true );
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
    ui->statusBar->showMessage(QString(msg).remove("\n<qTame> "));
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
    {
        user = false;
        password = false;
        telnet->disconnectFromHost();
    }
    else
        telnet->connectToHost(ui->txtAddr->text(), ui->sbPort->value());
}

void qTame::addText(const char *msg, int count)
{   
    //autologin
    settings->beginGroup("User");

    if(settings->value("autologin").toInt() == 1 &&
            settings->value("login").toString() != "" &&
            settings->value("pass").toString() != "")
    {
        if(QString::fromUtf8(msg).contains("Spieler) ?") && !user)
        {
            user = true;
            if( telnet->isConnected() )
            {
                telnet->sendData(settings->value("login").toString().toUtf8());
                telnet->sendData("\n");
            }
        }

        if(QString::fromUtf8(msg).contains("Password") && !password)
        {
            password = true;
            if( telnet->isConnected() )
            {
                telnet->sendData(crypt->DecryptBase64(settings->value("pass").toString()).toUtf8());
                telnet->sendData("\n");
            }
        }
    }
    settings->endGroup(); //autologin ende

    ui->txtConsole->insertPlainText( QByteArray(msg, count) );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);
}

void qTame::btnClose()
{
    QApplication::exit();
}

void qTame::btn1()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button1cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn2()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button2cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn3()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button3cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn4()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button4cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn5()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button5cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn6()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button6cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn7()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button7cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn8()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button8cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn9()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button9cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::btn10()
{
    settings->beginGroup("Buttons");

    if( telnet->isConnected() )
    {
        telnet->sendData(settings->value("button10cmd").toString().toUtf8());
        telnet->sendData("\n");
    }

    settings->endGroup();
}

void qTame::openInfo()
{
    QString release;
#ifdef _WIN32
    release = "win-amd64 Release";
#endif

#ifdef Q_OS_ANDROID
    release = "android-arm Release";
#endif

#ifdef unix
    release = "UNIX-amd64 Release";
#endif

    ui->txtConsole->insertPlainText(QString("\n<qTame> Tamedhon qTame Version %1 %2\n\n").arg(APP_VERSION).arg(release));
    moveCursorToEnd();
    info->exec();
}

void qTame::openSettings()
{
    set->exec();
    LoadButtonTexts();
    SetMode();
}

void qTame::openFunktionen()
{
    ui->txtConsole->insertPlainText(
                "\n\n<qTame> Funktionen: \n"
                "<qTame> ===========\n\n"
                "<qTame> => Scrollrad in der Konsole scrollt durch die Ausgabe\n"
                "<qTame> => Strg + Scrollrad vergrößert/verkleinert die Schriftgröße\n"
                "<qTame> => Cursortasten navigieren durch die Befehlshistory\n"
                "<qTame> => Änderungen von Adresse, Port und TLS werden automatisch sofort gespeichert\n"
                "<qTame> => Änderungen im Einstellungsmenü werden automatisch sofort gespeichert\n"
                "<qTame> => !!! Autologin funktioniert nur MIT hinterlegten Logindaten!\n"
                "<qTame> => !!! TLS funktioniert nur, wenn es der Server unterstützt!\n\n"
                );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);

    moveCursorToEnd();
}

void qTame::openNavigation()
{
    ui->txtConsole->insertPlainText(
                "\n\n<qTame> Navigation: \n"
                "<qTame> ===========\n\n"
                "<qTame> => Navigiert wird mit dem Nummernblock (NUM muss aktiv sein)\n"
                "<qTame> => Die Eingaben können mit Strg und Alt modifiziert werden\n\n"
                "<qTame> => 7 = nordwesten\n"
                "<qTame> => 8 = norden\n"
                "<qTame> => 9 = nordosten\n"
                "<qTame> => 6 = osten\n"
                "<qTame> => 3 = südosten\n"
                "<qTame> => 2 = süden\n"
                "<qTame> => 1 = südwesten\n"
                "<qTame> => 4 = westen\n\n"
                "<qTame> => Alt + 5 = oben\n"
                "<qTame> => Strg + 5 = unten\n"
                "<qTame> => Alt + 8 = nordoben\n"
                "<qTame> => Strg + 8 = nordunten\n"
                "<qTame> => Alt + 6 = ostoben\n"
                "<qTame> => Strg + 6 = ostunten\n"
                "<qTame> => Alt + 2 = südoben\n"
                "<qTame> => Strg + 2 = südunten\n"
                "<qTame> => Alt + 4 = westoben\n"
                "<qTame> => Strg + 4 = westunten\n\n"
                );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);

    moveCursorToEnd();
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

void qTame::autologinChanged(int val)
{
    settings->beginGroup("User");
    if(val == 2)
        settings->setValue("autologin", "1");
    else if(val == 0)
        settings->setValue("autologin", "0");
    settings->endGroup();
}

void qTame::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_Escape)
    {
        // close window when Escape is pressed
        close();
    }

    if(event->modifiers() & Qt::KeypadModifier)
    {
        if(event->key() == Qt::Key_1)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("suedwesten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_2)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("sueden").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_3)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("suedosten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_4)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("westen").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_6)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("osten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_7)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("nordwesten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_8)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("norden").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        else if(event->key() == Qt::Key_9)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("nordosten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
    }

    if((event->modifiers() & Qt::KeypadModifier) && (event->modifiers() & Qt::ControlModifier))
    {
        if(event->key() == Qt::Key_2)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("suedunten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }

        if(event->key() == Qt::Key_4)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("westunten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        if(event->key() == Qt::Key_5)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("unten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
        if(event->key() == Qt::Key_8)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("nordunten").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }
    }

    if((event->modifiers() & Qt::KeypadModifier) && (event->modifiers() & Qt::AltModifier))
    {
        if(event->key() == Qt::Key_2)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("suedoben").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }

        if(event->key() == Qt::Key_4)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("westoben").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }

        if(event->key() == Qt::Key_5)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("oben").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }

        if(event->key() == Qt::Key_8)
        {
            if( telnet->isConnected() )
            {
                telnet->sendData(QString("nordoben").toUtf8());
                telnet->sendData("\n");
            }
            return;
        }

    }
    QWidget::keyPressEvent(event);
}

void qTame::moveCursorToEnd()
{
    QTextCursor cursor = ui->txtConsole->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->txtConsole->setTextCursor(cursor);
}

void qTame::LoadButtonTexts()
{
    settings->beginGroup("Buttons");

    ui->btn1->setText(settings->value("button1text").toString());
    ui->btn2->setText(settings->value("button2text").toString());
    ui->btn3->setText(settings->value("button3text").toString());
    ui->btn4->setText(settings->value("button4text").toString());
    ui->btn5->setText(settings->value("button5text").toString());
    ui->btn6->setText(settings->value("button6text").toString());
    ui->btn7->setText(settings->value("button7text").toString());
    ui->btn8->setText(settings->value("button8text").toString());
    ui->btn9->setText(settings->value("button9text").toString());
    ui->btn10->setText(settings->value("button10text").toString());

    settings->endGroup();
}

void qTame::LoadValues()
{
    settings->beginGroup("User");

    SetMode();
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

    if(settings->value("autologin").toInt() == 1)
    {
        ui->cbAutologin->setChecked(true);
    }
    else if(settings->value("autologin").toInt() == 0)
    {
        ui->cbAutologin->setChecked(false);
    }

    settings->endGroup();
}
