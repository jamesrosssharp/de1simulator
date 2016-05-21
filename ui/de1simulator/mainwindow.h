#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QCloseEvent>
#include "hexdisplaylabel.h"

class QLabel;
class QCheckBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMemoryMap(void* map);
    void setRedLed(int LED, int on);
    void setGreenLed(int LED, int on);

public slots:
    void key1Pressed();
    void key2Pressed();
    void key3Pressed();
    void key4Pressed();
    void key1Released();
    void key2Released();
    void key3Released();
    void key4Released();

signals:
    void closingSignal();

protected:
     void timerEvent(QTimerEvent*) Q_DECL_OVERRIDE;
     void closeEvent(QCloseEvent* e) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;

    QLabel* redLeds[10];
    QLabel* greenLeds[8];
    QCheckBox* slideSwitches[10];
    HexDisplayLabel* hexDisplays[4];

    int m_timerId;
    struct SharedMemoryLayout* m_shm;
};

#endif // MAINWINDOW_H
