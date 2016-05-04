#pragma once
#include <QtWidgets>
#define GrWIDTH  1000
#define GrHEIGHT 600
#define WiWIDTH  GrWIDTH+30
#define WiHEIGHT GrHEIGHT+100
#define BgCOLOR   Qt::black
#define LineTYPE  Qt::SolidLine
#define AxisCOLOR  Qt::yellow
#define TextCOLOR Qt::lightGray
#define NamesCOLOR Qt::red
#define GraphCOLOR Qt::yellow
#define LineWIDTH 1

class mainWindow : public QWidget
{
    Q_OBJECT
    protected:
        QApplication *papp;
        QStringList *parg;
        QString fileName;
        QGraphicsScene *pscene;
        QGraphicsView *pview;
        int xx;
        int yy;
        int start;
        QPen *TextColorPen;
        QPen *AxisColorPen;
        QPen *GraphColorPen;
        void drawGraphic();
    public:
        mainWindow(QApplication *, QStringList *, QWidget * = 0);
        void setAxisNames(QString, QString);
        void setAxisRanges(double,double,double,double);
        void drawPoint(int, int);
        void alert(QString);
    public slots:
        void askFileName();
    protected:
        virtual void dragMoveEvent(QDragEnterEvent *);
        virtual void dragEnterEvent(QDragEnterEvent *);
        virtual void dropEvent(QDropEvent *);
        void keyPressEvent(QKeyEvent *);
};

extern void getData(QString, mainWindow *);
