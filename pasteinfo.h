#ifndef PASTE_INFO_H
#define PASTE_INFO_H

#include <QObject>
#include <QDateTime>
#include <QUrl>

#include "PastebinAPI.h"

class WIIKING2_EXPORT PasteInfo : public QObject
{
    Q_OBJECT
public:
    enum Visibility
    {
        Public,
        Unlisted,
        Private
    };

    PasteInfo(QObject* parent = 0);

    QString key() const;
    void setKey(const QString& key);

    QString postDate() const;
    void  setPostDate(const QString& date);

    QString title() const;
    void setTitle(const QString& title);

    quint32 size() const;
    void setSize(quint32 size);

    QString expireDate() const;
    void   setExpireDate(const QString& expire);

    Visibility visibility() const;
    void   setVisibiliy(Visibility visibility);

    QString longFormat() const;
    void setLongFormat(const QString& format);

    QString shortFormat() const;
    void setShortFormat(const QString& format);

    QUrl    url() const;
    void setUrl(const QUrl& url);

    quint32 hits() const;
    void setHits(quint32 hits);

    static PasteInfo* fromData(const QByteArray& data);
    static QList<PasteInfo*> fromList(const QByteArray& data);
public slots:

signals:

private slots:

private:
    QString    m_key;
    QString    m_postDate;
    QString    m_title;
    quint32    m_size;
    QString    m_expireDate;
    Visibility m_visibility;
    QString    m_longFormat;
    QString    m_shortFormat;
    QUrl       m_url;
    quint32    m_hits;
};

#ifndef QT_NO_DEBUG_STREAM
WIIKING2_EXPORT QDebug operator<<(QDebug debug, const PasteInfo* pasteinfo);
#endif // QT_NO_DEBUG_STREAM

#endif // PASTE_INFO_H
