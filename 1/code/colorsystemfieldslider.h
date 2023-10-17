#pragma once

#include <QWidget>
#include <QSlider>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>

class colorField;

class colorSlider : public QSlider
{
    Q_OBJECT
public:
    explicit colorSlider(QWidget *parent = nullptr);
    void ChangeActiveField(colorField *new_field);
    void ClearActiveField();
    colorField* GetActiveField() const;
    virtual ~colorSlider(){}
private:
    colorField *activeField = nullptr;
};

class colorField : public QLineEdit
{
    Q_OBJECT
public:
    explicit colorField(QWidget *parent = nullptr, colorSlider *control_slider = nullptr, qreal _left = 0, qreal _right = 100, qreal _id = 0);
    qreal getValue() const;
    void ChangeValue(qreal newValue);
    void setActive();
    virtual ~colorField(){}
public slots:
    void EnterPressed();
    void ChangeValueText(const QString& newValue);
    void ChangeValueFromSlider(int newValue);

signals:
    void valueChanged(qreal newValue, int fieldId);
    void sliderActivated();
private:
    int fieldId;
    qreal leftThreshold, rightThreshold;
    qreal value;
    colorSlider *slider;
    void mousePressEvent(QMouseEvent *) override;
};

