#ifndef MENU_H
#define MENU_H

#include <QMainWindow>

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private:
    Ui::Menu *ui;
    void connectButtons();
    void openGenerateKeysWindow();
    void openEncryptionWindow();
    void openDecryptionWindow();
};

#endif // MENU_H
