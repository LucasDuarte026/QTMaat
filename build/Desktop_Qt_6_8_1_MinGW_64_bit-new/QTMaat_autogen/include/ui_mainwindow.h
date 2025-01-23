/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionVer_sensores;
    QAction *actionAdicionar;
    QAction *actionRemover;
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *sensorBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *sensorSelected;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLabel *label_angle_start;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLabel *label_angle_end;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QLabel *label_turn_direction;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_mainCircle;
    QLabel *label_greenCircle;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSensor;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1248, 711);
        actionVer_sensores = new QAction(MainWindow);
        actionVer_sensores->setObjectName("actionVer_sensores");
        actionVer_sensores->setCheckable(false);
        actionVer_sensores->setChecked(false);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste));
        actionVer_sensores->setIcon(icon);
        actionAdicionar = new QAction(MainWindow);
        actionAdicionar->setObjectName("actionAdicionar");
        actionAdicionar->setCheckable(false);
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        actionAdicionar->setIcon(icon1);
        actionRemover = new QAction(MainWindow);
        actionRemover->setObjectName("actionRemover");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::EditClear));
        actionRemover->setIcon(icon2);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget_3 = new QWidget(centralwidget);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(10, 290, 258, 301));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        sensorBox = new QGroupBox(centralwidget);
        sensorBox->setObjectName("sensorBox");
        sensorBox->setGeometry(QRect(10, 10, 200, 260));
        sensorBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        sensorBox->setFlat(false);
        sensorBox->setCheckable(false);
        verticalLayoutWidget_2 = new QWidget(sensorBox);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(0, 0, 202, 261));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(10, 0, 10, 0);
        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_3);

        sensorSelected = new QPushButton(verticalLayoutWidget_2);
        sensorSelected->setObjectName("sensorSelected");

        verticalLayout_2->addWidget(sensorSelected);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        label_angle_start = new QLabel(verticalLayoutWidget_2);
        label_angle_start->setObjectName("label_angle_start");
        label_angle_start->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_angle_start->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(label_angle_start);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        label_angle_end = new QLabel(verticalLayoutWidget_2);
        label_angle_end->setObjectName("label_angle_end");
        label_angle_end->setMouseTracking(false);
        label_angle_end->setTabletTracking(false);
        label_angle_end->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_angle_end->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_angle_end);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_7 = new QLabel(verticalLayoutWidget_2);
        label_7->setObjectName("label_7");

        horizontalLayout_4->addWidget(label_7);

        label_turn_direction = new QLabel(verticalLayoutWidget_2);
        label_turn_direction->setObjectName("label_turn_direction");
        label_turn_direction->setMouseTracking(false);
        label_turn_direction->setTabletTracking(false);
        label_turn_direction->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_turn_direction->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_turn_direction);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");

        verticalLayout_2->addLayout(verticalLayout_4);

        label_mainCircle = new QLabel(centralwidget);
        label_mainCircle->setObjectName("label_mainCircle");
        label_mainCircle->setEnabled(true);
        label_mainCircle->setGeometry(QRect(350, 250, 300, 300));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_mainCircle->sizePolicy().hasHeightForWidth());
        label_mainCircle->setSizePolicy(sizePolicy);
        label_mainCircle->setMinimumSize(QSize(20, 20));
        label_mainCircle->setMaximumSize(QSize(1200, 1677));
        label_mainCircle->setFrameShape(QFrame::Shape::NoFrame);
        label_mainCircle->setPixmap(QPixmap(QString::fromUtf8("resources/main_circle.png")));
        label_mainCircle->setScaledContents(true);
        label_mainCircle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_mainCircle->setWordWrap(false);
        label_mainCircle->setOpenExternalLinks(false);
        label_mainCircle->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse);
        label_greenCircle = new QLabel(centralwidget);
        label_greenCircle->setObjectName("label_greenCircle");
        label_greenCircle->setGeometry(QRect(460, 510, 36, 36));
        label_greenCircle->setPixmap(QPixmap(QString::fromUtf8(":/images/resources/green_circle.png")));
        label_greenCircle->setScaledContents(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1248, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuSensor = new QMenu(menubar);
        menuSensor->setObjectName("menuSensor");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSensor->menuAction());
        menuSensor->addAction(actionVer_sensores);
        menuSensor->addAction(actionAdicionar);
        menuSensor->addAction(actionRemover);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionVer_sensores->setText(QCoreApplication::translate("MainWindow", "Ver sensores", nullptr));
        actionAdicionar->setText(QCoreApplication::translate("MainWindow", "Adicionar", nullptr));
        actionRemover->setText(QCoreApplication::translate("MainWindow", "Remover", nullptr));
        sensorBox->setTitle(QCoreApplication::translate("MainWindow", "Sele\303\247\303\243o do sensor", nullptr));
        sensorSelected->setText(QCoreApplication::translate("MainWindow", "Selecionar modelo ...", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo de partida", nullptr));
        label_angle_start->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo de chegada", nullptr));
        label_angle_end->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Sentido de giro", nullptr));
        label_turn_direction->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        label_mainCircle->setText(QString());
        label_greenCircle->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuSensor->setTitle(QCoreApplication::translate("MainWindow", "Sensor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
