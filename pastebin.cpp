#include "pastebin.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "pasteinfo.h"
#include "userinfo.h"

#include <QDebug>

const QString PASTEBIN_URL = "http://pastebin.com/api/";
const QString PASTEBIN_LOGIN = "api_login.php";
const QString PASTEBIN_POST = "api_post.php";

Pastebin::Pastebin(QObject *parent)
    : QObject(parent)
{
    m_networkAccess = new QNetworkAccessManager(this);

    connect(m_networkAccess, SIGNAL(finished(QNetworkReply*)), this, SLOT(readData(QNetworkReply*)));
}

Pastebin::~Pastebin()
{
    delete m_networkAccess;
}

QString Pastebin::userKey() const
{
    return m_userKey;
}

void Pastebin::setUserKey(const QString &key)
{
    m_userKey = key;
}

QString Pastebin::userName() const
{
    return m_userName;
}

void Pastebin::setUserName(const QString &name)
{
    m_userName = name;
}

QString Pastebin::password() const
{
    return m_password;
}

void Pastebin::setPassword(const QString &password)
{
    m_password = password;
}

QString Pastebin::developerKey() const
{
    return m_developerKey;
}

void Pastebin::setDeveloperKey(const QString &key)
{
    m_developerKey = key;
}

QByteArray Pastebin::data() const
{
    return m_data;
}

void Pastebin::setData(const QByteArray &data)
{
    m_data = data;
}

Pastebin::Language Pastebin::language() const
{
    return m_language;
}

void Pastebin::setLanguage(Pastebin::Language lang)
{
    m_language = lang;
}

QString Pastebin::errorString() const
{
    return m_errorString;
}

