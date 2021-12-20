#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <keygeneration.h>
#include <gmpxx.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/pem.h>
#include <QMainWindow>

namespace Ui {
class Encryption;
}

class Encryption : public QMainWindow
{
    Q_OBJECT

public:
    explicit Encryption(QWidget *parent = nullptr);
    ~Encryption();

private:
    Ui::Encryption *ui;
    void setup();
    void loadMenu();
    void addHomeButtonToToolbar();
    void connectButtons();
    bool checkUserInput();
    publicKey initializePublicKey();
    bool doubleInputCheck();

    void selectPublicKey();
    void setKeyLabel(bool keySelected);
    void selectFileToEncrypt();
    void setFilepathLabel(bool filepathSelected);
    void selectOutputFilepath();
    void setOutputFilepathLabel(bool outputFilepathSelected);
    std::string readFromFile();

    publicKey loadPublicKey(publicKey publicKeyStruct);
    void goButton();
    void encryptString(std::string stringToEncrypt, publicKey publicKeyStruct);
    void encryptBlock(std::string blockToEncrypt, publicKey publicKeyStruct);
    void writeEncryptedTextToFile();
    void outputErrorMessage(std::string windowHeader, std::string messageContent);
    void outputSuccessMessage(std::string windowHeader, std::string messageContent);
    void resetWindow();
};

#endif // ENCRYPTION_H
