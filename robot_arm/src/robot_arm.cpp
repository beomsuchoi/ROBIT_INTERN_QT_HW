#include "robot_arm.h"
#include "./ui_robot_arm.h"
#include <QGraphicsEllipseItem>  // QGraphicsEllipseItem 사용
#include <QBrush>                // 색상을 설정하기 위한 QBrush 사용
#include <QGraphicsLineItem>     // QGraphicsLineItem 사용
#include <QPushButton>           // QPushButton 사용
#include <QSpinBox>              // QSpinBox 사용
#include <QSlider>
#include <QTextBrowser>
#include <cmath>                 // 각도 계산을 위한 삼각함수 사용

// 각도를 라디안으로 변환하는 함수
inline double degToRad(int degrees) {
    return degrees * M_PI / 180.0;
}

robot_arm::robot_arm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::robot_arm)
    , scene(new QGraphicsScene(this))  // QGraphicsScene 생성
{
    ui->setupUi(this);  // UI 파일을 로드하고 설정

    // QGraphicsView에 QGraphicsScene을 연결
    ui->graphicsView->setScene(scene);

    // QGraphicsScene 크기 설정
    scene->setSceneRect(0, 0, 900, 600);

    // QPushButton의 시그널과 슬롯 연결
    connect(ui->Generate, &QPushButton::clicked, this, &robot_arm::generateConnections);

    // QSlider 및 QTextBrowser 연결 (슬라이더 값이 변경될 때 업데이트)
    connect(ui->slider1, &QSlider::valueChanged, this, &robot_arm::updateAngle1);
    connect(ui->slider2, &QSlider::valueChanged, this, &robot_arm::updateAngle2);
    connect(ui->slider3, &QSlider::valueChanged, this, &robot_arm::updateAngle3);

    // Reset 버튼 연결
    connect(ui->Reset, &QPushButton::clicked, this, &robot_arm::resetAngles);

    // 슬라이더 범위 설정
    ui->slider1->setRange(0, 360);
    ui->slider2->setRange(0, 360);
    ui->slider3->setRange(0, 360);
}

robot_arm::~robot_arm()
{
    delete ui;
    delete scene;  // scene 메모리 해제
}

void robot_arm::generateConnections()
{
    // 기존 항목 삭제
    scene->clear();

    // 각 QSpinBox의 값을 가져오기
    int size1 = ui->spinBox1->value();
    int size2 = ui->spinBox2->value();
    int size3 = ui->spinBox3->value();

    // 공 사이의 거리 (직선의 길이)
    int distance1 = size1 * 3;
    int distance2 = size2 * 3;
    int distance3 = size3 * 3;  // 세 번째 직선 길이를 첫 번째 직선과 동일하게 설정

    // 각도 슬라이더 값 가져오기
    int angle1 = ui->slider1->value();
    int angle2 = ui->slider2->value();
    int angle3 = ui->slider3->value();

    // 1번째 원의 위치 및 직선 그리기
    QPointF basePos(200, 300);  // 첫 번째 공의 기준 위치
    QGraphicsEllipseItem *ball1 = new QGraphicsEllipseItem(0, 0, size1, size1);
    ball1->setBrush(QBrush(Qt::red));
    ball1->setPos(basePos);
    scene->addItem(ball1);

    // 2번째 원의 위치 계산 및 직선 그리기
    QPointF ball1Center = QPointF(basePos.x() + size1 / 2, basePos.y() + size1 / 2);
    QPointF ball2Pos = calcNextPosition(ball1Center, distance1, angle1);
    QGraphicsEllipseItem *ball2 = new QGraphicsEllipseItem(0, 0, size2, size2);
    ball2->setBrush(QBrush(Qt::blue));
    ball2->setPos(ball2Pos.x() - size2 / 2, ball2Pos.y() - size2 / 2);  // 원 중심을 맞추기 위해 수정
    scene->addItem(ball2);

    // 첫 번째 직선 그리기 (원의 중심에서 시작)
    QPointF ball2Center = QPointF(ball2Pos.x(), ball2Pos.y());
    drawLineBetweenPoints(ball1Center, ball2Center);

    // 3번째 원의 위치 계산 및 직선 그리기
    QPointF ball3Pos = calcNextPosition(ball2Center, distance2, angle2);
    QGraphicsEllipseItem *ball3 = new QGraphicsEllipseItem(0, 0, size3, size3);
    ball3->setBrush(QBrush(Qt::black));
    ball3->setPos(ball3Pos.x() - size3 / 2, ball3Pos.y() - size3 / 2);  // 원 중심을 맞추기 위해 수정
    scene->addItem(ball3);

    // 두 번째 직선 그리기 (원의 중심에서 시작)
    QPointF ball3Center = QPointF(ball3Pos.x(), ball3Pos.y());
    drawLineBetweenPoints(ball2Center, ball3Center);

    // 제일 오른쪽 원에서 오른쪽으로 길이 조절 가능한 직선 그리기 (첫 번째 직선 길이와 동일하게 설정)
    drawAdjustableLineFromBall(ball3, angle3, distance3);
}

// 두 점 사이에 직선을 그리는 함수 (원의 중심에서 시작)
void robot_arm::drawLineBetweenPoints(QPointF start, QPointF end)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(start.x(), start.y(), end.x(), end.y()));
    line->setPen(QPen(Qt::black));
    scene->addItem(line);
}

// 각도에 따라 다음 원의 위치를 계산하는 함수
QPointF robot_arm::calcNextPosition(QPointF start, int distance, int angle)
{
    double radians = degToRad(angle);
    double x = start.x() + distance * cos(radians);
    double y = start.y() + distance * sin(radians);
    return QPointF(x, y);
}

// 제일 오른쪽 원에서 오른쪽으로 직선을 그리는 함수 (길이 조절 가능)
void robot_arm::drawAdjustableLineFromBall(QGraphicsEllipseItem *ball, int angle, int lineLength)
{
    // 원의 중심을 기준으로 계산
    QPointF ballCenter(ball->pos().x() + ball->rect().width()/2,
                       ball->pos().y() + ball->rect().height()/2);
    QPointF endPos = calcNextPosition(ballCenter, lineLength, angle);

    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(ballCenter, endPos));
    line->setPen(QPen(Qt::black));
    scene->addItem(line);
}

void robot_arm::updateAngle1(int value)
{
    ui->text1->setText(QString::number(value));  // 텍스트 박스에 슬라이더 값 반영
    updateScene();  // 장면 업데이트 (직선 각도 반영)
}

void robot_arm::updateAngle2(int value)
{
    ui->text2->setText(QString::number(value));
    updateScene();
}

void robot_arm::updateAngle3(int value)
{
    ui->text3->setText(QString::number(value));
    updateScene();
}

void robot_arm::updateScene()
{
    generateConnections();  // 직선을 포함하여 장면을 다시 그리기
}

void robot_arm::resetAngles()
{
    // 모든 슬라이더 값을 0으로 초기화
    ui->slider1->setValue(0);
    ui->slider2->setValue(0);
    ui->slider3->setValue(0);

    // 각도 0으로 텍스트 박스도 초기화
    ui->text1->setText("0");
    ui->text2->setText("0");
    ui->text3->setText("0");

    // 초기화된 값으로 장면 업데이트
    updateScene();
}
