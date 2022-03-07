#pragma once
#include <QSpinBox>
#include <QWidget>

// Modified from: https://stackoverflow.com/a/26581445/11039217
class HexSpinBox : public QSpinBox {
public:
    HexSpinBox(QWidget *parent = 0) : QSpinBox(parent) {
        setPrefix("0x");
        setDisplayIntegerBase(16);
        setRange(INT_MIN, INT_MAX);
    }
    unsigned int hexValue() const {
        return u(value());
    }
    void setHexValue(unsigned int value) {
        setValue(i(value));
    }
    void setBitWidth(unsigned char width) {
        fillWidth = width >> 2;
        setRange(0, (1 << width) - 1);
    }
protected:
    QString textFromValue(int value) const {
        return QString("%1").arg(u(value), fillWidth, 16, QChar('0')).toUpper();
    }
    int valueFromText(const QString &text) const {
        return i(text.toUInt(0, 16));
    }
    QValidator::State validate(QString &input, int &pos) const {
        QString copy(input);
        if (copy.startsWith("0x"))
            copy.remove(0, 2);
        pos -= copy.size() - copy.trimmed().size();
        copy = copy.trimmed();
        if (copy.isEmpty())
            return QValidator::Intermediate;
        input = QString("0x") + copy.toUpper();
        bool okay;
        auto value{ copy.toUInt(&okay, 16) };
        if (!okay)
            return QValidator::Invalid;
        return QValidator::Acceptable;
    }

private:
    uchar fillWidth{ 8 };
    inline unsigned int u(int i) const {
        return *reinterpret_cast<unsigned int *>(&i);
    }
    inline int i(unsigned int u) const {
        return *reinterpret_cast<int *>(&u);
    }
};