#include "pasteinfo.h"
#include <QXmlStreamReader>
#include <QDebug>

PasteInfo::PasteInfo(QObject *parent)
    : QObject(parent)
{
}

QString PasteInfo::key() const
{
    return m_key;
}

void PasteInfo::setKey(const QString &key)
{
    m_key = key;
}

QString PasteInfo::postDate() const
{
    return m_postDate;
}

void PasteInfo::setPostDate(const QString &date)
{
    m_postDate = date;
}

QString PasteInfo::title() const
{
    return m_title;
}

void PasteInfo::setTitle(const QString &title)
{
    m_title = title;
}

quint32 PasteInfo::size() const
{
    return m_size;
}

void PasteInfo::setSize(quint32 size)
{
    m_size = size;
}

QString PasteInfo::expireDate() const
{
    return m_expireDate;
}

void PasteInfo::setExpireDate(const QString &expire)
{
    m_expireDate = expire;
}

PasteInfo::Visibility PasteInfo::visibility() const
{
    return m_visibility;
}

void PasteInfo::setVisibiliy(PasteInfo::Visibility visibility)
{
    m_visibility = visibility;
}

QString PasteInfo::longFormat() const
{
    return m_longFormat;
}

void PasteInfo::setLongFormat(const QString &format)
{
    m_longFormat = format;
}

QString PasteInfo::shortFormat() const
{
    return m_shortFormat;
}

void PasteInfo::setShortFormat(const QString &format)
{
    m_shortFormat = format;
}

QUrl PasteInfo::url() const
{
    return m_url;
}

void PasteInfo::setUrl(const QUrl &url)
{
    m_url = url;
}

quint32 PasteInfo::hits() const
{
    return m_hits;
}

void PasteInfo::setHits(quint32 hits)
{
    m_hits = hits;
}


QList<PasteInfo* > PasteInfo::fromList(const QByteArray &data)
{
    QList<PasteInfo*> pastes;
    QXmlStreamReader reader(data);

    while(!reader.atEnd())
    {
        PasteInfo* info = NULL;
        if (reader.name() == "paste")
        {
            info = new PasteInfo;
            while(!(reader.name() == "paste" && reader.isEndElement()))
            {
                reader.readNext();
                if (reader.name().isEmpty())
                    continue;

                if (reader.name() == "paste_key")
                {
                    info->setKey(reader.readElementText());
                }
                else if (reader.name() == "paste_date")
                {
                    info->setPostDate(reader.readElementText());
                }
                else if (reader.name() == "paste_title")
                {
                    info->setTitle(reader.readElementText());
                }
                else if (reader.name() == "paste_size")
                {
                    info->setSize(reader.readElementText().toInt());
                }
                else if (reader.name() == "paste_expire_date")
                {
                    info->setExpireDate(reader.readElementText());
                }
                else if (reader.name() == "paste_private")
                {
                    Visibility vis = (Visibility)(reader.readElementText().toInt());
                    info->setVisibiliy(vis);
                }
                else if (reader.name() == "paste_format_long")
                {
                    info->setLongFormat(reader.readElementText());
                }
                else if (reader.name() == "paste_format_short")
                {
                    info->setShortFormat(reader.readElementText());
                }
                else if (reader.name() == "paste_url")
                {
                    info->setUrl(reader.readElementText());
                }
                else if (reader.name() == "paste_hits")
                {
                    info->setHits(reader.readElementText().toInt());
                }
            }
        }

        // Is the paste valid?
        if (info)
        {
            // Add it to the list
            pastes.append(info);
        }

        reader.readNext();
    }

    return pastes;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const PasteInfo* pasteinfo)
{
    if (!pasteinfo)
        return debug << "PasteInfo(0x0)";

    debug.nospace() << pasteinfo->metaObject()->className() << '(' << (void*) pasteinfo;
    if (!pasteinfo->objectName().isEmpty())
        debug << ", name = " << pasteinfo->objectName();

    debug << ")";

    return debug.space();
}
#endif
