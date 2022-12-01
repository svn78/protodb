#pragma once

#include "Sequence.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>

namespace protodb {

class SequenceModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        kColumnName        = 0,
        kColumnBindedName  = 1,
        kColumnPeriod      = 2,
        kColumnDescription = 3,
        kColumnDsl         = 4,
        kColumnSyntaxId    = 5,
        kColumnActiveFlag  = 6,

        kColumnCount
    };

    enum Role {
        kClickRole = Qt::UserRole,
        kSequenceRole
    };

public:
    SequenceModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &aValue, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setIncomingMode();
    void setOutgoingMode();
    bool isModeIncoming() const;

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);

    QSharedPointer<const Sequence> getSequenceByUuid(const QUuid& uid, bool active_only = false) const;
    QSharedPointer<const Sequence> getSequenceByName(const QString& name, bool active_only = false) const;
    QSharedPointer<const Sequence> getSequenceByBytes(const QByteArray& bytes, bool active_only = false) const;

    int findSequenceByUuid(const QUuid& uuid, bool active_only = false) const;
    int findSequenceByName(const QString& name, bool active_only = false) const;
    int findSequenceByBytes(const QByteArray& bytes, bool active_only = false) const;

signals:
    void sSequenceActivated(QUuid uid);
    void sSequenceDisactivated(QUuid uid);

private:
    QList< QSharedPointer<Sequence> > m_sequences;
    bool m_mode;
};

} // namespace protodb
