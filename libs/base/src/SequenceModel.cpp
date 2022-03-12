#include "SequenceModel.h"

#include <QPointer>
#include <QDebug>

SequenceModel::SequenceModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_mode(false)
{

}


int SequenceModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_sequences.size();
}

int SequenceModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : kColumnCount;
}


QVariant SequenceModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    auto col = index.column();


    if (checkIndex(index)) {
        auto& sq = m_sequences.at(row);

        if (role == Qt::DisplayRole) {
            switch (col) {
                case kColumnName:
                    return sq.name();

                case kColumnBindedName:
                    return sq.bindedName();

                case kColumnPeriod: {
                    quint32 period  = sq.period();
                    QString special = (isModeIncoming()) ?
                                            tr("No Delay") : tr("No Repeat");

                    return (sq.period() == 0) ? special : QString("%1ms").arg(period);
                }

                case kColumnDescription:
                    return sq.description();

                case kColumnDsl:
                    return sq.dslString();

                case kColumnActiveFlag:
                    return sq.active();
            }
        }

        if (role == Qt::EditRole) {
            switch (col) {
                case kColumnName:
                    return sq.name();

                case kColumnBindedName:
                    return sq.bindedName();

                case kColumnPeriod:
                    return sq.period();

                case kColumnDescription:
                    return sq.description();

                case kColumnDsl:
                    return sq.dslString();

                case kColumnActiveFlag:
                    return sq.active();
            }
        }
    }


   return QVariant();
}

QVariant SequenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case kColumnName: return QString(tr("Name"));
                case kColumnBindedName: return (isModeIncoming()) ? QString(tr("Triggered")) : QString();
                case kColumnPeriod: return (isModeIncoming()) ? QString(tr("Delay")) : QString(tr("Repeat"));
                case kColumnDescription: return QString(tr("Description"));
                case kColumnDsl: return QString(tr("Sequence"));
                case kColumnActiveFlag: return (isModeIncoming()) ? QString(tr("Use")): QString("");
                default: break;
            }
        }
        else {
            return QString::number(section+1);
        }

    }

    return QVariant();
}


bool SequenceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ret = false;

    int row = index.row();
    int col = index.column();

    if ( checkIndex(index) )
    {
        ret = true;
        auto& sq = m_sequences[row];

        if (role == Qt::EditRole)
        {
            switch (col) {
                case kColumnName:
                    sq.setName(value.toString());
                    break;

                case kColumnBindedName:
                    sq.setBindedName(value.toString());
                    break;

                case kColumnPeriod:
                    sq.setPeriod(value.toInt());
                    break;
                case kColumnDescription:
                    sq.setDescription(value.toString());
                    break;
                case kColumnDsl:
                    sq.setDslString(value.toString());
                    break;
                case kColumnActiveFlag:
                    sq.setActive( value.toBool() );
                default:
                    break;
            }

            emit dataChanged(index, index);
        }

        if (role == kClickRole) {
            if (col == kColumnActiveFlag) {
                sq.setActive( !sq.active() );
            }
            emit dataChanged(index, index);
        }
    }

    return ret;
}

Qt::ItemFlags SequenceModel::flags(const QModelIndex& index) const
{
    int row[[maybe_unused]] = index.row();
    int col = index.column();

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (col != kColumnName) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool SequenceModel::insertRows(int row, int count, const QModelIndex& parent)
{
    Sequence sq;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    sq.setName(tr("Sequence %1").arg(m_sequences.size()+1));
    m_sequences.insert(row, sq);

    endInsertRows();


    return true;
}

bool SequenceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row >= 0 && row < m_sequences.size()) {
      count = ((row + count) > m_sequences.size()) ? m_sequences.size() - row : count;
      beginRemoveRows(QModelIndex(), row, row + count - 1);

      if (row == 0 && count == m_sequences.size()) {
        m_sequences.clear();
      }
      else
      {
        for (int i = 0; i < count; i++) {
          m_sequences.removeAt(row);
        }

      }

      endRemoveRows();
    }

    return true;
}

void SequenceModel::setIncomingMode()
{
    m_mode = true;
}

void SequenceModel::setOutgoingMode()
{
    m_mode = false;
}

bool SequenceModel::isModeIncoming() const
{
    return m_mode;
}

void SequenceModel::toJson(nlohmann::json& json)
{
    for (const auto& sequence: qAsConst(m_sequences)) {
        nlohmann::json fields;
            fields["name"]        = sequence.name();
            fields["binded_name"] = sequence.bindedName();
            fields["period"]      = sequence.period();
            fields["description"] = sequence.description();
            fields["dsl"]         = sequence.dslString();
            fields["active"]      = sequence.active();

        json.push_back(fields);
    }
}

void SequenceModel::fromJson(const nlohmann::json& json)
{
    QList<Sequence> imported;
    if ( !json.is_array() )
        return;

    for (const auto& it: json) {
        if (!it.is_object())
            continue;

        Sequence s;
        if ( it.contains("name")) {
            if ( it["name"].is_string() ) {
                s.setName( it["name"] );
            }
        }

        if ( it.contains("binded_name") ) {
            if ( it["binded_name"].is_string() ) {
                s.setBindedName( it["binded_name"] );
            }
        }

        if ( it.contains("period") ) {
            if ( it["period"].is_number() ) {
                s.setPeriod( it["period"] );
            }
        }

        if ( it.contains("description") ) {
            if ( it["description"].is_string() ) {
                s.setDescription( it["description"] );
            }
        }

        if ( it.contains("dsl") ) {
            if ( it["dsl"].is_string() ) {
                s.setDslString( it["dsl"] );
            }
        }

        if ( it.contains("active") ) {
            if ( it["active"].is_boolean() ) {
                s.setActive( it["active"] );
            }
        }

        imported.append(s);
    }

    if (imported.size() > 0) {
        int row = rowCount();
        beginInsertRows(QModelIndex(), row, row + imported.count() - 1);
            m_sequences.append(imported);
        endInsertRows();
    }
}


