#include "menu.h"
#include "ui_menu.h"
#include "decryption.h"
#include "encryption.h"
#include "keygeneration.h"
#include <iostream>

Menu::Menu(QWidget *parent) : QMainWindow(parent), ui(new Ui::Menu){
/***********************************************************************
* Constructor for the menu window, called when the program is first ran,
* responsible for setting up the user interface and calling connectButtons()
***********************************************************************/
    ui->setupUi(this);
    Menu::connectButtons();
}

Menu::~Menu(){
/***********************************************************************
* Destructor for the menu window, which is called when the window is closed,
* responsible for freeing the ui from the memory.
***********************************************************************/
    delete ui;
}

void Menu::connectButtons(){
/***********************************************************************
* Connects all of the buttons on the homepage to respective functions.
***********************************************************************/
    connect(ui->GenerateKeysButton, &QPushButton::released, this, &Menu::openGenerateKeysWindow);
    connect(ui->EncryptionWindowButton, &QPushButton::released, this, &Menu::openEncryptionWindow);
    connect(ui->DecryptionWindowButton, &QPushButton::released, this, &Menu::openDecryptionWindow);
}

void Menu::openGenerateKeysWindow(){
/***********************************************************************
* Opens the key generation window when button is pressed.
* Also closes the current window.
***********************************************************************/
    KeyGeneration *keyGenerationWindow = new KeyGeneration();
    keyGenerationWindow->show();
    this->close();
}

void Menu::openEncryptionWindow(){
/***********************************************************************
* Opens the Encryption window when the button is pressed.
* Also closes the current window.
***********************************************************************/
    Encryption *encryptionWindow = new Encryption();
    encryptionWindow->show();
    this->close();
}

void Menu::openDecryptionWindow(){
/***********************************************************************
* Opens the Decryption window when the button is pressed.
* Also closes the current window.
***********************************************************************/
    Decryption *decryptionWindow = new Decryption();
    decryptionWindow->show();
    this->close();
}
