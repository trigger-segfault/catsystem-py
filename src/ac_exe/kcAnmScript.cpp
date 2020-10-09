
// OOP kcAnmScript class function definitions

#include "common.h"
#ifdef KCLIB_OOP

#include "kcAnmScript.h"
#include "kclib_common.h"


//kcAnmScript_new [005986f0]
kclib::kcAnmScript::kcAnmScript()
{
    this->TimelineCount = 0; // *unaff_ESI = 0;
    this->Timelines = nullptr; // unaff_ESI[1] = 0;
    this->Unk2 = 0; // unaff_ESI[2] = 0;
    this->Unk3 = 0; // unaff_ESI[3] = 0;
    this->Counter = 0; // unaff_ESI[4] = 0;
    this->Instruction = 0; // unaff_ESI[5] = 0;
    this->Wait = 0; // unaff_ESI[6] = 0;
    this->MaxFrame = 0; // unaff_ESI[7] = 0;
    for (int i = 0; i < 64; i++) {
        this->Variables[i] = 0;
    }

    this->Unk73 = 1.00000000; //0x3f800000; // unaff_ESI[0x49] = 0x3f800000;
    this->FrameID = 0; // unaff_ESI[0x4a] = 0;
    this->Blend = 255; // unaff_ESI[0x4b] = 0xff;
    this->Disp = TRUE; // unaff_ESI[0x4c] = 1;
    this->PosX = 0; // unaff_ESI[0x4d] = 0;
    this->PosY = 0; // unaff_ESI[0x4e] = 0;

    // stl allocation
    this->Unk72stl = new ANM_UNK72(); //unaff_ESI[0x48] = iVar2;
    // this->Unk72stl = nullptr; // unaff_ESI[0x48] = 0;
}

//kcAnmScript_dtor_sub [00598540]
kclib::kcAnmScript::~kcAnmScript()
{
    int *pUnkStruct;
    this->TimelineCount = 0; // *this = 0;
    if (this->Timelines != nullptr) {
        delete[] this->Timelines;
        this->Timelines = nullptr;
    }
    this->Unk2 = 0;          // this[2] = 0;
    this->Unk3 = 0;          // this[3] = 0;
    this->Counter = 0;       // this[4] = 0;
    this->Wait = 0;          // this[6] = 0;
    for (int i = 0; i < 64; i++) {
        this->Variables[i] = 0;
    }
    
    // Some stl deallocation
    delete this->Unk72stl;
    // this->Unk72stl = nullptr;
    this->Unk72stl->Field72Unk6ptr[1] = (int *) this->Unk72stl->Field72Unk6ptr;
    this->Unk72stl->Field72Unk7 = 0;
    this->Unk72stl->Field72Unk6ptr[0] = (int *) this->Unk72stl->Field72Unk6ptr;
    this->Unk72stl->Field72Unk6ptr[2] = (int *) this->Unk72stl->Field72Unk6ptr;
    // *(int *)(*(int *)(iVar1 + 0x18) + 4) = *(int *)(iVar1 + 0x18);
    // *(undefined4 *)(iVar1 + 0x1c) = 0;
    // *(undefined4 *)*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(iVar1 + 0x18);
    // *(int *)(*(int *)(iVar1 + 0x18) + 8) = *(int *)(iVar1 + 0x18);

    this->FrameID = 0; // this[0x4a] = 0;
    this->Blend = 255; // this[0x4b] = 0xff;
    this->Disp = TRUE; // this[0x4c] = 1;
    this->PosX = 0; // this[0x4d] = 0;
    this->PosY = 0; // this[0x4e] = 0;
}

bool kclib::kcAnmScript::OpenFile(IN const char *filename)
{
    FILE *file = std::fopen(filename, "rb");
    if (file == nullptr)
    {
        ///JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        return false;
    }
    
    std::fseek(file, 0, SEEK_END);
    unsigned int length = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);

    bool result = false;
    unsigned char *filedata = nullptr;
    if (length != 0U)
    {
        filedata = new unsigned char[length];
        if (std::fread(filedata, length, 1, file) != 0)
        {
            result = this->ReadFile((ANM_FILEINFO *)filedata, length);
        }
    }
    if (filedata != nullptr)
    {
        delete[] filedata;
        filedata = nullptr;
    }
    std::fclose(file);
    return false;
}

