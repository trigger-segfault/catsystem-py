
#define IN
#define OUT
#define OPTIONAL
typedef int BOOL;


// initial Blowfish Passes array values (PI hex digits)
extern unsigned int BF_PARRAY[18];
// initial Blowfish Substitution boxes values (PI hex digits)
extern unsigned int BF_SBOXES[4 * 256];

class Blowfish
{
    /*$0,4*/   unsigned int *PArray; // Passes array [18]
    /*$4,4*/   unsigned int *SBoxes; // Substitution boxes [4][256]
    /*$8*/
public:
    ///FID:cs2_full_v401/tool/ac.exe: FUN_0040dc50
    Blowfish();
    ///FID:cs2_full_v401/tool/ac.exe: FUN_0040dc70
    ~Blowfish();

    ///FID:cs2_full_v401/tool/ac.exe: FUN_0040e4f0
    void SetKey(IN const unsigned char *key, unsigned int keylength);

    void EncryptLR(IN OUT unsigned int *xl, IN OUT unsigned int *xr);
    void DecryptLR(IN OUT unsigned int *xl, IN OUT unsigned int *xr);

    void Encrypt(IN const unsigned char *inBuffer, OUT unsigned char *outBuffer, unsigned int length);
    void Decrypt(IN const unsigned char *inBuffer, OUT unsigned char *outBuffer, unsigned int length);
};

class kcProtect
{
    /*$0,4*/   Blowfish *Blowfish;
    /*$4,34*/  char BeaufortFwd[52];
    /*$38,34*/ char BeaufortRev[52];
    /*$6c*/
public:
    ///FID:cs2_full_v401/tool/ac.exe: FUN_0040c890
    kcProtect();
    ///FID:cs2_full_v401/tool/ac.exe: FUN_0040d0d0
    ~kcProtect();


    // A CRC-32-like hash of a V_CODE string. Returns a 32-bit
    //  unsigned integer used in all sorts of encryption functions
    unsigned int VCodeSeed(IN const char *vcode) const;

    
    // Beaufort cipher (two functions, but the cipher is two-way, same function will have the same result)
    void EncryptText(IN const char *inText, OUT char *outText, unsigned int seed);
    void DecryptText(IN const char *inText, OUT char *outText, unsigned int seed);

    // Blowfish cipher functions (unlike the Blowfish class, these guard against invalid byte-lengths (not multiple of 8))
    // Create a Blowfish cipher (if not created yet), then call Blowfish->SetKey
    void SetDataKey(IN const unsigned char *key, unsigned int keylength);
    void EncryptData(IN const unsigned char *inBuffer, OUT unsigned char *outBuffer, unsigned int length);
    void DecryptData(IN const unsigned char *inBuffer, OUT unsigned char *outBuffer, unsigned int length);

    // check "cs2_debug_key.dat" for to enable debug mode (appends "@@--cs2-debug-key--@@" to V_CODE string)
    void CheckDebugKey();
    // unique will add the %WINDOWS% volume serial number to the keyseed
    // refGameseed seems to be modified during the lifetime of this
    //  function so that failure will keep it broken?... not too sure
    bool CompareKeyFile(IN const char *filename, unsigned int keyseed, IN OUT unsigned int *refGameseed, bool unique);
    bool WriteKeyFile(IN const char *filename, unsigned int keyseed, bool unique);

    // Old cs2_gk.dat stuff. Interestingly enough this STILL exists in Happiness 2: Sakura Celebration
    //  AND the Happiness 2 version uses a name length of 64 bytes intead of 32. cool
    void ReadGlobalKey(); // dummy func, parameters not added
    void WriteGlobalKey(); // dummy func, parameters not added

    // other functions are in here too, but haven't been added yet
};