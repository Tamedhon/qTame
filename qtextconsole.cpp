#include "qtextconsole.h"

void QTextConsole::insertAnsiText(const QString &text)
{
    QTextCursor cursor = textCursor();
    QRegularExpression re("\x1b\\[([0-9;]+)m");
    QRegularExpressionMatch match;
    int lastPos = 0;
    auto it = re.globalMatch(text);

    while (it.hasNext())
    {
        match = it.next();
        QString before = text.mid(lastPos, match.capturedStart() - lastPos);
        QString ansiCodes = match.captured(1);

        cursor.insertText(before);
        applyAnsiCodes(cursor, ansiCodes.split(';'));

        lastPos = match.capturedEnd();
    }
    cursor.insertText(text.mid(lastPos));
}

void QTextConsole::applyAnsiCodes(QTextCursor &cursor, const QStringList &codes)
{
    QTextCharFormat format = cursor.charFormat();
    for (const QString &code : codes)
    {
        bool ok;
        int n = code.toInt(&ok);
        if (!ok) continue;

        switch (n)
        {
        case 0:
            //            format = QTextCharFormat(); // Reset
            format.clearBackground();
            format.clearForeground();
            format.setFontWeight(QFont::Normal);
            format.setFontUnderline(false);
            bright = false;
            break;
        case 1:
            //            format.setFontWeight(QFont::Bold);
            //            format.setForeground(QColor(255, 255, 255)); //hellweiß (richtiges weiß)
            bright = true;
            break;
        case 22:
            bright = false; // Normal intensity
            break;
        case 4:
            format.setFontUnderline(true);
            break;
        case 24:
            format.setFontUnderline(false);
            break;
        case 30:
            format.setForeground(bright ? QColor(85,85,85) : QColor(0,0,0)); //grau (eher dunkelgrau) / schwarz
            break;
        case 31:
            format.setForeground(bright ? QColor(255,85,85) : QColor(187,0,0)); //hellrot / rot
            break;
        case 32:
            format.setForeground(bright ? QColor(85,255,85) : QColor(0,187,0)); //hellgrün / grün
            break;
        case 33:
            format.setForeground(bright ? QColor(255,255,85) : QColor(187,187,0)); //hellgelb(ingame gaelb) / gelb (ingame braun)
            break;
        case 34:
            format.setForeground(bright ? QColor(85,85,255) : QColor(0,0,187)); //hellblau / blau
            break;
        case 35:
            format.setForeground(bright ? QColor(255,85,255) : QColor(187,0,187)); //hellmagenta(ingame helllila) / magenta(ingame lila)
            break;
        case 36:
            format.setForeground(bright ? QColor(85,255,255) : QColor(0,187,187)); //hellcyan / cyan
            break;
        case 37:
            format.setForeground(bright ? QColor(255, 255, 255) : QColor(187,187,187)); //hellweiß (richtiges weiß) / weiß (eher hellgrau)
            break;
        case 40:
            format.setBackground(bright ? QColor(85,85,85) : QColor(0,0,0)); //grau (eher dunkelgrau) / schwarz
            break;
        case 41:
            format.setBackground(bright ? QColor(255,85,85) : QColor(187,0,0)); //hellrot / rot
            break;
        case 42:
            format.setBackground(bright ? QColor(85,255,85) : QColor(0,187,0)); //hellgrün / grün
            break;
        case 43:
            format.setBackground(bright ? QColor(255,255,85) : QColor(187,187,0)); //hellgelb(ingame gaelb) / gelb (ingame braun)
            break;
        case 44:
            format.setBackground(bright ? QColor(85,85,255) : QColor(0,0,187)); //hellblau / blau
            break;
        case 45:
            format.setBackground(bright ? QColor(255,85,255) : QColor(187,0,187)); //hellmagenta(ingame helllila) / magenta(ingame lila)
            break;
        case 46:
            format.setBackground(bright ? QColor(85,255,255) : QColor(0,187,187)); //hellcyan / cyan
            break;
        case 47:
            format.setBackground(bright ? QColor(255, 255, 255) : QColor(187,187,187)); //hellweiß (richtiges weiß) / weiß (eher hellgrau)
            break;
        }
    }
    cursor.setCharFormat(format);
}
