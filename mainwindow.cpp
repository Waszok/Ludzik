#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this -> setMinimumSize(800,610);

    width = 600;
    height = 600;
    img = QImage(width, height, QImage::Format_RGB32);
    img.fill(0xFFFFFF);
    //img.fill(0x000000);
    bitImg = img.bits();

    k = 0; // pomniejszanie i powiekszanie obrazu
    d = 256; // odleglosc od obserwatora
    rot = Point3D(0, 0, 0); // punkt o ktory obracamy
    R = 50; G = 205; B = 50; // kolor siatki (seledynowy)
    angle = 30; // polozenie rak i nog (animacja) - na starcie jest na srodku

    textures[0] = QImage(":/tex/arm.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[1] = QImage(":/tex/armUp.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[2] = QImage(":/tex/back.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[3] = QImage(":/tex/bodyUp.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[4] = QImage(":/tex/face.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[5] = QImage(":/tex/front.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[6] = QImage(":/tex/hair.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[7] = QImage(":/tex/hairLeft.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[8] = QImage(":/tex/hairRight.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[9] = QImage(":/tex/legDown.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[10] = QImage(":/tex/legFront.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[11] = QImage(":/tex/legThreeSides.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[12] = QImage(":/tex/legUp.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[13] = QImage(":/tex/sideBody.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    textures[14] = QImage(":/tex/skin.jpg", "JPEG").scaled(QSize(250, 250), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    for ( int i = 0; i < 15; ++i )
        texturesBits[i] = textures[i].bits();

    // Rozmieszczenie checkBoxow i sliderow:
    checkGrid = new QCheckBox("SIATKA", this);
    checkGrid -> setGeometry(615, 20, 160, 30);
    checkTexture = new QCheckBox("TEKSTUROWANIE", this);
    checkTexture -> setGeometry(615, 60, 160, 30);
    checkAnimation = new QCheckBox("ANIMACJA", this);
    checkAnimation -> setGeometry(615, 100, 160, 30);

    animationSpeedText = new QLabel("PRĘDKOŚĆ ANIMACJI", this);
    animationSpeedText -> setGeometry(610, 140, 160, 30);
    animationSpeedValue = new QLabel("10", this);
    animationSpeedValue -> setGeometry(750, 170, 30, 23);

    animationSpeedSlider = new QSlider(Qt::Horizontal, this);
    animationSpeedSlider -> setGeometry(615, 170, 125, 23);
    animationSpeedSlider -> setMinimum(1);
    animationSpeedSlider -> setMaximum(50);
    animationSpeedSlider-> setValue(10);

    // ustawienie predkosci timera:
    timer = new QTimer(this);
    timer -> setInterval(10);

    // sygnaly i sloty:
    connect(checkGrid, SIGNAL(stateChanged(int)), this, SLOT(showGrid()));
    connect(checkTexture, SIGNAL(stateChanged(int)), this, SLOT(showTexture()));
    connect(checkAnimation, SIGNAL(stateChanged(int)), this, SLOT(animationStart()));
    connect(animationSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(animationSpeed()));
    connect(timer, SIGNAL(timeout()), this, SLOT(doAnimation()));

    SetInitialCor();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete checkAnimation;
    delete checkGrid;
    delete checkTexture;
    delete animationSpeedValue;
    delete animationSpeedText;
    delete animationSpeedSlider;
    delete timer;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawImage(0, 0, img);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    start.x = e->x();
    start.y = e->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    rot.y = (rot.y + start.x - e -> x()) % 360;
    rot.x = (rot.x - start.y + e -> y()) % 360;
    start.x = e->x();
    start.y = e->y();

    Draw();
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    k = k + (e -> angleDelta().y())/10;
    if (k > 80) k = 80;

    Draw();
}

void MainWindow::showGrid()
{
    Draw();
}

void MainWindow::showTexture()
{
    Draw();
}

void MainWindow::animationStart()
{
    if(checkAnimation->isChecked())
        timer -> start();
    else
        timer -> stop();
}

void MainWindow::doAnimation()
{
    static int tmp = 0; // wartosc statycznej zmiennej pozostaje caly czas taka sama
    tmp = (tmp + 1) % 120;
    if ( tmp <= 30 )
        angle++;
    else if (tmp > 30 && tmp <= 90)
        angle--;
    else
        angle++;
    Draw();
}

void MainWindow::animationSpeed()
{
    timer -> setInterval(animationSpeedSlider -> value());
    animationSpeedValue -> setText(QString::number(animationSpeedSlider -> value()));
}

void MainWindow::Swap(int &x, int &y){
    int tmp = x;
    x = y;
    y = tmp;
}

// Kolorowanie pixela:
void MainWindow::PutPixel(int x, int y, int r, int g, int b)
{
    if(x >= 0 && x < width && y >= 0 && y < height){
        pix = img.scanLine(y);
        pix[x*4] = b;
        pix[x*4 + 1] = g;
        pix[x*4 + 2] = r;
    }
}

// Rysowanie odcinka:
void MainWindow::DrawSegment(int x1, int y1, int x2, int y2)
{

    if(x1 == x2 && y1 == y2) PutPixel(x1,y1, R, G, B);
    double wsp, freeTerm;
    if(abs(x1 - x2) >= abs(y1 - y2)){
        if(x2 < x1){
            Swap(x1,x2);
            Swap(y1,y2);
        }
        wsp = (y2-y1)/static_cast<double>(x2-x1);
        freeTerm = (y1*x2-x1*y2)/static_cast<double>(x2-x1);
        for(int i = x1; i < x2; i++){
            int tmp = static_cast<int>(wsp*i+freeTerm+0.5);
            PutPixel(i,tmp, R, G, B);
        }
    }
    else{
        if(y2 < y1){
            Swap(y1,y2);
            Swap(x1,x2);
        }
        wsp = (x2-x1)/static_cast<double>(y2-y1);
        freeTerm = (x1*y2-y1*x2)/static_cast<double>(y2-y1);
        for(int i = y1; i < y2; i++){
            int tmp = static_cast<int>(wsp*i+freeTerm+0.5);
            PutPixel(tmp,i, R, G, B);
        }
    }
}

// Inicjalizacja wierzcholkow:
void MainWindow::SetInitialCor()
{
    // Okreslenie wszystkich wierzcholkow wyznaczajaych ludzika (startowe):
    characterCor[0] = Solid(Point3D(100, 85, 80), Point3D(100, -175, 80), Point3D(100, 85, -30), Point3D(100, -175, -30),
                            Point3D(-100, 85, 80), Point3D(-100, -175, 80), Point3D(-100, 85, -30), Point3D(-100, -175, -30));
    characterCor[1] = Solid(Point3D(40, -175, 50), Point3D(40, -200, 50), Point3D(40, -175, -15), Point3D(40, -200, -15),
                            Point3D(-40, -175, 50), Point3D(-40, -200, 50), Point3D(-40, -175, -15), Point3D(-40, -200, -15));
    characterCor[2] = Solid(Point3D(70, -200, 70), Point3D(70, -300, 70), Point3D(70, -200, -20), Point3D(70, -300, -20),
                            Point3D(-70, -200, 70), Point3D(-70, -300, 70), Point3D(-70, -200, -20), Point3D(-70, -300, -20));

    characterLegArmCor[30][0] = Solid(Point3D(-100, 40, 80), Point3D(-100, -175, 80), Point3D(-100, 40, -30), Point3D(-100, -175, -30),
                                      Point3D(-150, 40, 80), Point3D(-150, -175, 80), Point3D(-150, 40, -30), Point3D(-150, -175, -30));
    characterLegArmCor[30][1] = characterLegArmCor[30][0] + Point3D(250, 0, 0);

    characterLegArmCor[30][2] = Solid(Point3D(0, 285, 80), Point3D(0, 85, 80), Point3D(0, 285, -30), Point3D(0, 85, -30),
                                      Point3D(-100, 285, 80), Point3D(-100, 85, 80), Point3D(-100, 285, -30), Point3D(-100, 85, -30));
    characterLegArmCor[30][3] = characterLegArmCor[30][2] + Point3D(100, 0, 0);

    int y, z;
    for(int j = 31; j < 61; j++){
        for(int i = 0; i < 4; i++){
            characterLegArmCor[j][i] = characterLegArmCor[30][i];
            characterLegArmCor[j-31][i] = characterLegArmCor[30][i];
        }
        // poruszanie sie rak (wszystkie mozliwe stany):
        for(int i = 0; i < 8; i+=2){
            y = characterLegArmCor[30][0].solidVertices[i].y;
            z = characterLegArmCor[30][0].solidVertices[i].z;
            characterLegArmCor[j][0].solidVertices[i].z = z * cos((30-j) * M_PI/180) - y * sin((30-j) * M_PI/180);
            characterLegArmCor[j][0].solidVertices[i].y = y * cos((30-j) * M_PI/180) + z * sin((30-j) * M_PI/180);


            characterLegArmCor[j-31][0].solidVertices[i].z = z * cos((61-j) * M_PI/180) - y * sin((61-j) * M_PI/180);
            characterLegArmCor[j-31][0].solidVertices[i].y = y * cos((61-j) * M_PI/180) + z * sin((61-j) * M_PI/180);

            y = characterLegArmCor[30][1].solidVertices[i].y;
            z = characterLegArmCor[30][1].solidVertices[i].z;
            characterLegArmCor[j][1].solidVertices[i].z = z * cos((j-30) * M_PI/180) - y * sin((j-30) * M_PI/180);
            characterLegArmCor[j][1].solidVertices[i].y = y * cos((j-30) * M_PI/180) + z * sin((j-30) * M_PI/180);

            characterLegArmCor[j-31][1].solidVertices[i].z = z * cos((j-61) * M_PI/180) - y * sin((j-61) * M_PI/180);
            characterLegArmCor[j-31][1].solidVertices[i].y = y * cos((j-61) * M_PI/180) + z * sin((j-61) * M_PI/180);
        }
        // poruszanie sie nog (wszystkie mozliwe stany):
        for(int i = 0; i < 8; i+=2){
            y = characterLegArmCor[30][2].solidVertices[i].y;
            z = characterLegArmCor[30][2].solidVertices[i].z;
            characterLegArmCor[j][2].solidVertices[i].z = z * cos((30-j) * M_PI/180) - y * sin((30-j) * M_PI/180);
            characterLegArmCor[j][2].solidVertices[i].y = y * cos((30-j) * M_PI/180) + z * sin((30-j) * M_PI/180);


            characterLegArmCor[j-31][2].solidVertices[i].z = z * cos((61-j) * M_PI/180) - y * sin((61-j) * M_PI/180);
            characterLegArmCor[j-31][2].solidVertices[i].y = y * cos((61-j) * M_PI/180) + z * sin((61-j) * M_PI/180);

            y = characterLegArmCor[30][3].solidVertices[i].y;
            z = characterLegArmCor[30][3].solidVertices[i].z;
            characterLegArmCor[j][3].solidVertices[i].z = z * cos((j-30) * M_PI/180) - y * sin((j-30) * M_PI/180);
            characterLegArmCor[j][3].solidVertices[i].y = y * cos((j-30) * M_PI/180) + z * sin((j-30) * M_PI/180);

            characterLegArmCor[j-31][3].solidVertices[i].z = z * cos((j-61) * M_PI/180) - y * sin((j-61) * M_PI/180);
            characterLegArmCor[j-31][3].solidVertices[i].y = y * cos((j-61) * M_PI/180) + z * sin((j-61) * M_PI/180);
        }
    }
}

// Zapisanie aktualnych wspolrzednych ludzika:
void MainWindow::SetCurrentCor(int angle)
{
    for(int i = 0; i < 3; i++){
        currentCharacterCor[i] = characterCor[i];
    }
    for(int i = 0; i < 4; i++){
        currentCharacterCor[i + 3] = characterLegArmCor[angle][i]; // tutaj wpisujemy aktualne polozenie rak/nog (angle)
    }
}

//void MainWindow::SortSolid()
//{
//    Ui::SolidCenter tmp;
//    int n = 7;
//    do{
//        for(int i = 0; i < 6; i++){
//            if(solidCenter[i].centerZ < solidCenter[i+1].centerZ){
//                tmp = solidCenter[i+1];
//                solidCenter[i+1] = solidCenter[i];
//                solidCenter[i] = tmp;
//            }
//        }
//        n = n - 1;
//    } while(n > 1);
//}

//void MainWindow::GetCenter()
//{
//    int z;
//    Ui::SolidCenter s;
//    for(int i = 0; i < 7; i++){
//        z = (currentCharacterCor[i].solidVertices[0].z + currentCharacterCor[i].solidVertices[7].z)/2;
//        s.centerZ = z;
//        s.numberOfObject = i;
//        solidCenter[i] = s;
//    }
//    SortSolid();
//    for( int j = 0 ; j < 7; j++){
//        qDebug("%d\n", solidCenter[j].numberOfObject);
//    }
//}

// Sortowanie tablicy 'wallCenter' wzgledem wspolrzednej 'centerZ' (dostajemy wlasciwa kolejnosc rysowania scian):
void MainWindow::SortWalls()
{
    Ui::WallCenter tmp;
    int n = 42;
    do{
        for(int i = 0; i < 41; i++){
            if(wallCenter[i].centerZ < wallCenter[i+1].centerZ){
                tmp = wallCenter[i+1];
                wallCenter[i+1] = wallCenter[i];
                wallCenter[i] = tmp;
            }
        }
        n = n - 1;
    } while(n > 1);
}

// Wyznaczanie srodkow kazdej ze scian:
void MainWindow::GetCenterWall()
{
    int z;
    char wall;
    Ui::WallCenter s;
    for(int i = 0; i < 7; i++){
        // prawa
        wall = 'r';
        z = (currentCharacterCor[i].solidVertices[0].z + currentCharacterCor[i].solidVertices[3].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i] = s;
        // lewa
        wall = 'l';
        z = (currentCharacterCor[i].solidVertices[4].z + currentCharacterCor[i].solidVertices[7].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i + 1] = s;
        // przod
        wall = 'f';
        z = (currentCharacterCor[i].solidVertices[2].z + currentCharacterCor[i].solidVertices[7].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i + 2] = s;
        // tyl
        wall = 'b';
        z = (currentCharacterCor[i].solidVertices[0].z + currentCharacterCor[i].solidVertices[5].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i + 3] = s;
        // gora
        wall = 'u';
        z = (currentCharacterCor[i].solidVertices[1].z + currentCharacterCor[i].solidVertices[7].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i + 4] = s;
        // dol
        wall = 'd';
        z = (currentCharacterCor[i].solidVertices[0].z + currentCharacterCor[i].solidVertices[6].z)/2;
        s.centerZ = z;
        s.numberOfObject = i;
        s.typeOfWall = wall;
        wallCenter[6*i + 5] = s;
    }
    SortWalls();
}

void MainWindow::Draw()
{
    img.fill(0xFFFFFF);
    //img.fill(0x000000);

    SetCurrentCor(angle);

    for ( int i = 0; i < 7; ++i ){
        tmpCor = currentCharacterCor[i];
        Rotation(rot);
        currentCharacterCor[i] = tmpCor;
    }
    //GetCenter();
    GetCenterWall();
    DrawGrid();
    update();
}

// Rysowanie siatki badz tekstury po wykonaniu rzutu perspektywicznego:
void MainWindow::DrawGrid()
{
    double x2d, y2d;
    int z;
    Point vertices2d[8];

    // Rzut perspektywiczny:
    for(int j = 0; j < 7; j++){
        for(int i = 0; i < 8; i++){
            z = 256 + currentCharacterCor[j].solidVertices[i].z;
            x2d = currentCharacterCor[j].solidVertices[i].x * (double)d/(z + d - k) + width/2; // d - odleglosc od obserwatora
            y2d = currentCharacterCor[j].solidVertices[i].y * (double)d/(z + d - k) + height/2;
            vertices2d[i] = Point(x2d, y2d);
            vertices2dAll[j][i] = Point(x2d, y2d);
        }
        if(checkGrid->isChecked()) DrawSegments(vertices2d);
    }
    if(checkTexture->isChecked()) DrawTexture();
    update();
}

// Rysujemy poszczegolne odcinki (proste) tworzace siatke:
void MainWindow::DrawSegments(Point vertices2d[])
{
    DrawSegment(vertices2d[1].x, vertices2d[1].y, vertices2d[0].x, vertices2d[0].y);
    DrawSegment(vertices2d[1].x, vertices2d[1].y, vertices2d[5].x, vertices2d[5].y);

    DrawSegment(vertices2d[1].x, vertices2d[1].y, vertices2d[3].x, vertices2d[3].y);
    DrawSegment(vertices2d[0].x, vertices2d[0].y, vertices2d[4].x, vertices2d[4].y);

    DrawSegment(vertices2d[0].x, vertices2d[0].y, vertices2d[2].x, vertices2d[2].y);
    DrawSegment(vertices2d[2].x, vertices2d[2].y, vertices2d[3].x, vertices2d[3].y);

    DrawSegment(vertices2d[2].x, vertices2d[2].y, vertices2d[6].x, vertices2d[6].y);
    DrawSegment(vertices2d[3].x, vertices2d[3].y, vertices2d[7].x, vertices2d[7].y);

    DrawSegment(vertices2d[4].x, vertices2d[4].y, vertices2d[6].x, vertices2d[6].y);
    DrawSegment(vertices2d[4].x, vertices2d[4].y, vertices2d[5].x, vertices2d[5].y);

    DrawSegment(vertices2d[5].x, vertices2d[5].y, vertices2d[7].x, vertices2d[7].y);
    DrawSegment(vertices2d[6].x, vertices2d[6].y, vertices2d[7].x, vertices2d[7].y);

    update();
}

// Funkcja odpowiadajaca za obrot ludzika (obracamy konkretna bryle). Obracamy wzgledem osi Ox i Oy:
void MainWindow::Rotation(Point3D rot)
{
    int x, y, z;
    for(int i = 0; i < 8; i++){
        x = tmpCor.solidVertices[i].x;
        y = tmpCor.solidVertices[i].y;
        z = tmpCor.solidVertices[i].z;
        tmpCor.solidVertices[i].x = x * cos(rot.y * M_PI/180) + z * sin(rot.y * M_PI/180);
        tmpCor.solidVertices[i].y = x * sin(rot.x * M_PI/180) * sin(rot.y * M_PI/180) +
                                    y * cos(rot.x * M_PI/180) - z * sin(rot.x * M_PI/180) * cos(rot.y * M_PI/180);
        tmpCor.solidVertices[i].z = -x * cos(rot.x * M_PI/180) * sin(rot.y * M_PI/180) +
                                    y * sin(rot.x * M_PI/180) + z * cos(rot.x * M_PI/180) * cos(rot.y * M_PI/180);
    }
    update();
}

// Funkcja "uruchamiajaca" teksturowanie scian (sciany rysujemy w odpowiedniej kolejnosci ustalonej w tablicy 'wallCenter'):
void MainWindow::DrawTexture()
{
    for(int i = 0; i < 42; i++){
        DrawWalls(vertices2dAll[wallCenter[i].numberOfObject], wallCenter[i].numberOfObject, wallCenter[i].typeOfWall);
    }
}

// Teksturujemy odpowiednia sciane:
void MainWindow::DrawWalls(Point punkty[8], int i, char wall){
    int l, r, f, b, u, d;

    switch(i){ // sprawdzamy z ktora czescia ludzika mamy do czynienia (glowa, szyja, tulow, nogi, rece),
               // na tej podstawie okreslamy ktore tekstury musimy w danym momencie "pobrac"
    case 0:
        l = 13;
        r = 13;
        f = 5;
        b = 2;
        u = 3;
        d = 14;
        break;
    case 1:
        l = 14;
        r = 14;
        f = 14;
        b = 14;
        u = 14;
        d = 14;
        break;
    case 2:
        l = 8;
        r = 7;
        f = 4;
        b = 6;
        u = 6;
        d = 14;
        break;
    case 3:
        l = 0;
        r = 0;
        f = 0;
        b = 0;
        u = 1;
        d = 14;
        break;
    case 4:
        l = 0;
        r = 0;
        f = 0;
        b = 0;
        u = 1;
        d = 14;
        break;
    case 5:
        l = 11;
        r = 11;
        f = 10;
        b = 11;
        u = 12;
        d = 9;
        break;
    case 6:
        l = 11;
        r = 11;
        f = 10;
        b = 11;
        u = 12;
        d = 9;
        break;
    }

    if(wall == 'r'){ // sciana prawo
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 2, 1, 3, r);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 2, 1, 0, r);
    }
    if(wall == 'l'){ // sciana lewo
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 4, 7, 5, l);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 4, 7, 6, l);
    }
    if(wall == 'b'){ // sciana tyl
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 0, 5, 1, b);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 0, 5, 4, b);
    }
    if(wall == 'f'){ // sciana przod
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 6, 3, 7, f);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 6, 3, 2, f);
    }
    if(wall == 'u'){ // sciana gora
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 7, 1, 5, u);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 7, 1, 3, u);
    }
    if(wall == 'd'){ // sciana dol
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 0; p3.y = 0;
        Morphing(punkty, 4 , 2, 6, d);
        p1.x = 0; p1.y = 250; p2.x = 250; p2.y = 0; p3.x = 250; p3.y = 250;
        Morphing(punkty, 4 , 2, 0, d);
    }
}

