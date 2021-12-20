#include "encryption.h"
#include "ui_encryption.h"
#include "menu.h"
#include <includes/base64.h>
#include <gmpxx.h>

#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <QFileDialog>
#include <QMessageBox>
#include <bitset>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/pem.h>

static int BLOCK_SIZE = 256; // Size of the blocks to be used in encryption.
static int SIZE_OF_CHAR = 8; // Number of bits that are taken up by a character.

std::string encryptedString = ""; // The string which has been encrypted using RSA public key.
std::string b64EncryptedString = ""; // The RSA encrypted string, base64 encoded, ready to be written to a file.

std::string inputFilepath = ""; // The filepath of the plain-text file (which will have it's contents encrypted).
std::string publicKeyFilepath = ""; // The filepath of the public key.
std::string outputEncryptedFilepath = ""; // The filepath which the base64 encrypted file will be saved.

bool inputFileSelected = false; // A flag which indicates if the user has selected the plaintext file to encrypt.
bool publicKeySelected = false; // A flag which indicates if the user has selected the public key.
bool outputEncryptedFilepathSelected = false; // A flag which indicates if the user has selected an output filepath.



Encryption::Encryption(QWidget *parent):QMainWindow(parent), ui(new Ui::Encryption){
/***********************************************************************
* Constructor for the Encryption Window,
* Gets called when the window is initialised, sets up the user interface
* and calls the setup function.
***********************************************************************/
    ui->setupUi(this);
    Encryption::setup();
}

Encryption::~Encryption(){
/***********************************************************************
* Destructor for the Encryption Window,
* Gets called automatically by QT when the UI window is closed.
***********************************************************************/
    delete ui;
}

void Encryption::setup(){
/***********************************************************************
* The setup function which does the following:
* - Sets the keyLabel to false as no filepath selected.
* - Sets the filepathLabel to false as no filepath selected.
* - Sets the outputFilepathLabel to false as no filepath selected.
* - Adds the homepage action button to the toolbar.
* - Connects all of the buttons to respective functions.
***********************************************************************/
    Encryption::setKeyLabel(false);
    Encryption::setFilepathLabel(false);
    Encryption::setOutputFilepathLabel(false);
    Encryption::addHomeButtonToToolbar();
    Encryption::connectButtons();
}

void Encryption::loadMenu(){
/***********************************************************************
* Closes the current window widget and loads the menu.
***********************************************************************/
    Menu *w = new Menu;
    this->close();
    w->show();
}

void Encryption::addHomeButtonToToolbar(){
/***********************************************************************
* - Creates a new action for the toolbar.
* - Assigns the action an icon of home.png and text.
* - Creates a shortcut so the menu can be opened via Ctrl+M
* - Adds the action to the toolbar
* - Connects the action icon to the function loadMenu()
***********************************************************************/
    QAction *homeActionButton = new QAction("Home", this);
    homeActionButton->setText("Go To The Main Menu");
    homeActionButton->setIcon(QIcon(":/icons/home.png"));
    homeActionButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    ui->toolBar->addAction(homeActionButton);
    connect(homeActionButton, &QAction::triggered, this, &Encryption::loadMenu);
}

void Encryption::connectButtons(){
/***********************************************************************
* Connects each button to their respective functions:
* - PublicKeyButton connected to the selectPublicKey function
* - FileToEncryptButton connected to the selectFileToEncrypt function
* - OutputButton connected to the selectOutputFilepath function
* - GoButton connected to the goButton function
***********************************************************************/
    connect(ui->PublicKeyButton, &QPushButton::released, this, &Encryption::selectPublicKey);
    connect(ui->FileToEncryptButton, &QPushButton::released, this, &Encryption::selectFileToEncrypt);
    connect(ui->OutputButton, &QPushButton::released, this, &Encryption::selectOutputFilepath);
    connect(ui->GoButton, &QPushButton::released, this, &Encryption::goButton);

}

bool Encryption::checkUserInput(){
/***********************************************************************
* Checks that all the required information has been provided.
* Returns:
*  True: If the user has entered all of the required information (i.e. pass)
*  False: If the user has not entered all of the required information (i.e. fail)
***********************************************************************/
    if(publicKeySelected == false || outputEncryptedFilepathSelected == false){
        return false;
    }
    if(inputFileSelected == false && ui->InputTextBox->toPlainText().toStdString() == ""){
        return false;
    }
    return true;
}

