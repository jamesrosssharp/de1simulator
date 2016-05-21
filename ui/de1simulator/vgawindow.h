#ifndef VGAWINDOW_H
#define VGAWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "sharedmem.h"

class VGAWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VGAWindow(QWidget *parent = 0);

    void setMemoryMap(void* map);

signals:

protected:
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

public slots:

signals:
    void closingSignal();

private:
     struct SharedMemoryLayout* m_shm;
     int m_timerId;
};

#endif // VGAWINDOW_H
