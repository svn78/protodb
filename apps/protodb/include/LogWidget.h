#pragma once

#include "Logger.h"
#include <QWidget>

class LogTableView;
class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class ConnectionConfigDialog;
class Logger;
class LogDecorationDialog;
class LogProxyModel;
class LuaApi;

class LogWidget: public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget() = default;

signals:
    void sByteFormatChanged(Logger::ByteFormat format);

private:
    void createGui();
    void createConnections();

private:
     QPushButton* m_mode_btn;
     QPushButton* m_clr_btn;
     QPushButton* m_run;
     QPushButton* m_cfg_btn;
     QLineEdit* m_find_le;
     QLineEdit* m_msg_le;
     LogTableView* m_view;
     ConnectionConfigDialog* m_conn_dialog;
    // LogDecorationDialog* m_decoration_dialog;

     LuaApi* m_lua_api;
     Logger* m_log_model;
     LogProxyModel* m_log_proxy_model;
};

