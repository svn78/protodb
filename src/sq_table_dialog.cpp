#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QDataWidgetMapper>
#include <QAbstractItemModel>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QDialogButtonBox>

#include "sq_model.h"
#include "sq_table_dialog.h"

SqTableDialog::SqTableDialog(QWidget* aParent)
    : QDialog(aParent)
    , mMapper(nullptr)
{
    createGui();
    connectSignals();
}

QDataWidgetMapper* SqTableDialog::mapper() const
{
    return mMapper;
}

void SqTableDialog::setMapper(QDataWidgetMapper* aMapper)
{
    if (mMapper != aMapper) {
        if ( mMapper != nullptr ) {
            mMapper->removeMapping(mNameLe);
            mMapper->removeMapping(mDescrEditor);
            mMapper->removeMapping(mSeqEditor);
            disconnect(mMapper);
        }

        mMapper = aMapper;
        mMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mMapper->addMapping(mNameLe, SqModel::kColumnSqName);
        mMapper->addMapping(mDescrEditor, SqModel::kColumnDescription);
        mMapper->addMapping(mSeqEditor, SqModel::kColumnCharStr);

        auto onIndexChanged = [this]() {
            itemLabel->setText(QString("%1/%2").arg(mMapper->currentIndex()+1).arg(mMapper->model()->rowCount()));
        };

        connect(mMapper, &QDataWidgetMapper::currentIndexChanged, this, onIndexChanged);
        onIndexChanged();
    }
}

void SqTableDialog::createGui()
{
    setWindowTitle(tr("Sequence Editor"));

    auto h_layout = new QHBoxLayout();

        btn_back  = new QPushButton();
            btn_back->setIcon(QIcon(":/icons/arrow_back.svg"));

        btn_prev  = new QPushButton();
            btn_prev->setIcon(QIcon(":/icons/arrow_left.svg"));

        btn_next  = new QPushButton();
            btn_next->setIcon(QIcon(":/icons/arrow_right.svg"));

        btn_front = new QPushButton();
            btn_front->setIcon(QIcon(":/icons/arrow_front.svg"));

        btn_add   = new QPushButton();
            btn_add->setIcon(QIcon(":/icons/add.svg"));

        itemLabel = new QLabel(tr("0/0"));

        h_layout->addWidget(btn_back);
        h_layout->addWidget(btn_prev);
        h_layout->addWidget(itemLabel);
        h_layout->addWidget(btn_next);
        h_layout->addWidget(btn_front);
        h_layout->addStretch();
        h_layout->addWidget(btn_add);

    mNameLe = new QLineEdit();
        mNameLe->setPlaceholderText(tr("Sequence name"));

    mDescrEditor = new QTextBrowser();
        mDescrEditor->setPlaceholderText(tr("Description..."));

    mSeqEditor = new QTextBrowser();
        mSeqEditor->setPlaceholderText(tr("CRC:Modbus{bytes}"));

    mButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok|
                                       QDialogButtonBox::Apply|
                                       QDialogButtonBox::Reset|
                                       QDialogButtonBox::Cancel );

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(mNameLe,      1, 0);
        main_layout->addWidget(mDescrEditor, 2, 0);
        main_layout->addWidget(mSeqEditor,   3, 0);
        main_layout->addWidget(mButtonBox,   4, 0);

    setLayout(main_layout);
    setWindowModality(Qt::NonModal);
}

void SqTableDialog::connectSignals()
{
    connect(btn_back, &QPushButton::released, this, [this]() { if (mMapper != nullptr) {
        mMapper->toFirst();
    }});

    connect(btn_prev, &QPushButton::released, this, [this]() { if (mMapper != nullptr) {
        mMapper->toPrevious();
    }});

    connect(btn_next, &QPushButton::released, this, [this]() { if (mMapper != nullptr) {
        mMapper->toNext();
    }});

    connect(btn_front, &QPushButton::released, this, [this]() {if (mMapper != nullptr) {
        mMapper->toLast();
    }});

    connect(btn_add, &QPushButton::released, this, [this]() {
      if (mMapper != nullptr) {
          auto row = mMapper->currentIndex() + 1;
          mMapper->model()->insertRow(row);
          mMapper->toNext();
      }
    });

    connect(mButtonBox, &QDialogButtonBox::clicked, this, &SqTableDialog::onDialogBtnClicked);
}


void SqTableDialog::onDialogBtnClicked(QAbstractButton* aBtn)
{
    if( mMapper != nullptr ) {

        switch( mButtonBox->standardButton( aBtn ) )
        {
            case QDialogButtonBox::Apply:
                mMapper->submit();
                break;

            case QDialogButtonBox::Ok:
                mMapper->submit();
                accept();
                break;

            case QDialogButtonBox::Reset:
                mMapper->revert();
                break;

            case QDialogButtonBox::Cancel:
                hide();
                break;

            default:
                break;
        }
    }
}
