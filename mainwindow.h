#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QImage>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>
#include <QtMath>
#include <point.h>
#include <point3d.h>
#include <solid.h>

namespace Ui {
//typedef struct solidcenter{
//    int centerZ;
//    int numberOfObject;
//} SolidCenter;

typedef struct wallcenter{
    int centerZ;
    int numberOfObject;
    char typeOfWall;
} WallCenter;

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Wykorzystywane w PutPixel():
    uchar* pix;

    // Teksturowanie (uzyte w PutPixelTex()):
    unsigned char *texturesBits[15];
    unsigned char *bitImg;
    Point p1, p2, p3;

    //Wymiary image:
    int width;
    int height;

    QImage img;
    int R, G, B;

    // CheckBoxy:
    QCheckBox *checkGrid, *checkTexture, *checkAnimation;
    //Labelki:
    QLabel *animationSpeedValue, *animationSpeedText;
    //Slider (szybkosc animacji):
    QSlider *animationSpeedSlider;

    // Wspolrzedne:
    Solid characterCor[3]; // przechowujemy wierzcholki tworzace postac (tulow, szyja, glowa)
    Solid characterLegArmCor[61][4]; // przechowujemy wierzcholki tworzace postac (nogi, rece)
                                     // we wszystkich mozliwych stanach (animacja, od -30 do 30 (zmiana polozenia))
    Solid tmpCor; // zmienna pomocnicza (do obrotow)
    Solid currentCharacterCor[7]; // przechowujemy aktualne wspolrzedne wierzcholkow (dla konkretnego stanu)

    // ustalanie srodkow scian (42 sciany):
    Ui::WallCenter wallCenter[42];
    Point vertices2dAll[7][8]; // info o kolejnosci rysowania scian

    // zmienne pomocnicze do rzutowania na 2d:
    int d; // odleglosc od obserwatora (zakladamy ze obserwator znajduje sie na osi OZ)
    int k; // przyblizenie

    // zmienne wykorzystywane przy obrotach:
    Point start;
    Point3D rot;

    // animacja:
    int angle;
    QTimer *timer;

    // tablica obrazkow (tekstur):
    QImage textures[15];

    // Metody:
    void Swap(int &x, int &y);
    void PutPixel(int x, int y, int r, int g, int b);
    void SetInitialCor();
    void SetCurrentCor(int angle);
    void Rotation(Point3D);
    void DrawSegment(int x1, int y1, int x2, int y2);
    void DrawSegments(Point []);
    void Draw();
    void DrawGrid();
    void DrawTexture();
    void GetCenterWall();
    void SortWalls();
    void DrawWalls(Point[], int, char);
    void Morphing(Point[], int, int, int, int k);
    void PutPixelTex(double u, double v, double w, int i, int j, int k);
    bool ReplaceVertex(Point[]);

    // wersja ze srodkami bryl:
    //void GetCenter();
    //void SortSolid();

    // ustalenie srodkow prostopadloscianow:
    //Ui::SolidCenter solidCenter[7];

public slots:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void showGrid();
    void showTexture();
    void animationSpeed();
    void animationStart();
    void doAnimation();
};

#endif // MAINWINDOW_H
