//
// precomp.h    Precompiled header file for SymCrypt unit test 
//
// Copyright (c) Microsoft Corporation. Licensed under the MIT license. 
//

#ifdef KERNEL_MODE
    //#include <ntddksec.h>
    //#include <ntverp.h>

    //#include <stdio.h>

    #pragma warning(push)
    #pragma warning(disable:4201)
    #include <ntosp.h>
    #pragma warning(pop)

    #include <winerror.h>
    #include <windef.h>

    #include <string>
    #include <winternl.h>

#elif defined(__APPLE_CC__)

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <unistd.h>

    #include <vector>
    #include <string>
    #include <memory>
    #include <algorithm>
    #include <map>
    #include <strstream>
    #include <set>

    #include "precomp_iOS.h"

#else
    #include <ntstatus.h>
    #pragma prefast(push)
    #pragma prefast(disable: 26071, "Avoid error in validation of RtlULongLongMult")
    #include <ntintsafe.h>
    #pragma prefast(pop)

    // Ensure that windows.h doesn't re-define the status_* symbols
    #define WIN32_NO_STATUS	
    #include <windows.h>
    #include <winternl.h>
    #include <winioctl.h>

    //
    // Hack to get all the BCrypt declations even though our binaries target down-level platforms.
    //
    #pragma push_macro("NTDDI_VERSION")
    #undef NTDDI_VERSION
    #define NTDDI_VERSION NTDDI_WINTHRESHOLD
    #include <bcrypt.h>
    #pragma pop_macro("NTDDI_VERSION")

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    #include <powrprof.h>

    #include <vector>
    #include <string>
    #include <memory>
    #include <algorithm>
    #include <map>
    #include <strstream>
    #include <set>
    #include <strsafe.h>
#endif


#include "symcrypt.h"
#include "symcrypt_low_level.h"

#if !SYMCRYPT_APPLE_CC
    #include "ioctldefs.h"
#else
    VOID GenRandom( PBYTE  pbBuf, SIZE_T cbBuf );
#endif

#if SYMCRYPT_CPU_X86 | SYMCRYPT_CPU_AMD64
#include <wmmintrin.h>
#include <immintrin.h>
#endif


//
// Missing from our standard headers when compiling C++
//
extern "C"
{
    void __cpuid(int CPUInfo[4], int InfoType );
}

/*
// Exclude rarely-used stuff from Windows headers
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#define PERF_MEASURE_WIPE   0

//#include <nt.h>
//#include <ntrtl.h>
//#include <nturtl.h>
#include <ntstatus.h>

#pragma prefast(push)
#pragma prefast(disable: 26071, "Avoid error in validation of RtlULongLongMult")
#include <ntintsafe.h>
#pragma prefast(pop)

// Ensure that windows.h doesn't re-define the status_* symbols
#define WIN32_NO_STATUS	
#include <windows.h>

#include <wincrypt.h>
#include <bcrypt.h>

//
// Missing from our standard headers when compiling C++
//
extern "C"
{
    void __cpuid(int CPUInfo[4], int InfoType );
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <powrprof.h>

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <map>
#include <strstream>
#include <set>

//
// Header files for RSA32.lib
//
//#include <modes.h>
#include <aes.h>
#include "aesfast.h"
#include "sha.h"
#include "sha2.h"
#include "md5.h"
#include "md4.h"
#include "md2.h"
#include "hmac.h"
#include "modes.h"
#include "aesfast.h"
#include "des.h"
#include "tripldes.h"
#include "rc2.h"
#include "hmac.h"
#include "aes_ccm.h"
extern "C" {
    #include "aes_gcm.h"
}
#include "rc4.h"

//
// Header files for symcrypt.lib
//
#include "symcrypt.h"

//
// Some test hooks to allow the unit test to have its own environment.
//
extern "C" {
#include "..\lib\sc_lib-testhooks.h"
}

#if SYMCRYPT_CPU_X86 | SYMCRYPT_CPU_AMD64
#include <wmmintrin.h>
#include <immintrin.h>
#endif
*/
//
// Disable certain strange warnings
//
#pragma warning( disable: 4505 )        // unreferenced local function has been removed.
                                        // Don't understand why I get that one; something about templates...
#pragma warning( disable: 4127 )        // conditional expression is constant
#pragma warning( disable: 6262 )        // excessive stack usage. This is test code, I don't care.
#pragma warning( disable: 4702 )        // unreachable code. The compilers are not equally smart, and some complain 
                                        // aobut 'function must return a value' and some about 'unreachable code'
                                        
//
// Macros for different environments
//

#if SYMCRYPT_MS_VC

    #define STRICMP                     _stricmp
    #define STRNICMP                    _strnicmp
    
    #define SNPRINTF_S(a,b,c,d,...)     _snprintf_s((a),(b),(c),(d),__VA_ARGS__)
    #define VSNPRINTF_S(a,b,c,d,...)    _vsnprintf_s((a),(b),(c),(d),__VA_ARGS__)
    
    #define GENRANDOM(pbBuf, cbBuf)     BCryptGenRandom( NULL, (PBYTE) (pbBuf), (cbBuf), BCRYPT_USE_SYSTEM_PREFERRED_RNG )
    
    #define SLEEP                       Sleep
    
    #if defined( _X86_ ) | defined( _ARM_ )
        #define BitScanReverseSizeT  _BitScanReverse
    #elif defined( _AMD64_ ) || defined( _ARM64_ )
        #define BitScanReverseSizeT _BitScanReverse64
    #endif
    
    #define SECUREZEROMEMORY(dest, sz)  RtlSecureZeroMemory( (dest), (sz) )
    
