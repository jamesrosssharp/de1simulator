#include "vgawindow.h"

VGAWindow::VGAWindow(QWidget *parent) :
    QMainWindow(parent),
    m_shm(NULL)
{
    resize(640,480);
    setWindowTitle("VGA output");

    m_timerId = startTimer(100);

}

void VGAWindow::setMemoryMap(void* map)
{
    m_shm = (struct SharedMemoryLayout*)map;
}

void VGAWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {

        if (m_shm != NULL)
        {
            // poll the

            repaint();

        }

    }
}

void VGAWindow::closeEvent(QCloseEvent* e)
{
    //printf("Closing\n");
    emit closingSignal();

}

void VGAWindow::paintEvent(QPaintEvent *e)
{
    // create a QImage from the shared memory buffer

    if(m_shm != NULL)
    {
        QImage im = QImage((uchar*)m_shm->VGAData,640,480,QImage::Format_RGB32);

        QPainter painter(this);

        painter.drawImage(0,0,im);
    }
}
