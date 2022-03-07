#include <MainWindow.hpp>
#include <QApplication>
#include <QFont>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

// References:
// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/defining-i-o-control-codes
// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/d4drvif/nf-d4drvif-ctl_code

namespace {
    void AddToDeviceMap(DeviceMap& deviceMap, const QString& path) {
        QSettings settings{ path, QSettings::IniFormat };
        foreach (const QString& device, settings.childGroups()) {
            settings.beginGroup(device);
            foreach (const QString& function, settings.childKeys()) {
                bool ok;
                auto ioctl{ settings.value(function).toString().toUInt(&ok, 16) };
                if (ok) {
                    const uint deviceMask{ 0x7FFF0000 };
                    const uint functionMask{ 0x1FFC };
                    if (!deviceMap.contains(ioctl & deviceMask)) {
                        deviceMap[ioctl & deviceMask] = DeviceInfo{ device, FunctionMap{} };
                    }
                    deviceMap[ioctl & deviceMask].second[ioctl & functionMask] = function;
                }
            }
            settings.endGroup();
        }
    }
}

void main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QObject::tr("IOCTL Parser"));
    QApplication::setOrganizationName(QObject::tr("Evan McBroom"));
    app.setFont(QFont("Open Sans", 10));

    // Build the list of IOCTLs
    DeviceMap deviceMap;
    AddToDeviceMap(deviceMap, ":/IOCTLs.ini");
    for (int index{ 1 }; index < argc; index++) {
        if (QFileInfo::exists(argv[index]) && QFileInfo(argv[index]).isFile()) {
            AddToDeviceMap(deviceMap, argv[index]);
        }
    }

    MainWindow mainWindow;
    mainWindow.setDeviceMap(deviceMap);
    mainWindow.show();
    
    app.exec();
}