// Zamiana kolejnosci wierzcholkow w trojkacie (trojkat o wierzcholkach podanych w argumencie)
// (funkcja wykorzystywana w Morphing()):
bool MainWindow::ReplaceVertex(Point repVertexTab[3]){
    if(repVertexTab[0].y == repVertexTab[1].y && repVertexTab[1].y == repVertexTab[2].y)
        return false;

    if(repVertexTab[0].y > repVertexTab[1].y){
        if(repVertexTab[1].y > repVertexTab[2].y){
        }
        else if(repVertexTab[0].y > repVertexTab[2].y)
            repVertexTab[1].swapPoint(repVertexTab[2] );
        else{
            repVertexTab[0].swapPoint(repVertexTab[1]);
            repVertexTab[0].swapPoint(repVertexTab[2]);
        }
    }
    else{
        if(repVertexTab[0].y > repVertexTab[2].y)
            repVertexTab[0].swapPoint(repVertexTab[1]);
        else if(repVertexTab[1].y > repVertexTab[2].y){
            repVertexTab[1].swapPoint(repVertexTab[0]);
            repVertexTab[1].swapPoint(repVertexTab[2]);
        }
        else repVertexTab[2].swapPoint(repVertexTab[0] );
    }
    return true;
}

// Kolorowanie scian - pobieramy pixel na pozycji x, y w teksturze i zamalowujemy odpowiedni pixel w bitImg
// (funkcja wykorzystywana w Morphing()):
void MainWindow::PutPixelTex(double u, double v, double w, int i, int j, int k){
    double x, y;

    if ( u < 0 || v < 0 || w < 0 || u > 1 || v > 1 || w > 1 ){ // wspolrzedne u,v,w musza byc z przedzialu [0,1]
    }
    else {
        x = u*p1.x + v*p2.x + w*p3.x;
        y = u*p1.y + v*p2.y + w*p3.y;

        if (i < width && i >= 0 && j < height && j >= 0 && round(x+0.5) < 250 &&
             round(x+0.5) >= 0 && round(y+0.5) < 250 && round(y+0.5) >= 0){
            bitImg[i*4 + j*width*4] = texturesBits[k][int(round(x))*4 + int(round(y))*250*4];
            bitImg[i*4 + j*width*4 + 1] = texturesBits[k][int(round(x))*4 + int(round(y))*250*4 + 1];
            bitImg[i*4 + j*width*4 + 2] = texturesBits[k][int(round(x))*4 + int(round(y))*250*4 + 2];
        }
    }
}

