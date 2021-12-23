#include "keygeneration.h"
#include "ui_keygeneration.h"
#include "menu.h"
#include <gmpxx.h>

#include <QMessageBox>
#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/rsa.h>
#include <cryptopp/pem.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <QFileDialog>

int PUBLIC_EXPONENT = 65537; // Needs to be a constant prime, 65537 used as default as stored nicely as hex (0x10001).
int SIZE_OF_KEY = 4096; // Size of the keys in bits.
int SIZE_OF_PRIMES = SIZE_OF_KEY / 2; // Size of the primes in bits.
int BUFFER_SIZE = SIZE_OF_KEY / 16; // Determines what base the keys will be generated in, by default base16 (hexadecimal).

std::string KeyFilepath = ""; // Global string of the filepath, which will later be set by the user.
bool filepathChoosen = false; // Flag to indicate whether the filepath to save keys to have been choosen.



KeyGeneration::KeyGeneration(QWidget *parent):QMainWindow(parent), ui(new Ui::KeyGeneration){
/***********************************************************************
* Constructor for the User Interface,
* Gets called when the window is initialised, sets up the user interface
* and calls the setup function.
***********************************************************************/
    ui->setupUi(this);
    KeyGeneration::setup();
}

KeyGeneration::~KeyGeneration()
/***********************************************************************
* Destructor for the User Interface,
* Gets called automatically by QT when the UI window is closed.
***********************************************************************/
{
    delete ui;
}

void KeyGeneration::setup(){
/***********************************************************************
* The setup function which does the following:
* - connects all buttons to their respective functions,
* - sets labels image to a cross to indicate filepath hasnt been selected,
* - adds the Home button to the toolbar along the top of the window,
* - seeds the random number generator.
***********************************************************************/
    KeyGeneration::connectButtons();
    KeyGeneration::setLabelImage(false);
    KeyGeneration::addHomeButtonToToolbar();
    //seed the random number generator
    srand(time(NULL));
}


void KeyGeneration::addHomeButtonToToolbar(){
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
    connect(homeActionButton, &QAction::triggered, this, &KeyGeneration::loadMenu);
}

