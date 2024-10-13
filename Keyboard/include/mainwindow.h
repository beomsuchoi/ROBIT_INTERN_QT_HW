
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buttonClicked();    // 버튼 클릭 핸들러
    void clearScreen();      // 화면 지우기
    void copyText();         // 텍스트 복사

private:
    Ui::MainWindow *ui;
    QLabel *screen;          // 화면 디스플레이
    QPushButton *clearButton, *copyButton; // Clear, Copy 버튼
    QMap<QString, QPushButton*> buttons;   // 키보드 버튼들
    QMap<QString, QString> originalButtonTexts; // 기본 버튼 텍스트
    QString currentText;     // 화면에 표시되는 텍스트
    bool isKoreanMode;       // 한글 모드 여부
    bool isCapsLockOn;       // Caps Lock 상태

    void createButtons();    // 버튼 생성 함수
    void updateScreen();     // 화면 업데이트 함수
    void updateKeyboardLabels();  // 키보드 라벨 업데이트 함수 (한글/영어, Caps Lock 전환에 따른 라벨 변경)
};

#endif // MAINWINDOW_H
