#include "decryption.h"
#include "ui_decryption.h"
#include "menu.h"
#include <gmpxx.h>
#include <includes/base64.h>

#include <iostream>
#include <ctime>
#include <cstring>
#include <string>
#include <QFileDialog>
#include <bitset>
#include <QMessageBox>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/pem.h>

static int SIZE_OF_CHAR = 8; // Number of bits that are taken up by a character.

std::string decryptedString = ""; // The string which will be written to the file at the end of execution.

std::string privateKeyFilepath = ""; // The filepath of the private key.
std::string encryptedFilepath = ""; // The filepath of the encrypted file (which will be decrypted).
std::string outputFilepath = ""; // The filepath which the decrypted, plain-text file will be saved.

bool privateKeySelected = false; // A flag which indicates if the user has selected the private key.
bool encryptedFileSelected = false; // A flag which indicates if the user has selected the encrypted file.
bool outputFilepathSelected = false; // A flag which indicates if the user has selected an output filepath.

Decryption::Decryption(QWidget *parent):QMainWindow(parent), ui(new Ui::Decryption){
/***********************************************************************
* Constructor for the Decryption Window,
* Gets called when the window is initialised, sets up the user interface
* and calls the setup function.
***********************************************************************/
    ui->setupUi(this);
    setup();
}

Decryption::~Decryption()
/***********************************************************************
* Destructor for the Decryption Window,
* Gets called automatically by QT when the UI window is closed.
***********************************************************************/
{
    delete ui;
}

void Decryption::setup(){
/***********************************************************************
* The setup function which does the following:
* - Sets the keyLabel to false as no filepath selected.
* - Sets the filepathLabel to false as no filepath selected.
* - Sets the outputFilepathLabel to false as no filepath selected.
* - Adds the homepage action button to the toolbar.
* - Connects all of the buttons to respective functions.
* - Initializes the private key.
***********************************************************************/
    Decryption::setKeyLabel(false);
    Decryption::setFilepathLabel(false);
    Decryption::setOutputFilepathLabel(false);
    Decryption::addHomeButtonToToolbar();
    Decryption::connectButtons();
    Decryption::initializePrivateKey();
}

void Decryption::loadMenu(){
/***********************************************************************
* Closes the current window widget and loads the menu.
***********************************************************************/
    Menu *w = new Menu;
    this->close();
    w->show();
}

void Decryption::addHomeButtonToToolbar(){
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
    connect(homeActionButton, &QAction::triggered, this, &Decryption::loadMenu);
}

void Decryption::connectButtons(){
/***********************************************************************
* Connects each button to their respective functions:
* - PrivateKeyButton connected to the selectPrivateKey function
* - FileToDecryptButton connected to the selectFileToDecrypt function
* - OutputButton connected to the selectOutputFilepath function
* - GoButton connected to the goButton function
***********************************************************************/
    connect(ui->PrivateKeyButton, &QPushButton::released, this, &Decryption::selectPrivateKey);
    connect(ui->FileToDecryptButton, &QPushButton::released, this, &Decryption::selectFileToDecrypt);
    connect(ui->OutputButton, &QPushButton::released, this, &Decryption::selectOutputFilepath);
    connect(ui->GoButton, &QPushButton::released, this, &Decryption::goButton);
}

bool Decryption::checkUserInput(){
/***********************************************************************
* Checks that all the required information has been provided.
* Returns:
*  True: If the user has entered all of the required information (i.e. pass)
*  False: If the user has not entered all of the required information (i.e. fail)
***********************************************************************/
    if(privateKeySelected == false || encryptedFileSelected == false || outputFilepathSelected == false){
        return false;
    }
    return true;
}

privateKey Decryption::initializePrivateKey(){
/***********************************************************************
* A function which creates a privateKey stucture, and initializes each
* multiprecision variable.
*
* Returns:
* @ privateKeyStruct: A privateKey structure, named privateKeyStruct, that has been initialised.
***********************************************************************/
    privateKey privateKeyStruct;
    mpz_init(privateKeyStruct.modulus);
    mpz_init(privateKeyStruct.publicExponent);
    mpz_init(privateKeyStruct.privateExponent);
    mpz_init(privateKeyStruct.prime1);
    mpz_init(privateKeyStruct.prime2);
    return privateKeyStruct;
}

