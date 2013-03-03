#ifndef PASTEBINAPI_GLOBAL_H
#define PASTEBINAPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PASTEBIN_LIBRARY)
#  define WIIKING2_EXPORT Q_DECL_EXPORT
#else
#  define WIIKING2_EXPORT Q_DECL_IMPORT
#endif

#endif // PASTEBINAPI_GLOBAL_H
