#ifndef USERDETAILS_H
#define USERDETAILS_H

#include <QObject>
#include <QUrl>

#include "PastebinAPI.h"

class WIIKING2_EXPORT UserInfo : public QObject
{
    Q_OBJECT
public:
    enum Visibility
    {
        Public,
        Unlisted,
        Private
    };
    enum AccountType
    {
        Normal,
        Pro
    };

    explicit UserInfo(QObject *parent = 0);

    QString userName() const;
    void setUserName(const QString& userName);

    QString shortFormat() const;
    void setShortFormat(const QString& format);

    QString expiration() const;
    void setExpiration(const QString& expire);

    QUrl avatarUrl() const;
    void setAvatarUrl(const QUrl& url);

    Visibility visibility() const;
    void setVisibility(Visibility visibility);

    QUrl website() const;
    void setWebsite(const QUrl& website);

    QString email() const;
    void setEmail(const QString& email);

    QString location() const;
    void setLocation(const QString& location);

    AccountType accountType() const;
    void setAccountType(AccountType type);

    static UserInfo* fromData(const QByteArray& data);
signals:
    
public slots:
    
private:
    QString     m_userName;
    QString     m_shortFormat;
    QString     m_expiration;
    QUrl        m_avatarUrl;
    Visibility  m_visibility;
    QUrl        m_website;
    QString     m_email;
    QString     m_location;
    AccountType m_accountType;
};

#ifndef QT_NO_DEBUG_STREAM
WIIKING2_EXPORT QDebug operator<<(QDebug debug, const UserInfo* userinfo);
#endif // QT_NO_DEBUG_STREAM

#endif // USERDETAILS_H
