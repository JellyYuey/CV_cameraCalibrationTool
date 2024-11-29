#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QObject>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(const QPixmap& img, QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *) override ;
    void leaveEvent(QEvent *) override;

signals:
    void sig_switch_img(const QPixmap&);
};

#endif // MYLABEL_H
