#include "qTame.h"
#include "ui_qTame.h"


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
    col = new DataCollector();
    col->start();
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    QFont font = QFont("Consolas", 13, 1);
    ui->txtConsole->setFont(font);

    LoadValues();
    SetMode();
    LoadButtonTexts();
    SetScreenreaderAccessables();
    Connects();

    GetNews();

    ui->cbCmd->setFocus();
}

qTame::~qTame()
{
    telnet->disconnectFromHost();
    delete settings;
    delete crypt;
    delete telnet;
    delete info;
    delete set;
    if(log != nullptr)
        delete log;
    col->Cancel();
    while(col->isRunning()){}
    delete col;
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
    connect(ui->txtAddr, SIGNAL(returnPressed()), this, SLOT(addressEnter()));
    connect(ui->sbPort, SIGNAL(valueChanged(int)), this, SLOT(portChanged(int)));
    connect(ui->cbTLS, SIGNAL(stateChanged(int)), this, SLOT(tlsChanged(int)));
    connect(ui->cbAutologin, SIGNAL(stateChanged(int)), this, SLOT(autologinChanged(int)));
    connect(ui->cbLog, SIGNAL(stateChanged(int)), this, SLOT(logChanged(int)));

    connect(ui->actionBeenden, SIGNAL(triggered(bool)), this, SLOT(btnClose()));
    connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(openInfo()));
    connect(ui->actionFunktionen, SIGNAL(triggered(bool)), this, SLOT(openFunktionen()));
    connect(ui->actionNavigation, SIGNAL(triggered(bool)), this, SLOT(openNavigation()));
    connect(ui->actionEinstellungen, SIGNAL(triggered(bool)), this, SLOT(openSettings()));

    connect(ui->cbCmd, &QCmdWidget::numPadKeyEvent, this, &qTame::handleNumPadEvent);
}

void qTame::SetMode()
{
    settings->beginGroup("User");

    switch(static_cast<Themes>(settings->value("mode").toInt()))
    {
    case Themes::dark:
    {
        SetColors("rgb(235, 235, 235)","rgb(58, 58, 58)");
        break;
    }
    case Themes::light:
    {
        SetColors("rgb(0, 0, 0)", "rgb(235, 235, 235)");
        break;
    }
    case Themes::green:
    {
        SetColors("rgb(127, 255, 127)","rgb(0, 0, 0)");
        break;
    }
    case Themes::bernstein:
    {
        SetColors("rgb(238, 160, 64)","rgb(0, 0, 0)");
        break;
    }
    }

    settings->endGroup();
}

void qTame::SetColors(QString foreground, QString background)
{
    setStyleSheet(QString("background-color: %1; color: %2;").arg(background).arg(foreground));
    QList<QMenu *> allMenu = findChildren<QMenu *>();
    for (QMenu *menu: allMenu)
    {
        menu->setStyleSheet(QString("selection-background-color: %1; selection-color: %2;").arg(foreground).arg(background));
    }
    QList<QMenuBar *> allMenuBar = findChildren<QMenuBar *>();
    for (QMenuBar *menubar: allMenuBar)
    {
        menubar->setStyleSheet(QString("QMenuBar::item:selected{background: %1; color: %2;} QMenuBar::item:pressed{background: %1; color: %2;}").arg(foreground).arg(background));
    }
    QList<QCheckBox *> allCheckBox = findChildren<QCheckBox *>();
    for (QCheckBox *checkbox: allCheckBox)
    {
        checkbox->setStyleSheet(QString("QCheckBox::indicator{border: 1px solid; border-color: %1;} QCheckBox::indicator:checked{background-color: %1;}").arg(foreground));
    }
    QList<QLineEdit *> allLe = findChildren<QLineEdit *>();
    for (QLineEdit *le: allLe)
    {
        le->setStyleSheet(QString("selection-background-color: %1; selection-color: %2;").arg(foreground).arg(background));
    }
    QList<QPushButton *> allButtons = findChildren<QPushButton *>();
    for (QPushButton *buttons : allButtons)
    {
        buttons->setStyleSheet(QString("QPushButton{border: 2px solid; border-color: %1;} QPushButton:hover{background-color: %1; color: %2;}").arg(foreground).arg(background));
    }
    ui->txtConsole->setStyleSheet(QString("border: 1px solid; border-color: %1;").arg(foreground));
    ui->cbCmd->setStyleSheet(QString("font-size: 24px; border: 1px solid; border-color: %1; selection-background-color: %1; selection-color: %2;").arg(foreground).arg(background));
    ui->txtAddr->setStyleSheet(QString("border: 1px solid; border-color: %1;").arg(foreground));
    ui->sbPort->setStyleSheet(QString("border: 1px solid; border-color: %1; selection-background-color: %1; selection-color: %2;").arg(foreground).arg(background));
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
    TelnetCommand(cmd);
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

    ui->cbCmd->setFocus();
}

