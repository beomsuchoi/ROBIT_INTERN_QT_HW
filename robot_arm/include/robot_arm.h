#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>  // QGraphicsEllipseItem 사용
#include <QPointF>               // QPointF 사용 (좌표 계산을 위해)

QT_BEGIN_NAMESPACE
namespace Ui { class robot_arm; }
QT_END_NAMESPACE

class robot_arm : public QMainWindow
{
    Q_OBJECT

public:
    robot_arm(QWidget *parent = nullptr);
    ~robot_arm();

private:
    Ui::robot_arm *ui;
    QGraphicsScene *scene;

    // 두 점 사이에 직선을 그리는 함수
    void drawLineBetweenPoints(QPointF start, QPointF end);

    // 각도에 따라 다음 원의 위치를 계산하는 함수
    QPointF calcNextPosition(QPointF start, int distance, int angle);

private slots:
    void generateConnections();  // 전체 장면을 다시 그리는 함수
    void drawAdjustableLineFromBall(QGraphicsEllipseItem *ball, int angle, int lineLength); // 오른쪽 직선 그리기
    void updateAngle1(int value);  // 슬라이더1 값에 따라 갱신
    void updateAngle2(int value);  // 슬라이더2 값에 따라 갱신
    void updateAngle3(int value);  // 슬라이더3 값에 따라 갱신
    void updateScene();            // 슬라이더 값 변경 시 장면 갱신
    void resetAngles();            // 슬라이더와 각도를 초기화하는 함수
};

#endif // ROBOT_ARM_H
