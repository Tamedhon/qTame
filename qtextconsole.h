#include <QKeyEvent>
#include <QTextBrowser>

class QTextConsole : public QTextBrowser
{
public:
    QTextConsole(QWidget *parent = nullptr) : QTextBrowser(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        event->ignore();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        event->ignore();
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        event->ignore();
    }
};
