#include "mainwindow.h"
#include "vgawindow.h"
#include "sharedmem.h"
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


int main(int argc, char *argv[])
{

    int retval;
    int fd;
    const char *shmFname = SHAREDMEMORYFILE;
    void *map = NULL;
    struct SharedMemoryLayout* shm;

    // open shared memory file

    fd = open(shmFname, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening file for reading");

    }
    else
    {

        map = mmap(0, SHAREDMEMORYSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED)
        {
            close(fd);
            perror("Error mmapping the file");
            exit(EXIT_FAILURE);
        }

    }

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("DE1 Simulator");
    w.show();

    VGAWindow v;
    v.show();

    // Connect up signals so that closing any window exits simulation
    QObject::connect(&w, SIGNAL(closingSignal()),&v, SLOT(close()));
    QObject::connect(&v, SIGNAL(closingSignal()),&w, SLOT(close()));

    w.setMemoryMap(map);
    v.setMemoryMap(map);



    retval = a.exec();

    // post exit signal

    printf("Exiting UI...\n");
    fflush(0);

    if (map != NULL)
    {
        shm = (struct SharedMemoryLayout*)map;
        shm->uiExited = 1;

        // munmap

        if (munmap(map, SHAREDMEMORYSIZE) == -1) {
            perror("Error un-mmapping the file");
        }
    }

    // exit
    return retval;

}
