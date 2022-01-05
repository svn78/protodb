#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <protodb/PluginManagerDialog.h>

PluginManagerDialog::PluginManagerDialog(QWidget* parent)
    : QDialog(parent)
{
    createGui();
    connectSignals();

    setMinimumSize(640, 480);
}

void PluginManagerDialog::createGui()
{
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok |
                                             QDialogButtonBox::Apply |
                                             QDialogButtonBox::Cancel |
                                             QDialogButtonBox::Reset );

    m_filter_line = new QLineEdit();
        m_filter_line->setPlaceholderText(tr("Find plugin"));

    m_tree = new QTreeWidget();

    auto m_layout = new QVBoxLayout();
        m_layout->addWidget(m_filter_line);
        m_layout->addWidget(m_tree);
        m_layout->addWidget(m_dialog_buttons);

    setLayout(m_layout);
}

void PluginManagerDialog::connectSignals()
{

}
