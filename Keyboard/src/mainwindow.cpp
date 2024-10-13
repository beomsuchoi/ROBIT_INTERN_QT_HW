#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QMap>

// 글자 변환을 위한 맵
QMap<QString, QString> englishToKorean = {
    {"Q", "ㅂ"}, {"W", "ㅈ"}, {"E", "ㄷ"}, {"R", "ㄱ"}, {"T", "ㅅ"},
    {"Y", "ㅛ"}, {"U", "ㅕ"}, {"I", "ㅑ"}, {"O", "ㅐ"}, {"P", "ㅔ"},
    {"A", "ㅁ"}, {"S", "ㄴ"}, {"D", "ㅇ"}, {"F", "ㄹ"}, {"G", "ㅎ"},
    {"H", "ㅗ"}, {"J", "ㅓ"}, {"K", "ㅏ"}, {"L", "ㅣ"}, {"Z", "ㅋ"},
    {"X", "ㅌ"}, {"C", "ㅊ"}, {"V", "ㅍ"}, {"B", "ㅠ"}, {"N", "ㅜ"}, {"M", "ㅡ"}
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isKoreanMode(false)  // 처음에는 영어 모드
    , isCapsLockOn(false)  // 처음에는 Caps Lock이 꺼져 있음
{
    ui->setupUi(this);
    setGeometry(100, 100, 800, 400);
    setWindowTitle("Keyboard Simulator");

    screen = new QLabel(this);
    screen->setGeometry(64, 80, 601, 111);
    screen->setStyleSheet("background-color: gray;");
    screen->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    screen->setFont(QFont("Arial", 20));

    clearButton = new QPushButton("Clear", this);
    clearButton->setGeometry(674, 140, 45, 45);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearScreen);

    copyButton = new QPushButton("Copy", this);
    copyButton->setGeometry(674, 85, 45, 45);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::copyText);

    createButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createButtons()
{
    QStringList buttonInfo = {
        "Wave,~,59,200", "One,1,104,200", "Two,2,149,200",
        "Three,3,194,200", "Four,4,239,200", "Five,5,284,200",
        "Six,6,329,200", "Seven,7,374,200", "Eight,8,419,200",
        "Nine,9,464,200", "Zero,0,509,200", "Plus,+,599,200",
        "Backspace,back,644,200,81,45",
        "Tab,Tab,59,245,61,45", "Qq,Q,120,245", "Ww,W,165,245",
        "Ee,E,210,245", "Rr,R,255,245", "Tt,T,300,245",
        "Yy,Y,345,245", "Uu,U,390,245", "Ii,I,435,245",
        "Oo,O,480,245", "Pp,P,525,245",
        "Parenthesis_Start,(,570,245", "Parenthesis_Finish,),615,245",
        "ReverseSlash,\\,660,245,65,45",
        "Caps,Caps,59,290,71,45", "Aa,A,130,290", "Ss,S,175,290",
        "Dd,D,220,290", "Ff,F,265,290", "Gg,G,310,290",
        "Hh,H,355,290", "Jj,J,400,290", "Kk,K,445,290",
        "Ll,L,490,290", "Semicolon,;,535,290", "Quotes,\",580,290",
        "Enter,Enter,625,290,101,45",
        "Shift_L,Shift,59,335,81,45", "Zz,Z,140,335", "Xx,X,185,335",
        "Cc,C,230,335", "Vv,V,275,335", "Bb,B,320,335",
        "Nn,N,365,335", "Mm,M,410,335", "Rest,,,455,335",
        "Spot,.,500,335", "Slash,/,545,335",
        "Shift_R,Shift,590,335,136,45",
        "Ctrl_L,Ctrl,59,380,51,45", "Alt,Alt,155,380,51,45",
        "Space, ,206,380,327,45",
        "Change,한/영,533,380,51,45", "Menu,Menu,629,380,45,45",
        "Ctrl_R,Ctrl,674,380,51,45"
    };

    for (const QString &info : buttonInfo) {
        QStringList parts = info.split(",");
        QString name = parts[0];
        QString text = parts[1];
        int x = parts[2].toInt();
        int y = parts[3].toInt();
        int width = parts.size() > 5 ? parts[4].toInt() : 45;
        int height = parts.size() > 5 ? parts[5].toInt() : 45;

        QPushButton *button = new QPushButton(text, this);
        button->setGeometry(x, y, width, height);
        connect(button, &QPushButton::clicked, this, &MainWindow::buttonClicked);
        buttons[name] = button;

        // 버튼의 기본 텍스트를 저장
        originalButtonTexts[name] = text;
    }
}

void MainWindow::buttonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString key = button->text();
        if (key == "back") {
            currentText.chop(1);
        } else if (key == "Tab") {
            currentText += "    ";  // Tab 키: 4개의 스페이스 추가
        } else if (key == "Shift" || key == "Ctrl" || key == "Alt" || key == "Menu") {
            // These keys don't add to the display
        } else if (key == "Caps") {
            isCapsLockOn = !isCapsLockOn;  // Caps Lock 상태 전환
            updateKeyboardLabels();  // 자판에 대소문자 반영
        } else if (key == "한/영") {
            isKoreanMode = !isKoreanMode;  // 한/영 모드 전환
            updateKeyboardLabels();  // 자판에 한글/영어 반영
        } else if (key == "Enter") {
            currentText += "\n";
        } else {
            if (isKoreanMode && englishToKorean.contains(key)) {
                currentText += englishToKorean[key];  // 한글 입력
            } else {
                currentText += (isCapsLockOn ? key.toUpper() : key.toLower());  // 대소문자 구분
            }
        }
        updateScreen();
    }
}

void MainWindow::updateScreen()
{
    screen->setText(currentText);
}

void MainWindow::clearScreen()
{
    currentText.clear();
    updateScreen();
}

void MainWindow::copyText()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(currentText);
}

void MainWindow::updateKeyboardLabels()
{
    // Caps Lock 상태 및 한/영 전환에 따라 버튼의 텍스트를 업데이트
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        QString key = it.key();
        QPushButton *button = it.value();

        if (key == "Caps" || key == "Shift" || key == "Ctrl" || key == "Alt" || key == "한/영" || key == "Enter" || key == "back" || key == "Tab" || key == "Menu") {
            continue;
        }

        // 한글 모드
        if (isKoreanMode && englishToKorean.contains(originalButtonTexts[key])) {
            button->setText(englishToKorean[originalButtonTexts[key]]);
        }
        // 영어 모드
        else {
            QString originalText = originalButtonTexts[key];  // 원래 텍스트로 복구
            button->setText(isCapsLockOn ? originalText.toUpper() : originalText.toLower());
        }
    }
}
