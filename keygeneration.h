#ifndef KEYGENERATION_H
#define KEYGENERATION_H

#include <gmpxx.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/pem.h>
#include <QMainWindow>

struct publicKey{
    mpz_t modulus;
    mpz_t publicExponent;
};

struct privateKey{
    mpz_t modulus;
    mpz_t publicExponent;
    mpz_t privateExponent;
    mpz_t prime1;
    mpz_t prime2;
};

namespace Ui {
class KeyGeneration;
}

class KeyGeneration : public QMainWindow
{
    Q_OBJECT

public:
    explicit KeyGeneration(QWidget *parent = nullptr);
    ~KeyGeneration();

private:
    Ui::KeyGeneration *ui;
    void setup();
    void addHomeButtonToToolbar();
    void setLabelImage(bool filepathChosen);
    publicKey initializePublicKey();
    privateKey initializePrivateKey();
    void connectButtons();
    bool checkUserInput();
    void loadMenu();
    void filepathButton();
    void goButton();
    void generatePublicKey(publicKey* publicKeyStruct, privateKey* privateKeyStruct);
    void generatePrivateKey(privateKey* privateKeyStruct);
    std::string generateRandomNumber();
    std::string generatePrimeNumber();
    bool singlePrimeCheck(mpz_t numberToCheck, mpz_t possibleCompositeNumber);
    bool millerRabinPrimeCheck(mpz_t numberToCheck, int numberOfChecks);
    void savePublicKeyToPEMFile(publicKey* publicKeyStruct);
    void savePrivateKeyToPEMFile(privateKey* privateKeyStruct);
    void outputErrorMessage(std::string windowHeader, std::string messageContent);
    void outputSuccessMessage(std::string windowHeader, std::string messageContent);
    void resetWindow();
};

#endif // KEYGENERATION_H
