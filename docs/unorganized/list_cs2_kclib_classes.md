


```cpp
class cs2Conf {};

class kcClassBase {};

class CMemAllocator {};

class kcImage : public kcClassBase {};
class kcImageBmp : public kcImage {};
class kcImageHg2 : public kcImage {};
class kcImageHg3 : public kcImage {};
class kcImageJpeg : public kcImage {};
class kcImageKcf : public kcImage {};
class kcImagePsd : public kcImage {};

class kcModelBase : public kcClassBase {};
class kc2DModel : public kcModelBase {};

class kcTaskBase : public kcClassBase {};
class kcTaskTop : public kcTaskBase {};
class kcTaskFEScript : public kcTaskBase {};
class kcTaskScript : public kcTaskBase {};

class kcTaskMesList : public kcTaskBase {};
class kcTaskSceneList : public kcTaskBase {};
class kcTaskSound : public kcTaskBase {};

class kcTaskInput : public kcTaskBase {};

class kcTaskZTExport : public kcTaskBase {};
class kcTaskZTExportWinSnd : public kcTaskZTExport {};
class kcTaskChangeWP : public kcTaskBase {};

class kcTaskQzDebug : public kcTaskBase {};

class kcTaskCtrlScroll : public kcTaskBase {};
class kcTaskCtrlScroll2 : public kcTaskBase {};
class kcTaskCtrlSlider : public kcTaskBase {};

class kcTaskButton : public kcTaskBase {};
class kcTaskSimplePlane : public kcTaskButton {};

class kcTaskStringPlane : public kcTaskSimplePlane {};
class kcTaskStringPlane2 : public kcTaskSimplePlane {};
class kcTaskStringPlaneNv : public kcTaskSimplePlane {};

class kcTaskImageList : public kcTaskBase {};

class kcTaskRenderBlur : public kcTaskBase {};
class kcTaskRenderDraw : public kcTaskBase {};
class kcTaskRenderFMosaic : public kcTaskBase {};
class kcTaskRenderMosaic : public kcTaskBase {};
class kcTaskRenderShade : public kcTaskBase {};
class kcTaskRenderWBreak : public kcTaskBase {};
class kcTaskRenderWipe : public kcTaskBase {};
class kcTaskRenderWMosaic : public kcTaskBase {};

class Task_Eff_WipeCircle : public kcTaskBase {};
class Task_Eff_Wipe : public kcTaskBase {};
class Task_Eff_UnderWater : public kcTaskBase {};
class Task_Eff_GlassBreak : public kcTaskBase {};
class Task_Eff2D_Fade : public kcTaskBase {};
class Task_Eff_Weather : public kcTaskBase {};
class Task_Eff2D_WipeAlpha : public kcTaskBase {};
class Task_Eff2D_Wipe2 : public kcTaskBase {};
class Task_Eff2D_Wipe3 : public kcTaskBase {};
class Task_Eff2D_WipeAlpha3 : public kcTaskBase {};
class Task_Eff2D_Wipe : public kcTaskBase {};
class Task_Eff_TileRev : public kcTaskBase {};
class Task_Eff_RainDrop : public kcTaskBase {};
class Task_Eff_Raster : public kcTaskBase {};
class Task_Eff_Hydro : public kcTaskBase {};



class kcWLBase : public kcClassBase {};
class kcWLAdPcm : public class_kcWLBase {};
class kcWLOgg : public class_kcWLBase {};
class kcWLWav : public class_kcWLBase {};


class kcFEScriptObjBase : public kcClassBase {};

class kcFEScriptObjCPlane : public kcFEScriptObjBase {};
class kcFEScriptObjPlane : public kcFEScriptObjCPlane {};

class kcFEScriptObjNvString : public kcFEScriptObjPlane {};
class kcFEScriptObjString : public kcFEScriptObjPlane {};

class kcFEScriptObjSound : public kcFEScriptObjBase {};
class kcFEScriptObjSndTable : public kcFEScriptObjSound {};

class kcFEScriptObjDraw : public kcFEScriptObjBase {};
class kcFEScriptObjRDraw : public kcFEScriptObjBase {};
class kcFEScriptObjRWipe : public kcFEScriptObjBase {};
class kcFEScriptObjWBreak : public kcFEScriptObjBase {};
class kcFEScriptObjWipe : public kcFEScriptObjBase {};

class kcFEScriptObjLogString : public kcFEScriptObjBase {};

class kcFEScriptObjParticle : public kcFEScriptObjBase {};

class kcFEScriptObjScroll : public kcFEScriptObjBase {};
class kcFEScriptObjSlider : public kcFEScriptObjBase {};
class kcFEScriptObjScroll2 : public kcFEScriptObjBase {};

class kcFEScriptObjCImage : public kcFEScriptObjBase {};
class kcFEScriptObjImage : public kcFEScriptObjCImage {};
class kcFEScriptObjDifImage : public kcFEScriptObjCImage {};

class kcFEScriptObjButton : public kcFEScriptObjImage {};

class kcFEScriptObjNumImg : public kcFEScriptObjImage {};
class kcFEScriptObjImgWnd : public kcFEScriptObjImage {};
class kcFEScriptObjBgTable : public kcFEScriptObjImage {};
class kcFEScriptObjBgTable2 : public kcFEScriptObjImage {};
class kcFEScriptObjChTable : public kcFEScriptObjImage {};
class kcFEScriptObjChTable2 : public kcFEScriptObjImage {};



class kcFEScriptObjPanel : public kcFEScriptObjImage {};
class kcFEScriptObjSavePanel : public kcFEScriptObjPanel {};



//class ScriptDecode2 {}; // seen in error log message

//s_e:\prg\vs2008\kclib-dx9\include\_0061585c
//"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"

// search for kclib classes
//REGEX: kc[A-Z][A-Za-z0-0_]+


//https://docs.microsoft.com/en-us/windows/win32/api/ddeml/nf-ddeml-ddecreatestringhandlea

//Creates a handle that identifies the specified string. A Dynamic Data Exchange (DDE) client or server application can pass the string handle as a parameter to other Dynamic Data Exchange Management Library (DDEML) functions.

// Dynamic Data Exchange (DDE)
class kcDDE {
    void GetServerData() {

    }
};
class kcCaps {};

class kcTaskParticlePluginFunc {};
// vftable 31 functions, including Mersenne Twister Int/Real1

typedef enum {
    KCCAPS_DDE_DXVER = 0x4,
    KCCAPS_DDE_DXSTR = 0x80,
    KCCAPS_DDE_CPU = 0x140,
} KCDDE_TOPIC;


struct INonDelegatingUnknown {};
struct IPin {};
struct IQualityControl {};
struct IAsyncReader {};
struct IUnknown {};
struct IVMRSurfaceAllocator9 {};
struct IVMRImagePresenter9 {};

RTTIBaseClassDescriptor
RTTICompleteObjectLocator
RTTIClassHierarchyDescriptor
RTTIBaseClassDescriptor *[2]

//RTTI_Base_Class_Descriptor_at_(0,-1,0,64)
//kcVMRAllocator::RTTI Base Class Descriptor at (0,-1,0,64)class_kcVMRAllocator_RTTI_Type_Descriptor

//CUnknown::RTTI_Base_Class_Descriptor_at_(0,-1,0,64)
//INonDelegatingUnknown::RTTI_Base_Class_Descriptor_at_(0,-1,0,64)
//CBaseObject::RTTI_Base_Class_Descriptor_at_(4,-1,0,64)
//IUnknown::RTTI_Base_Class_Descriptor_at_(12,-1,0,66)
//IUnknown::RTTI_Base_Class_Descriptor_at_(16,-1,0,66)

class CBasePin {};
class CAsyncReader {};
class CBaseFilter {};
class CUnknown {};
class CAsyncStream {};
class CAsyncOutputPin : public IAsyncReader, public IUnknown, 
class CBaseObject {};

class kcVideoSourceFilter_FileReader_RTTI_Type_Descriptor

class kcVideoSourceFilter_FileReader_RTTI_Type_Descriptor

class kcVideoSourceFilter_MemStream_RTTI_Type_Descriptor

class kcVMRAllocator {};

class kcFileBase : public kcClassBase {};
class kcFile : public kcFileBase {};
class kcBigFile : public kcFileBase {};
class kcMemFile : public kcFileBase {};
class kcFileServer {};

class kcProtect { /* Blowfish */
    unsigned int *PArray;
    unsigned int *SBoxes;
};

class kcTaskParticlePlugin {};

template<class T>
class PluginServer {};

class PluginServer<class_cs2Conf> {};
class PluginServer<kcTaskParticlePlugin> {};


class kcD3DCore9 {};
class kcView {};
class kc2DTextureManager {};

class kcXml {};
class kcWindow {};

class kcWndMenu : public kcClassBase {};

class MainWndMenu : public kcWndMenu {};

```