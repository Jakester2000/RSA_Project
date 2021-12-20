#ifndef DECRYPTION_H
#define DECRYPTION_H

#include <keygeneration.h>
#include <gmpxx.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/pem.h>
#include <QMainWindow>


namespace Ui {
class Decryption;
}

class Decryption : public QMainWindow
{
    Q_OBJECT

public:
    explicit Decryption(QWidget *parent = nullptr);
    ~Decryption();

private:
    Ui::Decryption *ui;
    void setup();
    void loadMenu();
    void addHomeButtonToToolbar();
    void connectButtons();
    bool checkUserInput();
    privateKey initializePrivateKey();

    void selectPrivateKey();
    void setKeyLabel(bool keySelected);
    void selectFileToDecrypt();
    void setFilepathLabel(bool filepathSelected);
    void selectOutputFilepath();
    void setOutputFilepathLabel(bool outputFilepathSelected);

    privateKey loadPrivateKey(privateKey privateKeyStruct);
    void goButton();
    void decryptString(std::string stringToDecrypt, privateKey privateKeyStruct);
    void decryptBlock(std::string blockToDecrypt, privateKey privateKeyStruct);
    std::string readFromFile();
    void writeDecryptedTextToFile();
    void addLeadingZeros(std::string &binaryString);
    void outputErrorMessage(std::string windowHeader, std::string messageContent);
    void outputSuccessMessage(std::string windowHeader, std::string messageContent);
    void resetWindow();
};

#endif // DECRYPTION_H
