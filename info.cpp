#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);

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

    ui->lblC->setText(QString("(c) 2024 - Aloy\nVersion: %1 %2").arg(APP_VERSION).arg(release));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose()));
}

Info::~Info()
{
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
