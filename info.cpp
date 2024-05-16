#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);

    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    QFont fontTitle = QFont("Carolingia", 26, 1);
    QFont font = QFont("Carolingia", 16, 1);

    ui->lblTitle->setFont(fontTitle);

    ui->lblC->setFont(font);

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

    ui->lblC->setText(QString("(c) 2024 - Aloy\nVersion: %1 %2\nGroßer Dank an\nmeinen Lieblingsork Gralkor und Hallgrim\nfür das tolle Feedback\nund die Featurevorschläge.").arg(APP_VERSION).arg(release));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose()));
}

Info::~Info()
{
    delete settings;
    delete ui;
}

void Info::btnClose()
{
    close();
}

void Info::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress &&
        event->key() == Qt::Key_Escape)
    {
        // close window when Escape is pressed
        close();
    }
}

void Info::showEvent( QShowEvent* event )
{
    SetMode();
}

void Info::SetMode()
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

void Info::SetColors(QString foreground, QString background)
{
    setStyleSheet(QString("background-color: %1; color: %2;").arg(background).arg(foreground));

    ui->btnClose->setStyleSheet(QString("QPushButton{border: 2px solid; border-color: %1;} QPushButton:hover{background-color: %1; color: %2;}").arg(foreground).arg(background));
}