void qTame::addText(const char *msg, int count)
{   
    if(writeLog)
    {
        WriteLog(QByteArray(msg, count));
    }

    ui->txtConsole->insertAnsiText(QByteArray(msg, count));
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);

    //autologin
    settings->beginGroup("User");

    if(settings->value("autologin").toInt() == 1 &&
            settings->value("login").toString() != "" &&
            settings->value("pass").toString() != "")
    {
        if(QString::fromUtf8(QByteArray(msg, count)).contains("Spieler) ?") && !user)
        {
            user = true;
            TelnetCommand(settings->value("login").toString());
        }

        if(QString::fromUtf8(QByteArray(msg, count)).contains("Password") && !password)
        {
            password = true;
            if(telnet->isConnected())
            {
                telnet->sendData(crypt->DecryptBase64(settings->value("pass").toString()).toUtf8());
                telnet->sendData("\n");
            }
        }
    }
    settings->endGroup(); //autologin ende
}

void qTame::btnClose()
{
    QApplication::exit();
}

void qTame::btn1()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button1cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }
    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn2()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button2cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn3()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button3cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn4()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button4cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn5()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button5cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn6()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button6cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn7()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button7cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn8()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button8cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn9()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button9cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }
    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::btn10()
{
    settings->beginGroup("Buttons");

    foreach(QString str, settings->value("button10cmd").toString().split(";"))
    {
        TelnetCommand(str);
    }

    settings->endGroup();
    ui->cbCmd->setFocus();
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
    ui->cbCmd->setFocus();
}

void qTame::openSettings()
{
    set->exec();
    LoadButtonTexts();
    SetMode();
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);
    ui->cbCmd->setFocus();
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
                "<qTame> => In den Einstellungen können Befehle auf den Buttons mit ';' verkettet werden.\n"
                "           z.B. 'sag test;sag test2'\n"
                "<qTame> => Wenn Logging gewünscht ist, muss das bei jeder neuen Sitzung neu aktiviert werden.\n"
                "<qTame> => Die Eingabezeile wertet Zahlen nur von der Zahlenreihe aus.\n"
                "           Der Nummernblock ist für die Navigation reserviert.\n"
                "<qTame> => !!! Autologin funktioniert nur MIT hinterlegten Logindaten!\n"
                "<qTame> => !!! TLS funktioniert nur, wenn es der Server unterstützt!\n"
                "<qTame> => Die config-Datei qTame.td liegt unter /bin.\n"
                "           Z.B. für Backups wichtig."
                );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);

    moveCursorToEnd();
    ui->cbCmd->setFocus();
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
                "<qTame> => Strg + 5 = unten\n"
                "<qTame> => Strg + 7 = nordwestunten\n"
                "<qTame> => Strg + 8 = nordunten\n"
                "<qTame> => Strg + 9 = nordostunten\n"
                "<qTame> => Strg + 6 = ostunten\n"
                "<qTame> => Strg + 3 = südostunten\n"
                "<qTame> => Strg + 2 = südunten\n"
                "<qTame> => Strg + 1 = südwestunten\n"
                "<qTame> => Strg + 4 = westunten\n\n"
                "<qTame> => Alt + 5 = oben\n"
                "<qTame> => Alt + 7 = nordwestoben\n"
                "<qTame> => Alt + 8 = nordoben\n"
                "<qTame> => Alt + 9 = nordostoben\n"
                "<qTame> => Alt + 6 = ostoben\n"
                "<qTame> => Alt + 3 = südostoben\n"
                "<qTame> => Alt + 2 = südoben\n"
                "<qTame> => Alt + 1 = südwestoben\n"
                "<qTame> => Alt + 4 = westoben\n\n"
                );
    ui->txtConsole->verticalScrollBar()->setValue(0xFFFFFFF);

    moveCursorToEnd();
    ui->cbCmd->setFocus();
}

