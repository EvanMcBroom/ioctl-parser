#include <MainWindow.hpp>
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi(this);
    setWindowIcon(QPixmap(":images/gear_icon.png"));
    setWindowTitle(QCoreApplication::applicationName());

    spinBoxDeviceType->setBitWidth(15);
    spinBoxFunction->setBitWidth(11);

    // Layout the table of bytes
    tableWidgetBytes->setFont(QFont("Source Code Pro", 10));
    tableWidgetBytes->verticalHeader()->hide();
    tableWidgetBytes->setColumnCount(32);
    tableWidgetBytes->setRowCount(2);
    for (size_t index{ 0 }; index < 32; index++) {
        if (index % 8 == 0) {
            auto item{ new QTableWidgetItem(QString::number(4 - (index / 8))) };
            item->setTextAlignment(Qt::AlignCenter);
            tableWidgetBytes->setItem(0, index, item);
            tableWidgetBytes->setSpan(0, index, 1, 8);
        }
        auto item{ new QTableWidgetItem() };
        item->setBackground(QBrush{ QColor(255, 255, 255) }); // Set to white
        tableWidgetBytes->setItem(1, index, item);
    }
    tableWidgetBytes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetBytes->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setDeviceMap(const DeviceMap& deviceMap) {
    this->deviceMap = deviceMap;
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

    if (deviceMap.contains(value & 0x7FFF0000)) {
        auto& deviceInfo{ deviceMap[value & 0x7FFF0000] };
        labelDeviceNameValue->setText(deviceInfo.first);
        labelFunctionNameValue->setText((deviceInfo.second.contains(value & 0x1FFC)) ? deviceInfo.second[value & 0x1FFC] : tr("Unknown"));
    }
    else {
        labelDeviceNameValue->setText(tr("Unknown"));
        labelFunctionNameValue->setText(tr("Unknown"));
    }

    // Highlight the set bits in the table of bytes
    QColor highlighted{ 170, 167, 255 };
    QColor white{ 255, 255, 255 };
    for (size_t index{ 0 }; index < 32; index++) {
        tableWidgetBytes->item(1, index)->setBackground(QBrush{ (value & (0x80000000 >> index)) ? highlighted : white });
    }
}