void Decryption::selectPrivateKey(){
/***********************************************************************
* Opens a file browser for the user to select the PrivateKey.pem file
* If no file is selected the global variable privateKeyFilepath is reset
* and privateKeySelected bool is set to false.
* If a file is selected then the variable gets set to the path of the file
* and the privateKeySelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::ExistingFile);
    fileBrowser.setNameFilter("*.pem");
    fileBrowser.setWindowTitle(QObject::tr("Open Private Key..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Decryption::outputErrorMessage("Error!", "ERROR: Please select a valid private key!");
        privateKeyFilepath = "";
        privateKeySelected = false;
        Decryption::setKeyLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        privateKeyFilepath = FileLocation.join("").toStdString();
        privateKeySelected = true;
        Decryption::setKeyLabel(true);
    }
}

void Decryption::setKeyLabel(bool keySelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "keySelected"
* whose value is decided whether a valid key filepath has been selected.
*
* Arguments:
* @ keySelected: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->PrivateKeyLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->PrivateKeyLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* privateKeyLabel = ui->PrivateKeyLabel;
    if(keySelected == true){
        privateKeyLabel->setPixmap(tick);
        privateKeyLabel->setMask(tick.mask());
        privateKeyLabel->show();
    }
    else{
        privateKeyLabel->setPixmap(cross);
        privateKeyLabel->setMask(cross.mask());
        privateKeyLabel->show();
    }
}

void Decryption::selectFileToDecrypt(){
/***********************************************************************
* Opens a file browser for the user to select the file to decrypt.
* If no file is selected the global variable encryptedFilepath is reset
* and encryptedFileSelected is set to false.
* If a file is selected then the variable gets set to the path of the file
* and the encryptedFileSelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::ExistingFile);
    fileBrowser.setNameFilter("*.txt");
    fileBrowser.setWindowTitle(QObject::tr("Open File To Decrypt..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Decryption::outputErrorMessage("Error!", "ERROR: Please select a valid encrypted file!");
        encryptedFilepath = "";
        encryptedFileSelected = false;
        Decryption::setFilepathLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        encryptedFilepath = FileLocation.join("").toStdString();
        encryptedFileSelected = true;
        Decryption::setFilepathLabel(true);
    }
}

void Decryption::setFilepathLabel(bool filepathSelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "filepathSelected"
* whose value is decided whether a valid file to decrypt has been selected.
*
* Arguments:
* @ filepathSelected: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->FileToDecryptLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->FileToDecryptLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* FileToDecryptLabel = ui->FileToDecryptLabel;
    if(filepathSelected == true){
        FileToDecryptLabel->setPixmap(tick);
        FileToDecryptLabel->setMask(tick.mask());
        FileToDecryptLabel->show();
    }
    else{
        FileToDecryptLabel->setPixmap(cross);
        FileToDecryptLabel->setMask(cross.mask());
        FileToDecryptLabel->show();
    }
}

void Decryption::selectOutputFilepath(){
/***********************************************************************
* Opens a file browser for the user to select the output decrypted file location.
* If no file is selected the global variable outputFilepath is reset
* and outputFilepathSelected is set to false.
* If a file is selected then the variable gets set to the path of the file location
* and the outputFilepathSelected bool is set to true.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::AnyFile);
    fileBrowser.setNameFilter("*.txt");
    fileBrowser.setWindowTitle(QObject::tr("Save Decrypted File..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        Decryption::outputErrorMessage("Error!", "ERROR: Please select a valid path for the decrypted file to be saved!");
        outputFilepath = "";
        outputFilepathSelected = false;
        Decryption::setOutputFilepathLabel(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        outputFilepath = FileLocation.join("").toStdString() + ".txt";
        outputFilepathSelected = true;
        Decryption::setOutputFilepathLabel(true);
    }
}

void Decryption::setOutputFilepathLabel(bool outputFilepathSelected){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "outputFilepathSelected"
* whose value is decided whether a valid output filepath has been selected
*
* Arguments:
* @ filepathSelected: A flag which indicates whether to set the icon to a tick or a cross.
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

privateKey Decryption::loadPrivateKey(privateKey privateKeyStruct){
/***********************************************************************
* A function which loads the private key from the .pem file the user has selected
* and then assigns the privateExponent and modulus values to our privateKey structure.
*
* Arguments:
* @ privateKeyStruct: The initialised, but not-yet assigned privateKey structure.
*
* Returns:
*  privateKeyStruct: The privateKey structure once the values have been assigned from the .pem file.
***********************************************************************/
    CryptoPP::FileSource privateKeySource(privateKeyFilepath.c_str(), true);
    CryptoPP::RSA::PrivateKey cryptoPrivateKey;
    CryptoPP::PEM_Load(privateKeySource, cryptoPrivateKey);

    CryptoPP::Integer privateExponent = cryptoPrivateKey.GetPrivateExponent();
    CryptoPP::Integer modulus = cryptoPrivateKey.GetModulus();

    std::string privateExponentString;
    std::ostringstream privateExponentStream;
    privateExponentStream << privateExponent;
    privateExponentString = privateExponentStream.str();
    // The privateExponentString ends in a full-stop ('.') char, therefore we pop_back the string to remove the last character.
    privateExponentString.pop_back();

    std::string modulusString;
    std::ostringstream modulusStream;
    modulusStream << modulus;
    modulusString = modulusStream.str();
    // The modulusString ends in a full-stop ('.') char, therefore we pop_back the string to remove the last character.
    modulusString.pop_back();

    mpz_set_str(privateKeyStruct.privateExponent, privateExponentString.c_str(), 10);
    mpz_set_str(privateKeyStruct.modulus, modulusString.c_str(), 10);

    return privateKeyStruct;
}


