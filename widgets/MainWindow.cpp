#include <MainWindow.hpp>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi(this);
    setWindowIcon(MainWindow::defaultWindowIcon());
    setWindowTitle(QCoreApplication::applicationName());

    // Layout the table of bytes
    tableWidgetBytes->setFont(QFont("Source Code Pro", 10));
}

QPixmap MainWindow::defaultWindowIcon() {
    return QPixmap(":images/gear_icon.png");
}

void MainWindow::on_spinBoxIoctlCode_valueChanged(int value) {
    spinBoxDeviceType->setValue(value);
}