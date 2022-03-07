#include <MainWindow.hpp>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi(this);
    setWindowIcon(MainWindow::defaultWindowIcon());
    setWindowTitle(QCoreApplication::applicationName());

    spinBoxDeviceType->setBitWidth(15);
    spinBoxFunction->setBitWidth(11);

    // Layout the table of bytes
    tableWidgetBytes->setFont(QFont("Source Code Pro", 10));

}

QPixmap MainWindow::defaultWindowIcon() {
    return QPixmap(":images/gear_icon.png");
}

void MainWindow::on_checkBoxCommonBit_stateChanged(int state) {
    spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~(1 << 31)) | (((state == Qt::CheckState::Checked) ? 1 : 0) << 31));
}

void MainWindow::on_checkBoxCustomBit_stateChanged(int state) {
    spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~(1 << 13)) | (((state == Qt::CheckState::Checked) ? 1 : 0) << 13));
}

void MainWindow::on_comboBoxAccess_currentIndexChanged(int index) {
    if (index != -1)
        spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~0xC000) | (index << 14));
}

void MainWindow::on_comboBoxTransferMethod_currentIndexChanged(int index) {
    if (index != -1)
        spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~0x3) | index);
}

void MainWindow::on_spinBoxDeviceType_valueChanged(int value) {
    spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~0x7FFF0000) | (value << 16));
}

void MainWindow::on_spinBoxFunction_valueChanged(int value) {
    spinBoxIoctlCode->setValue((spinBoxIoctlCode->value() & ~0x1FFC) | (value << 2));
}

void MainWindow::on_spinBoxIoctlCode_valueChanged(int value) {
    checkBoxCommonBit->setChecked(value & (1 << 31));
    checkBoxCustomBit->setChecked(value & (1 << 13));
    comboBoxAccess->setCurrentIndex((value & 0xC000) >> 14);
    comboBoxTransferMethod->setCurrentIndex(value & 0x3);
    spinBoxDeviceType->setValue((value & 0x7FFF0000) >> 16);
    spinBoxFunction->setValue((value & 0x1FFC) >> 2);
}