#include "redshiftqt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setOrganizationName(a.applicationName());
    RedShiftQt w;

    return a.exec();
}
