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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *login_menuBar;
    QAction *actionVer_Sensores;
    QAction *actionAdicionar;
    QAction *actionAtualizar;
    QAction *actionRemover;
    QAction *actionVer_usu_rios;
    QAction *actionAdicionar_2;
    QAction *actionAtualizar_2;
    QAction *actionRemover_2;
    QAction *actionLogout;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *left_tab;
    QGroupBox *horizontalGroupBox_2;
    QHBoxLayout *horizontalLayout_18;
    QFrame *verticalFrame;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_2;
    QLabel *label_8;
    QGroupBox *groupBoxgg;
    QVBoxLayout *verticalLayout_16;
    QLabel *user_label;
    QLabel *permission_label;
    QGroupBox *sensorBox;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *dados_sensor;
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
    QGroupBox *dial_elements;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *animate_dial_button;
    QProgressBar *animate_progress_bar;
    QVBoxLayout *verticalLayout_22;
    QLabel *operation_label;
    QLabel *actualOperationMode_label;
    QVBoxLayout *verticalLayout;
    QDial *plan_dial;
    QFrame *line;
    QPushButton *disable_servo_button_2;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_12;
    QTabWidget *config_tag;
    QWidget *servo_tab;
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *init_servo_button;
    QPushButton *enable_servo_communication;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *disable_servo_button;
    QPushButton *homing_button;
    QPushButton *clear_errors;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBoxg;
    QVBoxLayout *verticalLayout_18;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_10;
    QLineEdit *step_lineEdit;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_9;
    QLineEdit *amountJog_lineEdit;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *left_jog;
    QPushButton *right_jog;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label;
    QLineEdit *absoluteRevolution;
    QWidget *tab_2;
    QFormLayout *formLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_21;
    QPushButton *prepare_operation;
    QPushButton *start_operation;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_20;
    QCheckBox *engineeringEdit_checkBox;
    QTreeView *treeView_engenharia;
    QWidget *micronasTab;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *logpositionservo;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *readServoPosition_button;
    QLabel *actualAngelIndicator;
    QLabel *servo_angle_position;
    QLabel *label_6;
    QLabel *servo_hex_position;
    QLineEdit *servo_velocity_setup;
    QWidget *logPlaceHolder;
    QVBoxLayout *verticalLayout_9;
    QTabWidget *tab_general;
    QWidget *servo_output;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_11;
    QLineEdit *filter_servo_log;
    QHBoxLayout *horizontalLayout_10;
    QTextEdit *servo_log_screen;
    QScrollBar *log_scroll_bar;
    QPushButton *clean_log_button;
    QWidget *general_output;
    QVBoxLayout *verticalLayout_14;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_12;
    QLineEdit *filter_general_log;
    QHBoxLayout *horizontalLayout_14;
    QTextEdit *general_log_screen;
    QScrollBar *verticalScrollBar_2;
    QPushButton *clean_all_output;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSensor;
    QMenu *menuteste;
    QMenu *menuSensores;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1248, 711);
        MainWindow->setAutoFillBackground(false);
        login_menuBar = new QAction(MainWindow);
        login_menuBar->setObjectName("login_menuBar");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DialogPassword));
        login_menuBar->setIcon(icon);
        actionVer_Sensores = new QAction(MainWindow);
        actionVer_Sensores->setObjectName("actionVer_Sensores");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::EditFind));
        actionVer_Sensores->setIcon(icon1);
        actionAdicionar = new QAction(MainWindow);
        actionAdicionar->setObjectName("actionAdicionar");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        actionAdicionar->setIcon(icon2);
        actionAtualizar = new QAction(MainWindow);
        actionAtualizar->setObjectName("actionAtualizar");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat));
        actionAtualizar->setIcon(icon3);
        actionRemover = new QAction(MainWindow);
        actionRemover->setObjectName("actionRemover");
        QIcon icon4(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        actionRemover->setIcon(icon4);
        actionVer_usu_rios = new QAction(MainWindow);
        actionVer_usu_rios->setObjectName("actionVer_usu_rios");
        QIcon icon5(QIcon::fromTheme(QIcon::ThemeIcon::SystemSearch));
        actionVer_usu_rios->setIcon(icon5);
        actionAdicionar_2 = new QAction(MainWindow);
        actionAdicionar_2->setObjectName("actionAdicionar_2");
        QIcon icon6(QIcon::fromTheme(QIcon::ThemeIcon::ContactNew));
        actionAdicionar_2->setIcon(icon6);
        actionAtualizar_2 = new QAction(MainWindow);
        actionAtualizar_2->setObjectName("actionAtualizar_2");
        actionAtualizar_2->setIcon(icon3);
        actionRemover_2 = new QAction(MainWindow);
        actionRemover_2->setObjectName("actionRemover_2");
        actionRemover_2->setIcon(icon4);
        actionLogout = new QAction(MainWindow);
        actionLogout->setObjectName("actionLogout");
        QIcon icon7(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));
        actionLogout->setIcon(icon7);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        left_tab = new QVBoxLayout();
        left_tab->setObjectName("left_tab");
        horizontalGroupBox_2 = new QGroupBox(centralwidget);
        horizontalGroupBox_2->setObjectName("horizontalGroupBox_2");
        horizontalLayout_18 = new QHBoxLayout(horizontalGroupBox_2);
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        verticalFrame = new QFrame(horizontalGroupBox_2);
        verticalFrame->setObjectName("verticalFrame");
        verticalLayout_19 = new QVBoxLayout(verticalFrame);
        verticalLayout_19->setObjectName("verticalLayout_19");
        label_2 = new QLabel(verticalFrame);
        label_2->setObjectName("label_2");
        label_2->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        verticalLayout_19->addWidget(label_2, 0, Qt::AlignmentFlag::AlignRight);

        label_8 = new QLabel(verticalFrame);
        label_8->setObjectName("label_8");
        label_8->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        verticalLayout_19->addWidget(label_8, 0, Qt::AlignmentFlag::AlignRight);


        horizontalLayout_18->addWidget(verticalFrame);

        groupBoxgg = new QGroupBox(horizontalGroupBox_2);
        groupBoxgg->setObjectName("groupBoxgg");
        verticalLayout_16 = new QVBoxLayout(groupBoxgg);
        verticalLayout_16->setObjectName("verticalLayout_16");
        user_label = new QLabel(groupBoxgg);
        user_label->setObjectName("user_label");

        verticalLayout_16->addWidget(user_label, 0, Qt::AlignmentFlag::AlignHCenter);

        permission_label = new QLabel(groupBoxgg);
        permission_label->setObjectName("permission_label");

        verticalLayout_16->addWidget(permission_label, 0, Qt::AlignmentFlag::AlignHCenter);


        horizontalLayout_18->addWidget(groupBoxgg);


        left_tab->addWidget(horizontalGroupBox_2);

        sensorBox = new QGroupBox(centralwidget);
        sensorBox->setObjectName("sensorBox");
        sensorBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        sensorBox->setFlat(false);
        sensorBox->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(sensorBox);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(10, 20, 10, 10);
        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        dados_sensor = new QVBoxLayout();
        dados_sensor->setObjectName("dados_sensor");
        dados_sensor->setContentsMargins(5, 20, 5, -1);
        sensorSelected = new QPushButton(sensorBox);
        sensorSelected->setObjectName("sensorSelected");
        sensorSelected->setEnabled(true);
        sensorSelected->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        dados_sensor->addWidget(sensorSelected);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_4 = new QLabel(sensorBox);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        label_angle_start = new QLabel(sensorBox);
        label_angle_start->setObjectName("label_angle_start");
        label_angle_start->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_angle_start->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(label_angle_start);


        dados_sensor->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(sensorBox);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        label_angle_end = new QLabel(sensorBox);
        label_angle_end->setObjectName("label_angle_end");
        label_angle_end->setMouseTracking(false);
        label_angle_end->setTabletTracking(false);
        label_angle_end->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_angle_end->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_angle_end);


        dados_sensor->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_7 = new QLabel(sensorBox);
        label_7->setObjectName("label_7");

        horizontalLayout_4->addWidget(label_7);

        label_turn_direction = new QLabel(sensorBox);
        label_turn_direction->setObjectName("label_turn_direction");
        label_turn_direction->setMouseTracking(false);
        label_turn_direction->setTabletTracking(false);
        label_turn_direction->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_turn_direction->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_turn_direction);


        dados_sensor->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(dados_sensor);


        left_tab->addWidget(sensorBox);

        dial_elements = new QGroupBox(centralwidget);
        dial_elements->setObjectName("dial_elements");
        verticalLayout_5 = new QVBoxLayout(dial_elements);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        frame = new QFrame(dial_elements);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMaximumSize(QSize(300, 16777215));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName("gridLayout");
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        animate_dial_button = new QPushButton(frame);
        animate_dial_button->setObjectName("animate_dial_button");
        animate_dial_button->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon8(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        animate_dial_button->setIcon(icon8);

        horizontalLayout_8->addWidget(animate_dial_button);


        verticalLayout_6->addLayout(horizontalLayout_8);


        gridLayout->addLayout(verticalLayout_6, 1, 0, 1, 1);

        animate_progress_bar = new QProgressBar(frame);
        animate_progress_bar->setObjectName("animate_progress_bar");
        animate_progress_bar->setValue(0);
        animate_progress_bar->setInvertedAppearance(false);
        animate_progress_bar->setTextDirection(QProgressBar::Direction::TopToBottom);

        gridLayout->addWidget(animate_progress_bar, 0, 0, 1, 1);


        horizontalLayout_5->addWidget(frame);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName("verticalLayout_22");
        operation_label = new QLabel(dial_elements);
        operation_label->setObjectName("operation_label");

        verticalLayout_22->addWidget(operation_label, 0, Qt::AlignmentFlag::AlignHCenter);

        actualOperationMode_label = new QLabel(dial_elements);
        actualOperationMode_label->setObjectName("actualOperationMode_label");

        verticalLayout_22->addWidget(actualOperationMode_label, 0, Qt::AlignmentFlag::AlignHCenter);


        horizontalLayout_5->addLayout(verticalLayout_22);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");

        horizontalLayout_5->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_5);


        verticalLayout_5->addLayout(verticalLayout_3);

        plan_dial = new QDial(dial_elements);
        plan_dial->setObjectName("plan_dial");
        plan_dial->setMouseTracking(false);
        plan_dial->setWrapping(true);
        plan_dial->setNotchTarget(3.700000000000000);
        plan_dial->setNotchesVisible(true);

        verticalLayout_5->addWidget(plan_dial);

        line = new QFrame(dial_elements);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout_5->addWidget(line);

        disable_servo_button_2 = new QPushButton(dial_elements);
        disable_servo_button_2->setObjectName("disable_servo_button_2");
        QIcon icon9(QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));
        disable_servo_button_2->setIcon(icon9);

        verticalLayout_5->addWidget(disable_servo_button_2);

        verticalLayout_5->setStretch(1, 10);

        left_tab->addWidget(dial_elements);


        horizontalLayout_2->addLayout(left_tab);

        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);

        horizontalLayout_2->addWidget(frame_3);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName("verticalLayout_12");
        config_tag = new QTabWidget(centralwidget);
        config_tag->setObjectName("config_tag");
        config_tag->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        config_tag->setTabPosition(QTabWidget::TabPosition::North);
        config_tag->setElideMode(Qt::TextElideMode::ElideMiddle);
        config_tag->setUsesScrollButtons(true);
        config_tag->setDocumentMode(false);
        config_tag->setTabsClosable(false);
        config_tag->setMovable(false);
        config_tag->setTabBarAutoHide(false);
        servo_tab = new QWidget();
        servo_tab->setObjectName("servo_tab");
        horizontalLayout_9 = new QHBoxLayout(servo_tab);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        groupBox = new QGroupBox(servo_tab);
        groupBox->setObjectName("groupBox");
        verticalLayout_7 = new QVBoxLayout(groupBox);
        verticalLayout_7->setObjectName("verticalLayout_7");
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        init_servo_button = new QPushButton(groupBox);
        init_servo_button->setObjectName("init_servo_button");
        init_servo_button->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon10(QIcon::fromTheme(QIcon::ThemeIcon::SoftwareUpdateAvailable));
        init_servo_button->setIcon(icon10);

        horizontalLayout_17->addWidget(init_servo_button);

        enable_servo_communication = new QPushButton(groupBox);
        enable_servo_communication->setObjectName("enable_servo_communication");
        enable_servo_communication->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(enable_servo_communication->sizePolicy().hasHeightForWidth());
        enable_servo_communication->setSizePolicy(sizePolicy1);
        enable_servo_communication->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon11(QIcon::fromTheme(QIcon::ThemeIcon::UserAvailable));
        enable_servo_communication->setIcon(icon11);
        enable_servo_communication->setCheckable(true);

        horizontalLayout_17->addWidget(enable_servo_communication);


        verticalLayout_7->addLayout(horizontalLayout_17);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName("verticalLayout_15");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(-1, -1, 10, -1);
        disable_servo_button = new QPushButton(groupBox);
        disable_servo_button->setObjectName("disable_servo_button");
        QIcon icon12(QIcon::fromTheme(QIcon::ThemeIcon::EditClear));
        disable_servo_button->setIcon(icon12);

        horizontalLayout_6->addWidget(disable_servo_button);

        homing_button = new QPushButton(groupBox);
        homing_button->setObjectName("homing_button");
        QIcon icon13(QIcon::fromTheme(QIcon::ThemeIcon::GoHome));
        homing_button->setIcon(icon13);

        horizontalLayout_6->addWidget(homing_button);

        clear_errors = new QPushButton(groupBox);
        clear_errors->setObjectName("clear_errors");
        QIcon icon14(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
        clear_errors->setIcon(icon14);

        horizontalLayout_6->addWidget(clear_errors);


        verticalLayout_15->addLayout(horizontalLayout_6);


        verticalLayout_7->addLayout(verticalLayout_15);


        horizontalLayout_9->addWidget(groupBox);

        config_tag->addTab(servo_tab, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName("gridLayout_3");
        groupBoxg = new QGroupBox(tab);
        groupBoxg->setObjectName("groupBoxg");
        verticalLayout_18 = new QVBoxLayout(groupBoxg);
        verticalLayout_18->setObjectName("verticalLayout_18");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName("horizontalLayout_19");
        label_10 = new QLabel(groupBoxg);
        label_10->setObjectName("label_10");

        horizontalLayout_19->addWidget(label_10);

        step_lineEdit = new QLineEdit(groupBoxg);
        step_lineEdit->setObjectName("step_lineEdit");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(step_lineEdit->sizePolicy().hasHeightForWidth());
        step_lineEdit->setSizePolicy(sizePolicy2);
        step_lineEdit->setMinimumSize(QSize(240, 0));
        step_lineEdit->setMaximumSize(QSize(400, 16777215));
        step_lineEdit->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        step_lineEdit->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_19->addWidget(step_lineEdit, 0, Qt::AlignmentFlag::AlignRight);


        verticalLayout_4->addLayout(horizontalLayout_19);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        label_9 = new QLabel(groupBoxg);
        label_9->setObjectName("label_9");

        horizontalLayout_16->addWidget(label_9);

        amountJog_lineEdit = new QLineEdit(groupBoxg);
        amountJog_lineEdit->setObjectName("amountJog_lineEdit");
        sizePolicy2.setHeightForWidth(amountJog_lineEdit->sizePolicy().hasHeightForWidth());
        amountJog_lineEdit->setSizePolicy(sizePolicy2);
        amountJog_lineEdit->setMinimumSize(QSize(240, 0));
        amountJog_lineEdit->setMaximumSize(QSize(400, 16777215));
        amountJog_lineEdit->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        amountJog_lineEdit->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_16->addWidget(amountJog_lineEdit);


        verticalLayout_4->addLayout(horizontalLayout_16);


        verticalLayout_18->addLayout(verticalLayout_4);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        left_jog = new QPushButton(groupBoxg);
        left_jog->setObjectName("left_jog");
        QIcon icon15(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));
        left_jog->setIcon(icon15);
        left_jog->setIconSize(QSize(30, 30));
        left_jog->setCheckable(false);

        horizontalLayout_13->addWidget(left_jog);

        right_jog = new QPushButton(groupBoxg);
        right_jog->setObjectName("right_jog");
        QIcon icon16(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        right_jog->setIcon(icon16);
        right_jog->setIconSize(QSize(30, 30));
        right_jog->setCheckable(false);

        horizontalLayout_13->addWidget(right_jog);


        verticalLayout_18->addLayout(horizontalLayout_13);


        gridLayout_3->addWidget(groupBoxg, 2, 1, 1, 1);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_17 = new QVBoxLayout(groupBox_3);
        verticalLayout_17->setObjectName("verticalLayout_17");
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        label = new QLabel(groupBox_3);
        label->setObjectName("label");

        horizontalLayout_15->addWidget(label);

        absoluteRevolution = new QLineEdit(groupBox_3);
        absoluteRevolution->setObjectName("absoluteRevolution");

        horizontalLayout_15->addWidget(absoluteRevolution);


        verticalLayout_17->addLayout(horizontalLayout_15);


        gridLayout_3->addWidget(groupBox_3, 2, 0, 1, 1);

        config_tag->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        formLayout = new QFormLayout(tab_2);
        formLayout->setObjectName("formLayout");
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_21 = new QVBoxLayout(groupBox_2);
        verticalLayout_21->setObjectName("verticalLayout_21");
        prepare_operation = new QPushButton(groupBox_2);
        prepare_operation->setObjectName("prepare_operation");
        prepare_operation->setEnabled(false);

        verticalLayout_21->addWidget(prepare_operation);

        start_operation = new QPushButton(groupBox_2);
        start_operation->setObjectName("start_operation");
        start_operation->setEnabled(false);

        verticalLayout_21->addWidget(start_operation);


        formLayout->setWidget(0, QFormLayout::LabelRole, groupBox_2);

        config_tag->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayout_20 = new QVBoxLayout(tab_3);
        verticalLayout_20->setObjectName("verticalLayout_20");
        engineeringEdit_checkBox = new QCheckBox(tab_3);
        engineeringEdit_checkBox->setObjectName("engineeringEdit_checkBox");

        verticalLayout_20->addWidget(engineeringEdit_checkBox);

        treeView_engenharia = new QTreeView(tab_3);
        treeView_engenharia->setObjectName("treeView_engenharia");
        treeView_engenharia->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CursorShape::PointingHandCursor)));
        treeView_engenharia->setLineWidth(2);
        treeView_engenharia->setAutoExpandDelay(-1);
        treeView_engenharia->setIndentation(12);
        treeView_engenharia->setUniformRowHeights(true);
        treeView_engenharia->setSortingEnabled(true);
        treeView_engenharia->setAnimated(true);
        treeView_engenharia->setWordWrap(true);
        treeView_engenharia->setHeaderHidden(false);

        verticalLayout_20->addWidget(treeView_engenharia);

        config_tag->addTab(tab_3, QString());
        micronasTab = new QWidget();
        micronasTab->setObjectName("micronasTab");
        config_tag->addTab(micronasTab, QString());

        verticalLayout_12->addWidget(config_tag);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy3);
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_11 = new QVBoxLayout(frame_2);
        verticalLayout_11->setObjectName("verticalLayout_11");
        logpositionservo = new QGroupBox(frame_2);
        logpositionservo->setObjectName("logpositionservo");
        horizontalLayout_7 = new QHBoxLayout(logpositionservo);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(-1, -1, 10, -1);
        readServoPosition_button = new QPushButton(logpositionservo);
        readServoPosition_button->setObjectName("readServoPosition_button");
        readServoPosition_button->setEnabled(false);
        readServoPosition_button->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_7->addWidget(readServoPosition_button);

        actualAngelIndicator = new QLabel(logpositionservo);
        actualAngelIndicator->setObjectName("actualAngelIndicator");

        horizontalLayout_7->addWidget(actualAngelIndicator, 0, Qt::AlignmentFlag::AlignRight);

        servo_angle_position = new QLabel(logpositionservo);
        servo_angle_position->setObjectName("servo_angle_position");
        servo_angle_position->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        servo_angle_position->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        servo_angle_position->setWordWrap(false);

        horizontalLayout_7->addWidget(servo_angle_position);

        label_6 = new QLabel(logpositionservo);
        label_6->setObjectName("label_6");

        horizontalLayout_7->addWidget(label_6, 0, Qt::AlignmentFlag::AlignHCenter);

        servo_hex_position = new QLabel(logpositionservo);
        servo_hex_position->setObjectName("servo_hex_position");

        horizontalLayout_7->addWidget(servo_hex_position);

        servo_velocity_setup = new QLineEdit(logpositionservo);
        servo_velocity_setup->setObjectName("servo_velocity_setup");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(100);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(servo_velocity_setup->sizePolicy().hasHeightForWidth());
        servo_velocity_setup->setSizePolicy(sizePolicy4);
        servo_velocity_setup->setMinimumSize(QSize(100, 0));
        servo_velocity_setup->setMaximumSize(QSize(300, 16777215));
        servo_velocity_setup->setSizeIncrement(QSize(50, 0));

        horizontalLayout_7->addWidget(servo_velocity_setup);


        verticalLayout_11->addWidget(logpositionservo);

        logPlaceHolder = new QWidget(frame_2);
        logPlaceHolder->setObjectName("logPlaceHolder");
        sizePolicy3.setHeightForWidth(logPlaceHolder->sizePolicy().hasHeightForWidth());
        logPlaceHolder->setSizePolicy(sizePolicy3);
        verticalLayout_9 = new QVBoxLayout(logPlaceHolder);
        verticalLayout_9->setObjectName("verticalLayout_9");
        tab_general = new QTabWidget(logPlaceHolder);
        tab_general->setObjectName("tab_general");
        tab_general->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        servo_output = new QWidget();
        servo_output->setObjectName("servo_output");
        verticalLayout_10 = new QVBoxLayout(servo_output);
        verticalLayout_10->setObjectName("verticalLayout_10");
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        filter_servo_log = new QLineEdit(servo_output);
        filter_servo_log->setObjectName("filter_servo_log");
        sizePolicy1.setHeightForWidth(filter_servo_log->sizePolicy().hasHeightForWidth());
        filter_servo_log->setSizePolicy(sizePolicy1);
        filter_servo_log->setEchoMode(QLineEdit::EchoMode::Normal);
        filter_servo_log->setCursorPosition(0);

        horizontalLayout_11->addWidget(filter_servo_log);


        verticalLayout_8->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        servo_log_screen = new QTextEdit(servo_output);
        servo_log_screen->setObjectName("servo_log_screen");

        horizontalLayout_10->addWidget(servo_log_screen);

        log_scroll_bar = new QScrollBar(servo_output);
        log_scroll_bar->setObjectName("log_scroll_bar");
        log_scroll_bar->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_10->addWidget(log_scroll_bar);


        verticalLayout_8->addLayout(horizontalLayout_10);

        clean_log_button = new QPushButton(servo_output);
        clean_log_button->setObjectName("clean_log_button");

        verticalLayout_8->addWidget(clean_log_button, 0, Qt::AlignmentFlag::AlignLeft);


        verticalLayout_10->addLayout(verticalLayout_8);

        tab_general->addTab(servo_output, QString());
        general_output = new QWidget();
        general_output->setObjectName("general_output");
        verticalLayout_14 = new QVBoxLayout(general_output);
        verticalLayout_14->setObjectName("verticalLayout_14");
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName("verticalLayout_13");
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        filter_general_log = new QLineEdit(general_output);
        filter_general_log->setObjectName("filter_general_log");
        filter_general_log->setEchoMode(QLineEdit::EchoMode::Normal);

        horizontalLayout_12->addWidget(filter_general_log);


        verticalLayout_13->addLayout(horizontalLayout_12);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        general_log_screen = new QTextEdit(general_output);
        general_log_screen->setObjectName("general_log_screen");

        horizontalLayout_14->addWidget(general_log_screen);

        verticalScrollBar_2 = new QScrollBar(general_output);
        verticalScrollBar_2->setObjectName("verticalScrollBar_2");
        verticalScrollBar_2->setOrientation(Qt::Orientation::Vertical);

        horizontalLayout_14->addWidget(verticalScrollBar_2);


        verticalLayout_13->addLayout(horizontalLayout_14);

        clean_all_output = new QPushButton(general_output);
        clean_all_output->setObjectName("clean_all_output");

        verticalLayout_13->addWidget(clean_all_output, 0, Qt::AlignmentFlag::AlignRight);


        verticalLayout_14->addLayout(verticalLayout_13);

        tab_general->addTab(general_output, QString());

        verticalLayout_9->addWidget(tab_general);


        verticalLayout_11->addWidget(logPlaceHolder);


        verticalLayout_12->addWidget(frame_2);


        horizontalLayout_2->addLayout(verticalLayout_12);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(2, 3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1248, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuFile->setEnabled(true);
        menuFile->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        menuSensor = new QMenu(menubar);
        menuSensor->setObjectName("menuSensor");
        menuSensor->setEnabled(true);
        menuSensor->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        menuteste = new QMenu(menubar);
        menuteste->setObjectName("menuteste");
        menuSensores = new QMenu(menubar);
        menuSensores->setObjectName("menuSensores");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(sensorSelected, plan_dial);
        QWidget::setTabOrder(plan_dial, animate_dial_button);
        QWidget::setTabOrder(animate_dial_button, init_servo_button);
        QWidget::setTabOrder(init_servo_button, enable_servo_communication);
        QWidget::setTabOrder(enable_servo_communication, tab_general);
        QWidget::setTabOrder(tab_general, filter_servo_log);
        QWidget::setTabOrder(filter_servo_log, servo_log_screen);
        QWidget::setTabOrder(servo_log_screen, clean_log_button);
        QWidget::setTabOrder(clean_log_button, filter_general_log);
        QWidget::setTabOrder(filter_general_log, general_log_screen);
        QWidget::setTabOrder(general_log_screen, clean_all_output);
        QWidget::setTabOrder(clean_all_output, disable_servo_button_2);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSensor->menuAction());
        menubar->addAction(menuteste->menuAction());
        menubar->addAction(menuSensores->menuAction());
        menuFile->addSeparator();
        menuteste->addAction(login_menuBar);
        menuteste->addAction(actionLogout);
        menuteste->addSeparator();
        menuteste->addAction(actionVer_usu_rios);
        menuteste->addAction(actionAdicionar_2);
        menuteste->addAction(actionAtualizar_2);
        menuteste->addAction(actionRemover_2);
        menuSensores->addAction(actionVer_Sensores);
        menuSensores->addAction(actionAdicionar);
        menuSensores->addAction(actionAtualizar);
        menuSensores->addAction(actionRemover);

        retranslateUi(MainWindow);

        config_tag->setCurrentIndex(1);
        tab_general->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        login_menuBar->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        actionVer_Sensores->setText(QCoreApplication::translate("MainWindow", "Ver sensores", nullptr));
        actionAdicionar->setText(QCoreApplication::translate("MainWindow", "Adicionar", nullptr));
        actionAtualizar->setText(QCoreApplication::translate("MainWindow", "Atualizar", nullptr));
        actionRemover->setText(QCoreApplication::translate("MainWindow", "Remover", nullptr));
        actionVer_usu_rios->setText(QCoreApplication::translate("MainWindow", "Ver usu\303\241rios", nullptr));
        actionAdicionar_2->setText(QCoreApplication::translate("MainWindow", "Adicionar", nullptr));
        actionAtualizar_2->setText(QCoreApplication::translate("MainWindow", "Atualizar", nullptr));
        actionRemover_2->setText(QCoreApplication::translate("MainWindow", "Remover", nullptr));
        actionLogout->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
        horizontalGroupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Permiss\303\243o de acesso", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Usu\303\241rio:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Permiss\303\243o:", nullptr));
        user_label->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        permission_label->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        sensorBox->setTitle(QCoreApplication::translate("MainWindow", "Sele\303\247\303\243o do sensor", nullptr));
        sensorSelected->setText(QCoreApplication::translate("MainWindow", "Selecionar modelo ...", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo de partida", nullptr));
        label_angle_start->setText(QCoreApplication::translate("MainWindow", "---\302\272", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo de chegada", nullptr));
        label_angle_end->setText(QCoreApplication::translate("MainWindow", "---\302\272", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Sentido de giro", nullptr));
        label_turn_direction->setText(QCoreApplication::translate("MainWindow", "---", nullptr));
        animate_dial_button->setText(QCoreApplication::translate("MainWindow", "Simular opera\303\247\303\243o", nullptr));
        operation_label->setText(QCoreApplication::translate("MainWindow", "Em modo:", nullptr));
        actualOperationMode_label->setText(QCoreApplication::translate("MainWindow", "SIMULA\303\207\303\203O", nullptr));
        disable_servo_button_2->setText(QCoreApplication::translate("MainWindow", "Parar opera\303\247\303\243o no Servo ", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Servo Minas", nullptr));
        init_servo_button->setText(QCoreApplication::translate("MainWindow", "Inicializar Servo Minas", nullptr));
        enable_servo_communication->setText(QCoreApplication::translate("MainWindow", "Habilitar comandos ao servo", nullptr));
        disable_servo_button->setText(QCoreApplication::translate("MainWindow", "Parar opera\303\247\303\243o no Servo ", nullptr));
        homing_button->setText(QCoreApplication::translate("MainWindow", "Homing", nullptr));
        clear_errors->setText(QCoreApplication::translate("MainWindow", "Limpar erros", nullptr));
        config_tag->setTabText(config_tag->indexOf(servo_tab), QCoreApplication::translate("MainWindow", "Configura\303\247\303\265es do servo", nullptr));
        groupBoxg->setTitle(QCoreApplication::translate("MainWindow", "Jog", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo do passo", nullptr));
#if QT_CONFIG(tooltip)
        step_lineEdit->setToolTip(QCoreApplication::translate("MainWindow", "em graus", nullptr));
#endif // QT_CONFIG(tooltip)
        step_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Passo em graus(\302\272)", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Passos por jog", nullptr));
        amountJog_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Quantidade de passos por jog", nullptr));
        left_jog->setText(QString());
        right_jog->setText(QString());
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Movimenta\303\247\303\243o absoluta - referencial sempre CCW", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo desejado:", nullptr));
        absoluteRevolution->setPlaceholderText(QCoreApplication::translate("MainWindow", "Graus (\302\272) - CCW", nullptr));
        config_tag->setTabText(config_tag->indexOf(tab), QCoreApplication::translate("MainWindow", "Controle manual", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Opera\303\247\303\243o no Sensor", nullptr));
        prepare_operation->setText(QCoreApplication::translate("MainWindow", "Posicionar servo", nullptr));
        start_operation->setText(QCoreApplication::translate("MainWindow", "Iniciar opera\303\247\303\243o", nullptr));
        config_tag->setTabText(config_tag->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Opera\303\247\303\243o", nullptr));
        engineeringEdit_checkBox->setText(QCoreApplication::translate("MainWindow", "Editar", nullptr));
        config_tag->setTabText(config_tag->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Engenharia", nullptr));
        config_tag->setTabText(config_tag->indexOf(micronasTab), QCoreApplication::translate("MainWindow", "Configura\303\247\303\265es Micronas", nullptr));
        readServoPosition_button->setText(QCoreApplication::translate("MainWindow", "Atualizar", nullptr));
        actualAngelIndicator->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo atual:", nullptr));
        servo_angle_position->setText(QCoreApplication::translate("MainWindow", "\303\202ngulo", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", " >", nullptr));
        servo_hex_position->setText(QCoreApplication::translate("MainWindow", "Posi\303\247\303\243o em hexa", nullptr));
        servo_velocity_setup->setInputMask(QString());
        servo_velocity_setup->setText(QString());
        servo_velocity_setup->setPlaceholderText(QCoreApplication::translate("MainWindow", "Velocidade do servo (rad/s):  padr\303\243o ->50", nullptr));
        filter_servo_log->setInputMask(QString());
        filter_servo_log->setText(QString());
        clean_log_button->setText(QCoreApplication::translate("MainWindow", "Limpar log", nullptr));
        tab_general->setTabText(tab_general->indexOf(servo_output), QCoreApplication::translate("MainWindow", "Servo log", nullptr));
        filter_general_log->setText(QString());
        clean_all_output->setText(QCoreApplication::translate("MainWindow", "Limpar sa\303\255da", nullptr));
        tab_general->setTabText(tab_general->indexOf(general_output), QCoreApplication::translate("MainWindow", "Sa\303\255das gerais", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "Maat", nullptr));
        menuSensor->setTitle(QCoreApplication::translate("MainWindow", "Software", nullptr));
        menuteste->setTitle(QCoreApplication::translate("MainWindow", "Usu\303\241rios", nullptr));
        menuSensores->setTitle(QCoreApplication::translate("MainWindow", "Sensores", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