//kcAnmScript_readFile [005985e0]
bool kclib::kcAnmScript::ReadFile(ANM_FILEINFO *fileData, unsigned int fileSize)
{
    this->Close();

    ANM_FILEHEADER *fileHeader = &fileData->Header;
    
    //kcAnmScript_dtor_sub();
    // kcAnmScript_dtor_sub(this);
    if (fileHeader->Magic != MAGIC_ANM)
    { // "ANM"
        ///JP: kclib::LogError("kcAnmScript : データ形式が違います");
        kclib::LogError("kcAnmScript : Data format is different");
        return false;
    }
    if (fileHeader->TimelineCount == 0)
    {
        ///JP: kclib::LogError("kcAnmScript : タイムライン数が不正です");
        kclib::LogError("kcAnmScript : Timeline count is invalid");
        return false;
    }
    //if (fileSize < fileHeader->TimelineCount * 0x44 + 0x20U)
    if (fileSize < fileHeader->TimelineCount * sizeof(ANM_TIMELINE) + sizeof(ANM_FILEHEADER))
    {
        ///JP: kclib::LogError("kcAnmScript : データサイズが不正です");
        kclib::LogError("kcAnmScript : Data size is invalid");
        return false;
    }
    this->Unk2 = fileHeader->AnmUnk1; // param_1[2] = in_EAX[1]; ANM.[....]tcnt .... ....
    this->TimelineCount = fileHeader->TimelineCount; // in_EAX[2]; ANM. ....[tcnt].... ....
    //iVar1 = (ANM_TIMELINE *)cs2_HeapAlloc_logged_004b02b0(0, fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    //this->Timelines = iVar1;
    //cs2_strmemcpy_004b1600(iVar1, &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    this->Timelines = new ANM_TIMELINE[fileHeader->TimelineCount]; // param_1[1] = iVar1;
    std::memcpy(&this->Timelines[0], &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE));
    this->ResetScript();
    for (int i = 0; i < 64; i++) {
        this->Variables[i] = 0;
    }
    return true;
}

bool kclib::kcAnmScript::OpenScript(IN const char *filename, OPTIONAL OUT unsigned int *outErrorCount)
{
    if (filename == nullptr)
        return false;

    ScriptReader reader;
    if (reader.OpenFile(filename))
    {
        bool result = this->ParseScript(&reader, outErrorCount);
        reader.Close();
        return result;
    }
    else
    {
        ///JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        return false;
    }
}

bool kclib::kcAnmScript::ParseScript(const char *str, OPTIONAL OUT unsigned int *outErrorCount)
{
    if (str == nullptr)
        return false;

    ScriptReader reader;
    if (reader.OpenBuffer(str))
    {
        bool result = this->ParseScript(&reader, outErrorCount);
        reader.Close();
        return result;
    }
    return false;
}

bool kclib::kcAnmScript::IsOpen() const
{
    return this->Timelines != nullptr;
}


bool kclib::kcAnmScript::ResetScript()
{
    this->Instruction = -1; // *(undefined4 *)(in_EAX + 0x14) = 0xffffffff;
    this->FrameID = 0; // *(undefined4 *)(in_EAX + 0x128) = 0;
    this->Blend = 255; // *(undefined4 *)(in_EAX + 300) = 0xff;
    this->Disp = TRUE; // *(undefined4 *)(in_EAX + 0x130) = 1;
    this->PosX = 0; // *(undefined4 *)(in_EAX + 0x134) = 0;
    this->PosY = 0; // *(undefined4 *)(in_EAX + 0x138) = 0;
    return true;
}

// Evaluate the value of an argument.
int kclib::kcAnmScript::EvaluateArg(kclib::ANM_ARG arg)
{
    switch (arg.VarType) {
    case ANM_TYPE_CONST:
    case ANM_TYPE_LABEL:
        return arg.Value;
    
    case ANM_TYPE_VARIABLE:
        return this->Variables[arg.Value];

    default:
        return 0; // invalid variable type defaults to 0
    }
}

static unsigned int mt_genrand() // rng
{
    return (unsigned int)std::rand(); // dummy func
}

static int FUN_00572440() // local_20
{
    return 1; // dummy func
}
static int FUN_004b1490() // result
{
    return 1; // dummy func
}

