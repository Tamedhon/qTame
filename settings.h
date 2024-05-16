#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QKeyEvent>
#include <QSettings>
#include "crypto.h"
#include "enum.h"

using namespace Enums;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    QSettings *settings;
    Crypto::Crypto *crypt;

    void Connects();
    void LoadSettings();

    void SetMode();
    void SetColors(QString foreground, QString background);

    void showEvent( QShowEvent* event ) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void btnClose();

    void UserChanged(QString txt);
    void PasswordChanged(QString txt);

    void Button1TextChanged(QString txt);
    void Button1CMDChanged(QString txt);
    void Button2TextChanged(QString txt);
    void Button2CMDChanged(QString txt);
    void Button3TextChanged(QString txt);
    void Button3CMDChanged(QString txt);
    void Button4TextChanged(QString txt);
    void Button4CMDChanged(QString txt);
    void Button5TextChanged(QString txt);
    void Button5CMDChanged(QString txt);
    void Button6TextChanged(QString txt);
    void Button6CMDChanged(QString txt);
    void Button7TextChanged(QString txt);
    void Button7CMDChanged(QString txt);
    void Button8TextChanged(QString txt);
    void Button8CMDChanged(QString txt);
    void Button9TextChanged(QString txt);
    void Button9CMDChanged(QString txt);
    void Button10TextChanged(QString txt);
    void Button10CMDChanged(QString txt);

    void ThemeChanged(int idx);
};

#endif // SETTINGS_H
