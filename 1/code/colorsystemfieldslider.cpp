#include "colorsystemfieldslider.h"
#include "QDebug"

colorField::colorField(QWidget *parent, colorSlider *control_slider, qreal _left, qreal _right, qreal _id) :
    QLineEdit(parent), fieldId(_id), leftThreshold(_left), rightThreshold(_right), slider(control_slider)
{
    slider -> setVisible(false);
    connect(this, &QLineEdit::returnPressed, this, &colorField::EnterPressed);
    connect(this, &QLineEdit::textChanged, this, &colorField::ChangeValueText);
    QString warningMessage = "This field must be in range [" + QString::number(leftThreshold) + "," + QString::number(rightThreshold) + "] with max two digits after comma";
    connect(this, &QLineEdit::inputRejected, this, [this, warningMessage](){QMessageBox::warning(this, "Error", warningMessage);});
    QDoubleValidator *val = new QDoubleValidator(leftThreshold, rightThreshold, 2, this);
    setValidator(val);
    ChangeValue(0);
}
void colorField::EnterPressed()
{
    slider -> setVisible(false);
    clearFocus();
}
qreal colorField::getValue() const
{
    return value;
}
void colorField::ChangeValue(qreal newValue)
{
    value = newValue;
    blockSignals(true);
    QString s = QString::number((int)(value * 100) / 100.0);
    setText(s.replace('.', ','));
    if (slider -> GetActiveField() == this)
        slider -> setValue((int)(value * 100));

    blockSignals(false);
}
void colorField::ChangeValueText(const QString& newValue)
{
    bool ok;
    double parsedDouble = newValue.toDouble(&ok);
    if (ok)
    {
        if (parsedDouble > rightThreshold)
            parsedDouble = rightThreshold;

        if (parsedDouble < leftThreshold)
            parsedDouble = leftThreshold;

        ChangeValue(parsedDouble);
        emit valueChanged(value, fieldId);
    }
}
void colorField::ChangeValueFromSlider(int newValue)
{
    ChangeValue(newValue / 100.0);
    emit valueChanged(value, fieldId);
}
void colorField::mousePressEvent(QMouseEvent *)
{
    if (slider -> GetActiveField())
        slider -> ClearActiveField();

    slider -> ChangeActiveField(this);
    slider -> setVisible(true);
    slider -> setRange((int)(leftThreshold * 100), (int)(rightThreshold * 100));
    slider -> setValue(value * 100);
    connect(slider, &QSlider::sliderReleased, this, &colorField::setActive);
    connect(slider, &QSlider::valueChanged, this, &colorField::ChangeValueFromSlider);
    emit sliderActivated();
}
void colorField::setActive()
{
    setFocus();
}
colorSlider::colorSlider(QWidget *parent) : QSlider(Qt::Horizontal, parent)
{
    setTracking(true);
}
colorField* colorSlider::GetActiveField() const
{
    return activeField;
}
void colorSlider::ClearActiveField()
{
    disconnect(this, &QSlider::sliderReleased, GetActiveField(), &colorField::setActive);
    disconnect(this, &QSlider::valueChanged, GetActiveField(), &colorField::ChangeValueFromSlider);
    activeField = nullptr;
}
void colorSlider::ChangeActiveField(colorField *new_field)
{
    activeField = new_field;
}