void KeyGeneration::setLabelImage(bool filepathChosen){
/***********************************************************************
* Sets the icon to a tick or a cross depending on the value of flag "filepathChosen"
* whose value is decided whether a valid filepath has been selected.
*
* Arguments:
* @ filepathChosen: A flag which indicates whether to set the icon to a tick or a cross.
***********************************************************************/
    QPixmap cross(":/icons/cross.png");
    cross = cross.scaled(ui->ImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap tick(":/icons/tick.png");
    tick = tick.scaled(ui->ImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel* ImageLabel = ui->ImageLabel;

    if(filepathChosen==true){
        ImageLabel->setPixmap(tick);
        ImageLabel->setMask(tick.mask());
        ImageLabel->show();
    }
    else{
        ImageLabel->setPixmap(cross);
        ImageLabel->setMask(cross.mask());
        ImageLabel->show();
    }
}

publicKey KeyGeneration::initializePublicKey(){
/***********************************************************************
* A function which creates a publicKey struct, and initializes each
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

privateKey KeyGeneration::initializePrivateKey(){
/***********************************************************************
* A function which creates a privateKey stucture, and initializes each
* multiprecision variable.
*
* Returns:
* @ privateKeyStruct:       A privateKey structure, named privateKeyStruct, that has been initialised.
***********************************************************************/
    privateKey privateKeyStruct;
    mpz_init(privateKeyStruct.modulus);
    mpz_init(privateKeyStruct.publicExponent);
    mpz_init(privateKeyStruct.privateExponent);
    mpz_init(privateKeyStruct.prime1);
    mpz_init(privateKeyStruct.prime2);
    return privateKeyStruct;
}

void KeyGeneration::connectButtons(){
/***********************************************************************
* Connects each button to their respective functions:
* - SelectFilepathButton connected to FilepathButton function
* - goButton connected to generateKeys funtion
***********************************************************************/
    connect(ui->SelectFilepathButton, &QPushButton::released, this, &KeyGeneration::filepathButton);
    connect(ui->goButton, &QPushButton::released, this, &KeyGeneration::generateKeys);
}

bool KeyGeneration::checkUserInput(){
/***********************************************************************
* A validation function which checks if a value has been selected from
* the drop down menu. If a value has been choosen the global variables
* get updated / re-assigned.
*
* Returns:
*  True: If the user has choosen a value from the dropdown menu.
*  False: If the user has not choosen a value.
***********************************************************************/
    std::string dropDownValue = ui->KeySizeComboBox->currentText().toStdString();

    if(dropDownValue == ""){
        KeyGeneration::outputErrorMessage("ERROR!", "Error: Please select a key size using the drop down menu!");
        return false;
    }
    else if(filepathChoosen == false){
        KeyGeneration::outputErrorMessage("ERROR!", "Error: Please select a filepath for the keys to be saved!");
        return false;
    }
    else{
        // This else statement indicates that the validation checks have passed.
        KeyGeneration::setGlobalVariables();
        return true;
    }
}

void KeyGeneration::setGlobalVariables(){
/***********************************************************************
* Sets the values of the KeySize to the users input, and updates all related variables.
***********************************************************************/
    int dropDownValueInt = ui->KeySizeComboBox->currentText().toInt();
    SIZE_OF_KEY = dropDownValueInt;
    SIZE_OF_PRIMES = SIZE_OF_KEY / 2;
    BUFFER_SIZE = SIZE_OF_KEY / 16;
}

void KeyGeneration::loadMenu(){
/***********************************************************************
* Closes the current window widget and loads the menu.
***********************************************************************/
    Menu *menuWindow = new Menu;
    this->~KeyGeneration();
    //destruct the current window to prevent memory leaks
    menuWindow->show();
}


void KeyGeneration::filepathButton(){
/***********************************************************************
* Opens the file browser, so the user can choose a filepath to save the PEM
* Files to.
* The file browser has been Directory file mode, meaning only folders will
* be shown.
* If a valid directory is choosen, the global KeyFilepath variable is updated.
* The label image is set depending on whether a valid outcome is choosen.
***********************************************************************/
    QFileDialog fileBrowser;
    fileBrowser.setFileMode(QFileDialog::Directory);
    fileBrowser.setWindowTitle(QObject::tr("Save Keys To..."));
    if(fileBrowser.exec()!=QDialog::Accepted){
        KeyGeneration::outputErrorMessage("ERROR", "Error: Please select a valid filepath to save the keys!");
        KeyFilepath = "";
        filepathChoosen = false;
        KeyGeneration::setLabelImage(false);
    }
    else{
        QStringList FileLocation = fileBrowser.selectedFiles();
        KeyFilepath = FileLocation.join("").toStdString();
        filepathChoosen = true;
        KeyGeneration::setLabelImage(true);
    }
}

void KeyGeneration::generateKeys(){
/***********************************************************************
* This is the function that gets run when the button on the UI gets pressed.
* It validates that a keySize has been chosen by the user, and if the check passes
* then the keys are generated.
***********************************************************************/
    if(checkUserInput() == true){
        publicKey publicKeyStruct = KeyGeneration::initializePublicKey();
        privateKey privateKeyStruct = KeyGeneration::initializePrivateKey();
        KeyGeneration::generatePrivateKey(&privateKeyStruct);
        KeyGeneration::generatePublicKey(&publicKeyStruct, &privateKeyStruct);
        KeyGeneration::savePublicKeyToPEMFile(&publicKeyStruct);
        KeyGeneration::savePrivateKeyToPEMFile(&privateKeyStruct);
        KeyGeneration::outputSuccessMessage("Success!", "Keys generated successfully and saved to: " + KeyFilepath);
    }
}


void KeyGeneration::generatePrivateKey(privateKey* privateKeyStruct){
/***********************************************************************
* This generates all the values needed for the variables in the privateKeyStruct
* This function doesnt return anything as instead the struct's values are updated / set
*
* Arguments:
* @ privateKeyStruct: the structure which contains all of the values needed to generate an RSA key
***********************************************************************/
    mpz_set_ui(privateKeyStruct->publicExponent, PUBLIC_EXPONENT);
    std::string primeString1 = generatePrimeNumber();
    std::string primeString2;
    do{
        primeString2 = generatePrimeNumber();
    }while(primeString1 == primeString2);
    mpz_set_str(privateKeyStruct->prime1, primeString1.c_str(), 10);
    mpz_set_str(privateKeyStruct->prime2, primeString2.c_str(), 10);
    mpz_mul(privateKeyStruct->modulus, privateKeyStruct->prime1, privateKeyStruct->prime2);

    mpz_t phi; mpz_init(phi);
    mpz_t temp1; mpz_init(temp1);
    mpz_t temp2; mpz_init(temp2);

    // Calculate phi(modulus) = (prime1 - 1) * (prime2 - 1)
    mpz_sub_ui(temp1, privateKeyStruct->prime1, 1);
    mpz_sub_ui(temp2, privateKeyStruct->prime2, 1);
    mpz_mul(phi, temp1, temp2);

    if(mpz_invert(privateKeyStruct->privateExponent, privateKeyStruct->publicExponent, phi) == 0)
        {
            mpz_gcd(temp1, privateKeyStruct->publicExponent, phi);
        }
}

void KeyGeneration::generatePublicKey(publicKey* publicKeyStruct, privateKey* privateKeyStruct){
/***********************************************************************
* Sets the publicKey publicExponent and modulus to the values we have
* already generated from the generatePrivateKey() function.
***********************************************************************/
    mpz_set(publicKeyStruct->publicExponent, privateKeyStruct->publicExponent);
    mpz_set(publicKeyStruct->modulus, privateKeyStruct->modulus);
}

std::string KeyGeneration::generateRandomNumber(){
/***********************************************************************
* Randomly generates a number which falls between (3/4)*(SIZE_OF_KEY) and (SIZE_OF_KEY)
* This function also guarantees the number it returns is odd as this is a criteria
* for prime numbers > 2.
*
* Returns:
* @ numberString: A string which contains the denary value of the random number.
***********************************************************************/
    mpz_t number; mpz_init(number);
    char hexArray[BUFFER_SIZE];
    // Randomly generates hexadecimal values (using the rng seed in setup).
    for(int i = 0; i < BUFFER_SIZE; i++){
        // Each value in hex array gets a hex value which in denary would be between 0 and 255.
        hexArray[i] = rand() % 0xFF;
    }
    // Applys a bitwise or operation to ensure the 2 most significant bits are 1's.
    // Without this there is a chance the number it generates could be small.
    hexArray[0] |= 0xC0;
    // bitwise or operator to the last bit in the array to ensure odd.
    hexArray[BUFFER_SIZE - 1] |= 0x01;
    mpz_import(number, BUFFER_SIZE, 1, sizeof(hexArray[0]), 0, 0, hexArray);
    std::string numberString = mpz_get_str(NULL, 10, number);
    return numberString;
}

std::string KeyGeneration::generatePrimeNumber(){
/***********************************************************************
* This function is used to call some of the other functions in the correct order,
* It randomly generates numbers (using generateRandomNumber()) until a number
* passes the millerRabinPrimeCheck.
*
* Returns:
* @ randomNumberString: The prime number which has been randomly generated, passed as a string in denary.
***********************************************************************/
    bool primeFound = false;
    mpz_t currentNumber; mpz_init(currentNumber);
    std::string randomNumberString;
    do{
        randomNumberString = generateRandomNumber();
        mpz_set_str(currentNumber, randomNumberString.c_str(), 10);
        if(millerRabinPrimeCheck(currentNumber, 20) == true){
            primeFound = true;
        }
    }while(primeFound == false);
    return randomNumberString;
}

bool KeyGeneration::singlePrimeCheck(mpz_t numberToCheck, mpz_t possibleCompositeNumber){
/***********************************************************************
* This function tests for compositeness.
* A return of False means non composite, which suggests potentially prime.
* This function gets called by millerRabinPrimeCheck().
* This function should be called multiple times with different values of possibleCompositeNumber.
*
* Arguments:
* @ numberToCheck: The number which is being checked if it is prime
* @ possibleCompositeNumber: The number which has been randomly generated to see if it shares any factors with numberToCheck
*
* Returns:
*  True: If numberToCheck is potentially not composite
*  False: If the value is proven by this function to be composite
***********************************************************************/
    mpz_t exponentValue; mpz_init(exponentValue);
    mpz_t tempValue; mpz_init(tempValue);
    mpz_t secondTempValue; mpz_init(secondTempValue);
    int flagValue = 0;

    mpz_sub_ui(exponentValue, numberToCheck, 1);

    while(mpz_even_p(exponentValue)){
        // Rightshift by one bit
        mpz_fdiv_q_2exp(exponentValue, exponentValue, 1);
    }

    mpz_powm(tempValue, possibleCompositeNumber, exponentValue, numberToCheck);
    unsigned long int tempValueInt = mpz_get_ui(tempValue);

    if(tempValueInt == 1){
        return true;
    }

    mpz_sub_ui(tempValue, numberToCheck, 1);
    flagValue = mpz_cmp(exponentValue, tempValue);

    while( flagValue < 0 ){
        mpz_powm(secondTempValue, possibleCompositeNumber, exponentValue, numberToCheck);
        flagValue = mpz_cmp(tempValue, secondTempValue);

        if(flagValue == 0){
            return true;
        }

        mpz_mul_2exp(exponentValue, exponentValue, 1);
        // re-calculate flag value
        flagValue = mpz_cmp(exponentValue, tempValue);
    }

    return false;
}

bool KeyGeneration::millerRabinPrimeCheck(mpz_t numberToCheck, int numberOfChecks = 25){
/***********************************************************************
* This function takes the number which the user wants to prime check and the
* number of checks which they wish to perform. This function calls the singlePrimeCheck()
* function until the number which is passed is proven to be composite,
* Only after all 25 tests pass will this function return true, indicating the number
* passed is likely prime.
*
* Probability of returning a non prime (i.e incorrect output) is apporximately 4^(-numberOfChecks)
* When numberOfChecks is 25, the chance of returning a non prime is 0.00000000000000088 ~= 0
*
* Arguments:
* @ numberToCheck: The number which is checked to see if it is prime.
* @ numberOfChecks: The number of iterations that singlePrimeCheck should be run (by default 25).
*
* Returns:
*  True: If the number has passed all numberOfChecks (usually 25) and isnt composite (i.e. prime).
*  False: If the number passed has been proven to be composite, return false (i.e. not prime).
***********************************************************************/
    int seed = 0;
    // Assume not prime until proven otherwise
    bool potentiallyPrime = true;

    seed = static_cast<long int> (time(NULL));
    mpz_t randomNumber; mpz_init(randomNumber);
    mpz_t upperBound; mpz_init(upperBound);

    gmp_randstate_t randomState;
    gmp_randinit_mt(randomState);
    gmp_randseed_ui(randomState, seed);

    mpz_sub_ui(upperBound, numberToCheck, 3);
    // The reason we subtract 3 from the upper bound, then add 2 after random generation
    // Is to ensure the random number falls between 2 =< rndNum =< numberToCheck - 1 (bounds inclusive)
    for(int i = 0; i < numberOfChecks; i++){
        mpz_urandomm(randomNumber, randomState, upperBound);
        mpz_add_ui(randomNumber, randomNumber, 2);
        // If singlePrimeCheck returns false, it has found the number passed is composite,
        // Which is proof it is not prime.
        potentiallyPrime = KeyGeneration::singlePrimeCheck(numberToCheck, randomNumber);
        if(potentiallyPrime == false){
            return false;
        }
    }
    return true;
}

void KeyGeneration::savePublicKeyToPEMFile(publicKey* publicKeyStruct){
/***********************************************************************
* loads the following variables from publicKeyStruct into the cryptoPP PublicKey Class:
* Letters in the brackets are what each value is usually displayed as in RSA equations.
* - Modulus (n)
* - Public Exponent (e)
* Once CryptoPP private key has been assigned values, the crypto key PEM_SAVE() function is run
* It is passed the filepath to save the keys to and also the crytpoPrivateKey object.
*
* Arguments:
* @ publicKeyStruct: The structure which contains the values needed for a RSA public Key
***********************************************************************/
    try {
        CryptoPP::RSA::PublicKey *cryptoPublicKey = new CryptoPP::RSA::PublicKey;
        CryptoPP::Integer cryptoModulus(mpz_get_str(NULL, 10, publicKeyStruct->modulus));
        cryptoPublicKey->SetModulus(cryptoModulus);
        CryptoPP::Integer cryptoPublicExponent(mpz_get_str(NULL, 10, publicKeyStruct->publicExponent));
        cryptoPublicKey->SetPublicExponent(cryptoPublicExponent);

        CryptoPP::FileSink file((KeyFilepath + "/PublicKey.pem").c_str(), true);
        CryptoPP::PEM_Save(file, *cryptoPublicKey);

        delete cryptoPublicKey;
        // deleting the cryptoPublicKey frees the memory, preventing a memory leak
    }
    catch (std::exception &e) {
        KeyGeneration::outputErrorMessage("Error!", "ERROR: Error when writing PEM file");
        // Goes back to the Menu window to prevent any errors carrying forward in this class.
        KeyGeneration::loadMenu();
    }

}

void KeyGeneration::savePrivateKeyToPEMFile(privateKey* privateKeyStruct){
/***********************************************************************
* loads the following variables from privateKeyStruct into the cryptoPP PrivateKey Class:
* Letters in the brackets are what each value is usually displayed as in RSA equations.
* Example - https://simple.wikipedia.org/wiki/RSA_algorithm
* - Modulus (n)
* - Public Exponent (e)
* - Private Exponent (d)
* - Prime 1 (p)
* - Prime 2 (q)
* Once CryptoPP private key has been assigned values, the crypto key PEM_SAVE() function is run
* It is passed the filepath to save the keys to and also the crytpoPrivateKey object.
*
* Arguments:
* @ privateKeyStruct: The structure which contains the values needed for a RSA private Key
***********************************************************************/
    try {
        CryptoPP::RSA::PrivateKey *cryptoPrivateKey = new CryptoPP::RSA::PrivateKey;
        CryptoPP::Integer cryptoModulus(mpz_get_str(NULL, 10, privateKeyStruct->modulus));
        cryptoPrivateKey->SetModulus(cryptoModulus);
        CryptoPP::Integer cryptoPublicExponent(mpz_get_str(NULL, 10, privateKeyStruct->publicExponent));
        cryptoPrivateKey->SetPublicExponent(cryptoPublicExponent);
        CryptoPP::Integer cryptoPrivateExponent(mpz_get_str(NULL, 10, privateKeyStruct->privateExponent));
        cryptoPrivateKey->SetPrivateExponent(cryptoPrivateExponent);
        CryptoPP::Integer cryptoPrime1(mpz_get_str(NULL, 10, privateKeyStruct->prime1));
        cryptoPrivateKey->SetPrime1(cryptoPrime1);
        CryptoPP::Integer cryptoPrime2(mpz_get_str(NULL, 10, privateKeyStruct->prime2));
        cryptoPrivateKey->SetPrime2(cryptoPrime2);

        CryptoPP::FileSink file((KeyFilepath + "/PrivateKey.pem").c_str(), true);
        CryptoPP::PEM_Save(file, *cryptoPrivateKey);
        delete cryptoPrivateKey;
        // deleting the cryptoPrivateKey frees the memory, preventing a memory leak
    }
    catch (std::exception &e) {
        KeyGeneration::outputErrorMessage("Error!", "ERROR: Error when writing PEM file");
        // Goes back to the Menu window to prevent any errors carrying forward in this class.
        KeyGeneration::loadMenu();
    }
}

void KeyGeneration::outputErrorMessage(std::string windowHeader, std::string messageContent){
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

void KeyGeneration::outputSuccessMessage(std::string windowHeader, std::string messageContent){
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

void KeyGeneration::resetWindow(){
/***********************************************************************
* Resets all of the global variables, flags and label images to their default values.
***********************************************************************/
    std::string KeyFilepath = "";
    bool filepathChoosen = false;
    KeyGeneration::setLabelImage(false);
}