publicKey Encryption::initializePublicKey(){
/***********************************************************************
* A function which creates a publicKey stucture, and initializes each
* multiprecision variable.
*
* Returns:
* @ publicKeyStruct: A publicKey structure, named publicKeyStruct, that has been initialised.
***********************************************************************/
    publicKey publicKeyStruct;
    mpz_init(publicKeyStruct.modulus);
    mpz_init(publicKeyStruct.publicExponent);
    return publicKeyStruct;
}

void Encryption::selectPublicKey(){
/***********************************************************************
* Opens a file browser for the user to select the PublicKey.pem file
* If no file is selected the global variable privateKeyFilepath is reset
* and privateKeySelected bool is set to false.
* If a file is selected then the variable gets set to the path of the file
* and the privateKeySelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::ExistingFile);
    fileBrowser.setNameFilter("*.pem");
    fileBrowser.setWindowTitle(QObject::tr("Open Public Key..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Encryption::outputErrorMessage("Error!", "ERROR: Please select a valid public key!");
        publicKeyFilepath = "";
        publicKeySelected = false;
        Encryption::setKeyLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        publicKeyFilepath = FileLocation.join("").toStdString();
        publicKeySelected = true;
        Encryption::setKeyLabel(true);
    }
}

void Encryption::setKeyLabel(bool keySelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "keySelected"
* whose value is decided whether a valid key filepath has been selected.
*
* Arguments:
* @ keySelected: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->PublicKeyLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->PublicKeyLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* publicKeyLabel = ui->PublicKeyLabel;
    if(keySelected == true){
        publicKeyLabel->setPixmap(tick);
        publicKeyLabel->setMask(tick.mask());
        publicKeyLabel->show();
    }
    else{
        publicKeyLabel->setPixmap(cross);
        publicKeyLabel->setMask(cross.mask());
        publicKeyLabel->show();
    }
}

void Encryption::selectFileToEncrypt(){
/***********************************************************************
* Opens a file browser for the user to select the file to Encrypt.
* If no file is selected the global variable encryptedFilepath is reset
* and encryptedFileSelected is set to false.
* If a file is selected then the variable gets set to the path of the file
* and the encryptedFileSelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::ExistingFile);
    fileBrowser.setNameFilter("*.txt");
    fileBrowser.setWindowTitle(QObject::tr("Open File To Encrypt..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Encryption::outputErrorMessage("Error!", "ERROR: Please select a valid file to encrypt!");
        inputFilepath= "";
        inputFileSelected = false;
        Encryption::setFilepathLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        inputFilepath = FileLocation.join("").toStdString();
        inputFileSelected = true;
        Encryption::setFilepathLabel(true);
    }
}

void Encryption::setFilepathLabel(bool filepathSelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "filepathSelected"
* whose value is decided whether a valid key filepath has been selected.
*
* Arguments:
* @ filepathSelected: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->FileToEncryptLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->FileToEncryptLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* FileToEncryptLabel = ui->FileToEncryptLabel;
    if(filepathSelected == true){
        FileToEncryptLabel->setPixmap(tick);
        FileToEncryptLabel->setMask(tick.mask());
        FileToEncryptLabel->show();
    }
    else{
        FileToEncryptLabel->setPixmap(cross);
        FileToEncryptLabel->setMask(cross.mask());
        FileToEncryptLabel->show();
    }
}

void Encryption::selectOutputFilepath(){
/***********************************************************************
* Opens a file browser for the user to select the output encrypted file location.
* If no file is selected the global variable outputEncryptedFilepath is reset
* and outputEncryptedFilepathSelected is set to false.
* If a file is selected then the variable gets set to the path of the file location
* and the outputEncryptedFilepathSelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::AnyFile);
    fileBrowser.setNameFilter("*.txt");
    fileBrowser.setWindowTitle(QObject::tr("Save Encyrpted File..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Encryption::outputErrorMessage("Error!", "ERROR: Please select a valid path for the Encrypted file to be saved!");
        outputEncryptedFilepath = "";
        outputEncryptedFilepathSelected = false;
        Encryption::setOutputFilepathLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        outputEncryptedFilepath = FileLocation.join("").toStdString() + ".txt";
        outputEncryptedFilepathSelected = true;
        Encryption::setOutputFilepathLabel(true);
    }
}

void Encryption::setOutputFilepathLabel(bool outputFilepathSelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "outputFilepathSelected"
* whose value is decided whether a valid key filepath has been selected.
*
* Arguments:
* @ outputFilepathSelected: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->OutputFilepathLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->OutputFilepathLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* outputFilepathLabel = ui->OutputFilepathLabel;
    if(outputFilepathSelected == true){
        outputFilepathLabel->setPixmap(tick);
        outputFilepathLabel->setMask(tick.mask());
        outputFilepathLabel->show();
    }
    else{
        outputFilepathLabel->setPixmap(cross);
        outputFilepathLabel->setMask(cross.mask());
        outputFilepathLabel->show();
    }
}

std::string Encryption::readFromFile(){
/***********************************************************************
* This function reads all of the text from a file, using a buffer stream.
* No validation checks are needed here as they are all done by another function.
*
* Returns:
*  strStream.str(): the text content from the file returned as a string.
***********************************************************************/
    std::ifstream inFile;
    inFile.open(inputFilepath);
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    return strStream.str();
}

