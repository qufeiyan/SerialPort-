#include "TextDispView.h"

TextDispView::TextDispView(QTextBrowser *text,QLCDNumber *lcd,QList<QRadioButton*> rbt_Disps)
{
    this->textBrowser = text;
    this->lcd_Receive = lcd;
    textBrowser->setTextColor(Qt::darkCyan);
    this->rbt_TextDisp = rbt_Disps[0];
    this->rbt_HexDisp = rbt_Disps[1];
}

void TextDispView::DispData(QByteArray bytes)
{
    QString buf;
    if(!bytes.isEmpty()){
        if(rbt_TextDisp ->isChecked()){
            buf = bytes;
        }else if(rbt_HexDisp->isChecked()){
            for(int i = 0; i < bytes.count(); i++){
                QString s;
                s.sprintf("0x%02x, ", (unsigned char)bytes.at(i));
                buf += s;
            }
        }
        textBrowser->setText(textBrowser->document()->toPlainText() + buf);
        textBrowser->moveCursor(QTextCursor::End);
       // statusBar->showMessage(tr("成功读取%1字节数据").arg(bytes.size()));
    }
    lcd_Receive->display(lcd_Receive->value() + bytes.size());
}

