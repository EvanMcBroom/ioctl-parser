#include <MainWindow.hpp>
#include <QApplication>
#include <QFont.h>

// CTL_CODE macro (d4drvif.h)
// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/d4drvif/nf-d4drvif-ctl_code

// Defining I/O Control Codes
// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/defining-i-o-control-codes

void main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QObject::tr("IOCTL Parser"));
    QApplication::setOrganizationName(QObject::tr("Evan McBroom"));
    app.setFont(QFont("Open Sans", 10));
    MainWindow mainWindow;
    mainWindow.show();
    app.exec();
}