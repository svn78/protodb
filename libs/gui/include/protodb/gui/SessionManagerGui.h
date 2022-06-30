#pragma once

#include <protodb/SessionManager.h>

#include <QDialog>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QAbstractButton;
class QCheckBox;
class QTextBrowser;
class QLineEdit;
class QSortFilterProxyModel;
class QItemSelection;

class SessionManagerGui: public QDialog
{
    Q_OBJECT

public:
    SessionManagerGui(QWidget* parent = nullptr);
   ~SessionManagerGui() = default;

    void setSessionManager(SessionManager* sm);
    SessionManager* sessionManager() const;

private:
    void create_gui();
    void create_connections();

private slots:
    void onCreateClicked();
    void onChangeClicked();
    void onRmClicked();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    SessionManager* m_sm = nullptr;

    QSortFilterProxyModel* m_proxy_model;

    QPushButton* m_close_btn;
    QTextBrowser* m_desc_browser;
    QTableView*   m_sessions_table;
    QLineEdit*    m_filter_le;
    QPushButton*  m_create_btn;
    QPushButton*  m_change_btn;
    QPushButton*  m_copy_btn;
    QPushButton*  m_rm_btn;
    QPushButton*  m_select_btn;
};