void qTame::CursorUp()
{
    ui->cbCmd->setFocus();
    ui->cbCmd->setCurrentIndex(ui->cbCmd->count()-1);
}

void qTame::CursorDown()
{
    ui->cbCmd->setFocus();
    ui->cbCmd->setCurrentIndex(0);
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
    ui->cbCmd->setFocus();
}

void qTame::autologinChanged(int val)
{
    settings->beginGroup("User");
    if(val == 2)
        settings->setValue("autologin", "1");
    else if(val == 0)
        settings->setValue("autologin", "0");
    settings->endGroup();
    ui->cbCmd->setFocus();
}

void qTame::logChanged(int val)
{
    if(val == 2)
    {
        writeLog = true;
        log = new QFile(QDir::currentPath() + "/logs/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + ".log");
    }
    else if(val == 0)
    {
        writeLog = false;
    }
    ui->cbCmd->setFocus();
}

void qTame::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_Up)
    {
        CursorUp();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_Down)
    {
        CursorDown();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F1)
    {
        btn1();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F2)
    {
        btn2();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F3)
    {
        btn3();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F4)
    {
        btn4();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F5)
    {
        btn5();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F6)
    {
        btn6();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F7)
    {
        btn7();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F8)
    {
        btn8();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F9)
    {
        btn9();
    }

    if(event->type() == QKeyEvent::KeyPress &&
            event->key() == Qt::Key_F10)
    {
        btn10();
    }

    if(event->modifiers() & Qt::KeypadModifier && !(event->modifiers() & Qt::ControlModifier) && !(event->modifiers() & Qt::AltModifier))
    {
        if(event->key() == Qt::Key_1)
        {
            TelnetCommand("suedwesten");
            return;
        }
        else if(event->key() == Qt::Key_2)
        {
            TelnetCommand("sueden");
            return;
        }
        else if(event->key() == Qt::Key_3)
        {
            TelnetCommand("suedosten");
            return;
        }
        else if(event->key() == Qt::Key_4)
        {
            TelnetCommand("westen");
            return;
        }
        else if(event->key() == Qt::Key_6)
        {
            TelnetCommand("osten");
            return;
        }
        else if(event->key() == Qt::Key_7)
        {
            TelnetCommand("nordwesten");
            return;
        }
        else if(event->key() == Qt::Key_8)
        {
            TelnetCommand("norden");
            return;
        }
        else if(event->key() == Qt::Key_9)
        {
            TelnetCommand("nordosten");
            return;
        }
    }
    else if((event->modifiers() & Qt::KeypadModifier) && (event->modifiers() & Qt::ControlModifier))
    {
        if(event->key() == Qt::Key_1)
        {
            TelnetCommand("suedwestunten");
            return;
        }
        else if(event->key() == Qt::Key_2)
        {
            TelnetCommand("suedunten");
            return;
        }
        else if(event->key() == Qt::Key_3)
        {
            TelnetCommand("suedostunten");
            return;
        }
        else if(event->key() == Qt::Key_4)
        {
            TelnetCommand("westunten");
            return;
        }
        else if(event->key() == Qt::Key_5)
        {
            TelnetCommand("unten");
            return;
        }
        else if(event->key() == Qt::Key_6)
        {
            TelnetCommand("ostunten");
            return;
        }
        else if(event->key() == Qt::Key_7)
        {
            TelnetCommand("nordwestunten");
            return;
        }
        else if(event->key() == Qt::Key_8)
        {
            TelnetCommand("nordunten");
            return;
        }
        else if(event->key() == Qt::Key_9)
        {
            TelnetCommand("nordostunten");
            return;
        }
    }
    else if((event->modifiers() & Qt::KeypadModifier) && (event->modifiers() & Qt::AltModifier))
    {
        if(event->key() == Qt::Key_1)
        {
            TelnetCommand("suedwestoben");
            return;
        }
        else if(event->key() == Qt::Key_2)
        {
            TelnetCommand("suedoben");
            return;
        }
        else if(event->key() == Qt::Key_3)
        {
            TelnetCommand("suedostoben");
            return;
        }
        else if(event->key() == Qt::Key_4)
        {
            TelnetCommand("westoben");
            return;
        }
        else if(event->key() == Qt::Key_5)
        {
            TelnetCommand("oben");
            return;
        }
        else if(event->key() == Qt::Key_6)
        {
            TelnetCommand("ostoben");
            return;
        }
        else if(event->key() == Qt::Key_7)
        {
            TelnetCommand("nordwestoben");
            return;
        }
        else if(event->key() == Qt::Key_8)
        {
            TelnetCommand("nordoben");
            return;
        }
        else if(event->key() == Qt::Key_9)
        {
            TelnetCommand("nordostoben");
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

    ui->btn1->setText("F1: " + settings->value("button1text").toString());
    ui->btn2->setText("F2: " + settings->value("button2text").toString());
    ui->btn3->setText("F3: " + settings->value("button3text").toString());
    ui->btn4->setText("F4: " + settings->value("button4text").toString());
    ui->btn5->setText("F5: " + settings->value("button5text").toString());
    ui->btn6->setText("F6: " + settings->value("button6text").toString());
    ui->btn7->setText("F7: " + settings->value("button7text").toString());
    ui->btn8->setText("F8: " + settings->value("button8text").toString());
    ui->btn9->setText("F9: " + settings->value("button9text").toString());
    ui->btn10->setText("F10: " + settings->value("button10text").toString());

    if(settings->value("buttons_vis").toInt() == 1)
    {
        ui->btn1->setVisible(true);
        ui->btn2->setVisible(true);
        ui->btn3->setVisible(true);
        ui->btn4->setVisible(true);
        ui->btn5->setVisible(true);
        ui->btn6->setVisible(true);
        ui->btn7->setVisible(true);
        ui->btn8->setVisible(true);
        ui->btn9->setVisible(true);
        ui->btn10->setVisible(true);
    }
    else if(settings->value("buttons_vis").toInt() == 0)
    {
        ui->btn1->setVisible(false);
        ui->btn2->setVisible(false);
        ui->btn3->setVisible(false);
        ui->btn4->setVisible(false);
        ui->btn5->setVisible(false);
        ui->btn6->setVisible(false);
        ui->btn7->setVisible(false);
        ui->btn8->setVisible(false);
        ui->btn9->setVisible(false);
        ui->btn10->setVisible(false);
    }

    settings->endGroup();
}

void qTame::SetScreenreaderAccessables()
{
//#include <QtAccessibilitySupport/QtAccessibilitySupport>
//    qDebug() << QAccessibility::isActive;

    ui->btn1->setAccessibleName("");
    ui->btn1->setAccessibleDescription("");
}

void qTame::LoadValues()
{
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

void qTame::TelnetCommand(QString cmd)
{
    if(telnet->isConnected())
    {
        if(writeLog)
        {
            WriteLog(cmd+"\n");
        }
        telnet->sendData(cmd.toUtf8());
        telnet->sendData("\n");
        ui->txtConsole->insertPlainText("\n");
    }
}

void qTame::WriteLog(QString txt)
{
    QRegularExpression regexp("\x1b\\[[0-9;]*[A-Za-z]");

    log->open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream out(log);
#ifndef QT_DEBUG
    out << txt.remove(regexp);
#else
    out << txt;
#endif

    if(log->isOpen())
        log->close();
}

void qTame::handleNumPadEvent(QKeyEvent *event)
{
    keyPressEvent(event);
}

void qTame::GetNews()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
                     this, [=](QNetworkReply *reply)
    {
        if (reply->error())
        {
            ui->txtConsole->insertPlainText("<qTame> News nicht erreichbar!");
            return;
        }

        ui->txtConsole->insertHtml(reply->readAll().replace("<hr>","__________________________________________________________________"));
    }
    );

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.tamedhon.de/content/news.php"));
    QSslConfiguration config(QSslConfiguration::defaultConfiguration());
    config.setPeerVerifyMode(QSslSocket::VerifyPeer);
    config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);
    manager->get(request);
}

void qTame::addressEnter()
{
    if(!telnet->isConnected())
        telnet->connectToHost(ui->txtAddr->text(), ui->sbPort->value());
    ui->cbCmd->setFocus();
}
