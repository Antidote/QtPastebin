#ifndef PASTEBIN_H
#define PASTEBIN_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QUrl>

#include "PastebinAPI.h"

class QNetworkAccessManager;
class QNetworkReply;

class PasteInfo;
class UserInfo;

class WIIKING2_EXPORT Pastebin : public QObject
{
    Q_OBJECT
public:
    enum Language
    {
        FourCS, // 4CS
        ACME6502,
        KickAss6502,
        TASM6502,
        ABAP,
        ActionScript,
        ActionScript3,
        Ada,
        Algol68,
        Apache,
        AppleScript,
        APTSource,
        ARM,
        NASM,
        ASP,
        Asymptote,
        AutoConf,
        AutoHotKey,
        AutoIt,
        AviSynth,
        Awk,
        BASCOM_AVR,
        Bash,
        Basic4GL,
        BibTeX,
        BlitzBasic,
        BNF,
        BOO,
        BrainFuck,
        CLang,
        CMAC,
        CIL,
        CSharp,
        CPP,
        CPPQt,
        CLoadRunner,
        CADDCL,
        CADLisp,
        CFDG,
        ChaiScript,
        Clojure,
        KloneC,
        KloneCPP,
        CMake,
        COBOL,
        CoffeeScript,
        ColdFusion,
        CSS,
        CueSheet,
        DLang,
        DCL,
        DCPU16,
        DCS,
        Delphi,
        Oxygene,
        Diff,
        DIV,
        DOS,
        DOT,
        ELang,
        ECMAScript,
        Eiffel,
        Email,
        EPC,
        Erlang,
        FSharp,
        Falcon,
        FOLang,
        FormulaOne,
        Fortran,
        FreeBasic,
        FreeSWITCH,
        GAMBAS,
        GameMaker,
        GDB,
        Genero,
        Genie,
        GetText,
        Go,
        Groovy,
        GwBasic,
        Haskell,
        Haxe,
        HicEst,
        HQ9Plus,
        HTML4,
        HTML5,
        Icon,
        IDL,
        INI,
        INNO,
        INTERCAL,
        IO,
        JLang,
        Java,
        Java5,
        JavaScript,
        jQuery,
        KiXtart,
        Latex,
        LDIF,
        LibertyBasic,
        LindenScript,
        Lisp,
        LLVM,
        LocoBasic,
        Logtalk,
        LOLCode,
        LotusFormulas,
        LotusScript,
        LScript,
        Lua,
        M68KAssembly,
        MagikSF,
        Make,api_options,
        MapBasic,
        MatLab,
        mIRC,
        MIXAssember,
        Modula2,
        Modula3,
        Motorola68KHiSoftDev,
        MPASM,
        MXML,
        MySQL,
        Nagios,
        NewLISP,
        PlainText, // Plaintext
        NSIS,
        Oberon2,
        ObjeckLang,
        ObjectiveC,
        OCamlBrief,
        OCaml,
        Octave,
        OpenBSDPacketFilter,
        GLSL,
        OOBasic,
        Oracle11,
        Oracle8,
        Oz,
        ParaSail,
        PariGP,
        Pascal,
        PAWN,
        PCRE,
        Per,
        Perl,
        Perl6,
        PHP,
        PHPBrief,
        Pic16,
        Pike,
        PixelBender,
        PLSQL,
        PostgreSQL,
        POVRay,
        PowerShell,
        PowerBuilder,
        ProFTPd,
        Progress,
        Prolog,
        Properties,
        ProvideX,
        PureBasic,
        PyCon,
        Python,
        PyS60,
        QKdb,
        QBasic,
        RSPlus,
        Rails,
        REBOL,
        REG,
        Rexx,
        Robots,
        RPMSpec,
        Ruby,
        GNUPlot,
        SAS,
        Scala,
        Scheme,
        Scilab,
        SdlBasic,
        SmallTalk,
        Smarty,
        SPARK,
        SPARQL,
        SQL,
        StoneScript,
        SystemVerilog,
        TSQL,
        TCL,
        TeraTerm,
        ThinBasic,
        TypoScript,
        Unicon,
        UnrealScript,
        UPC,
        Urbi,
        Vala,
        VBNet,
        Vedit,
        VeriLog,
        VHDL,
        VIM,
        VisualProLog,
        VB,
        VisualFoxPro,
        WhiteSpace,
        WHOIS,
        Winbatch,
        XBasic,
        XML,
        XorgConfig,
        XPP,
        YAML,
        Z80Assembly,
        ZXBasic
    };

    enum ErrorType
    {
        InvalidDevKey,
        InvalidUserKey,
        InvalidOption,
        InvalidExpireDate,
        InvalidVisibility,
        InvalidFormat,
        InvlaidPaste,
        InvalidPermissions,
        InvalidRequestType,
        InvalidPostParameters,
        InvalidLogin,
        InactiveAccount,
        EmptyCode,
        MaxSizeExceeded,
        Max10Exceeded,
        Max25Exceeded,
        BlockedIP
    };

    explicit Pastebin(QObject* parent=0);
    ~Pastebin();

    QString userKey() const;
    void setUserKey(const QString& key);

    QString userName() const;
    void setUserName(const QString& name);

    QString password() const;
    void setPassword(const QString& password);

    QString developerKey() const;
    void setDeveloperKey(const QString& key);

    QByteArray data() const;
    void    setData(const QByteArray& data);

    Language language() const;
    void  setLanguage(Language lang);

    QString errorString() const;
public slots:
    void deletePaste(const QString& pasteKey);
    void postPaste(const QByteArray& data, Language lang = PlainText, const QString& title=QString(), const QString& expire="N", quint32 vis = 0);
    void login();
    void userInfo();
    void trendingPastes();

signals:
    void error(Pastebin::ErrorType);
    void unknownError(const QString& error);
    void loggedIn(const QString& key);
    void pasteSuccessful(const QUrl& url);
    void deleteSuccessful();
    void pastes(QList<PasteInfo*> pastes);
    void user(UserInfo* user);

private slots:
    void readData(QNetworkReply*);
private:
    QString languageString(Language lang) const;
    QNetworkAccessManager* m_networkAccess;

    QString    m_userKey;
    QString    m_userName;
    QString    m_password;
    QString    m_developerKey;
    QString    m_errorString;
    QByteArray m_data;
    Language   m_language;
};

#ifndef QT_NO_DEBUG_STREAM
WIIKING2_EXPORT QDebug operator<<(QDebug debug, const Pastebin* pastebin);
#endif // QT_NO_DEBUG_STREAM

#endif // PASTEBIN_H
