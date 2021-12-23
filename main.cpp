#include "decryption.h"
#include "menu.h"
#include <QtPlugin>
#include <QApplication>

int main(int argc, char *argv[]){
/***********************************************************************
* Creates an instance of the Menu class, executes and shows the main window.
***********************************************************************/
    QApplication application(argc, argv);
    Menu menuWindow;
    menuWindow.show();
    return application.exec();
}