publicKey Encryption::loadPublicKey(publicKey publicKeyStruct){
/***********************************************************************
* A function which loads the public key from the .pem file the user has selected
* and then assigns the publicExponent and modulus values to our publicKey structure.
*
* Arguments:
* @ publicKeyStruct: The initialised, but not-yet assigned privateKey structure.
*
* Returns:
*  privateKeyStruct: The privateKey structure once the values have been assigned from the .pem file.
***********************************************************************/
    CryptoPP::FileSource publicKeySource(publicKeyFilepath.c_str(), true);
    CryptoPP::RSA::PublicKey cryptoPublicKey;
    CryptoPP::PEM_Load(publicKeySource, cryptoPublicKey);

    CryptoPP::Integer cryptoPublicKeyExponent = cryptoPublicKey.GetPublicExponent();
    CryptoPP::Integer cryptoPublicKeyModulus = cryptoPublicKey.GetModulus();

    std::ostringstream exponentStream;
    std::ostringstream modulusStream;

    exponentStream << cryptoPublicKeyExponent;
    modulusStream << cryptoPublicKeyModulus;
    std::string exponentString = exponentStream.str();
    std::string modulusString = modulusStream.str();

    exponentString.pop_back();
    modulusString.pop_back();

    mpz_set_str(publicKeyStruct.publicExponent, exponentString.c_str(), 10);
    mpz_set_str(publicKeyStruct.modulus, modulusString.c_str(), 10);

    return publicKeyStruct;
}

void Encryption::goButton(){
/***********************************************************************
* This function is run when the go button is clicked by the user.
* It essentially calls the other functions in the correct order, with some
* validation checks along the way
* A success message is output after encryption has been completed.
* All the filepaths are reset so the program can be run again.
***********************************************************************/
    if(Encryption::checkUserInput() == false){
        Encryption::outputErrorMessage("Error!", "ERROR: Please check all input fields and try again!");
        return;
    }
    publicKey publicKeyStruct = Encryption::initializePublicKey();
    publicKeyStruct = Encryption::loadPublicKey(publicKeyStruct);
    if(inputFileSelected == true){
        std::string stringFromFile = Encryption::readFromFile();
        Encryption::encryptString(stringFromFile, publicKeyStruct);
        b64EncryptedString = macaron::Base64::Encode(encryptedString);
    }
    else if(ui->InputTextBox->toPlainText().toStdString() != ""){
        Encryption::encryptString(ui->InputTextBox->toPlainText().toStdString(), publicKeyStruct);
        b64EncryptedString = macaron::Base64::Encode(encryptedString);
    }
    else{
        Encryption::outputErrorMessage("Error!", "ERROR: Please check all input fields and try again!");
        return;
    }
    Encryption::writeEncryptedTextToFile();
    Encryption::outputSuccessMessage("Success!", "File encrypted and written to filepath successfully!");
    Encryption::resetWindow();
}