// Teksturowanie:
void MainWindow::Morphing(Point punkty[], int a, int b, int c, int k){ // argument k to liczba okreslajaca teksture
                                                                       // (numer obrazka w tablicy 'textures')
    Point q1 = punkty[a], q2 = punkty[b], q3 = punkty[c];
    Point repVertexTab[3] = {q1, q2, q3};
    double u, v, w, tmp; // wyznaczenie punktu na teksturze
    double a1, b1, a2, b2, a3, b3; // wspolczynniki poszczegolnych prostych

    if (!ReplaceVertex(repVertexTab))
        return;

    //wspolczynniki prostych:
    a1 = double(repVertexTab[2].x - repVertexTab[0].x)/(repVertexTab[2].y - repVertexTab[0].y);
    b1 = repVertexTab[2].x - a1 * repVertexTab[2].y;

    if (repVertexTab[1].y - repVertexTab[0].y != 0){
        a2 = double(repVertexTab[1].x - repVertexTab[0].x)/(repVertexTab[1].y - repVertexTab[0].y);
    } else {
        a2 = 0;
    }
    b2 = repVertexTab[1].x - a2 * repVertexTab[1].y;

    if (repVertexTab[2].y - repVertexTab[1].y != 0){
        a3 = double(repVertexTab[2].x - repVertexTab[1].x)/(repVertexTab[2].y - repVertexTab[1].y);
    } else {
        a3 = 0;
    }
    b3 = repVertexTab[2].x - a3 * repVertexTab[2].y;

    // Dzielimy trojkat na dwie czesci i wyznaczamy 'u', 'v', 'w' dla kazdej z nich:
    for(int j = repVertexTab[2].y; j <= repVertexTab[1].y; ++j){
        for(int i = floor(a3*j + b3); i <= floor(a1*j + b1); ++i){
            tmp = (q2.x - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(q2.y - q1.y);

            v = (i - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(j - q1.y);
            v = v/tmp;
            w = (q2.x - q1.x)*(j - q1.y) - (i - q1.x)*(q2.y - q1.y);
            w = w/tmp;
            u = 1 - w - v;

            PutPixelTex(u, v, w, i, j, k);
        }
        for(int i = a1*j + b1; i <= a3*j + b3; ++i){
            tmp = (q2.x - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(q2.y - q1.y);

            v = (i - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(j - q1.y);
            v = v/tmp;
            w = (q2.x - q1.x)*(j - q1.y) - (i - q1.x)*(q2.y - q1.y);
            w = w/tmp;
            u = 1 - w - v;

            PutPixelTex(u, v, w, i, j, k);
        }
    }
   for(int j = repVertexTab[1].y; j <= repVertexTab[0].y; ++j){
       for(int i = floor(a2*j + b2); i <= floor(a1*j + b1); ++i){
           tmp = (q2.x - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(q2.y - q1.y);

           v = (i - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(j - q1.y);
           v = v/tmp;
           w = (q2.x - q1.x)*(j - q1.y) - (i - q1.x)*(q2.y - q1.y);
           w = w/tmp;
           u = 1 - w - v;

           PutPixelTex(u, v, w, i, j, k);
       }
       for(int i = a1*j + b1; i <= a2*j + b2; ++i){
           tmp = (q2.x - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(q2.y - q1.y);

           v = (i - q1.x)*(q3.y - q1.y) - (q3.x - q1.x)*(j - q1.y);
           v = v/tmp;
           w = (q2.x - q1.x)*(j - q1.y) - (i - q1.x)*(q2.y - q1.y);
           w = w/tmp;
           u = 1 - w - v;

           PutPixelTex(u, v, w, i, j, k);
        }
   }
   update();
}
