#ifndef TEXTDISPVIEW_H
#define TEXTDISPVIEW_H

#include <QObject>
#include <QTextBrowser>
#include <QRadioButton>
#include <QLCDNumber>

class TextDispView : public QObject
{
    Q_OBJECT
public:
    TextDispView(QTextBrowser *,QLCDNumber *,QList<QRadioButton*>);
public slots:

    void DispData(QByteArray);

private:
    QTextBrowser *textBrowser;

    QRadioButton *rbt_TextDisp;
    QRadioButton *rbt_HexDisp;

    QLCDNumber *lcd_Receive;

};

#endif // TEXTDISPVIEW_H
