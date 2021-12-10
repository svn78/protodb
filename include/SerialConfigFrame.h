#pragma once

#include "ConfigFrame.h"

class QComboBox;
class QValidator;

class SerialConfigFrame: public ConfigFrame
{
  Q_OBJECT

public:
    SerialConfigFrame(QWidget *parent = nullptr);
    ~SerialConfigFrame() = default;

private:
    void createGui();
    void connectSignals();
    void test(int);

private:
    QValidator* m_baudrate_validator;
    QComboBox* m_baudrate;
    QComboBox* m_flow_ctrl;
    QComboBox* m_data_bits;
    QComboBox* m_parity;
    QComboBox* m_stop_bits;
    QComboBox* m_open_mode;
    QComboBox* m_device;
};
