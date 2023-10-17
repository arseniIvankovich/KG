#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLayout>
#include <colorsystemfieldslider.h>
#include <QLabel>
#include <vector>
#include <utility>

enum class ColorSystem
{
    RGB,
    CMYK,
    HSV
};

class colorSystem : public QWidget
{
    Q_OBJECT
public:
    explicit colorSystem(QWidget *parent = nullptr, int number_of_fields = 3, std::vector<std::pair<qreal, qreal>> ranges = {}, QString name = "Sample color system", int id = 0);
    void ChangeFieldValue(qreal newValue, int fieldId);
    void DisableSlider();
    virtual ColorSystem getSystemType() const = 0;
    virtual ~colorSystem();
public slots:
    void OnChangeFieldValue(qreal newValue, int fieldId);
    void SliderActivated();
signals:
    void systemValueChanged(std::vector<qreal> new_values, int systemId);
    void systemSliderActivated(int systemId);

private:
    colorField *fields[4];
    qreal values[4];
    QLabel *nameLabel;
    int size;
    int systemId;
    colorSlider *slider;
};

class RGBSystem : public colorSystem
{
public:
    explicit RGBSystem(QWidget *parent, int id);
    ColorSystem getSystemType() const override;
};
class CMYKSystem : public colorSystem
{
public:
    explicit CMYKSystem(QWidget *parent, int id);
    ColorSystem getSystemType() const override;
};
class HSVSystem : public colorSystem
{
public:
    explicit HSVSystem(QWidget *parent, int id);
    ColorSystem getSystemType() const override;
};