#elif SYMCRYPT_APPLE_CC

    #define STRICMP                     strcasecmp
    #define STRNICMP                    strncasecmp
    
    #define SNPRINTF_S(a,b,c,d,...)     std::snprintf((a),(b),(d),__VA_ARGS__)
    #define VSNPRINTF_S(a,b,c,d,...)    std::vsnprintf((a),(b),(d),__VA_ARGS__)
    
    NTSTATUS IosGenRandom( PBYTE pbBuf, UINT32 cbBuf );
    
    #define GENRANDOM(pbBuf, cbBuf)     IosGenRandom( (PBYTE) pbBuf, cbBuf )
    
    #define SLEEP                       usleep
    
    #if defined(__LP64__)
        #define SIZET_BITS_1            63               
    #else
        #define SIZET_BITS_1            31
    #endif
    
    #define BitScanReverseSizeT(pInd, mask)  \
            ({*(pInd) = SIZET_BITS_1 - __builtin_clzl( (mask) ); \
            ( (mask)==0 )? 0 : 1; })
    
    #define SECUREZEROMEMORY(dest, sz)  memset_s( (dest), (sz), 0, (sz) )
    
#endif
    

//
// Our own header info
//

typedef std::string String;                     // String of characters
typedef std::basic_string<BYTE> BString;        // String of bytes

#define ARRAY_SIZE( x ) (sizeof(x)/sizeof(x[0]))

#define STRING_INT( x )     #x
#define STRING( x )         STRING_INT( x )         // This extra macro indirection ensures we get enough macro expansion.
#define LSTRING_INT( x )    L#x
#define LSTRING( x )        LSTRING_INT( x )

#define CONCAT_I2( a, b )       a##b
#define CONCAT_I3( a, b, c )    a##b##c
#define CONCAT_I4( a, b, c, d ) a##b##c##d


#define CONCAT2( a, b )         CONCAT_I2( a, b )
#define CONCAT3( a, b, c )      CONCAT_I3( a, b, c )
#define CONCAT4( a, b, c, d )   CONCAT_I4( a, b, c, d )

#define ImpXxx              CONCAT2( Imp, IMP_Name )
#define AlgXxx              CONCAT2( Alg, ALG_Name )
#define ModeXxx             CONCAT2( Mode, ALG_Mode )
#define BaseAlgXxx          CONCAT2( Alg, ALG_Base )


#define SYMCRYPT_Xxx                    CONCAT2( SymCrypt, ALG_Name )
#define SYMCRYPT_XXX_STATE              CONCAT3( SYMCRYPT_, ALG_NAME, _STATE )
#define SYMCRYPT_XXX_EXPANDED_KEY       CONCAT3( SYMCRYPT_, ALG_NAME, _EXPANDED_KEY )

#define SYMCRYPT_XxxStateCopy           CONCAT3( SymCrypt, ALG_Name, StateCopy )
#define SYMCRYPT_XxxInit                CONCAT3( SymCrypt, ALG_Name, Init )
#define SYMCRYPT_XxxAppend              CONCAT3( SymCrypt, ALG_Name, Append )
#define SYMCRYPT_XxxResult              CONCAT3( SymCrypt, ALG_Name, Result )
#define SYMCRYPT_XxxAppendBlocks        CONCAT3( SymCrypt, ALG_Name, AppendBlocks )
#define SYMCRYPT_XxxExpandKey           CONCAT3( SymCrypt, ALG_Name, ExpandKey )
#define SYMCRYPT_XxxKeyCopy             CONCAT3( SymCrypt, ALG_Name, KeyCopy )
#define SYMCRYPT_XxxEncrypt             CONCAT3( SymCrypt, ALG_Name, Encrypt )
#define SYMCRYPT_XxxDecrypt             CONCAT3( SymCrypt, ALG_Name, Decrypt )
#define SYMCRYPT_XxxXxxEncrypt          CONCAT4( SymCrypt, ALG_Name, ALG_Mode, Encrypt )
#define SYMCRYPT_XxxXxxDecrypt          CONCAT4( SymCrypt, ALG_Name, ALG_Mode, Decrypt )
#define SYMCRYPT_XxxStateExport         CONCAT3( SymCrypt, ALG_Name, StateExport )
#define SYMCRYPT_XxxStateImport         CONCAT3( SymCrypt, ALG_Name, StateImport )
#define SYMCRYPT_XxxAlgorithm           CONCAT3( SymCrypt, ALG_Name, Algorithm )

#define SYMCRYPT_BaseXxxAlgorithm        CONCAT3( SymCrypt, ALG_Base, Algorithm )

