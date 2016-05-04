#include "mainwindow.h"

mainWindow :: mainWindow(QApplication *_papp, QStringList *_parg, QWidget *pwg) : QWidget(pwg)
{
    papp = _papp;
    parg = _parg;

    QPushButton *btnFile = new QPushButton("Open another data file (Ctrl+o)");
    QPushButton *btnExit = new QPushButton("Exit (Ctrl+q)");
    QObject::connect(btnExit, SIGNAL(clicked()), papp, SLOT(quit()));
    QObject::connect(btnFile, SIGNAL(clicked()), this, SLOT(askFileName()));
    pscene = new QGraphicsScene(this);
    pscene->setBackgroundBrush(BgCOLOR);
    setAcceptDrops(true);
    pview = new QGraphicsView(pscene);
    QGraphicsTextItem *ptxt = pscene->addText("");
    ptxt -> setDefaultTextColor(TextCOLOR);
    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(btnFile);
    hl->addWidget(btnExit);
    vl->addLayout(hl);
    vl->addWidget(pview);
    setLayout(vl);
    QRect sg = QApplication::desktop()->screenGeometry(-1);
    move((sg.width()-WiWIDTH)/2, (sg.height()-WiHEIGHT)/2);
    setFixedSize(WiWIDTH, WiHEIGHT);
    show();

    TextColorPen = new QPen(QColor(TextCOLOR));
    AxisColorPen = new QPen(QColor(AxisCOLOR));
    GraphColorPen = new QPen (QColor(GraphCOLOR));
    if(!parg->isEmpty() && QFile(parg->at(0)).exists()) {
        fileName = parg->at(0);
        drawGraphic();
    }
}

void mainWindow :: askFileName()
{
    fileName = QFileDialog::getOpenFileName(this, "Open data file for the graf generation", "", "      *.dat (*.dat);;      * (*)");
    if (!fileName.isEmpty()) {
        drawGraphic();
    }
}
void mainWindow :: dragMoveEvent(QDragEnterEvent *pe) {
    if(pe->mimeData()->hasUrls()) {
        pe->acceptProposedAction();
        qDebug() << pe->mimeData();
    }
}
void mainWindow :: dragEnterEvent(QDragEnterEvent *pe) {
    if(pe->mimeData()->hasUrls()) {
        pe->acceptProposedAction();
        qDebug() << pe->mimeData();
    }
}
void mainWindow :: dropEvent(QDropEvent *pe){
    pe->acceptProposedAction();
    fileName = pe->mimeData()->urls()[0].toString();
    qDebug() << " +++ " << fileName;
}

void mainWindow :: keyPressEvent(QKeyEvent *pe) {
    if(pe->modifiers() & Qt::ControlModifier) {
        switch (pe->key()) {
            case Qt::Key_Q:
                papp->quit();
                break;
            case Qt::Key_O:
                askFileName();
                break;
        }
    }
}
void mainWindow :: alert(QString message)
{
    QMessageBox::critical(this,"Error!", message, QMessageBox::Cancel);
}
void mainWindow :: setAxisNames(QString xname, QString yname)
{
    QGraphicsTextItem *xn = pscene->addText(xname);
    QGraphicsTextItem *yn = pscene->addText(yname);
    xn -> setDefaultTextColor(NamesCOLOR);
    yn -> setDefaultTextColor(NamesCOLOR);
    xn->setPos(GrWIDTH-50,GrHEIGHT/2+20);
    yn->setPos(50, 30);
}
void mainWindow :: setAxisRanges(double x1,double y1,double x2,double y2)
{

}
void mainWindow :: drawPoint(int x, int y)
{
    y = GrHEIGHT-y;
    if(start == 1) {
        start = 0;
    } else {
        pscene->addLine(xx, yy, x, y, *GraphColorPen);
    }
    xx = x;
    yy = y;
}

void mainWindow :: drawGraphic() {
    pscene->clear();
    start = 1;
    pscene->addLine(0,GrHEIGHT/2,GrWIDTH,GrHEIGHT/2,*AxisColorPen);
    pscene->addLine(50,0,50,GrHEIGHT,*AxisColorPen);
    getData(fileName, this);
    pview->show();
}
