#include "decryption.h"
#include "menu.h"
#include <QtPlugin>
#include <QApplication>

int main(int argc, char *argv[]){
/***********************************************************************
* Creates an instance of the Menu class, executes and shows the main window.
***********************************************************************/
    QApplication a(argc, argv);
    Menu w;
    w.show();
    return a.exec();
}
