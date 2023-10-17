#include "controller.h"
#include <QDebug>
#include <QtMath>

Controller::Controller(QWidget *parent, const std::vector<ColorSystem>& data, QWidget *displayWidget, QStatusBar *_statusBar):
    QWidget(parent), colorDisplayWidget(displayWidget), statusBar(_statusBar)
{
    selectedColorPalette = new QPalette;
    setMainColor(Qt::white);
    vLayout = nullptr;
    for (int i = 0; i < 3; i++)
        systems[i] = nullptr;

    mainRGBvalues = {95.047, 100, 108.883};
    ChangeSystems(data);
}
void Controller::ChangeSystems(const std::vector<ColorSystem>& data)
{
    if (vLayout)
        delete vLayout;
    vLayout = new QVBoxLayout(this);
    for (int i = 0; i < 3; i++)
    {
        if (systems[i])
        {
            disconnect(systems[i], &colorSystem::systemValueChanged, this, &Controller::OnChangeSystemValues);
            disconnect(systems[i], &colorSystem::systemSliderActivated, this, &Controller::OnSystemSliderActivated);
            delete systems[i];
        }
        switch (data[i])
        {
            case ColorSystem::RGB:
                systems[i] = new RGBSystem(this, i);
            break;
            case ColorSystem::CMYK:
                systems[i] = new CMYKSystem(this, i);
            break;
            case ColorSystem::HSV:
                systems[i] = new HSVSystem(this, i);
            break;
        }
        vLayout->addWidget(systems[i]);
        connect(systems[i], &colorSystem::systemValueChanged, this, &Controller::OnChangeSystemValues);
        connect(systems[i], &colorSystem::systemSliderActivated, this, &Controller::OnSystemSliderActivated);
    }
    QColor color = getMainColor();
    OnChangeSystemValues({color.redF(), color.greenF(), color.blueF()}, -1);
}
QColor Controller::getMainColor() const
{
    QColor c;
    c.setRgbF(mainRGBvalues[0], mainRGBvalues[1], mainRGBvalues[2]);
    return c;
}
void Controller::OnSystemSliderActivated(int systemId)
{
    for(int i = 0; i < 3; i++)
        if (i != systemId)
            systems[i] -> DisableSlider();

}

void Controller::ChangeSystemValues(const std::vector<qreal> &newRGBvalues, int systemId)
{
    QColor newColor;
    newColor.setRgbF(newRGBvalues[0], newRGBvalues[1], newRGBvalues[2]);
    colorSystem *currentSystem = systems[systemId];
    switch (currentSystem->getSystemType())
    {
    case ColorSystem::RGB:
        currentSystem -> ChangeFieldValue(newColor.redF() * 255, 0);
        currentSystem -> ChangeFieldValue(newColor.greenF() * 255, 1);
        currentSystem -> ChangeFieldValue(newColor.blueF() * 255, 2);
        break;
    case ColorSystem::CMYK:
        currentSystem -> ChangeFieldValue(newColor.cyanF() * 255, 0);
        currentSystem -> ChangeFieldValue(newColor.magentaF() * 255, 1);
        currentSystem -> ChangeFieldValue(newColor.yellowF() * 255, 2);
        currentSystem -> ChangeFieldValue(newColor.blackF() * 255, 3);
        break;
    case ColorSystem::HSV:
        currentSystem -> ChangeFieldValue(newColor.hueF() * 360, 0);
        currentSystem -> ChangeFieldValue(newColor.saturationF() * 255, 1);
        currentSystem -> ChangeFieldValue(newColor.valueF() * 255, 2);
        break;
    }
}
void Controller::OnChangeSystemValues(const std::vector<qreal>& newValues, int systemId)
{
    QColor newColor;
    std::vector<qreal> newRGBvalues;
    if (systemId != -1)
    {
        switch (systems[systemId]->getSystemType())
        {
        case ColorSystem::RGB:
            newColor.setRgbF(newValues[0] / 255, newValues[1] / 255, newValues[2] / 255);
            newRGBvalues = {newColor.redF(), newColor.greenF(), newColor.blueF()};
            break;
        case ColorSystem::CMYK:
            newColor.setCmykF(newValues[0] / 255, newValues[1] / 255, newValues[2] / 255, newValues[3] / 255);
            newRGBvalues = {newColor.redF(), newColor.greenF(), newColor.blueF()};
            break;

        case ColorSystem::HSV:
            newColor.setHsvF(newValues[0] / 360, newValues[1] / 255, newValues[2] / 255);
            newRGBvalues = {newColor.redF(), newColor.greenF(), newColor.blueF()};
            break;
        }
    }
    else
       newRGBvalues = newValues;

    for (int i = 0; i < 3; i++)
        if (i != systemId)
            ChangeSystemValues(newRGBvalues, i);

    mainRGBvalues = newRGBvalues;
    newColor.setRgbF(newRGBvalues[0], newRGBvalues[1], newRGBvalues[2]);
    setMainColor(newColor);
}
void Controller::setMainColor(const QColor &newColor)
{
    if (newColor.isValid())
    {
        selectedColorPalette -> setColor(QPalette::Window, newColor);
        mainRGBvalues = {newColor.redF(), newColor.greenF(), newColor.blueF()};
        colorDisplayWidget -> setPalette(*selectedColorPalette);
    }
}
Controller::~Controller()
{
    delete vLayout;
    for (int i = 0; i < 3; i++)
        delete systems[i];

    delete selectedColorPalette;
}
