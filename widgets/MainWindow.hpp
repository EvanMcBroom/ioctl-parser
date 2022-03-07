#pragma once
#include <HexSpinBox.hpp>
#include <QStandardItem>
#include <QTreeView>
#include <QVector>
#include <ui_MainWindow.h>

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    static QPixmap defaultWindowIcon();

private Q_SLOTS:
    void on_checkBoxCommonBit_stateChanged(int state);
    void on_checkBoxCustomBit_stateChanged(int state);
    void on_comboBoxAccess_currentIndexChanged(int index);
    void on_comboBoxTransferMethod_currentIndexChanged(int index);
    void on_spinBoxDeviceType_valueChanged(int value);
    void on_spinBoxFunction_valueChanged(int value);
    void on_spinBoxIoctlCode_valueChanged(int value);

private:
};