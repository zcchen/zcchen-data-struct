#ifndef ZCCHEN_CONFIG_DLL_H
#define ZCCHEN_CONFIG_DLL_H

#ifndef CONFIGAPI
#ifdef WINNT
    #if defined(DLL_EXPORT)
        #define CONFIGAPI __declspec(dllexport)
    #else
        #define CONFIGAPI __declspec(dllimport)
    #endif
#else
    #define CONFIGAPI
#endif
#endif /* ifndef CONFIGAPI */

#ifndef CONFIGCALL
#ifdef WINNT
    #define CONFIGCALL __stdcall
#else
    #define CONFIGCALL
#endif
#endif /* ifndef CONFIG_CALL */

#endif /* ifndef ZCCHEN_CONFIG_DLL_H */