void Decryption::goButton(){
/***********************************************************************
* This function is run when the go button is clicked by the user.
* It essentially calls the other functions in the correct order, with some
* validation checks along the way
* A success message is output after decryption has been completed.
* All the filepaths are reset so the program can be run again.
***********************************************************************/
    if(Decryption::checkUserInput() == false){
        Decryption::outputErrorMessage("Error!", "ERROR: Please check all input fields and try again!");
        return;
    }
    privateKey privateKeyStruct = initializePrivateKey();
    privateKeyStruct = Decryption::loadPrivateKey(privateKeyStruct);
    std::string b64textFromFile = Decryption::readFromFile();
    std::string textFromFile;
    //Base64 Decrypts the b64textFromFile, and stores the output in textFromFile.
    macaron::Base64::Decode(b64textFromFile, textFromFile);
    Decryption::decryptString(textFromFile, privateKeyStruct);
    Decryption::writeDecryptedTextToFile();
    Decryption::outputSuccessMessage("Success!", "File decrypted and written to filepath successfully!");


}

void Decryption::decryptString(std::string stringToDecrypt, privateKey privateKeyStruct){
/***********************************************************************
* A function which iterates through the string which will be decrypted.
* It searches for the delimiter, which in this case is a forward slash ('/'),
* as this indicates where the blocks were split when the message was encrypted.
*
* Arguments:
*  @ stringToDecrypt: The entire string, from the encrypted file, once it has been base64 decoded.
*  @ privateKeyStruct: The structure which contains the values needed for decryption.
***********************************************************************/
    std::string blockToDecrypt = "";
    for (int charCounter = 0; static_cast<unsigned int>(charCounter) <= static_cast<unsigned int>(stringToDecrypt.length()) ; charCounter++) {
        char currentChar = stringToDecrypt[charCounter];
        if(currentChar != '/'){
            blockToDecrypt += currentChar;
        }
        else{
            decryptBlock(blockToDecrypt, privateKeyStruct);
            blockToDecrypt = "";
        }
    }
}