#define SYMCRYPT_XXX_BLOCK_SIZE         CONCAT3( SYMCRYPT_, ALG_NAME, _BLOCK_SIZE )
#define SYMCRYPT_XXX_INPUT_BLOCK_SIZE   CONCAT3( SYMCRYPT_, ALG_NAME, _INPUT_BLOCK_SIZE )
#define SYMCRYPT_XXX_RESULT_SIZE        CONCAT3( SYMCRYPT_, ALG_NAME, _RESULT_SIZE )
#define SYMCRYPT_XXX_STATE_EXPORT_SIZE  CONCAT3( SYMCRYPT_, ALG_NAME, _STATE_EXPORT_SIZE )


#define RSA32_XXX_INPUT_BLOCK_SIZE      CONCAT3( RSA32_, ALG_NAME, _INPUT_BLOCK_SIZE )
#define RSA32_XXX_RESULT_SIZE           CONCAT3( RSA32_, ALG_NAME, _RESULT_SIZE )
#define RSA32_XXX_BLOCK_SIZE            CONCAT3( RSA32_, ALG_NAME, _BLOCK_SIZE )

#define CNG_XXX_CHAIN_MODE              CONCAT2( BCRYPT_CHAIN_MODE_, ALG_MODE )

#define CNG_XXX_HASH_ALG_NAMEU          CONCAT3( Cng, ALG_Base, HashAlgNameU )

#define SYMCRYPT_2DES_BLOCK_SIZE        SYMCRYPT_3DES_BLOCK_SIZE
#define BCRYPT_2DES_ALGORITHM           BCRYPT_3DES_112_ALGORITHM 


#define MAX_SIZE_T                      ((SIZE_T) -1)

//
// Discriminator classes, one for each algorithm. 
// These are used to specialize our algorithm implementation template classes.
//

class AlgMd2{
public:
    static char * name;
};

class AlgMd4{
public:
    static char * name;
};

class AlgMd5{
public:
    static char * name;
};

class AlgSha1{
public:
    static char * name;
};

class AlgSha256{
public:
    static char * name;
};

class AlgSha384{
public:
    static char * name;
};

class AlgSha512{
public:
    static char * name;
};

class AlgHmacMd5{
public:
    static char * name;
};

class AlgHmacSha1{
public:
    static char * name;
};

class AlgHmacSha256{
public:
    static char * name;
};

class AlgHmacSha384{
public:
    static char * name;
};

class AlgHmacSha512{
public:
    static char * name;
};

class AlgAesCmac{
public:
	static char * name;
};

class AlgMarvin32{
public:
	static char * name;
};

class AlgAes{
public:
    static char * name;
};

class AlgDes{
public:
    static char * name;
};

class Alg2Des{
public:
    static char * name;
};

class Alg3Des{
public:
    static char * name;
};

class AlgDesx{
public:
    static char * name;
};

class AlgRc2{
public:
    static char * name;
};

class AlgRc4{
public:
    static char * name;
    static BOOL isRandomAccess;
};

class AlgChaCha20 {
public:
    static char * name;
    static BOOL isRandomAccess;
};

class AlgPoly1305 {
public:
    static char * name;
};

class AlgAesCtrDrbg{
public:
    static char * name;
};

class AlgAesCtrF142{
public:
    static char * name;
};

class AlgParallelSha256{
public:
    static char * name;
    static WCHAR * pwstrBasename;      // e.g. L"SHA256" 
};

class AlgParallelSha384{
public:
    static char * name;
    static WCHAR * pwstrBasename;
};

class AlgParallelSha512{
public:
    static char * name;
    static WCHAR * pwstrBasename;
};

class AlgPbkdf2{
public:
    static char * name;
};

class AlgSp800_108{
public:
    static char * name;
};

class AlgTlsPrf1_1{
public:
    static char * name;
};

class AlgTlsPrf1_2{
public:
    static char * name;
};

class AlgHkdf{
public:
    static char * name;
};

class AlgXtsAes{
public:
    static char * name;
};

class AlgTlsCbcHmacSha1 {
public:
    static char * name;
};

class AlgTlsCbcHmacSha256 {
public:
    static char * name;
};

class AlgTlsCbcHmacSha384 {
public:
    static char * name;
};

#define MODE_FLAG_CHAIN 1
#define MODE_FLAG_CFB   2

class ModeEcb{
public:
    static char * name;
    static ULONG flags;
};

class ModeCbc{
public:
    static char * name;
    static ULONG flags;
};

class ModeCfb{
public:
    static char * name;
    static ULONG flags;
};

class ModeCcm{
public:
    static char * name;
};

class ModeGcm{
public:
    static char * name;
};

class AlgIntAdd{
public:
    static char * name;
};

class AlgIntSub{
public:
    static char * name;
};

class AlgIntMul{
public:
    static char * name;
};

class AlgIntSquare{
public:
    static char * name;
};

class AlgIntDivMod{
public:
    static char * name;
};

class AlgModAdd{
public:
    static char * name;
};

class AlgModSub{
public:
    static char * name;
};

class AlgModMul{
public:
    static char * name;
};

class AlgModSquare{
public:
    static char * name;
};

class AlgModInv{
public:
    static char * name;
};

class AlgModExp{
public:
    static char * name;
};

class AlgScsTable{
public:
    static char * name;
};

class AlgIEEE802_11SaeCustom{
public:
    static char * name;
};

class AlgTrialDivision{
public:
    static char * name;
};

class AlgTrialDivisionContext{
public:
    static char * name;
};