void Encryption::encryptString(std::string stringToEncrypt, publicKey publicKeyStruct){
/***********************************************************************
* A function which iterates through the plaintext string which will be encrypted.
* Splits the string into blocks of size BLOCK_SIZE / SIZE_OF_CHAR (by default 32 characters).
* The current block in each iteration gets passed into the encryptBlock() function.
* The if statement handles the padding of the last block, to ensure it meets the block size.
*
* Arguments:
*  @ stringToEncrypt: The string, read from the inputted file, which will be encrypted.
*  @ publicKeyStruct: The structure which contains the values needed for encryption.
***********************************************************************/
    std::string currentBlock = "";
    while(stringToEncrypt.length() >= static_cast<unsigned long long>((BLOCK_SIZE / SIZE_OF_CHAR))){
        currentBlock = stringToEncrypt.substr(0, ((BLOCK_SIZE / SIZE_OF_CHAR) - 1));
        stringToEncrypt.erase(0, (BLOCK_SIZE / SIZE_OF_CHAR));
        encryptBlock(currentBlock, publicKeyStruct);
    }
    if(stringToEncrypt.length() != 0){
        //This if statement handles the padding, by calculating how much padding is required and adding that many spaces.
        int paddingRequired = (BLOCK_SIZE / SIZE_OF_CHAR) - stringToEncrypt.length();
        for(int i = 0; i < paddingRequired; i++){
            stringToEncrypt += " ";
        }
        encryptBlock(stringToEncrypt, publicKeyStruct);
    }
}

void Encryption::encryptBlock(std::string blockToEncrypt, publicKey publicKeyStruct){
/***********************************************************************
* This function is called iteratively by encryptString, it encrypts the current block
* which gets passed in the variable "blockToEncrypt".
* This function assumes padding has already been applied to the blockToEncrypt string
* Once successfully Encrypted, the encrypted block is concatenated onto the global variable encryptedString.
* After the encrypted block has been appended to the encryptedString, a delimiter ('/')
* is used to signify the end of a block (helpful when needing to decrypt).
*
* Arguments:
*  @ blockToEncrypt: The current block, passed from encryptString function, which needs to be decoded.
*  @ publicKeyStruct: The structure which contains the values needed for encryption.
***********************************************************************/
    std::string binaryString = "";
    for (int charCounter = 0; static_cast<unsigned long long>(charCounter) < blockToEncrypt.size(); charCounter++){
        binaryString += std::bitset<8>(blockToEncrypt[charCounter]).to_string();
    }
    mpz_t valueToEncrypt; mpz_init(valueToEncrypt);
    mpz_t outputValue; mpz_init(outputValue);

    mpz_set_str(valueToEncrypt, binaryString.c_str(), 2);
    mpz_powm(outputValue, valueToEncrypt, publicKeyStruct.publicExponent, publicKeyStruct.modulus);

    std::string encryptedBlockAsString = mpz_get_str(NULL, 10, outputValue);
    encryptedString += encryptedBlockAsString + "/";
}

void Encryption::writeEncryptedTextToFile(){
/***********************************************************************
* A function which writes the global variable b64EncryptedString into a file
* at the location of the contents of global variable outputEncryptedFilepath.
***********************************************************************/
    std::ofstream outputFileStream(outputEncryptedFilepath);
    outputFileStream << b64EncryptedString;
    outputFileStream.close();
}

void Encryption::outputErrorMessage(std::string windowHeader, std::string messageContent){
/***********************************************************************
* A function which handles the error outputting.
*
* Arguments:
* @ windowHeader: The title of the window of the error box.
* @ messageContent: The message which will be shown in the error message box.
***********************************************************************/
    QMessageBox messageBox;
    messageBox.critical(0,QString::fromStdString(windowHeader),QString::fromStdString(messageContent));
    messageBox.setFixedSize(500,200);
}

void Encryption::outputSuccessMessage(std::string windowHeader, std::string messageContent){
/***********************************************************************
* A function which outputs a success message to the user.
*
* Arguments:
* @ windowHeader: The title of the window of the success box.
* @ messageContent: The message which will be shown in the success box.
***********************************************************************/
    QMessageBox messageBox;
    messageBox.setWindowTitle(QString::fromStdString(windowHeader));
    messageBox.setText(QString::fromStdString(messageContent));
    messageBox.setFixedSize(500,200);
    messageBox.exec();
}

void Encryption::resetWindow(){
/***********************************************************************
* Resets all of the global variables, flags and label images to their default values.
***********************************************************************/
    encryptedString = "";
    b64EncryptedString = "";
    inputFilepath = "";
    publicKeyFilepath = "";
    outputEncryptedFilepath = "";
    inputFileSelected = false;
    publicKeySelected = false;
    outputEncryptedFilepathSelected = false;
    Encryption::setKeyLabel(false);
    Encryption::setFilepathLabel(false);
    Encryption::setOutputFilepathLabel(false);
    return;
}


