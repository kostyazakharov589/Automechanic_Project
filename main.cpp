#include "osnova.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "automechanic_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    databasemanager dbAuth;    // подключаем бд для проверки паролей
    if (!dbAuth.connectToDataBase()) {
        QMessageBox::critical(nullptr, "Ошибка", "Нет доступа к базе данных");
        return -1;
    }

    osnova w;//главное окно
    w.show();
    QApplication::processEvents();

    QDialog loginDialog(&w); // само окно регистрации, &w нормально его располагает
    loginDialog.setWindowTitle("Вход");
    loginDialog.setFixedSize(320, 220);

    loginDialog.setStyleSheet(//тут дизайн
        "QDialog { background-color: rgb(40, 45, 55); }"
        "QLabel { color: white; font: 700 10pt 'Segoe UI'; }"
        "QLineEdit { "
        "   background-color: rgb(63, 73, 85); color: white; "
        "   border: 2px solid rgb(37, 106, 153); border-radius: 8px; "
        "   padding: 6px; font-size: 14px;"
        "}"
        "QLineEdit:focus { border: 2px solid rgb(50, 150, 220); }"
        "QPushButton { "
        "   background-color: rgb(63, 73, 85); color: white; "
        "   border: 2px solid rgb(37, 106, 153); border-radius: 8px; "
        "   padding: 8px; font: 700 10pt 'Segoe UI';"
        "}"
        "QPushButton:hover { background-color: rgb(37, 106, 153); border: 2px solid rgb(50, 150, 220); }"
        "QPushButton:pressed { background-color: rgb(50, 60, 70); }"
        );

    QVBoxLayout *layout = new QVBoxLayout(&loginDialog);//бокс
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel("Аутентификация");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; color: #3498db; margin-bottom: 5px;");

    QLineEdit *loginEdit = new QLineEdit(&loginDialog);
    loginEdit->setPlaceholderText("Логин");

    QLineEdit *passEdit = new QLineEdit(&loginDialog);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnLogin = new QPushButton("Вход", &loginDialog);
    QPushButton *btnReg = new QPushButton("Регистрация", &loginDialog);
    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnReg);

    layout->addWidget(titleLabel);
    layout->addWidget(loginEdit);
    layout->addWidget(passEdit);
    layout->addLayout(btnLayout);

    QString authenticatedUser = "";

    QObject::connect(btnReg, &QPushButton::clicked, [&]() {//регистрация
        if (loginEdit->text().isEmpty() || passEdit->text().isEmpty()) {
            QMessageBox::warning(&loginDialog, "Ошибка", "Заполните все поля");
            return;
        }
        if (dbAuth.registerUser(loginEdit->text(), passEdit->text())) {
            QMessageBox::information(&loginDialog, "Успех", "Вы зарегистрированы! Нажмите Вход.");
        } else {
            QMessageBox::warning(&loginDialog, "Ошибка", "Логин занят");
        }
    });

    QObject::connect(btnLogin, &QPushButton::clicked, [&]() {//вход в приложение
        if (dbAuth.loginUser(loginEdit->text(), passEdit->text())) {
            authenticatedUser = loginEdit->text();
            loginDialog.accept(); // закрываем окно пускаем юзера
        } else {
            QMessageBox::critical(&loginDialog, "Ошибка", "Неверный логин или пароль");
        }
    });

    if (loginDialog.exec() != QDialog::Accepted) {//запускаем окно
        return 0; // если нажали крестик, программа полностью закрывается
    }

    w.setCurrentUser(authenticatedUser); // отдаем логин

    return a.exec(); // запускаем основной цикл приложения
}