class AlgWipe{
public:
    static char * name;
};

class AlgRsaEncRaw{
public:
    static char * name;
};

class AlgRsaDecRaw{
public:
    static char * name;
};

class AlgRsaEncPkcs1{
public:
    static char * name;
};

class AlgRsaDecPkcs1{
public:
    static char * name;
};

class AlgRsaEncOaep{
public:
    static char * name;
};

class AlgRsaDecOaep{
public:
    static char * name;
};

class AlgRsaSignPkcs1{
public:
    static char * name;
};

class AlgRsaVerifyPkcs1{
public:
    static char * name;
};

class AlgRsaSignPss{
public:
    static char * name;
};

class AlgRsaVerifyPss{
public:
    static char * name;
};

class AlgDsaSign{
public:
    static char * name;
};

class AlgDsaVerify{
public:
    static char * name;
};

class AlgDh{
public:
    static char * name;
};

class AlgEcurveAllocate{
public:
    static char * name;
};

class AlgEcpointSetZero{
public:
    static char * name;
};

class AlgEcpointSetDistinguished{
public:
    static char * name;
};

class AlgEcpointSetRandom{
public:
    static char * name;
};

class AlgEcpointIsEqual{
public:
    static char * name;
};

class AlgEcpointIsZero{
public:
    static char * name;
};

class AlgEcpointOnCurve{
public:
    static char * name;
};

class AlgEcpointAdd{
public:
    static char * name;
};

class AlgEcpointAddDiffNz{
public:
    static char * name;
};

class AlgEcpointDouble{
public:
    static char * name;
};

class AlgEcpointScalarMul{
public:
    static char * name;
};

class AlgEcdsaSign{
public:
    static char * name;
};

class AlgEcdsaVerify{
public:
    static char * name;
};

class AlgEcdh{
public:
    static char * name;
};

class AlgDeveloperTest{
public:
    static char * name;
};

//
// Macros for easy testing
//
#define FATAL( text ) {fatal( __FILE__, __LINE__, text );}
#define FATAL2( text, a ) {fatal( __FILE__, __LINE__, text, a );}
#define FATAL3( text, a, b ) {fatal( __FILE__, __LINE__, text, a, b );}
#define FATAL4( text, a, b, c  ) {fatal( __FILE__, __LINE__, text, a, b, c );}
#define FATAL5( text, a, b, c, d ) {fatal( __FILE__, __LINE__, text, a, b, c, d );}
#define FATAL6( text, a, b, c, d, e ) {fatal( __FILE__, __LINE__, text, a, b, c, d, e );}
#define CHECK( cond, text )           { if( !(cond) ) { fatal(__FILE__, __LINE__, text          );}; _Analysis_assume_( cond );}
#define CHECK3( cond, text, a )       { if( !(cond) ) { fatal(__FILE__, __LINE__, text, a       );}; _Analysis_assume_( cond );}
#define CHECK4( cond, text, a, b )    { if( !(cond) ) { fatal(__FILE__, __LINE__, text, a, b    );}; _Analysis_assume_( cond );}
#define CHECK5( cond, text, a, b, c ) { if( !(cond) ) { fatal(__FILE__, __LINE__, text, a, b, c );}; _Analysis_assume_( cond );}
#define SOFTCHECK( cond, text ) if( !(cond) ) { print( "%s(%d): %s\n", __FILE__, __LINE__, text ); }

extern DWORD g_osVersion;       // 0xaabb for major version aa and minor version bb

#define OS_VERSION_VISTA    0x0600
#define OS_VERSION_WIN7     0x0601
#define OS_VERSION_WIN8     0x0602
#define OS_VERSION_WIN8_1   0x0603

_Analysis_noreturn_
VOID
fatal( _In_ PSTR file, ULONG line, _In_ PSTR text, ... );

typedef CONST CHAR * PCCHAR;

#include "kat.h"
#include "rng.h"
#include "perf.h"

extern SIZE_T   g_modeCfbShiftParam;

#include "algorithm_base.h"

typedef std::vector<AlgorithmImplementation *> AlgorithmImplementationVector;
extern AlgorithmImplementationVector g_algorithmImplementation;

#include "perfprint.h"

typedef std::set<String> StringSet;
extern StringSet g_algorithmsToTest;     
extern StringSet g_implementationsToTest;
BOOL setContainsPrefix( const StringSet & set, const std::string & str );

#include "main_inline.h"
#include "resultMerge.h"

extern char * g_implementationNames[];

//#include "sc_implementations.h"
//#include "rsa32_implementations.h"
//#include "capi_implementations.h"
//#include "cng_implementations.h"
#include "printtable.h"

#include "rnddriver.h"

extern Rng g_rng;

extern BOOL g_showPerfRangeInfo;

extern BOOL g_verbose;

extern BOOL g_profile;
extern UINT32 g_profile_iterations;
extern UINT32 g_profile_key;

extern BOOL g_perfTestsRunning;

extern ULONG    g_rc2EffectiveKeyLength;

extern ULONG g_cngKeySizeFlag;

extern double g_tscFreqTickCtr;
extern double g_tscFreqPerfCtr;

extern BOOL g_sgx;

//template< typename AlgType >
//VOID getAlgorithmsOfOneType();
//std::auto_ptr<std::vector< AlgType * >> getAlgorithmsOfOneType();

