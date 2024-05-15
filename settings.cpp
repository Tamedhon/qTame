#include "settings.h"
#include "ui_settings.h"


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);

    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    crypt = new Crypto::Crypto();

    ui->cmbTheme->addItems({"Darkmode", "Lightmode", "grün monochrom", "bernstein"});

    LoadSettings();
    Connects();

    QFont font = QFont("Carolingia", 15, 1);

    ui->lblB1->setFont(font);
    ui->lblB2->setFont(font);
    ui->lblB3->setFont(font);
    ui->lblB4->setFont(font);
    ui->lblB5->setFont(font);
    ui->lblB6->setFont(font);
    ui->lblB7->setFont(font);
    ui->lblB8->setFont(font);
    ui->lblB9->setFont(font);
    ui->lblB10->setFont(font);
    ui->lblPW->setFont(font);
    ui->lblUser->setFont(font);
    ui->lblBtnText->setFont(font);
    ui->lblCMD->setFont(font);
    ui->lblTheme->setFont(font);
}

Settings::~Settings()
{
    delete crypt;
    delete settings;
    delete ui;
}

void Settings::Connects()
{
    connect(ui->cmbTheme, SIGNAL(currentIndexChanged(int)), this, SLOT(ThemeChanged(int)));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose()));

    connect(ui->leChar, SIGNAL(textChanged(QString)), this, SLOT(UserChanged(QString)));
    connect(ui->lePW, SIGNAL(textChanged(QString)), this, SLOT(PasswordChanged(QString)));

    connect(ui->leButtonText1, SIGNAL(textChanged(QString)), this, SLOT(Button1TextChanged(QString)));
    connect(ui->leCMD1, SIGNAL(textChanged(QString)), this, SLOT(Button1CMDChanged(QString)));
    connect(ui->leButtonText2, SIGNAL(textChanged(QString)), this, SLOT(Button2TextChanged(QString)));
    connect(ui->leCMD2, SIGNAL(textChanged(QString)), this, SLOT(Button2CMDChanged(QString)));
    connect(ui->leButtonText3, SIGNAL(textChanged(QString)), this, SLOT(Button3TextChanged(QString)));
    connect(ui->leCMD3, SIGNAL(textChanged(QString)), this, SLOT(Button3CMDChanged(QString)));
    connect(ui->leButtonText4, SIGNAL(textChanged(QString)), this, SLOT(Button4TextChanged(QString)));
    connect(ui->leCMD4, SIGNAL(textChanged(QString)), this, SLOT(Button4CMDChanged(QString)));
    connect(ui->leButtonText5, SIGNAL(textChanged(QString)), this, SLOT(Button5TextChanged(QString)));
    connect(ui->leCMD5, SIGNAL(textChanged(QString)), this, SLOT(Button5CMDChanged(QString)));
    connect(ui->leButtonText6, SIGNAL(textChanged(QString)), this, SLOT(Button6TextChanged(QString)));
    connect(ui->leCMD6, SIGNAL(textChanged(QString)), this, SLOT(Button6CMDChanged(QString)));
    connect(ui->leButtonText7, SIGNAL(textChanged(QString)), this, SLOT(Button7TextChanged(QString)));
    connect(ui->leCMD7, SIGNAL(textChanged(QString)), this, SLOT(Button7CMDChanged(QString)));
    connect(ui->leButtonText8, SIGNAL(textChanged(QString)), this, SLOT(Button8TextChanged(QString)));
    connect(ui->leCMD8, SIGNAL(textChanged(QString)), this, SLOT(Button8CMDChanged(QString)));
    connect(ui->leButtonText9, SIGNAL(textChanged(QString)), this, SLOT(Button9TextChanged(QString)));
    connect(ui->leCMD9, SIGNAL(textChanged(QString)), this, SLOT(Button9CMDChanged(QString)));
    connect(ui->leButtonText10, SIGNAL(textChanged(QString)), this, SLOT(Button10TextChanged(QString)));
    connect(ui->leCMD10, SIGNAL(textChanged(QString)), this, SLOT(Button10CMDChanged(QString)));
}

