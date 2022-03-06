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
    void on_spinBoxIoctlCode_valueChanged(int value);

private:
};