template< typename AlgType >
std::auto_ptr<std::vector<AlgType *>> getAlgorithmsOfOneType( );


extern BOOLEAN     TestSelftestsEnabled;
extern BOOLEAN     TestSaveXmmEnabled;
extern BOOLEAN     TestSaveYmmEnabled;

extern ULONGLONG   TestFatalCount;
extern ULONGLONG   TestErrorInjectionCount;
extern ULONGLONG   TestErrorInjectionCalls;
extern ULONG       TestErrorInjectionProb;

extern BYTE TestErrorInjectionSeed[ SYMCRYPT_SHA1_RESULT_SIZE ];

#if SYMCRYPT_CPU_X86
//
// These Save/Restore functions are used by user mode SaveXmm code, plus the testing code.
//
extern "C" {
VOID SYMCRYPT_CALL SymCryptEnvUmSaveXmmRegistersAsm( __m128i * buffer );
VOID SYMCRYPT_CALL SymCryptEnvUmRestoreXmmRegistersAsm( __m128i * buffer );
}
#endif

#if SYMCRYPT_CPU_X86 | SYMCRYPT_CPU_AMD64
//
// These Save/Restore functions are used by user mode SaveXmm code, plus the testing code.
//
extern "C" {
VOID SYMCRYPT_CALL SymCryptEnvUmSaveYmmRegistersAsm( __m256i * buffer );
VOID SYMCRYPT_CALL SymCryptEnvUmRestoreYmmRegistersAsm( __m256i * buffer );
}
#endif


VOID
testWipe();

VOID
testUtil();

VOID
testHashAlgorithms();

VOID
testMacAlgorithms();

VOID
testBlockCipherAlgorithms();

VOID
testAuthEncAlgorithms();

VOID
testStreamCipherAlgorithms();

VOID
testKdfAlgorithms();

VOID
testXtsAlgorithms();

VOID
testIEEE802_11SaeCustom();

VOID
testTlsCbcHmacAlgorithms();

VOID
testAesCtrDrbg();

VOID
testArithmetic();

VOID
testScsTable();

VOID
testScsTools();

VOID
testEcc();

VOID
testRsa();

VOID
testDl();

KatData *
getCustomResource( _In_ PSTR resourceName, _In_ PSTR resourceType );

VOID
randomTestGetSubstringPosition( _In_reads_( bufSize )  PCBYTE buf, 
                                                        SIZE_T bufSize,
                                _Inout_                 SIZE_T * idx,
                                _Out_                   SIZE_T * pos,
                                _Out_                   SIZE_T * len );


VOID measurePerf( AlgorithmImplementation * pAlgImp );

VOID measurePerfOfWipe();

VOID initPerfSystem();

VOID testSelftest();

CHAR charToLower( CHAR c );

#define PERF_WIPE_MAX_SIZE  64
#define PERF_WIPE_N_OFFSETS      16

extern double g_wipePerf[PERF_WIPE_MAX_SIZE+1][PERF_WIPE_N_OFFSETS];

VOID
initXmmRegisters();

VOID
verifyXmmRegisters();

VOID
initYmmRegisters();

VOID
verifyYmmRegisters();


VOID
addCapiAlgs();

VOID
addCngAlgs();

VOID
addRsa32Algs();

VOID
addMsBignumAlgs();

VOID
addSymCryptAlgs();

VOID
addRefAlgs();

VOID
initTestInfrastructure( int argc, _In_reads_( argc ) char * argv[] );

VOID
runFunctionalTests();

VOID
runPerfTests();

VOID
runProfiling();

VOID
exitTestInfrastructure();

//
// Function pointers to deal with various BCrypt versions
//
#if !SYMCRYPT_APPLE_CC

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDeriveKeyPBKDF2Fn)(
    _In_                            BCRYPT_ALG_HANDLE   hPrf,
    _In_reads_bytes_( cbPassword )       PUCHAR              pbPassword,
    _In_                            ULONG               cbPassword,
    _In_reads_bytes_opt_( cbSalt )       PUCHAR              pbSalt,
    _In_                            ULONG               cbSalt,
    _In_                            ULONGLONG           cIterations,
    _Out_writes_bytes_( cbDerivedKey )    PUCHAR              pbDerivedKey,
    _In_                            ULONG               cbDerivedKey,
    _In_                            ULONG               dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptKeyDerivationFn)(
    _In_        BCRYPT_KEY_HANDLE hKey,
    _In_opt_    BCryptBufferDesc     *pParameterList,
    _Out_writes_bytes_to_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
    _In_        ULONG                cbDerivedKey,
    _Out_       ULONG                *pcbResult,
    _In_        ULONG                dwFlags);

typedef _Must_inspect_result_
NTSTATUS
(WINAPI * BCryptCreateMultiHashFn)(
    _Inout_                                     BCRYPT_ALG_HANDLE   hAlgorithm,
    _Out_                                       BCRYPT_HASH_HANDLE *phHash,
    _In_                                        ULONG               nHashes,
    _Out_writes_bytes_all_opt_(cbHashObject)    PUCHAR              pbHashObject,
    _In_                                        ULONG               cbHashObject,
    _In_reads_bytes_opt_(cbSecret)              PUCHAR              pbSecret,   // optional
    _In_                                        ULONG               cbSecret,   // optional
    _In_                                        ULONG               dwFlags);