void Settings::LoadSettings()
{
    settings->beginGroup("User");

    ui->cmbTheme->setCurrentIndex(settings->value("mode").toInt());

    ui->leChar->setText(settings->value("login").toString());

    if(settings->value("pass").toString() != "")
        ui->lePW->setText(crypt->DecryptBase64(settings->value("pass").toString()));

    settings->endGroup();

    settings->beginGroup("Buttons");

    ui->leButtonText1->setText(settings->value("button1text").toString());
    ui->leCMD1->setText(settings->value("button1cmd").toString());
    ui->leButtonText2->setText(settings->value("button2text").toString());
    ui->leCMD2->setText(settings->value("button2cmd").toString());
    ui->leButtonText3->setText(settings->value("button3text").toString());
    ui->leCMD3->setText(settings->value("button3cmd").toString());
    ui->leButtonText4->setText(settings->value("button4text").toString());
    ui->leCMD4->setText(settings->value("button4cmd").toString());
    ui->leButtonText5->setText(settings->value("button5text").toString());
    ui->leCMD5->setText(settings->value("button5cmd").toString());
    ui->leButtonText6->setText(settings->value("button6text").toString());
    ui->leCMD6->setText(settings->value("button6cmd").toString());
    ui->leButtonText7->setText(settings->value("button7text").toString());
    ui->leCMD7->setText(settings->value("button7cmd").toString());
    ui->leButtonText8->setText(settings->value("button8text").toString());
    ui->leCMD8->setText(settings->value("button8cmd").toString());
    ui->leButtonText9->setText(settings->value("button9text").toString());
    ui->leCMD9->setText(settings->value("button9cmd").toString());
    ui->leButtonText10->setText(settings->value("button10text").toString());
    ui->leCMD10->setText(settings->value("button10cmd").toString());

    settings->endGroup();
}

void Settings::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress &&
        event->key() == Qt::Key_Escape)
    {
        // close window when Escape is pressed
        close();
    }
}

void Settings::btnClose()
{
    close();
}

void Settings::UserChanged(QString txt)
{
    settings->beginGroup("User");
    settings->setValue("login", txt);
    settings->endGroup();
}

void Settings::PasswordChanged(QString txt)
{
    settings->beginGroup("User");
    settings->setValue("pass", crypt->EncryptBase64(txt));
    settings->endGroup();
}

void Settings::Button1TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button1text", txt);
    settings->endGroup();
}

void Settings::Button1CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button1cmd", txt);
    settings->endGroup();
}

void Settings::Button2TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button2text", txt);
    settings->endGroup();
}

void Settings::Button2CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button2cmd", txt);
    settings->endGroup();
}

void Settings::Button3TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button3text", txt);
    settings->endGroup();
}

void Settings::Button3CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button3cmd", txt);
    settings->endGroup();
}

void Settings::Button4TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button4text", txt);
    settings->endGroup();
}

void Settings::Button4CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button4cmd", txt);
    settings->endGroup();
}

void Settings::Button5TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button5text", txt);
    settings->endGroup();
}

void Settings::Button5CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button5cmd", txt);
    settings->endGroup();
}

void Settings::Button6TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button6text", txt);
    settings->endGroup();
}

void Settings::Button6CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button6cmd", txt);
    settings->endGroup();
}

void Settings::Button7TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button7text", txt);
    settings->endGroup();
}

void Settings::Button7CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button7cmd", txt);
    settings->endGroup();
}

void Settings::Button8TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button8text", txt);
    settings->endGroup();
}

void Settings::Button8CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button8cmd", txt);
    settings->endGroup();
}

void Settings::Button9TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button9text", txt);
    settings->endGroup();
}

void Settings::Button9CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button9cmd", txt);
    settings->endGroup();
}

void Settings::Button10TextChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button10text", txt);
    settings->endGroup();
}

void Settings::Button10CMDChanged(QString txt)
{
    settings->beginGroup("Buttons");
    settings->setValue("button10cmd", txt);
    settings->endGroup();
}

void Settings::ThemeChanged(int idx)
{
    settings->beginGroup("User");
    settings->setValue("mode", idx);
    settings->endGroup();
}
