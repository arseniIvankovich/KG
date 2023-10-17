#include "colorsystem.h"
#include <QDebug>

colorSystem::colorSystem(QWidget *parent, int numberOfFields, std::vector<std::pair<qreal, qreal>> ranges, QString name, int _id) :
    QWidget(parent), size(numberOfFields), systemId(_id)
{
    slider = new colorSlider(this);
    QHBoxLayout *h = new QHBoxLayout;
    for (int i = 0; i < size; i++)
        fields[i] = nullptr;

    for (int i = 0; i < size; i++)
    {
        fields[i] = new colorField(this, slider, ranges[i].first, ranges[i].second, i);
        ChangeFieldValue(0, i);
        connect(fields[i], &colorField::valueChanged, this, &colorSystem::OnChangeFieldValue);
        connect(fields[i], &colorField::sliderActivated, this, &colorSystem::SliderActivated);
        if (i > 0)
            h -> addStretch(30);
        h -> addWidget(fields[i]);
    }
    QVBoxLayout *v = new QVBoxLayout(this);
    nameLabel = new QLabel;
    nameLabel -> setText(name);
    v -> addWidget(nameLabel);
    v -> addLayout(h);
    v -> addWidget(slider);
}
void colorSystem::SliderActivated()
{
    emit systemSliderActivated(systemId);
}
void colorSystem::DisableSlider()
{
    if (slider -> isVisible())
    {
        slider -> ClearActiveField();
        slider -> setVisible(false);
    }
}
void colorSystem::ChangeFieldValue(qreal newValue, int fieldId)
{
    values[fieldId] = newValue;
    fields[fieldId]->ChangeValue(newValue);
}
void colorSystem::OnChangeFieldValue(qreal newValue, int fieldId)
{
    ChangeFieldValue(newValue, fieldId);
    std::vector<qreal> newValues;
    for (int i = 0; i < size; i++)
        newValues.push_back(values[i]);

    emit systemValueChanged(newValues, systemId);
}
colorSystem::~colorSystem()
{
    for (int i = 0; i < size; i++)
        delete fields[i];

    delete slider;
    delete nameLabel;
}
RGBSystem::RGBSystem(QWidget *parent, int id):colorSystem(parent, 3, {{0, 255}, {0, 255}, {0, 255}}, "RGB", id)
{}
ColorSystem RGBSystem::getSystemType() const
{
    return ColorSystem::RGB;
}
CMYKSystem::CMYKSystem(QWidget *parent, int id):colorSystem(parent, 4, {{0, 255}, {0, 255}, {0, 255}, {0, 255}}, "CMYK", id)
{}
ColorSystem CMYKSystem::getSystemType() const
{
    return ColorSystem::CMYK;
}
HSVSystem::HSVSystem(QWidget *parent, int id):colorSystem(parent, 3, {{0, 360}, {0, 255}, {0, 255}}, "HSV", id)
{}
ColorSystem HSVSystem::getSystemType() const
{
    return ColorSystem::HSV;
}
