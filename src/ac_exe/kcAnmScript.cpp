
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

    this->Unk73 = 1.00000000f; //0x3f800000; // unaff_ESI[0x49] = 0x3f800000;
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
    this->TimelineCount = 0; // *this = 0;
    if (this->Timelines != nullptr)
    {
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
    if (this->Unk72stl != nullptr)
    {
        delete this->Unk72stl;
        this->Unk72stl = nullptr;
    }
    
    // this->Unk72stl = nullptr;
    // this->Unk72stl->Field72Unk6ptr[1] = (int *) this->Unk72stl->Field72Unk6ptr;
    // this->Unk72stl->Field72Unk7 = 0;
    // this->Unk72stl->Field72Unk6ptr[0] = (int *) this->Unk72stl->Field72Unk6ptr;
    // this->Unk72stl->Field72Unk6ptr[2] = (int *) this->Unk72stl->Field72Unk6ptr;
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
        ///JP: std::printf("ファイルのロードに失敗しました : %s", filename);
        std::printf("Failed to load file : %s", filename);
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
            result = this->ReadFile(filedata, length);
        }
    }
    if (filedata != nullptr)
    {
        delete[] filedata;
        filedata = nullptr;
    }
    std::fclose(file);
    return result; //false;
}
bool kclib::kcAnmScript::OpenFile(IN const wchar_t *filename)
{
    FILE *file = _wfopen(filename, L"rb");
    if (file == nullptr)
    {
        ///JP: std::printf("ファイルのロードに失敗しました : %s", filename);
        std::wprintf(L"Failed to load file : %ls", filename);
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
            result = this->ReadFile(filedata, length);
        }
    }
    if (filedata != nullptr)
    {
        delete[] filedata;
        filedata = nullptr;
    }
    std::fclose(file);
    return result; //false;
}

//kcAnmScript_readFile [005985e0]
bool kclib::kcAnmScript::ReadFile(IN const unsigned char *filedata, unsigned int filesize)
{
    this->Close();

    ANM_FILEINFO  *fileinfo = (ANM_FILEINFO *)filedata;
    ANM_FILEHEADER *filehdr = &fileinfo->Header;
    
    //kcAnmScript_dtor_sub();
    // kcAnmScript_dtor_sub(this);
    if (filehdr->Magic != MAGIC_ANM)
    { // "ANM"
        ///JP: kclib::LogError("kcAnmScript : データ形式が違います");
        kclib::LogError("kcAnmScript : Data format is different");
        return false;
    }
    if (filehdr->TimelineCount == 0)
    {
        ///JP: kclib::LogError("kcAnmScript : タイムライン数が不正です");
        kclib::LogError("kcAnmScript : Timeline count is invalid");
        return false;
    }
    //if (filesize < filehdr->TimelineCount * 0x44 + 0x20U)
    if (filesize < filehdr->TimelineCount * sizeof(ANM_TIMELINE) + sizeof(ANM_FILEHEADER))
    {
        ///JP: kclib::LogError("kcAnmScript : データサイズが不正です");
        kclib::LogError("kcAnmScript : Data size is invalid");
        return false;
    }
    this->Unk2 = filehdr->AnmUnk1; // param_1[2] = in_EAX[1]; ANM.[....]tcnt .... ....
    this->TimelineCount = filehdr->TimelineCount; // in_EAX[2]; ANM. ....[tcnt].... ....
    //iVar1 = (ANM_TIMELINE *)cs2_HeapAlloc_logged_004b02b0(0, filehdr->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    //this->Timelines = iVar1;
    //cs2_strmemcpy_004b1600(iVar1, &filedata->Timelines[0], filehdr->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    this->Timelines = new ANM_TIMELINE[filehdr->TimelineCount]; // param_1[1] = iVar1;
    std::memcpy(&this->Timelines[0], &fileinfo->Timelines[0], filehdr->TimelineCount * sizeof(ANM_TIMELINE));
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
        bool result = this->ReadScript(&reader, outErrorCount);
        reader.Close();
        return result;
    }
    else
    {
        ///JP: std::printf("ファイルのロードに失敗しました : %s", filename);
        std::printf("Failed to load file : %s", filename);
        return false;
    }
}
bool kclib::kcAnmScript::OpenScript(IN const wchar_t *filename, OPTIONAL OUT unsigned int *outErrorCount)
{
    if (filename == nullptr)
        return false;

    ScriptReader reader;
    if (reader.OpenFile(filename))
    {
        bool result = this->ReadScript(&reader, outErrorCount);
        reader.Close();
        return result;
    }
    else
    {
        ///JP: std::printf(L"ファイルのロードに失敗しました : %ls", filename);
        std::wprintf(L"Failed to load file : %ls", filename);
        return false;
    }
}