typedef _Must_inspect_result_
NTSTATUS
(WINAPI * BCryptProcessMultiOperationsFn)(
    _Inout_                         BCRYPT_HANDLE                   hObject,
    _In_                            BCRYPT_MULTI_OPERATION_TYPE     operationType,
    _In_reads_bytes_(cbOperations)  PVOID                           pOperations,
    _In_                            ULONG                           cbOperations,
    _In_                            ULONG                           dwFlags );

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptOpenAlgorithmProviderFn)(
    _Out_ BCRYPT_ALG_HANDLE *phAlgorithm,
    _In_ LPCWSTR pszAlgId,
    _In_opt_ LPCWSTR pszImplementation,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptGetPropertyFn)(
    _In_ BCRYPT_HANDLE hObject,
    _In_ LPCWSTR pszProperty,
    _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptSetPropertyFn)(
    _Inout_ BCRYPT_HANDLE hObject,
    _In_ LPCWSTR pszProperty,
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptCloseAlgorithmProviderFn)(
    _Inout_ BCRYPT_ALG_HANDLE hAlgorithm,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptGenerateSymmetricKeyFn)(
    _Inout_ BCRYPT_ALG_HANDLE hAlgorithm,
    _Out_ BCRYPT_KEY_HANDLE *phKey,
    _Out_writes_bytes_all_opt_(cbKeyObject) PUCHAR pbKeyObject,
    _In_ ULONG cbKeyObject,
    _In_reads_bytes_(cbSecret) PUCHAR pbSecret,
    _In_ ULONG cbSecret,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptGenerateKeyPairFn)(
    _Inout_ BCRYPT_ALG_HANDLE hAlgorithm,
    _Out_ BCRYPT_KEY_HANDLE *phKey,
    _In_ ULONG dwLength,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptEncryptFn)(
    _Inout_ BCRYPT_KEY_HANDLE hKey,
    _In_reads_bytes_opt_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_opt_ VOID *pPaddingInfo,
    _Inout_updates_bytes_opt_(cbIV) PUCHAR pbIV,
    _In_ ULONG cbIV,
    _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDecryptFn)(
    _Inout_ BCRYPT_KEY_HANDLE hKey,
    _In_reads_bytes_opt_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_opt_ VOID *pPaddingInfo,
    _Inout_updates_bytes_opt_(cbIV) PUCHAR pbIV,
    _In_ ULONG cbIV,
    _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptExportKeyFn)(
    _In_ BCRYPT_KEY_HANDLE hKey,
    _In_opt_ BCRYPT_KEY_HANDLE hExportKey,
    _In_ LPCWSTR pszBlobType,
    _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptImportKeyFn)(
    _In_ BCRYPT_ALG_HANDLE hAlgorithm,
    _In_opt_ BCRYPT_KEY_HANDLE hImportKey,
    _In_ LPCWSTR pszBlobType,
    _Out_ BCRYPT_KEY_HANDLE *phKey,
    _Out_writes_bytes_all_opt_(cbKeyObject) PUCHAR pbKeyObject,
    _In_ ULONG cbKeyObject,
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptImportKeyPairFn)(
    _In_ BCRYPT_ALG_HANDLE hAlgorithm,
    _In_opt_ BCRYPT_KEY_HANDLE hImportKey,
    _In_ LPCWSTR pszBlobType,
    _Out_ BCRYPT_KEY_HANDLE *phKey,
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDuplicateKeyFn)(
    _In_ BCRYPT_KEY_HANDLE hKey,
    _Out_ BCRYPT_KEY_HANDLE *phNewKey,
    _Out_writes_bytes_all_opt_(cbKeyObject) PUCHAR pbKeyObject,
    _In_ ULONG cbKeyObject,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptFinalizeKeyPairFn)(
    _Inout_ BCRYPT_KEY_HANDLE hKey,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDestroyKeyFn)(
    _Inout_ BCRYPT_KEY_HANDLE hKey);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDestroySecretFn)(
    _Inout_ BCRYPT_SECRET_HANDLE hSecret);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptSignHashFn)(
    _In_ BCRYPT_KEY_HANDLE hKey,
    _In_opt_ VOID *pPaddingInfo,
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptVerifySignatureFn)(
    _In_ BCRYPT_KEY_HANDLE hKey,
    _In_opt_ VOID *pPaddingInfo,
    _In_reads_bytes_(cbHash) PUCHAR pbHash,
    _In_ ULONG cbHash,
    _In_reads_bytes_(cbSignature) PUCHAR pbSignature,
    _In_ ULONG cbSignature,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptSecretAgreementFn)(
    _In_ BCRYPT_KEY_HANDLE hPrivKey,
    _In_ BCRYPT_KEY_HANDLE hPubKey,
    _Out_ BCRYPT_SECRET_HANDLE *phAgreedSecret,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDeriveKeyFn)(
    _In_ BCRYPT_SECRET_HANDLE hSharedSecret,
    _In_ LPCWSTR pwszKDF,
    _In_opt_ BCryptBufferDesc *pParameterList,
    _Out_writes_bytes_to_opt_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
    _In_ ULONG cbDerivedKey,
    _Out_ ULONG *pcbResult,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptHashFn)(
    _Inout_ BCRYPT_ALG_HANDLE hAlgorithm,
    _In_reads_bytes_opt_(cbSecret) PUCHAR pbSecret, // for keyed algs
    _In_ ULONG cbSecret, // for keyed algs
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _Out_writes_bytes_all_(cbOutput) PUCHAR pbOutput,
    _In_ ULONG cbOutput);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptCreateHashFn)(
    _Inout_ BCRYPT_ALG_HANDLE hAlgorithm,
    _Out_ BCRYPT_HASH_HANDLE *phHash,
    _Out_writes_bytes_all_opt_(cbHashObject) PUCHAR pbHashObject,
    _In_ ULONG cbHashObject,
    _In_reads_bytes_opt_(cbSecret) PUCHAR pbSecret, // optional
    _In_ ULONG cbSecret, // optional
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptHashDataFn)(
    _Inout_ BCRYPT_HASH_HANDLE hHash,
    _In_reads_bytes_(cbInput) PUCHAR pbInput,
    _In_ ULONG cbInput,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptFinishHashFn)(
    _Inout_ BCRYPT_HASH_HANDLE hHash,
    _Out_writes_bytes_all_(cbOutput) PUCHAR pbOutput,
    _In_ ULONG cbOutput,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDuplicateHashFn)(
    _In_ BCRYPT_HASH_HANDLE hHash,
    _Out_ BCRYPT_HASH_HANDLE *phNewHash,
    _Out_writes_bytes_all_opt_(cbHashObject) PUCHAR pbHashObject,
    _In_ ULONG cbHashObject,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDestroyHashFn)(
    _Inout_ BCRYPT_HASH_HANDLE hHash);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptGenRandomFn)(
    _In_opt_ BCRYPT_ALG_HANDLE hAlgorithm,
    _Out_writes_bytes_(cbBuffer) PUCHAR pbBuffer,
    _In_ ULONG cbBuffer,
    _In_ ULONG dwFlags);

