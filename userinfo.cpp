#include "userinfo.h"

#include <QXmlStreamReader>
#include <QDebug>

UserInfo::UserInfo(QObject *parent) :
    QObject(parent)
{
}

QString UserInfo::userName() const
{
    return m_userName;
}

void UserInfo::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString UserInfo::shortFormat() const
{
    return m_shortFormat;
}

void UserInfo::setShortFormat(const QString &format)
{
    m_shortFormat = format;
}

QString UserInfo::expiration() const
{
    return m_expiration;
}

void UserInfo::setExpiration(const QString &expire)
{
    m_expiration = expire;
}

QUrl UserInfo::avatarUrl() const
{
    return m_avatarUrl;
}

void UserInfo::setAvatarUrl(const QUrl &url)
{
    m_avatarUrl = url;
}

UserInfo::Visibility UserInfo::visibility() const
{
    return m_visibility;
}

void UserInfo::setVisibility(UserInfo::Visibility visibility)
{
    m_visibility = visibility;
}

QUrl UserInfo::website() const
{
    return m_website;
}

void UserInfo::setWebsite(const QUrl &website)
{
    m_website = website;
}

QString UserInfo::email() const
{
    return m_email;
}

void UserInfo::setEmail(const QString &email)
{
    m_email = email;
}

QString UserInfo::location() const
{
    return m_location;
}

void UserInfo::setLocation(const QString &location)
{
    m_location = location;
}

UserInfo::AccountType UserInfo::accountType() const
{
    return m_accountType;
}

void UserInfo::setAccountType(UserInfo::AccountType type)
{
    m_accountType = type;
}

UserInfo *UserInfo::fromData(const QByteArray &data)
{
    QXmlStreamReader reader(data);
    UserInfo* info = NULL;
    while(!reader.atEnd())
    {
        reader.readNext();
        if (reader.name().isEmpty())
            continue;

        if (reader.name() == "user")
        {
            info = new UserInfo();
            while (!(reader.name() == "user" && reader.isEndElement()))
            {
                reader.readNext();
                if (reader.isEndElement() || reader.name().isEmpty())
                    continue;

                if (reader.name() == "user_name")
                     info->setUserName(reader.readElementText());
                else if (reader.name() == "user_format_short")
                    info->setShortFormat(reader.readElementText());
                else if (reader.name() == "user_expiration")
                    info->setExpiration(reader.readElementText());
                else if (reader.name() == "user_avatar_url")
                    info->setAvatarUrl(reader.readElementText());
                else if (reader.name() == "user_private")
                    info->setVisibility((Visibility)(reader.readElementText().toInt()));
                else if (reader.name() == "user_website")
                    info->setWebsite(reader.readElementText());
                else if (reader.name() == "user_email")
                    info->setEmail(reader.readElementText());
                else if (reader.name() == "user_location")
                    info->setLocation(reader.readElementText());
                else if (reader.name() == "user_account_type")
                    info->setAccountType((AccountType)(reader.readElementText().toInt()));
            }
        }
    }

    return info;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const UserInfo* userinfo)
{
    if (!userinfo)
        return debug << "UserInfo(0x0)";

    debug.nospace() << userinfo->metaObject()->className() << '(' << (void*) userinfo;
    if (!userinfo->objectName().isEmpty())
        debug << ", name = " << userinfo->objectName();

    debug << ", userName = "    << userinfo->userName();
    debug << ", shortFormat = " << userinfo->shortFormat();
    debug << ", expiration = "  << userinfo->expiration();
    debug << ", avatarUrl = "   << userinfo->avatarUrl().toString();
    debug << ", visibility = "  << userinfo->visibility();
    debug << ", website = "     << userinfo->website().toString();
    debug << ", email = "       << userinfo->email();
    debug << ", location = "    << userinfo->location();
    debug << ", accountType = " << userinfo->accountType();
    debug << ")";

    return debug.space();
}
#endif