bool kclib::kcAnmScript::ReadScript(IN const char *str, OPTIONAL OUT unsigned int *outErrorCount)
{
    if (str == nullptr)
        return false;

    ScriptReader reader;
    if (reader.OpenBuffer(str))
    {
        bool result = this->ReadScript(&reader, outErrorCount);
        reader.Close();
        return result;
    }
    return false;
}

bool kclib::kcAnmScript::ReadScript(ScriptReader *reader, OPTIONAL OUT unsigned int *outErrorCount)
{
    this->Close();

    if (reader == nullptr)
        return false;

    if (this->ac_ParseScript(reader, outErrorCount))
    {
        // this->TimelineCount = (int)this->ac_AnmLines.size();
        // this->Timelines = new ANM_TIMELINE[this->TimelineCount];
        // for (unsigned int i = 0; i < this->TimelineCount; i++)
        // {
        //     std::memcpy(&this->Timelines[i], &this->ac_AnmLines[i], sizeof(ANM_TIMELINE));
        // }
        
        this->Unk2 = 0; // param_1[2] = in_EAX[1]; ANM.[....]tcnt .... ....
        this->TimelineCount = (int)this->ac_AnmLines.size();
        this->Timelines = new ANM_TIMELINE[this->TimelineCount]; // param_1[1] = iVar1;
        std::memcpy(&this->Timelines[0], &this->ac_AnmLines[0], this->TimelineCount * sizeof(ANM_TIMELINE));
        this->ResetScript();
        for (int i = 0; i < 64; i++) {
            this->Variables[i] = 0;
        }

        return true;
    }
    return false;
}

bool kclib::kcAnmScript::WriteFile(IN const char *filename)
{
    if (!this->IsOpen())
        return false;

    FILE *file = std::fopen(filename, "wb+");
    if (file == nullptr)
    {
        ///JP: std::printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        std::printf("Output file %s cannot be opened.\n", filename);
        return false;
    }
    ANM_FILEHEADER anmhdr;
    std::memset(&anmhdr, 0, sizeof(ANM_FILEHEADER));
    anmhdr.Magic = MAGIC_ANM; // "ANM\0"
    anmhdr.AnmUnk1 = 0;
    anmhdr.TimelineCount = this->TimelineCount;
    std::fwrite((void *)&anmhdr, sizeof(ANM_FILEHEADER), 1, file);

    std::fwrite((void *)&this->Timelines[0], sizeof(ANM_TIMELINE), this->TimelineCount, file);
    // for (int i = 0; i < anmhdr.TimelineCount; i++)
    // {
    //     const ANM_TIMELINE &anmline = timelines[i];

    //     std::fwrite((void *)&anmline, sizeof(ANM_TIMELINE), 1, file);
    // }

    std::fclose(file);

    return true;
}
bool kclib::kcAnmScript::WriteFile(IN const wchar_t *filename)
{
    if (!this->IsOpen())
        return false;

    FILE *file = _wfopen(filename, L"wb+");
    if (file == nullptr)
    {
        ///JP: std::printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        std::wprintf(L"Output file %ls cannot be opened.\n", filename);
        return false;
    }
    ANM_FILEHEADER anmhdr;
    std::memset(&anmhdr, 0, sizeof(ANM_FILEHEADER));
    anmhdr.Magic = MAGIC_ANM; // "ANM\0"
    anmhdr.AnmUnk1 = 0;
    anmhdr.TimelineCount = this->TimelineCount;
    std::fwrite((void *)&anmhdr, sizeof(ANM_FILEHEADER), 1, file);

    std::fwrite((void *)&this->Timelines[0], sizeof(ANM_TIMELINE), this->TimelineCount, file);
    // for (int i = 0; i < anmhdr.TimelineCount; i++)
    // {
    //     const ANM_TIMELINE &anmline = timelines[i];

    //     std::fwrite((void *)&anmline, sizeof(ANM_TIMELINE), 1, file);
    // }

    std::fclose(file);

    return true;
}