unsigned int kclib::kcAnmScript::RunScript()
{
    ANM_TIMELINE *timeln;
    ANM_VAR_TYPE vartype;
    int argNumber;
    int argVar;
    int argLabel;
    int argMin;
    int argMax;
    unsigned int range;
    unsigned int rnd;
    int result;
    int local_20;
    
    if (this->Instruction >= this->TimelineCount)
        return false;

    local_20 = FUN_00572440();
    if (this->Counter <= local_20) {
        do {
            this->Instruction++;

            if (this->TimelineCount <= this->Instruction)
                break;
            
            timeln = &this->Timelines[this->Instruction];

            switch(timeln->CmdType) {

            case ANM_CMD_ID: // 0: [ID] [min] (max)
                this->Wait = this->Wait + this->Counter;
                local_20 -= this->Counter;
                this->FrameID = this->EvaluateArg(timeln->Args[0]);
                argMin = this->EvaluateArg(timeln->Args[1]);
                argMax = this->EvaluateArg(timeln->Args[2]);
                range = argMax - argMin;
                if (range == 0) {
                    this->Counter = argMin;
                }
                else {
                    rnd = mt_genrand();
                    this->Counter = rnd % range + argMin;
                }
                break;

            case ANM_CMD_SET: // 1: set [var] [min] (max)
                argVar = this->EvaluateArg(timeln->Args[0]);
                argMin = this->EvaluateArg(timeln->Args[1]);
                argMax = this->EvaluateArg(timeln->Args[2]);
                range = argMax - argMin;
                if (range == 0) {
                    this->Variables[argVar] = argMin;
                }
                else {
                    rnd = mt_genrand();
                    this->Variables[argVar] = rnd % range + argMin;
                }
                break;

            case ANM_CMD_LOOP: // 2: loop [var] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argLabel = this->EvaluateArg(timeln->Args[1]);
                if (this->Variables[argVar] < 1) {
                    this->Instruction = argLabel - 1;
                }
                else {
                    this->Variables[argVar] -= 1;
                }
                break;

            case ANM_CMD_JUMP: // 3: jump [label]
                argLabel = this->EvaluateArg(timeln->Args[0]);
                this->Instruction = argLabel - 1;
                break;

            case ANM_CMD_IF: // 4: if [var] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argLabel = this->EvaluateArg(timeln->Args[1]);
                if (this->Variables[argVar] > 0) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFE: // 5: ife [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] == argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFN: // 6: ifn [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] != argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFG: // 7: ifg [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] > argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFS: // 8: ifs [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] < argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFGE: // 9: ifge [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] >= argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_IFSE: // 10: ifse [var] [value] [label]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                argLabel = this->EvaluateArg(timeln->Args[2]);
                if (this->Variables[argVar] <= argNumber) {
                    this->Instruction = argLabel - 1;
                }
                break;

            case ANM_CMD_MAX: // 11: max [var]
                argVar = this->EvaluateArg(timeln->Args[0]);
                this->Variables[argVar] = this->MaxFrame;
                break;

            case ANM_CMD_BLEND: // 12: blend [param]
                this->Blend = this->EvaluateArg(timeln->Args[0]);
                break;

            case ANM_CMD_DISP: // 13: disp [param]
                this->Disp = this->EvaluateArg(timeln->Args[0]);
                break;

            case ANM_CMD_POS: // 14: pos [x] [y]
                this->PosX = this->EvaluateArg(timeln->Args[0]);
                this->PosY = this->EvaluateArg(timeln->Args[1]);
                break;

            case ANM_CMD_WAIT: // 15: wait [min] [max]
                this->Wait = this->Wait + this->Counter;
                local_20 -= this->Counter;
                argMin = this->EvaluateArg(timeln->Args[0]);
                argMax = this->EvaluateArg(timeln->Args[1]);
                range = argMax - argMin;
                if (range == 0) {
                    this->Counter = argMin;
                }
                else {
                    rnd = mt_genrand();
                    this->Counter = rnd % range + argMin;
                }
                break;

            case ANM_CMD_ADD: // 16: add [var] [value]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                this->Variables[argVar] += argNumber;
                break;

            case ANM_CMD_SUB: // 17: sub [var] [value]
                argVar = this->EvaluateArg(timeln->Args[0]);
                argNumber = this->EvaluateArg(timeln->Args[1]);
                this->Variables[argVar] -= argNumber;
                break;
            
            default:
                ///JP: kclib::LogError("kcAnmScript : コマンドエラー");
                kclib::LogError("kcAnmScript : Command error");
                return this->FrameID;

            }
        } while (this->Counter <= local_20);
    }
    result = FUN_004b1490();
    return (unsigned int)(result != 0);
}

#endif