typedef _Must_inspect_result_ NTSTATUS
(WINAPI * BCryptDeriveKeyCapiFn)(
    _In_ BCRYPT_HASH_HANDLE hHash,
    _In_opt_ BCRYPT_ALG_HANDLE hTargetAlg,
    _Out_writes_bytes_(cbDerivedKey) PUCHAR pbDerivedKey,
    _In_ ULONG cbDerivedKey,
    _In_ ULONG dwFlags);

extern BCryptDeriveKeyPBKDF2Fn          CngPbkdf2Fn;
extern BCryptKeyDerivationFn            CngKeyDerivationFn;
extern BCryptCreateMultiHashFn          CngCreateMultiHashFn;
extern BCryptProcessMultiOperationsFn   CngProcessMultiOperationsFn;
extern BCryptCloseAlgorithmProviderFn   CngCloseAlgorithmProviderFn;
extern BCryptCreateHashFn               CngCreateHashFn;
extern BCryptDecryptFn                  CngDecryptFn;
extern BCryptDeriveKeyFn                CngDeriveKeyFn;
extern BCryptDeriveKeyCapiFn            CngDeriveKeyCapiFn;
extern BCryptDestroyHashFn              CngDestroyHashFn;
extern BCryptDestroyKeyFn               CngDestroyKeyFn;
extern BCryptDestroySecretFn            CngDestroySecretFn;
extern BCryptDuplicateHashFn            CngDuplicateHashFn;
extern BCryptDuplicateKeyFn             CngDuplicateKeyFn;
extern BCryptEncryptFn                  CngEncryptFn;
extern BCryptExportKeyFn                CngExportKeyFn;
extern BCryptFinalizeKeyPairFn          CngFinalizeKeyPairFn;
extern BCryptFinishHashFn               CngFinishHashFn;
extern BCryptGenerateKeyPairFn          CngGenerateKeyPairFn;
extern BCryptGenerateSymmetricKeyFn     CngGenerateSymmetricKeyFn;
extern BCryptGenRandomFn                CngGenRandomFn;
extern BCryptGetPropertyFn              CngGetPropertyFn;
extern BCryptHashFn                     CngHashFn;
extern BCryptHashDataFn                 CngHashDataFn;
extern BCryptImportKeyFn                CngImportKeyFn;
extern BCryptImportKeyPairFn            CngImportKeyPairFn;
extern BCryptOpenAlgorithmProviderFn    CngOpenAlgorithmProviderFn;
extern BCryptSecretAgreementFn          CngSecretAgreementFn;
extern BCryptSetPropertyFn              CngSetPropertyFn;
extern BCryptSignHashFn                 CngSignHashFn;
extern BCryptVerifySignatureFn          CngVerifySignatureFn;

#endif //SYMCRYPT_APPLE_CC


extern BOOLEAN g_fExitMultithreadTest;
extern ULONGLONG g_nMultithreadTestsRun;

typedef VOID (SYMCRYPT_CALL * SelfTestFn)();
typedef struct _SELFTEST_INFO
{
    SelfTestFn  f;
    LPSTR       name;
} SELFTEST_INFO;

extern const SELFTEST_INFO g_selfTests[];

VOID
runTestThread( VOID * seed );

