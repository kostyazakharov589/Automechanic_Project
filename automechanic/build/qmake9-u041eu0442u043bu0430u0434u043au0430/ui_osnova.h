/********************************************************************************
** Form generated from reading UI file 'osnova.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSNOVA_H
#define UI_OSNOVA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "myglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_osnova
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_5;
    QComboBox *profileSelector;
    QPushButton *btnAddNewCar;
    QWidget *page;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QSpinBox *mileageInput;
    QComboBox *yearInput;
    QLabel *Label_3;
    QLineEdit *modelInput;
    QLabel *Label_4;
    QLabel *Label_2;
    QLabel *Label;
    QLineEdit *makeInput;
    QPushButton *btnNext;
    QPushButton *btnBackToCar;
    QWidget *page_2;
    QTextEdit *textEdit;
    QRadioButton *radioRefine;
    QRadioButton *radioSkip;
    QGroupBox *detailsBox;
    QPushButton *btnCalculate;
    QScrollArea *verticalLayout_3;
    QWidget *timelineLayout;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnGenerateTimeline;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QComboBox *spinStartYear;
    QComboBox *cmbStartMonth;
    QComboBox *spinEndYear;
    QComboBox *cmbEndMonth;
    QPushButton *btnBackToCar2;
    QWidget *page_3;
    QTextEdit *textEdit_2;
    QRadioButton *radioReplaceYes;
    QRadioButton *radioReplaceSkip;
    QGroupBox *replaceBox;
    QPushButton *btnGoToResults;
    QComboBox *cmbReplacedPart;
    QComboBox *cmbReplaceSeason;
    QComboBox *cmbReplaceYear;
    QPushButton *btnAddReplacement;
    QListWidget *listReplacements;
    QPushButton *btnBackToSeasons;
    QWidget *page_4;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_results;
    QPushButton *btnRestart;
    QPushButton *btnBackToHistory;
    QStackedWidget *stackedWidget_2;
    QWidget *page_6;
    MyGLWidget *openGLWidget;
    QScrollArea *resultsScrollArea;
    QWidget *resultsLayout;
    QListWidget *listWidgetDetails;
    QWidget *page_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *osnova)
    {
        if (osnova->objectName().isEmpty())
            osnova->setObjectName("osnova");
        osnova->resize(884, 607);
        osnova->setStyleSheet(QString::fromUtf8("QLabel \n"
"{\n"
"    color: white;\n"
"    background-color: #808080\n"
"    border: 2px solid #3498db;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        centralwidget = new QWidget(osnova);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("QStackedWidget\n"
"{\n"
"    color: white;\n"
"    background-color: rgb(0, 0, 0)\n"
"    font-size: 16px;\n"
"\n"
"}"));
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        profileSelector = new QComboBox(page_5);
        profileSelector->setObjectName("profileSelector");
        profileSelector->setGeometry(QRect(230, 210, 391, 51));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setBold(true);
        font.setItalic(false);
        profileSelector->setFont(font);
        profileSelector->setStyleSheet(QString::fromUtf8("QComboBox\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        btnAddNewCar = new QPushButton(page_5);
        btnAddNewCar->setObjectName("btnAddNewCar");
        btnAddNewCar->setGeometry(QRect(300, 330, 261, 41));
        btnAddNewCar->setFont(font);
        btnAddNewCar->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        stackedWidget->addWidget(page_5);
        page = new QWidget();
        page->setObjectName("page");
        groupBox = new QGroupBox(page);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(250, 120, 341, 261));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        groupBox->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName("gridLayout_2");
        mileageInput = new QSpinBox(groupBox);
        mileageInput->setObjectName("mileageInput");
        mileageInput->setMaximum(999999999);

        gridLayout_2->addWidget(mileageInput, 3, 1, 1, 1);

        yearInput = new QComboBox(groupBox);
        yearInput->setObjectName("yearInput");

        gridLayout_2->addWidget(yearInput, 2, 1, 1, 1);

        Label_3 = new QLabel(groupBox);
        Label_3->setObjectName("Label_3");

        gridLayout_2->addWidget(Label_3, 2, 0, 1, 1);

        modelInput = new QLineEdit(groupBox);
        modelInput->setObjectName("modelInput");

        gridLayout_2->addWidget(modelInput, 1, 1, 1, 1);

        Label_4 = new QLabel(groupBox);
        Label_4->setObjectName("Label_4");

        gridLayout_2->addWidget(Label_4, 3, 0, 1, 1);

        Label_2 = new QLabel(groupBox);
        Label_2->setObjectName("Label_2");

        gridLayout_2->addWidget(Label_2, 1, 0, 1, 1);

        Label = new QLabel(groupBox);
        Label->setObjectName("Label");

        gridLayout_2->addWidget(Label, 0, 0, 1, 1);

        makeInput = new QLineEdit(groupBox);
        makeInput->setObjectName("makeInput");

        gridLayout_2->addWidget(makeInput, 0, 1, 1, 1);

        btnNext = new QPushButton(page);
        btnNext->setObjectName("btnNext");
        btnNext->setGeometry(QRect(290, 390, 261, 41));
        btnNext->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        btnBackToCar = new QPushButton(page);
        btnBackToCar->setObjectName("btnBackToCar");
        btnBackToCar->setGeometry(QRect(790, 500, 71, 41));
        btnBackToCar->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        textEdit = new QTextEdit(page_2);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(120, 20, 621, 111));
        textEdit->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        textEdit->setOverwriteMode(false);
        radioRefine = new QRadioButton(page_2);
        radioRefine->setObjectName("radioRefine");
        radioRefine->setGeometry(QRect(210, 150, 90, 21));
        radioSkip = new QRadioButton(page_2);
        radioSkip->setObjectName("radioSkip");
        radioSkip->setGeometry(QRect(560, 150, 91, 21));
        detailsBox = new QGroupBox(page_2);
        detailsBox->setObjectName("detailsBox");
        detailsBox->setGeometry(QRect(120, 200, 621, 351));
        detailsBox->setStyleSheet(QString::fromUtf8("QGroupBox\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        btnCalculate = new QPushButton(detailsBox);
        btnCalculate->setObjectName("btnCalculate");
        btnCalculate->setGeometry(QRect(190, 310, 231, 31));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        btnCalculate->setFont(font1);
        btnCalculate->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QScrollArea(detailsBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setGeometry(QRect(10, 100, 601, 201));
        verticalLayout_3->setWidgetResizable(true);
        timelineLayout = new QWidget();
        timelineLayout->setObjectName("timelineLayout");
        timelineLayout->setGeometry(QRect(0, 0, 599, 199));
        verticalLayout_2 = new QVBoxLayout(timelineLayout);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_3->setWidget(timelineLayout);
        btnGenerateTimeline = new QPushButton(detailsBox);
        btnGenerateTimeline->setObjectName("btnGenerateTimeline");
        btnGenerateTimeline->setGeometry(QRect(250, 70, 111, 21));
        layoutWidget = new QWidget(detailsBox);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(40, 30, 531, 31));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        spinStartYear = new QComboBox(layoutWidget);
        spinStartYear->setObjectName("spinStartYear");

        horizontalLayout->addWidget(spinStartYear);

        cmbStartMonth = new QComboBox(layoutWidget);
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->addItem(QString());
        cmbStartMonth->setObjectName("cmbStartMonth");

        horizontalLayout->addWidget(cmbStartMonth);

        spinEndYear = new QComboBox(layoutWidget);
        spinEndYear->setObjectName("spinEndYear");

        horizontalLayout->addWidget(spinEndYear);

        cmbEndMonth = new QComboBox(layoutWidget);
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->addItem(QString());
        cmbEndMonth->setObjectName("cmbEndMonth");

        horizontalLayout->addWidget(cmbEndMonth);

        btnBackToCar2 = new QPushButton(page_2);
        btnBackToCar2->setObjectName("btnBackToCar2");
        btnBackToCar2->setGeometry(QRect(790, 500, 71, 41));
        btnBackToCar2->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        textEdit_2 = new QTextEdit(page_3);
        textEdit_2->setObjectName("textEdit_2");
        textEdit_2->setGeometry(QRect(130, 20, 581, 91));
        textEdit_2->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        textEdit_2->setOverwriteMode(false);
        radioReplaceYes = new QRadioButton(page_3);
        radioReplaceYes->setObjectName("radioReplaceYes");
        radioReplaceYes->setGeometry(QRect(200, 130, 101, 21));
        radioReplaceSkip = new QRadioButton(page_3);
        radioReplaceSkip->setObjectName("radioReplaceSkip");
        radioReplaceSkip->setGeometry(QRect(540, 130, 121, 21));
        replaceBox = new QGroupBox(page_3);
        replaceBox->setObjectName("replaceBox");
        replaceBox->setGeometry(QRect(130, 180, 581, 361));
        replaceBox->setStyleSheet(QString::fromUtf8("QGroupBox\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"	font: 700 16pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        btnGoToResults = new QPushButton(replaceBox);
        btnGoToResults->setObjectName("btnGoToResults");
        btnGoToResults->setGeometry(QRect(180, 320, 231, 31));
        btnGoToResults->setFont(font1);
        cmbReplacedPart = new QComboBox(replaceBox);
        cmbReplacedPart->setObjectName("cmbReplacedPart");
        cmbReplacedPart->setGeometry(QRect(20, 40, 111, 24));
        cmbReplaceSeason = new QComboBox(replaceBox);
        cmbReplaceSeason->setObjectName("cmbReplaceSeason");
        cmbReplaceSeason->setGeometry(QRect(20, 140, 111, 24));
        cmbReplaceYear = new QComboBox(replaceBox);
        cmbReplaceYear->setObjectName("cmbReplaceYear");
        cmbReplaceYear->setGeometry(QRect(20, 230, 111, 24));
        btnAddReplacement = new QPushButton(replaceBox);
        btnAddReplacement->setObjectName("btnAddReplacement");
        btnAddReplacement->setGeometry(QRect(20, 280, 111, 24));
        listReplacements = new QListWidget(replaceBox);
        listReplacements->setObjectName("listReplacements");
        listReplacements->setGeometry(QRect(140, 40, 421, 261));
        btnBackToSeasons = new QPushButton(page_3);
        btnBackToSeasons->setObjectName("btnBackToSeasons");
        btnBackToSeasons->setGeometry(QRect(790, 500, 71, 41));
        btnBackToSeasons->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        verticalLayoutWidget = new QWidget(page_4);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(40, 30, 821, 471));
        verticalLayout_results = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_results->setObjectName("verticalLayout_results");
        verticalLayout_results->setContentsMargins(0, 0, 0, 0);
        btnRestart = new QPushButton(page_4);
        btnRestart->setObjectName("btnRestart");
        btnRestart->setGeometry(QRect(10, 500, 81, 41));
        btnRestart->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        btnBackToHistory = new QPushButton(page_4);
        btnBackToHistory->setObjectName("btnBackToHistory");
        btnBackToHistory->setGeometry(QRect(790, 500, 71, 41));
        btnBackToHistory->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"    color: white;\n"
"	background-color: rgb(63, 73, 85);\n"
"    border: 2px solid rgb(37, 106, 153);\n"
"	font: 700 9pt \"Segoe UI\";\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
"}"));
        stackedWidget_2 = new QStackedWidget(page_4);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(58, 38, 801, 461));
        page_6 = new QWidget();
        page_6->setObjectName("page_6");
        openGLWidget = new MyGLWidget(page_6);
        openGLWidget->setObjectName("openGLWidget");
        openGLWidget->setGeometry(QRect(30, 10, 361, 211));
        openGLWidget->setMinimumSize(QSize(250, 100));
        resultsScrollArea = new QScrollArea(page_6);
        resultsScrollArea->setObjectName("resultsScrollArea");
        resultsScrollArea->setGeometry(QRect(399, 9, 391, 431));
        resultsScrollArea->setWidgetResizable(true);
        resultsLayout = new QWidget();
        resultsLayout->setObjectName("resultsLayout");
        resultsLayout->setGeometry(QRect(0, 0, 389, 429));
        resultsScrollArea->setWidget(resultsLayout);
        listWidgetDetails = new QListWidget(page_6);
        listWidgetDetails->setObjectName("listWidgetDetails");
        listWidgetDetails->setGeometry(QRect(30, 230, 361, 211));
        stackedWidget_2->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName("page_7");
        stackedWidget_2->addWidget(page_7);
        stackedWidget->addWidget(page_4);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        osnova->setCentralWidget(centralwidget);
        menubar = new QMenuBar(osnova);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 884, 21));
        osnova->setMenuBar(menubar);
        statusbar = new QStatusBar(osnova);
        statusbar->setObjectName("statusbar");
        osnova->setStatusBar(statusbar);

        retranslateUi(osnova);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(osnova);
    } // setupUi

    void retranslateUi(QMainWindow *osnova)
    {
        osnova->setWindowTitle(QCoreApplication::translate("osnova", "osnova", nullptr));
#if QT_CONFIG(tooltip)
        profileSelector->setToolTip(QCoreApplication::translate("osnova", "<html><head/><body><p>\320\241\321\203\321\211\320\265\321\201\321\202\320\262\321\203\321\216\321\211\320\270\320\265 \320\277\321\200\320\276\321\204\320\270\320\273\320\270</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAddNewCar->setText(QCoreApplication::translate("osnova", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\275\320\276\320\262\321\213\320\271 \320\260\320\262\321\202\320\276\320\274\320\276\320\261\320\270\320\273\321\214", nullptr));
        groupBox->setTitle(QCoreApplication::translate("osnova", "\320\221\320\260\320\267\320\276\320\262\321\213\320\271 \320\277\321\200\320\276\321\204\320\270\320\273\321\214 \320\260\320\262\321\202\320\276\320\274\320\276\320\261\320\270\320\273\321\217", nullptr));
        Label_3->setText(QCoreApplication::translate("osnova", "\320\223\320\276\320\264 \320\262\321\213\320\277\321\203\321\201\320\272\320\260", nullptr));
        Label_4->setText(QCoreApplication::translate("osnova", "\320\221\320\260\320\267\320\276\320\262\321\213\320\271 \320\277\321\200\320\276\320\261\320\265\320\263", nullptr));
        Label_2->setText(QCoreApplication::translate("osnova", "\320\234\320\276\320\264\320\265\320\273\321\214 ", nullptr));
        Label->setText(QCoreApplication::translate("osnova", "\320\234\320\260\321\200\320\272\320\260 \320\260\320\262\321\202\320\276", nullptr));
        btnNext->setText(QCoreApplication::translate("osnova", "\320\224\320\260\320\273\320\265\320\265", nullptr));
        btnBackToCar->setText(QCoreApplication::translate("osnova", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        textEdit->setHtml(QCoreApplication::translate("osnova", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:16px; font-weight:700; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:400;\">\320\245\320\276\321\202\320\270\321\202\320\265 \320\273\320\270 \320\262\321\213 \321\203\321\202\320\276\321\207\320\275\320\270\321\202\321\214 </span><span style=\" font-size:14pt;\">\320\277\321\200\320\276\320\261\320\265\320\263 \320\260\320\262\321\202\320\276\320\274\320\276\320\261\320\270\320\273\321\217? </span><span styl"
                        "e=\" font-size:14pt; font-weight:400;\">\320\255\321\202\320\276 \320\277\320\276\320\274\320\276\320\266\320\265\321\202 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265</span><span style=\" font-size:14pt;\"> \320\261\320\276\320\273\320\265\320\265 \321\202\320\276\321\207\320\275\320\276</span><span style=\" font-size:14pt; font-weight:400;\"> \320\276\320\277\321\200\320\265\320\264\320\265\320\273\320\270\321\202\321\214 \321\201\321\202\320\265\320\277\320\265\320\275\321\214 \320\270\320\267\320\275\320\276\321\201\320\260 \320\264\320\265\321\202\320\260\320\273\320\265\320\271 \320\270 \320\265\320\263\320\276 \321\202\320\265\320\272\321\203\321\211\320\265\320\265 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265. \320\255\321\202\320\276\321\202 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 </span><span style=\" font-size:14pt;\">\320\262\321\201\320\265\320\263\320\264\320\260</span><span style=\" font-size:14pt; font-weight:400;\""
                        "> \320\274\320\276\320\266\320\275\320\276 </span><span style=\" font-size:14pt;\">\320\270\320\267\320\274\320\265\320\275\320\270\321\202\321\214</span><span style=\" font-size:14pt; font-weight:400;\">.</span></p></body></html>", nullptr));
        radioRefine->setText(QCoreApplication::translate("osnova", "\320\243\321\202\320\276\321\207\320\275\320\270\321\202\321\214", nullptr));
        radioSkip->setText(QCoreApplication::translate("osnova", "\320\237\321\200\320\276\320\277\321\203\321\201\321\202\320\270\321\202\321\214 ", nullptr));
        detailsBox->setTitle(QCoreApplication::translate("osnova", "                                                   \320\224\320\225\320\242\320\220\320\233\320\230\320\227\320\220\320\246\320\230\320\257 \320\237\320\240\320\236\320\221\320\225\320\223\320\220", nullptr));
        btnCalculate->setText(QCoreApplication::translate("osnova", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270 \320\277\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", nullptr));
        btnGenerateTimeline->setText(QCoreApplication::translate("osnova", "\320\276\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        cmbStartMonth->setItemText(0, QCoreApplication::translate("osnova", "\320\257\320\275\320\262\320\260\321\200\321\214", nullptr));
        cmbStartMonth->setItemText(1, QCoreApplication::translate("osnova", "\320\244\320\265\320\262\321\200\320\260\320\273\321\214", nullptr));
        cmbStartMonth->setItemText(2, QCoreApplication::translate("osnova", "\320\234\320\260\321\200\321\202", nullptr));
        cmbStartMonth->setItemText(3, QCoreApplication::translate("osnova", "\320\220\320\277\321\200\320\265\320\273\321\214", nullptr));
        cmbStartMonth->setItemText(4, QCoreApplication::translate("osnova", "\320\230\321\216\320\275\321\214", nullptr));
        cmbStartMonth->setItemText(5, QCoreApplication::translate("osnova", "\320\230\321\216\320\273\321\214", nullptr));
        cmbStartMonth->setItemText(6, QCoreApplication::translate("osnova", "\320\220\320\262\320\263\321\203\321\201\321\202", nullptr));
        cmbStartMonth->setItemText(7, QCoreApplication::translate("osnova", "\320\241\320\265\320\275\321\202\321\217\320\261\321\200\321\214", nullptr));
        cmbStartMonth->setItemText(8, QCoreApplication::translate("osnova", "\320\236\320\272\321\202\321\217\320\261\321\200\321\214", nullptr));
        cmbStartMonth->setItemText(9, QCoreApplication::translate("osnova", "\320\235\320\276\321\217\320\261\321\200\321\214", nullptr));
        cmbStartMonth->setItemText(10, QCoreApplication::translate("osnova", "\320\224\320\265\320\272\320\260\320\261\321\200\321\214", nullptr));

        cmbEndMonth->setItemText(0, QCoreApplication::translate("osnova", "\320\257\320\275\320\262\320\260\321\200\321\214", nullptr));
        cmbEndMonth->setItemText(1, QCoreApplication::translate("osnova", "\320\244\320\265\320\262\321\200\320\260\320\273\321\214", nullptr));
        cmbEndMonth->setItemText(2, QCoreApplication::translate("osnova", "\320\234\320\260\321\200\321\202", nullptr));
        cmbEndMonth->setItemText(3, QCoreApplication::translate("osnova", "\320\220\320\277\321\200\320\265\320\273\321\214", nullptr));
        cmbEndMonth->setItemText(4, QCoreApplication::translate("osnova", "\320\230\321\216\320\275\321\214", nullptr));
        cmbEndMonth->setItemText(5, QCoreApplication::translate("osnova", "\320\230\321\216\320\273\321\214", nullptr));
        cmbEndMonth->setItemText(6, QCoreApplication::translate("osnova", "\320\220\320\262\320\263\321\203\321\201\321\202", nullptr));
        cmbEndMonth->setItemText(7, QCoreApplication::translate("osnova", "\320\241\320\265\320\275\321\202\321\217\320\261\321\200\321\214", nullptr));
        cmbEndMonth->setItemText(8, QCoreApplication::translate("osnova", "\320\236\320\272\321\202\321\217\320\261\321\200\321\214", nullptr));
        cmbEndMonth->setItemText(9, QCoreApplication::translate("osnova", "\320\235\320\276\321\217\320\261\321\200\321\214", nullptr));
        cmbEndMonth->setItemText(10, QCoreApplication::translate("osnova", "\320\224\320\265\320\272\320\260\320\261\321\200\321\214", nullptr));

        btnBackToCar2->setText(QCoreApplication::translate("osnova", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        textEdit_2->setHtml(QCoreApplication::translate("osnova", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:16px; font-weight:700; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">\320\227\320\260\320\274\320\265\320\275\321\217\320\273\320\270\321\201\321\214 \320\273\320\270</span><span style=\" font-size:16pt; font-weight:400;\"> </span><span style=\" font-size:16pt;\">\320\264\320\265\321\202\320\260\320\273\320\270</span><span style=\" font-size:16pt; font-weight:400;\"> \320\267\320\260 \320\262\321\200\320\265\320\274\321"
                        "\217 \321\215\320\272\321\201\320\277\320\273\321\203\320\260\321\202\320\260\321\206\320\270\320\270 \320\260\320\262\321\202\320\276\320\274\320\276\320\261\320\270\320\273\320\265\320\274? </span></p></body></html>", nullptr));
        radioReplaceYes->setText(QCoreApplication::translate("osnova", "\320\224\320\220, \320\274\320\265\320\275\321\217\320\273\320\270\321\201\321\214", nullptr));
        radioReplaceSkip->setText(QCoreApplication::translate("osnova", "\320\235\320\225\320\242, \320\275\320\265 \320\274\320\265\320\275\321\217\320\273\320\270\321\201\321\214", nullptr));
        replaceBox->setTitle(QCoreApplication::translate("osnova", "     \320\232\320\260\320\272\320\270\320\265 \320\264\320\265\321\202\320\260\320\273\320\270 \320\262\320\260\321\210\320\265\320\263\320\276 \320\260\320\262\321\202\320\276\320\274\320\276\320\261\320\270\320\273\321\217 \320\274\320\265\320\275\321\217\320\273\320\270\321\201\321\214 \320\262\320\276 \320\262\321\200\320\265\320\274\321\217 \321\215\320\272\321\201\320\277\320\273\321\203\321\202\320\260\321\206\320\270\320\270?", nullptr));
        btnGoToResults->setText(QCoreApplication::translate("osnova", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270 \320\277\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", nullptr));
        btnAddReplacement->setText(QCoreApplication::translate("osnova", "+ \320\267\320\260\320\274\320\265\320\275\320\260", nullptr));
        btnBackToSeasons->setText(QCoreApplication::translate("osnova", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        btnRestart->setText(QCoreApplication::translate("osnova", "\320\235\320\260\321\207\320\260\320\273\320\276", nullptr));
        btnBackToHistory->setText(QCoreApplication::translate("osnova", "\320\235\320\260\320\267\320\260\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class osnova: public Ui_osnova {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSNOVA_H