void Pastebin::deletePaste(const QString& pasteKey)
{
    QUrl requestUrl(PASTEBIN_URL + PASTEBIN_POST);
    QUrl requestArgs;
    QNetworkRequest request(requestUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    requestArgs.addEncodedQueryItem("api_dev_key",   m_developerKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_option",    "delete");
    requestArgs.addEncodedQueryItem("api_paste_key", pasteKey.toUtf8());

    if (!m_userKey.isEmpty())
        requestArgs.addEncodedQueryItem("api_user_key", m_userKey.toUtf8());

    m_networkAccess->post(request, requestArgs.encodedQuery());
}

void Pastebin::postPaste(const QByteArray& data, Language lang, const QString& title, const QString& expire, quint32 vis)
{
    QUrl requestUrl(PASTEBIN_URL + PASTEBIN_POST);
    QUrl requestArgs;
    QNetworkRequest request(requestUrl);
    QString langOpt = "text";

    if (lang != PlainText)
        langOpt = languageString(lang);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    requestArgs.addEncodedQueryItem("api_dev_key",           m_developerKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_option",            "paste");
    requestArgs.addEncodedQueryItem("api_paste_expire_date", expire.toUtf8());
    requestArgs.addEncodedQueryItem("api_paste_private",     QString("%1").arg(vis).toUtf8());
    requestArgs.addEncodedQueryItem("api_paste_format",      langOpt.toUtf8());
    requestArgs.addEncodedQueryItem("api_paste_code",        QUrl::toPercentEncoding(QString(data).toAscii()));

    if (!title.isEmpty())
        requestArgs.addEncodedQueryItem("api_paste_name", QUrl::toPercentEncoding(title.toAscii()));

    if (!m_userKey.isEmpty())
        requestArgs.addEncodedQueryItem("api_user_key", m_userKey.toUtf8());

    m_networkAccess->post(request, requestArgs.encodedQuery());
}

void Pastebin::login()
{
    QUrl requestUrl(PASTEBIN_URL + PASTEBIN_LOGIN);
    QUrl requestArgs;
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    requestArgs.addEncodedQueryItem("api_dev_key",       m_developerKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_user_name",     QUrl::toPercentEncoding(m_userName));
    requestArgs.addEncodedQueryItem("api_user_password", QUrl::toPercentEncoding(m_password));

    m_networkAccess->post(request, requestArgs.encodedQuery());
}

void Pastebin::userInfo()
{
    QUrl requestUrl(PASTEBIN_URL + PASTEBIN_POST);
    QUrl requestArgs;
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    requestArgs.addEncodedQueryItem("api_dev_key", m_developerKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_user_key", m_userKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_option",  "userdetails");

    m_networkAccess->post(request, requestArgs.encodedQuery());
}

void Pastebin::trendingPastes()
{
    QUrl requestUrl(PASTEBIN_URL + PASTEBIN_POST);
    QUrl requestArgs;
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    requestArgs.addEncodedQueryItem("api_dev_key", m_developerKey.toUtf8());
    requestArgs.addEncodedQueryItem("api_option",  "trends");

    m_networkAccess->post(request, requestArgs.encodedQuery());
}

void Pastebin::readData(QNetworkReply* reply)
{
    QByteArray response = reply->readAll();
    qDebug() << reply->header(QNetworkRequest::ContentTypeHeader);
    if (response.contains("Bad API request"))
    {
        m_errorString = response;
        QString code = response.split(',')[1].simplified().toLower();

        if (code == "use post request, not get")
            emit error(InvalidRequestType);
        else if (code == "invalid api_dev_key")
            emit error(InvalidDevKey);
        else if (code == "invalid login")
            emit error(InvalidLogin);
        else if (code == "account not active")
            emit error(InactiveAccount);
        else if (code == "invalid post parameters")
            emit error(InvalidPostParameters);
        else if (code == "invalid permission to remove paste")
            emit error(InvalidPermissions);
        else if (code == "invalid api_option")
            emit error(InvalidOption);
        else if (code == "invalid api_user_key")
            emit error(InvalidUserKey);
        else if (code == "ip blocked")
            emit error(BlockedIP);
        else if (code.contains("maximum number of 10"))
            emit error(Max10Exceeded);
        else if (code.contains("maximum number of 25"))
            emit error(Max25Exceeded);
        else if (code == "invalid_expire_date")
            emit error(InvalidExpireDate);
        else if (code == "maximum paste file size exceeded")
            emit error(MaxSizeExceeded);
        else if (code == "invalid api_paste_format")
            emit error(InvalidFormat);
        else if (code == "api_paste_code was empty")
            emit error(EmptyCode);
        else
            emit unknownError(response);
    }
    else if (reply->url().toString().contains(PASTEBIN_LOGIN))
    {
        m_userKey = QString(response).simplified();
        emit loggedIn(m_userKey);
    }
    else if (reply->url().toString().contains(PASTEBIN_POST))
    {

        if (response.indexOf(PASTEBIN_URL) == 0)
            emit pasteSuccessful(QUrl(response.simplified()));
        if (response.indexOf("Paste Removed") == 0)
            emit deleteSuccessful();
        if (response.contains("<user>"))
            emit user(UserInfo::fromData(response));
        if (response.contains("<paste>"))
            // We have to add opening and closing tags so QXmlStreamReader reads the data correctly.
            // For some reason if the data is not enclosed, the reader flips a shit and goes into an
            // infinite loop.
            emit pastes(PasteInfo::fromList("<trends>\n"+response +"\n</trends>"));
    }
}

QString Pastebin::languageString(Pastebin::Language lang) const
{
    switch(lang)
    {
        case FourCS:               return "4cs";
        case ACME6502:             return "6502acme";
        case KickAss6502:          return "6502kickass";
        case TASM6502:             return "6502tasm";
        case ABAP:                 return "abap";
        case ActionScript:         return "actionscript";
        case ActionScript3:        return "actionscript3";
        case Ada:                  return "ada";
        case Algol68:              return "algol68";
        case Apache:               return "apache";
        case AppleScript:          return "applescript";
        case APTSource:            return "apt_sources";
        case ARM:                  return "arm";
        case NASM:                 return "asm";
        case ASP:                  return "asp";
        case Asymptote:            return "asymptote";
        case AutoConf:             return "autoconf";
        case AutoHotKey:           return "autohotkey";
        case AutoIt:               return "autoit";
        case AviSynth:             return "avisynth";
        case Awk:                  return "awk";
        case BASCOM_AVR:           return "bascomavr";
        case Bash:                 return "bash";
        case Basic4GL:             return "basic4gl";
        case BibTeX:               return "bibtex";
        case BlitzBasic:           return "blitzbasic";
        case BNF:                  return "bnf";
        case BOO:                  return "boo";
        case BrainFuck:            return "bf";
        case CLang:                return "c";
        case CMAC:                 return "c_mac";
        case CIL:                  return "cil";
        case CSharp:               return "csharp";
        case CPP:                  return "cpp";
        case CPPQt:                return "cpp-qt";
        case CLoadRunner:          return "c_loadrunner";
        case CADDCL:               return "caddcl";
        case CADLisp:              return "cadlisp";
        case CFDG:                 return "cfdg";
        case ChaiScript:           return "chaiscript";
        case Clojure:              return "clojure";
        case KloneC:               return "klonec";
        case KloneCPP:             return "klonecpp";
        case CMake:                return "cmake";
        case COBOL:                return "cobol";
        case CoffeeScript:         return "coffeescript";
        case ColdFusion:           return "cfm";
        case CSS:                  return "css";
        case CueSheet:             return "cuesheet";
        case DLang:                return "d";
        case DCL:                  return "dcl";
        case DCPU16:               return "dcpu16";
        case DCS:                  return "dcs";
        case Delphi:               return "delphi";
        case Oxygene:              return "oxygene";
        case Diff:                 return "diff";
        case DIV:                  return "div";
        case DOS:                  return "dos";
        case DOT:                  return "dot";
        case ELang:                return "e";
        case ECMAScript:           return "ecmascript";
        case Eiffel:               return "eiffel";
        case Email:                return "email";
        case EPC:                  return "epc";
        case Erlang:               return "erlang";
        case FSharp:               return "fsharp";
        case Falcon:               return "falcon";
        case FOLang:               return "fo";
        case FormulaOne:           return "f1";
        case Fortran:              return "fortran";
        case FreeBasic:            return "freebasic";
        case FreeSWITCH:           return "freeswitch";
        case GAMBAS:               return "gambas";
        case GameMaker:            return "gml";
        case GDB:                  return "gdb";
        case Genero:               return "genero";
        case Genie:                return "genie";
        case GetText:              return "gettext";
        case Go:                   return "go";
        case Groovy:               return "groovy";
        case GwBasic:              return "gwbasic";
        case Haskell:              return "haskell";
        case Haxe:                 return "haxe";
        case HicEst:               return "hicest";
        case HQ9Plus:              return "hq9plus";
        case HTML4:                return "html4strict";
        case HTML5:                return "html5";
        case Icon:                 return "icon";
        case IDL:                  return "idl";
        case INI:                  return "ini";
        case INNO:                 return "inno";
        case INTERCAL:             return "intercal";
        case IO:                   return "io";
        case JLang:                return "j";
        case Java:                 return "java";
        case Java5:                return "java5";
        case JavaScript:           return "javascript";
        case jQuery:               return "jquery";
        case KiXtart:              return "kixtart";
        case Latex:                return "latex";
        case LDIF:                 return "ldif";
        case LibertyBasic:         return "lb";
        case LindenScript:         return "lsl2";
        case Lisp:                 return "lisp";
        case LLVM:                 return "llvm";
        case LocoBasic:            return "locobasic";
        case Logtalk:              return "logtalk";
        case LOLCode:              return "lolcode";
        case LotusFormulas:        return "lotusformulas";
        case LotusScript:          return "lotusscript";
        case LScript:              return "lscript";
        case Lua:                  return "lua";
        case M68KAssembly:         return "m68k";
        case MagikSF:              return "magiksf";
        case Make:                 return "make";
        case MapBasic:             return "mapbasic";
        case MatLab:               return "matlab";
        case mIRC:                 return "mirc";
        case MIXAssember:          return "mmix";
        case Modula2:              return "modula2";
        case Modula3:              return "modula3";
        case Motorola68KHiSoftDev: return "68000devpac";
        case MPASM:                return "mpasm";
        case MXML:                 return "mxml";
        case MySQL:                return "mysql";
        case Nagios:               return "nagios";
        case NewLISP:              return "newlisp";
        case NSIS:                 return "nsis";
        case Oberon2:              return "oberon2";
        case ObjeckLang:           return "objeck";
        case ObjectiveC:           return "objc";
        case OCamlBrief:           return "ocaml-brief";
        case OCaml:                return "ocaml";
        case Octave:               return "octave";
        case OpenBSDPacketFilter:  return "pf";
        case GLSL:                 return "glsl";
        case OOBasic:              return "oobas";
        case Oracle11:             return "oracle11";
        case Oracle8:              return "oracle8";
        case Oz:                   return "oz";
        case ParaSail:             return "parasail";
        case PariGP:               return "parigp";
        case Pascal:               return "pascal";
        case PAWN:                 return "pawn";
        case PCRE:                 return "pcre";
        case Per:                  return "per";
        case Perl:                 return "perl";
        case Perl6:                return "perl6";
        case PHP:                  return "php";
        case PHPBrief:             return "php-brief";
        case Pic16:                return "pic16";
        case Pike:                 return "pike";
        case PixelBender:          return "pixelbender";
        case PLSQL:                return "plsql";
        case PostgreSQL:           return "postgresql";
        case POVRay:               return "povray";
        case PowerShell:           return "powershell";
        case PowerBuilder:         return "powerbuilder";
        case ProFTPd:              return "proftpd";
        case Progress:             return "progress";
        case Prolog:               return "prolog";
        case Properties:           return "properties";
        case ProvideX:             return "provodex";
        case PureBasic:            return "purebasic";
        case PyCon:                return "pycon";
        case Python:               return "python";
        case PyS60:                return "pys60";
        case QKdb:                 return "q";
        case QBasic:               return "qbasic";
        case RSPlus:               return "rsplus";
        case Rails:                return "rails";
        case REBOL:                return "rebol";
        case REG:                  return "reg";
        case Rexx:                 return "rexx";
        case Robots:               return "robots";
        case RPMSpec:              return "rpmspec";
        case Ruby:                 return "ruby";
        case GNUPlot:              return "gnuplot";
        case SAS:                  return "sas";
        case Scala:                return "scala";
        case Scheme:               return "scheme";
        case Scilab:               return "scilab";
        case SdlBasic:             return "sdlbasic";
        case Smarty:               return "smarty";
        case SPARK:                return "spark";
        case SPARQL:               return "sparql";
        case SQL:                  return "sql";
        case StoneScript:          return "stonescript";
        case SystemVerilog:        return "systemverilog";
        case TSQL:                 return "tsql";
        case TCL:                  return "tcl";
        case TeraTerm:             return "teraterm";
        case ThinBasic:            return "thinbasic";
        case TypoScript:           return "typoscript";
        case Unicon:               return "unicon";
        case UnrealScript:         return "uscript";
        case UPC:                  return "ups";
        case Urbi:                 return "urbi";
        case Vala:                 return "vala";
        case VBNet:                return "vbnet";
        case Vedit:                return "vedit";
        case VeriLog:              return "verilog";
        case VHDL:                 return "vhdl";
        case VIM:                  return "vim";
        case VisualProLog:         return "visualprolog";
        case VB:                   return "vb";
        case VisualFoxPro:         return "visualfoxpro";
        case WhiteSpace:           return "whitespace";
        case WHOIS:                return "whois";
        case Winbatch:             return "winbatch";
        case XBasic:               return "xbasic";
        case XML:                  return "xml";
        case XorgConfig:           return "xorg_conf";
        case XPP:                  return "xpp";
        case YAML:                 return "yaml";
        case Z80Assembly:          return "z80";
        case ZXBasic:              return "zxbasic";
        default:                   return "text";
    }
}