extern PSTR testDriverName;

VOID
printHexArray( PCBYTE pData, SIZE_T nElements, SIZE_T elementSize );

#define XMM_SAVE_ERR 4506

extern "C" {
extern ULONG g_nXmmSaves;

VOID
printXmmRegisters( PCSTR text );
}

#define MAX_INT_BITS        (1 << 10)
#define MAX_INT_BYTES       (MAX_INT_BITS/8)

//
// For testing the different moduli types, we signal the type of modulus in the upper bits of the size parameter.
//
#define PERF_KEY_SECRET     0x01000000  // Modulus is secret (Requires generic implementation)
#define PERF_KEY_PUB_ODD    0x02000000  // Modulus parity is public & odd (allows Montgomery reduction)
#define PERF_KEY_PUBLIC     0x03000000  // Modulus is public
#define PERF_KEY_PUB_PM     0x04000000  // Modulus is public & Pseudo-Mersenne
#define PERF_KEY_PUB_NIST   0x05000000  // Modulus is public & NIST curve prime

#define PERF_KEY_PRIME      0x80000000  // Modulus is prime (orthogonal to the other flags)

//
// For testing the different internal curves
// The first byte denotes the type of curve while the lower bytes the field length 
//
#define PERF_KEY_NIST_CURVE 0x10000000  // NIST curve
#define PERF_KEY_NUMS_CURVE 0x20000000  // NUMS curve
#define PERF_KEY_C255_CURVE 0x30000000  // 25519 curve

#define PERF_KEY_NIST192    ( PERF_KEY_NIST_CURVE | 24 )
#define PERF_KEY_NIST224    ( PERF_KEY_NIST_CURVE | 28 )
#define PERF_KEY_NIST256    ( PERF_KEY_NIST_CURVE | 32 )
#define PERF_KEY_NIST384    ( PERF_KEY_NIST_CURVE | 48 )
#define PERF_KEY_NIST521    ( PERF_KEY_NIST_CURVE | 66 )

#define PERF_KEY_NUMS256    ( PERF_KEY_NUMS_CURVE | 32 )
#define PERF_KEY_NUMS384    ( PERF_KEY_NUMS_CURVE | 48 )
#define PERF_KEY_NUMS512    ( PERF_KEY_NUMS_CURVE | 64 )

#define PERF_KEY_C255_19    ( PERF_KEY_C255_CURVE | 32 )

PCBYTE
getPerfTestModulus( UINT32 exKeySize );

//
// Checked alloc definitions
//
extern volatile INT64 g_nOutstandingCheckedAllocs;  // Global to track the number of outstanding allocations
extern volatile INT64 g_nAllocs;                    // Global to track the number of allocations (only in single threaded runs)

extern volatile INT64 g_nOutstandingCheckedAllocsMsBignum;
extern volatile INT64 g_nAllocsMsBignum;

VOID SYMCRYPT_CALL AllocWithChecksInit();

PVOID SYMCRYPT_CALL AllocWithChecksSc( SIZE_T nBytes );
VOID FreeWithChecksSc( PVOID ptr );

PVOID SYMCRYPT_CALL AllocWithChecksMsBignum( SIZE_T nBytes );
VOID FreeWithChecksMsBignum( PVOID ptr );

// Table with the internal curves' parameters and the mapping to PERF_KEYs
const struct {
    UINT32                      exKeyParam;
    PCSYMCRYPT_ECURVE_PARAMS    pParams;
} g_exKeyToCurve[] = {
    { PERF_KEY_NIST192, SymCryptEcurveParamsNistP192 },
    { PERF_KEY_NIST224, SymCryptEcurveParamsNistP224 },
    { PERF_KEY_NIST256, SymCryptEcurveParamsNistP256 },
    { PERF_KEY_NIST384, SymCryptEcurveParamsNistP384 },
    { PERF_KEY_NIST521, SymCryptEcurveParamsNistP521 },

    { PERF_KEY_NUMS256, SymCryptEcurveParamsNumsP256t1 },
    { PERF_KEY_NUMS384, SymCryptEcurveParamsNumsP384t1 },
    { PERF_KEY_NUMS512, SymCryptEcurveParamsNumsP512t1 },

    { PERF_KEY_C255_19, SymCryptEcurveParamsCurve25519 },
};

#define NUM_OF_HIGH_BIT_RESTRICTION_ITERATIONS   (100)

VOID
testMontgomery(PSYMCRYPT_ECURVE  pCurve);

VOID
runRsaAverageKeyGenPerf();

// Constants for RSA performance tests (OAEP, PKCS1, PSS modes)
#define PERF_RSA_PKCS1_LESS_BYTES           (11)

#define PERF_RSA_LABEL_LENGTH               (8)
#define PERF_RSA_SALT_LENGTH                (8)

#define PERF_RSA_HASH_ALG_SC                (SymCryptSha256Algorithm)
#define PERF_RSA_HASH_ALG_CNG               (BCRYPT_SHA256_ALGORITHM)
#define PERF_RSA_HASH_ALG_SIZE              (SYMCRYPT_SHA256_RESULT_SIZE)

#define PERF_RSA_OAEP_LESS_BYTES            (2 + 2*SYMCRYPT_SHA256_RESULT_SIZE)

