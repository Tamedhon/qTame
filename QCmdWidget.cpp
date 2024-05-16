#include "QCmdWidget.h"
#include <QLineEdit>

QCmdWidget::QCmdWidget(QWidget *papi) : QComboBox(papi)
{
    installEventFilter(this);
}

/**
 * @brief QCmdWidget::setHistorial
 * Adds historial commands to combo box.
 * @param cmds Command list.
 */
void QCmdWidget::setHistorial(const QStringList &cmds)
{
    addItems(cmds);
}

/**
 * @brief QCmdWidget::getHistorial
 * To retrieve all commands typed in command.
 * @return List of commands.
 */
QStringList QCmdWidget::getHistorial() const
{
    QStringList rtn;
    for( int i = 0; i < count(); i++ )
        rtn += itemText(1);
    return rtn;
}

/**
 * @brief QCmdWidget::keyPressEvent
 * override parent function to catch enter/return key to
 * signal the command typed.
 * @param e the key event.
 */
void QCmdWidget::keyPressEvent(QKeyEvent *e)
{
    if( ((e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter)))
    {
        if(!currentText().isEmpty())
        {
            addItem(currentText());
            lineEdit()->selectAll();
        }
        //clearEditText();
        emit command(currentText());
        clearEditText();
    }
    else
        QComboBox::keyPressEvent(e);
}

bool QCmdWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if ((keyEvent->modifiers() & Qt::KeypadModifier) && (keyEvent->modifiers() & Qt::AltModifier))
        {
            emit numPadKeyEvent(keyEvent);
            return true;
        }
        else if ((keyEvent->modifiers() & Qt::KeypadModifier) && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            emit numPadKeyEvent(keyEvent);
            return true;
        }
        else if ((keyEvent->modifiers() & Qt::KeypadModifier) && !(keyEvent->modifiers() & Qt::ControlModifier) && !(keyEvent->modifiers() & Qt::AltModifier))
        {
            emit numPadKeyEvent(keyEvent);
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

