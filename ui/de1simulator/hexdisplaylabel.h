#ifndef HEXDISPLAYLABEL_H
#define HEXDISPLAYLABEL_H

#include <QWidget>
#include <QLabel>

class HexDisplayLabel : public QWidget
{
    Q_OBJECT
public:
    explicit HexDisplayLabel(QWidget *parent = 0);
    void    setSegment(int segment, int onoff);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:

private:
    int m_seg[8];
};

#endif // HEXDISPLAYLABEL_H
