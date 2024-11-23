#include "mylabel.h"

#include <QMouseEvent>

MyLabel::MyLabel(const QPixmap &img, QWidget *parent)
    :QLabel(parent)
{
    QSize size(140, 140);
    setMaximumSize(size);
    setMinimumSize(size);
    setScaledContents(true);
    setPixmap(img);
}

QSize MyLabel::sizeHint() const
{
    return QSize(140, 140);
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit sig_switch_img(pixmap());
        qDebug("1");
    }
    QLabel::mousePressEvent(event);
}

void MyLabel::enterEvent(QEnterEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QLabel::enterEvent(event);
}

void MyLabel::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QLabel::leaveEvent(event);
}
