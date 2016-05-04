#include "mainwindow.h"

//QString _fileName = "";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("The simple test task (C++, Qt)");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.", "[file]");
    parser.process(app);
    QStringList args = parser.positionalArguments();

    new mainWindow(&app, &args);
    return app.exec();
}

void getData(QString fileName, mainWindow *view)
{
    QRegExp rxempty("^\\s*$");
    QRegExp rxhead("^#\\s+(\\w+)\\s+(\\w+).*$");
    QRegExp rxdata("^\\s*([Ee\\d\\-\\+\\.]+)\\s+([Ee\\d\\-\\+\\.]+)\\s*$");

    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString header = file.readLine();

    if(!rxhead.exactMatch(header)) {
        view->alert("Data file error: impossible to get the axis names!");
        file.close();
        return;
    } else {
        view->setAxisNames(rxhead.cap(1),rxhead.cap(2));
    };

    double Xmax = -10e30, Ymax = -10e30;
    double Xmin = 10e30, Ymin = 10e30;
    while (!file.atEnd())
    {
        QString data = file.readLine();
        if(!rxempty.exactMatch(data)) {
            if(!rxdata.exactMatch(data)) {
                view->alert("Data file error: impossible to read data from string");
                file.close();
                return;
            } else {
                double x=rxdata.cap(1).toDouble();
                double y=rxdata.cap(2).toDouble();
                Xmax = (Xmax > x)? Xmax : x;
                Ymax = (Ymax > y)? Ymax : y;
                Xmin = (Xmin < x)? Xmin : x;
                Ymin = (Ymin < y)? Ymin : y;
            }
        }
    }
    if(Xmax - Xmin <10e-30 || Xmax - Xmin <10e-30) {
        view->alert("Data file error: unreal data");
        file.close();
        return;
    }
    double NormX = GrWIDTH/(Xmax - Xmin);
    double NormY = GrHEIGHT/(Ymax - Ymin);

    view->setAxisRanges(Xmax, Ymax, Xmin, Ymin);
    file.seek(0);
    file.readLine();

    int k=0, xold=0;
    double yround=0;
    while (!file.atEnd())
    {
        QString data = file.readLine();
        if(rxempty.exactMatch(data)) { continue; }
        rxdata.indexIn(data);
        double x=rxdata.cap(1).toDouble() - Xmin;
        double y=rxdata.cap(2).toDouble() - Ymin;
        int xi = x * NormX;
        int yi = y * NormY;

        if(xold == xi) {
            k++;
            yround += y;
        } else {
            if (k == 0)  {
                yround = y;
                xold = xi;
            } else {
                yround /= k;
            }
            int xx = xold;
            int yy = yround * NormY;

            view->drawPoint(xx, yy);
            yround = 0;
            xold = xi;
            k=0;
        }
    }
    file.close();
}
