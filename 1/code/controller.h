#pragma once
#include "colorsystem.h"
#include <QBoxLayout>
#include <QStatusBar>

class Controller : public QWidget
{
    Q_OBJECT
public:
    Controller(QWidget *parent, const std::vector<ColorSystem>& data, QWidget *displayWidget, QStatusBar *_statusBar);
    QColor getMainColor() const;
    void setMainColor(const QColor& newColor);
    void updateMainColor();
    void ChangeSystems(const std::vector<ColorSystem>& data);
    void ChangeSystemValues(const std::vector<qreal>& newValues, int systemId);
    ~Controller();
public slots:
    void OnChangeSystemValues(const std::vector<qreal>& newValues, int systemId);
    void OnSystemSliderActivated(int systemId);
private:
    std::vector<qreal> mainRGBvalues;
    QWidget *colorDisplayWidget;
    QStatusBar *statusBar;
    QVBoxLayout *vLayout = nullptr;
    colorSystem *systems[3];
    QPalette *selectedColorPalette;
};

