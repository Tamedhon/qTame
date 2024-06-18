#ifndef QTEXTCONSOLE_H
#define QTEXTCONSOLE_H

#include <QKeyEvent>
#include <QRegularExpression>
#include <QTextBrowser>


class QTextConsole : public QTextBrowser
{

public:
    QTextConsole(QWidget *parent = nullptr) : QTextBrowser(parent) {}
    ~QTextConsole(){}
    void insertAnsiText(const QString &text);

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

    void contextMenuEvent(QContextMenuEvent *event) override
    {
        event->ignore();
    }

private:
    void applyAnsiCodes(QTextCursor &cursor, const QStringList &codes);
    bool bright = false;
};

#endif
