#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sharedmem.h"
#include "hexdisplaylabel.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_shm(NULL)
{
    ui->setupUi(this);

    m_timerId = startTimer(100);


    // loop and find handle to all red leds

    for (int i = 0; i < 10; i ++)
    {
        QString s;
        s.sprintf("REDLED%02d",i+1);

        redLeds[i] = findChild<QLabel*>(s);
    }

    // loop and find handle to all green leds

    for (int i = 0; i < 8; i ++)
    {
        QString s;
        s.sprintf("GREENLED%02d",i+1);

        greenLeds[i] = findChild<QLabel*>(s);
    }

    // loop and find handle to all slide switches

    for (int i = 0; i < 10; i++)
    {
        QString s;
        s.sprintf("SLIDESWITCH%02d",i+1);

        slideSwitches[i] = findChild<QCheckBox*>(s);
    }

    // create hex display widgets

    QHBoxLayout* box = findChild<QHBoxLayout*>("HEXDISPLAYBOX");

    for (int i = 0; i < 4; i ++){
        hexDisplays[i] = new HexDisplayLabel;
        box->addWidget(hexDisplays[i]);
    }
}

void MainWindow::setRedLed(int LED, int on)
{
    if (LED < 10 and LED >= 0)
    {
        if (on == 1)
            redLeds[LED]->setStyleSheet("QLabel { \
                    background: #000080 url(:/icons/LED-red.png) no-repeat center; \
                        border: none; \
                        height: 42px; \
                        width: 44px;  \
                    }");
        else
            redLeds[LED]->setStyleSheet("QLabel { \
                    background: #000080 url(:/icons/LED.png) no-repeat center; \
                        border: none; \
                        height: 42px; \
                        width: 44px;  \
                      }");
    }
}

void MainWindow::setGreenLed(int LED, int on)
{
    if (LED < 8 and LED >= 0)
    {
        if (on == 1)
            greenLeds[LED]->setStyleSheet("QLabel { \
                    background: #000080 url(:/icons/LED-green.png) no-repeat center; \
                        border: none; \
                        height: 42px; \
                        width: 44px;  \
                    }");
        else
            greenLeds[LED]->setStyleSheet("QLabel { \
                    background: #000080 url(:/icons/LED.png) no-repeat center; \
                        border: none; \
                        height: 42px; \
                        width: 44px;  \
                    }");
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {

        if (m_shm != NULL)
        {

            if (m_shm->requestUIExit == 1)
                close();

            for (int i = 0; i < 10; i++)
                setRedLed(i,m_shm->redLEDs[i]);

            for (int i = 0; i < 8; i++)
                setGreenLed(i,m_shm->greenLEDs[i]);

            for (int i = 0; i < 10; i++)
                if (slideSwitches[i]->isChecked())
                    m_shm->slideSwitches[i] = 1;
                else
                    m_shm->slideSwitches[i] = 0;

            for (int i = 0; i < 4; i ++)
            {
                for (int j = 0; j < 8; j ++)
                {
                    //printf("Display: %d Segment %d value %d\n", i, j, m_shm->sevenSegmentData[i][j]);
                    hexDisplays[i]->setSegment(j,m_shm->sevenSegmentData[i][j]);
                }
                hexDisplays[i]->repaint();
            }

        }
    }
}

void MainWindow::key1Pressed()
{
    if (m_shm != NULL)
    {
        m_shm->keys[0] = 1;
    }
}

void MainWindow::key2Pressed()
{
    if (m_shm != NULL)
    {
        m_shm->keys[1] = 1;
    }
}

void MainWindow::key3Pressed()
{
    if (m_shm != NULL)
    {
        m_shm->keys[2] = 1;
    }
}

void MainWindow::key4Pressed()
{
    if (m_shm != NULL)
    {
        m_shm->keys[3] = 1;
    }
}

void MainWindow::key1Released()
{
    if (m_shm != NULL)
    {
        m_shm->keys[0] = 0;
    }
}

void MainWindow::key2Released()
{
    if (m_shm != NULL)
    {
        m_shm->keys[1] = 0;
    }
}

void MainWindow::key3Released()
{
    if (m_shm != NULL)
    {
        m_shm->keys[2] = 0;
    }
}

void MainWindow::key4Released()
{
    if (m_shm != NULL)
    {
        m_shm->keys[3] = 0;
    }
}


void MainWindow::setMemoryMap(void* map)
{
    m_shm = (struct SharedMemoryLayout*)map;
}

MainWindow::~MainWindow()
{
    delete ui;
    for (int i = 0; i < 4; i++)
        delete hexDisplays[i];
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    //printf("Closing\n");
    emit closingSignal();

}
