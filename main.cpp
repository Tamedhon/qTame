#include "qTame.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QFontDatabase>
#include <QDir>
#include <QCommandLineParser>

void DefaultSettings();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;

    parser.setApplicationDescription("qTame Hilfe");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption debugOption(QStringList() << "d" << "debug", "Debugmodus! Loggt ALLES inkl. Steuerzeichen!");
    parser.addOption(debugOption);
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    bool debug = parser.isSet(debugOption);

    QRandomGenerator::securelySeeded();

    QFontDatabase::addApplicationFont(":/fonts/Consolas.ttf");
    QFontDatabase::addApplicationFont(":/fonts/carolingia.ttf");

    if(!QDir(QDir::currentPath() + "/logs").exists())
        QDir().mkpath(QDir::currentPath() + "/logs");

    DefaultSettings();

    qTame w;
    w.show();

    return a.exec();
}

void DefaultSettings()
{
    QSettings *settings;
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/qTame.td", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->beginGroup("User");
    if(settings->value("mode").toString() == "")
        settings->setValue("mode", "0");
    if(settings->value("address").toString() == "")
        settings->setValue("address", "mud.tamedhon.de");
    if(settings->value("port").toString() == "")
        settings->setValue("port", "4712");
    if(settings->value("tls").toString() == "")
        settings->setValue("tls", "1");
    if(settings->value("autologin").toString() == "")
        settings->setValue("autologin", "0");
    if(settings->value("login").toString() == "")
        settings->setValue("login", "");
    if(settings->value("pass").toString() == "")
        settings->setValue("pass", "");
    if(settings->value("key").toString() == "")
        settings->setValue("key", QUuid::createUuid().toString().replace("{","").replace("}","").replace("-",""));
    settings->endGroup();

    settings->beginGroup("Buttons");
    if(settings->value("button1text").toString() == "")
        settings->setValue("button1text", "Inventar");
    if(settings->value("button1cmd").toString() == "")
        settings->setValue("button1cmd", "i");
    if(settings->value("button2text").toString() == "")
        settings->setValue("button2text", "Ausrüstung");
    if(settings->value("button2cmd").toString() == "")
        settings->setValue("button2cmd", "ausruestung");
    if(settings->value("button3text").toString() == "")
        settings->setValue("button3text", "Attribute");
    if(settings->value("button3cmd").toString() == "")
        settings->setValue("button3cmd", "info");
    if(settings->value("button4text").toString() == "")
        settings->setValue("button4text", "Quests");
    if(settings->value("button4cmd").toString() == "")
        settings->setValue("button4cmd", "liste");
    if(settings->value("button5text").toString() == "")
        settings->setValue("button5text", "Fertigkeiten");
    if(settings->value("button5cmd").toString() == "")
        settings->setValue("button5cmd", "fertigkeiten");
    if(settings->value("button6text").toString() == "")
        settings->setValue("button6text", "freier Button 1");
    if(settings->value("button6cmd").toString() == "")
        settings->setValue("button6cmd", "");
    if(settings->value("button7text").toString() == "")
        settings->setValue("button7text", "freier Button 2");
    if(settings->value("button7cmd").toString() == "")
        settings->setValue("button7cmd", "");
    if(settings->value("button8text").toString() == "")
        settings->setValue("button8text", "freier Button 3");
    if(settings->value("button8cmd").toString() == "")
        settings->setValue("button8cmd", "");
    if(settings->value("button9text").toString() == "")
        settings->setValue("button9text", "freier Button 4");
    if(settings->value("button9cmd").toString() == "")
        settings->setValue("button9cmd", "");
    if(settings->value("button10text").toString() == "")
        settings->setValue("button10text", "freier Button 5");
    if(settings->value("button10cmd").toString() == "")
        settings->setValue("button10cmd", "");
    settings->endGroup();

    delete settings;
}