void Decryption::decryptBlock(std::string blockToDecrypt, privateKey privateKeyStruct){
/***********************************************************************
* This function is called iteratively by decryptString, it decrypts the current block
* which gets passed in the variable "blockToDecrypt".
* A validation check "addLeadingZeros" is called to correct the loss of leading zeros
* when converting numbers into integers.
* Once successfully decrypted, the decrypted block is concatenated onto the global variable decryptedString.
*
* Arguments:
*  @ blockToDecrypt: The current block, passed from decryptString function, which needs to be decoded.
*  @ privateKeyStruct: The structure which contains the values needed for decryption.
***********************************************************************/
    mpz_t valueToDecrypt; mpz_init(valueToDecrypt);
    mpz_t decryptedDenary; mpz_init(decryptedDenary);

    mpz_set_str(valueToDecrypt, blockToDecrypt.c_str(), 10);
    mpz_powm(decryptedDenary, valueToDecrypt, privateKeyStruct.privateExponent, privateKeyStruct.modulus);

    std::string decryptedBlockBinaryString = mpz_get_str(NULL, 2, decryptedDenary);
    Decryption::addLeadingZeros(decryptedBlockBinaryString);
    std::string currentByte = "";

    int temp_counter = 0;
    for(int currentBitCounter = 0; static_cast<unsigned long long>(currentBitCounter) <= decryptedBlockBinaryString.length(); currentBitCounter++) {
        currentByte += decryptedBlockBinaryString[currentBitCounter];
        temp_counter += 1;
        if(temp_counter == SIZE_OF_CHAR){
            decryptedString += static_cast<char>(stoi(currentByte, 0, 2));
            currentByte = "";
            temp_counter = 0;
        }
    }
}

std::string Decryption::readFromFile(){
/***********************************************************************
* This function reads all of the text from a file, using a buffer stream.
* No validation checks are needed here as they are all done by another function.
*
* Returns:
*  bufferStream.str(): the text content from the file returned as a string.
***********************************************************************/
    std::ifstream inputFileStream(encryptedFilepath);
    std::stringstream bufferStream;
    bufferStream << inputFileStream.rdbuf();
    return bufferStream.str();
}

void Decryption::writeDecryptedTextToFile(){
/***********************************************************************
* A function which writes the global variable decryptedString into a file
* at the location of the contents of global variable outputFilepath.
***********************************************************************/
    std::ofstream outputFileStream;
    outputFileStream.open (outputFilepath);
    outputFileStream << decryptedString;
    outputFileStream.close();
}

void Decryption::addLeadingZeros(std::string &binaryString){
/***********************************************************************
* Due to the conversion from string to binary to string, any leading zeros ('0') will
* be lost, which causes math problems later down the line if not fixed. This function
* ensures that the string has a whole number of bytes, so there are no errors in binary
* to character conversion. As the only data that could be lost is a zero, a variable number
* of zeros get added to the beginning of the string (depending on how many were lost).
*
* Arguments:
* @ binaryString: A string passed by reference, meaning any changes to the string
*                 are also reflected outside the function. This string represents
*                 the data in the current block being decrypted.
***********************************************************************/
    int numberOfZerosRequired = (SIZE_OF_CHAR - (binaryString.length() % SIZE_OF_CHAR));
    if(numberOfZerosRequired != SIZE_OF_CHAR){
        binaryString.insert(0, numberOfZerosRequired, '0');
    }
}

void Decryption::outputErrorMessage(std::string windowHeader, std::string messageContent){
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

void Decryption::outputSuccessMessage(std::string windowHeader, std::string messageContent){
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

void Decryption::resetWindow(){
/***********************************************************************
* Resets all of the global variables, flags and label images to their default values.
***********************************************************************/
    decryptedString = "";
    privateKeyFilepath = "";
    encryptedFilepath = "";
    outputFilepath = "";
    privateKeySelected = false;
    encryptedFileSelected = false;
    outputFilepathSelected = false;
    Decryption::setKeyLabel(false);
    Decryption::setFilepathLabel(false);
    Decryption::setOutputFilepathLabel(false);
    return;
}
