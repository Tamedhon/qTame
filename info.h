#ifndef INFO_H
#define INFO_H

#include <QDialog>
#include <QKeyEvent>
#include <QSettings>
#include "enum.h"

using namespace Enums;

namespace Ui {
class Info;
}

class Info : public QDialog
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();

private:
    Ui::Info *ui;
    QSettings *settings;

    void SetMode();
    void SetColors(QString foreground, QString background);

    void showEvent( QShowEvent* event ) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void btnClose();
};

#endif // INFO_H