bool kclib::kcAnmScript::WriteScript(IN const char *filename)
{
    if (!this->IsOpen())
        return false;

    FILE *file = std::fopen(filename, "wb+");
    if (file == nullptr)
    {
        ///JP: std::printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        std::printf("Output file %s cannot be opened.\n", filename);
        return false;
    }
    this->PrintScript(file);
    std::fflush(file);

    std::fclose(file);

    return true;
    // std::fprintf(stderr, "kcAnmScript::WriteScript function not implemented");
    // return false;
}
bool kclib::kcAnmScript::WriteScript(IN const wchar_t *filename)
{
    if (!this->IsOpen())
        return false;

    FILE *file = _wfopen(filename, L"wb+");
    if (file == nullptr)
    {
        ///JP: std::printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        std::wprintf(L"Output file %ls cannot be opened.\n", filename);
        return false;
    }
    this->PrintScript(file);
    std::fflush(file);

    std::fclose(file);

    return true;
    // std::fprintf(stderr, "kcAnmScript::WriteScript function not implemented");
    // return false;
}

void kclib::kcAnmScript::Close()
{
    ac_AnmLines.clear();
    ac_LabelNames.clear();

    if (this->Timelines != nullptr)
    {
        delete[] this->Timelines;
        this->Timelines = nullptr;
    }
    if (this->Unk72stl != nullptr)
    {
        delete this->Unk72stl;
        this->Unk72stl = nullptr;
    }
    
    // this->TimelineCount = 0;
    // this->Unk2 = 0;
    // this->Unk3 = 0;
    // this->Counter = 0;
    // this->Wait = 0;
    // for (int i = 0; i < 64; i++)
    // {
    //     this->Variables[i] = 0;
    // }

    this->TimelineCount = 0;
    this->Unk2 = 0;
    this->Unk3 = 0;
    this->Counter = 0;
    this->Instruction = 0;
    this->Wait = 0;
    this->MaxFrame = 0;
    for (int i = 0; i < 64; i++) {
        this->Variables[i] = 0;
    }

    this->Unk73 = 1.00000000f;
    this->FrameID = 0;
    this->Blend = 255;
    this->Disp = TRUE;
    this->PosX = 0;
    this->PosY = 0;

    // this->ResetScript();
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

void kclib::kcAnmScript::PrintTimeline(IN const kclib::ANM_TIMELINE *anmline) const
{
    this->PrintTimeline(stdout, anmline);
}
void kclib::kcAnmScript::PrintTimeline(FILE *file, IN const kclib::ANM_TIMELINE *anmline) const
{
    //const ANM_COMMAND_ENTRY *cmdinfo = &TABLE_ANM_COMMANDS[anmline->CmdType];
    const ANM_COMMAND_ENTRY *cmdinfo;
    if ((unsigned int)anmline->CmdType < (unsigned int)assert_enum(18, ANM_CMD_END))
    {
        cmdinfo = &TABLE_ANM_COMMANDS[anmline->CmdType];
        if (anmline->CmdType != assert_enum(0, ANM_CMD_ID))
        {
            std::fprintf(file, "%s ", cmdinfo->Name);
        }
    }
    else
    {
        cmdinfo = &TABLE_ANM_COMMANDS[(unsigned int)assert_enum(18, ANM_CMD_END)];
        std::fprintf(file, "<err_cmd_%u> ", (unsigned int)anmline->CmdType);
    }
    for (unsigned int i = 0; i < cmdinfo->ArgCount; i++)
    {
        const ANM_ARG *arg = &anmline->Args[i];
        // const char *space = (i == 0 ? (const char *)"" : (const char *)" ");
        if (i > 0)
        {
            std::fprintf(file, " "); // space between arguments
        }
        
        if (i + 1 == cmdinfo->ArgCount && (cmdinfo->Flags & ANM_FLAG_RANGE))
        {
            // We assume last range arg is never index 0
            if (anmline->Args[i - 1].VarType == arg->VarType &&
                anmline->Args[i - 1].Value   == arg->Value)
            {
                // a range argument that's identical,
                // as a clean approach, we'll skip printing, since it's redundant
                continue;
            }
        }
        
        if (arg->VarType == ANM_TYPE_CONST)
        {
            if (i + 1 == cmdinfo->ArgCount && (cmdinfo->Flags & ANM_FLAG_LABEL) && (unsigned int)arg->Value < this->TimelineCount)
            {
                std::fprintf(file, "label_%u", (unsigned int)arg->Value);
            }
            else
            {
                std::fprintf(file, "%u", (unsigned int)arg->Value);
            }
        }
        else if (arg->VarType == ANM_TYPE_VARIABLE)
        {
            std::fprintf(file, "@%u", (unsigned int)arg->Value);
        }
        else if (arg->VarType == ANM_TYPE_LABEL)
        {
            if (i == 0 && anmline->CmdType == assert_enum(0, ANM_CMD_ID))
            {
                // this would never parse, invalid
                std::fprintf(file, "<err_label_%u>", (unsigned int)arg->Value);
            }
            else
            {
                std::fprintf(file, "label_idx_%u", (unsigned int)arg->Value);
            }
        }
        else
        {
            std::fprintf(file, "<err_%u_%u>", (unsigned int)arg->VarType, (unsigned int)arg->Value);
        }
    }
    std::fprintf(file, "\n");
    // std::fflush(file);
}

void kclib::kcAnmScript::PrintScript() const
{
    this->PrintScript(stdout);
}

void kclib::kcAnmScript::PrintScript(FILE *file) const
{
    if (!this->IsOpen())
    {
        std::fprintf(file, "[script is closed]\n");
        std::fflush(file);
        return;
    }

    // std::fprintf(file, "TimelineCount = %u\n", this->TimelineCount);

    std::vector<ANM_LABEL> labels; // find labels in arguments
    for (unsigned int i = 0; i < this->TimelineCount; i++)
    {
        const ANM_TIMELINE *anmline = &this->Timelines[i];
        //const ANM_COMMAND_ENTRY *cmdinfo;
        if ((unsigned int)anmline->CmdType >= (unsigned int)assert_enum(18, ANM_CMD_END))
            continue;

        const ANM_COMMAND_ENTRY *cmdinfo = &TABLE_ANM_COMMANDS[anmline->CmdType];
        if (!(cmdinfo->Flags & ANM_FLAG_LABEL))
            continue;

        const ANM_ARG *arg = &anmline->Args[cmdinfo->ArgCount - 1];

        if (arg->VarType != assert_enum(0, ANM_TYPE_CONST))
            continue;

        if (arg->Value >= this->TimelineCount)
            continue; // out of range of possible addresses

        bool labelFound = false;
        // ANM_LABEL label = {0};
        for (unsigned int k = 0; k < (unsigned int)labels.size(); k++)
        {
            if (labels[k].LabelAddress == (unsigned int)arg->Value)
            {
                // label = labels[k];
                labelFound = true;
                break;
            }
        }
        if (!labelFound)
        {
            ANM_LABEL label;// = {0};
            std::memset(&label, 0, sizeof(ANM_LABEL));
            label.LabelAddress = (unsigned int)arg->Value;
            label.LabelIndex = (unsigned int)labels.size();
            std::snprintf(label.LabelName, 0x20, "label_%u", (unsigned int)arg->Value);
            labels.push_back(label);
        }
    }

    // Now print commands and any labels at the same address
    for (unsigned int i = 0; i < this->TimelineCount; i++)
    {
        for (unsigned int k = 0; k < (unsigned int)labels.size(); k++)
        {
            if (labels[k].LabelAddress == i)
            {
                std::fprintf(file, "#%s\n", labels[k].LabelName);
            }
        }
        std::fprintf(file, "\t"); // tab-indent commands
        this->PrintTimeline(file, &this->Timelines[i]);
        std::fflush(file);
    }
    std::fflush(file);
}

// Evaluate the value of an argument.
int kclib::kcAnmScript::EvaluateArg(kclib::ANM_ARG arg)
{
    switch (arg.VarType) {
    case ANM_TYPE_CONST:
    case ANM_TYPE_LABEL:
        return (int)arg.Value;
    
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
    // ANM_VAR_TYPE vartype;
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
