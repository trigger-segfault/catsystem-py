#include <stdio.h>
#include <string.h>
#include <zlib.h>

typedef int bool32;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
// typedef unsigned long long ulonglong;
typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
// typedef unsigned long long undefined8;

// "ANM" | Animation Script | *.anm
#define SIGNATURE_ANM   "ANM"
// "CSS" | CatScene Save Data | savegen.dat; save%04d.dat
#define SIGNATURE_CSS   "CSS"
// "CSTL" | CatScene Language | *.cstl
#define SIGNATURE_CSTL  "CSTL"
// "FES" | Front End Script | *.fes
#define SIGNATURE_FES   "FES"
// "KCF" | KISS Cel-shaded Image | *.kcf
#define SIGNATURE_KCF   "KCF"
// "KCS" | Kclib Script | *.kcs; *.mot
#define SIGNATURE_KCS   "KCS"
// "KIF" | Kclib Integrated File Archive | *.int
#define SIGNATURE_KIF   "KIF"
// "KX2" | Kclib Unknown 2D Script | *.kx2
#define SIGNATURE_KX2   "KX2"
// "KX3" | Kclib Unknown 2D Script | *.kx3
#define SIGNATURE_KX3   "KX3"
// "HG-2" | HG-2 Image | *.hg2
#define SIGNATURE_HG2   "HG-2"
// "HG-3" | H3-2 Image |*.hg3
#define SIGNATURE_HG3   "HG-3"
// "WPM" | WPM Image | *.wpm
#define SIGNATURE_WPM   "WPM"

// "OggS" | Ogg Vorbis Sound | *.ogg
#define SIGNATURE_OGG   "OggS"
// "RIFF" | Resource Interchange File Format | *.wav
#define SIGNATURE_RIFF   "RIFF"
// "WAVE" | Resource Interchange File Format WAVE Content ID | *.wav
#define TAG_RIFF_WAVE   "WAVE"
// "fmt " | Resource Interchange File Format WAVE Format Tag | *.wav
#define TAG_RIFF_FMT   "fmt "

// "BM" | Bitmap Image | *.bmp
#define SIGNATURE_BMP   "BM"

// "BM" | Bitmap Image | *.bmp
#define MAGIC_BMP   0x4D42

// ... | Windows Media Video Format | *.wmv
#define MAGIC_WMV    0x11CF668E75B22630
#define MAGIC_WMV_1  0x75B22630
#define MAGIC_WMV_2  0x11CF668E

// "OTTO" | Open Type Font | *.otf
#define SIGNATURE_OTF   "OTTO"
// "OTTO" | Open Type Font | *.otf
#define MAGIC_OTF   0x4F54544F


// ... | True Type Font | *.ttf
//   Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
// 00000000: 00 01 00 00 00 14 01 00 00 04 00 40 46 46 54 4D    ...........@FFTM
// 00000010: 6F 14 66 B3 00 AA 6B AC 00 00 00 1C 47 44 45 46    o.f3.*k,....GDEF
// 00000020: 3D DF 76 29 00 A9 58 24 00 00 06 E6 47 50 4F 53    =_v).)X$...fGPOS
// 00000030: E6 63 C6 40 00 A9 82 58 00 00 50 02 47 53 55 42    fcF@.).X..P.GSUB
// 00000040: B7 F5 F8 47 00 A9 5F 0C 00 00 23 4C 4F 53 2F 32    7uxG.)_...#LOS/2
// 00000050: 41 4E F4 F7 00 00 01 C8 00 00 00 56 63 6D 61 70    ANtw...H...Vcmap
// 00000060: 1F 78 7E 79 00 01 31 E0 00 01 75 86 63 76 74 20    .x~y..1`..u.cvt.
// 00000070: 06 93 02 FB 00 02 AA 00 00 00 00 0E 66 70 67 6D    ...{..*.....fpgm
// 00000080: 0F B4 2F A7 00 02 A7 68 00 00 02 65 67 61 73 70    .4/'..'h...egasp
// 00000090: FF FF 00 03 00 A9 58 1C 00 00 00 08 67 6C 79 66    .....)X.....glyf
// 000000a0: 28 65 D0 B8 00 03 DC 5C 00 A2 32 A8 68 65 61 64    (eP8..\\."2(head
// 000000b0: 01 97 22 D0 00 00 01 4C 00 00 00 36 68 68 65 61    .."P...L...6hhea
// 000000c0: 04 BE 4B C9 00 00 01 84 00 00 00 24 68 6D 74 78    .>KI.......$hmtx
// 000000d0: 27 37 6E 00 00 00 02 20 00 01 2F C0 6C 6F 63 61    '7n......./@loca
// 000000e0: EF A5 39 1C 00 02 AA 10 00 01 32 4C 6D 61 78 70    o%9...*...2Lmaxp
// 000000f0: 4D D3 02 9D 00 00 01 A8 00 00 00 20 6E 61 6D 65    MS.....(....name
// 00000100: A5 77 06 21 00 A6 0F 04 00 00 12 CC 70 6F 73 74    %w.!.&.....Lpost
// 00000110: 1B D4 A4 05 00 A6 21 D0 00 03 36 4B 70 72 65 70    .T$..&!P..6Kprep
// 00000120: B0 F2 2B 14 00 02 A9 D0 00 00 00 2E 76 68 65 61    0r+...)P....vhea
// 00000130: 05 75 0D B9 00 AA 6B 88 00 00 00 24 76 6D 74 78    .u.9.*k....$vmtx
// 00000140: 91 88 1E D7 00 A9 D2 5C 00 00 99 2C 00 01 00 00    ...W.)R\...,....


// ... | Icon Image File | *.ico; *.cur
//   Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
// 00000000: 00 00 01 00 09 00 20 20 10 00 01 00 04 00 E8 02    ..............h.
// 00000010: 00 00 96 00 00 00 10 10 10 00 01 00 04 00 28 01    ..............(.
// 00000020: 00 00 7E 03 00 00 30 30 00 00 01 00 08 00 A8 0E    ..~...00......(.
// 00000030: 00 00 A6 04 00 00 20 20 00 00 01 00 08 00 A8 08    ..&...........(.

// m2config.int extensions:
// all are plaintext formats
// .cnf (1)
// .def (1)
// .dif
// .inc
// .pfl
// .inf


// config.int extensions:
// plaintext formats:
// *.xml
// *.csv  (i.e. nametable.csv)
// *.txt
// *.ini (?)
// *.tbl  (i.e. bgtone.tbl)
// CharList.cl
//
// binary formats:
// cglist*.dat (no sig?)

// (plaintext) | Definition File | *.dfn  (used outside archives in CatSystem2)
//
//EXAMPLE FILE:

// インストール定義ファイル

// 初期化情報
// #INIT
// brand		"フロントウイング"
// title		"グリザイアの果実"
// bg			"install.hg3"
// folder		"frontwing\グリザイアの果実"
// regpath		"frontwing\グリザイアの果実"
// pathkey		"path"
// startmenu	"frontwing\グリザイアの果実"
// shortcut	0	"グリザイアの果実.lnk"
// link		0	"Grisaia.exe"
// icon		0	"icon.ico" 0
// uninst		"uninst.exe"
// uninstkey	"FW_Grisaia"
// mutex		"FWMUTEXOBJ"
// manual		"manual\manual.htm"
// cmode		1		// 0:norm  1:opt
// v_code		"XX-XXXXXXXX"  //CENSORED TO AVOID TAKEDOWNS
// instkey		"FW_Grisaia"
// keyurl		"http://somedomain/xxxxxxxxx"  //REMOVED TO AVOID TAKEDOWNS
// keycrc		0xDEADBEEF  //CENSORED TO AVOID TAKEDOWNS
// keyretry	3

// set1	"system"	"システムファイル"
// set1	"CG"		"CGデータ"
// set1	"BGM"		"BGMデータ"
// set1	"VOICE"		"音声データ"
// set1	"SE"		"効果音データ"
// set1	"MOVIE"		"ムービーデータ"


// #system
// copy	"Grisaia.exe"				"Grisaia.exe"
// copy	"bootmenu.exe"			"bootmenu.exe"
// copy	"bootmenu.hg3"			"bootmenu.hg3"
// copy	"boot.dfn"				"boot.dfn"
// copy	"uninst.exe"			"uninst.exe"
// copy	"uninst.dfn"			"uninst.dfn"
// copy	"uninstall.hg3"			"uninstall.hg3"
// copy	"config"				"config"
// copy	"config.int"			"config.int"
// copy	"fes.int"				"fes.int"
// copy	"kcs.int"				"kcs.int"
// copy	"kx2.int"				"kx2.int"
// copy	"ptcl.int"				"ptcl.int"
// copy	"mot.int"				"mot.int"
// dcopy	"scene.int.FWP"				"scene.int"
// copy	"icon.ico"			"icon.ico"
// copy	"cs2conf.dll"			"cs2conf.dll"
// copy	"readme.txt"			"readme.txt"
// copy	"export"			"export"

// #CG
// copy	"image.int"			"image.int"

// #BGM
// copy	"bgm.int"				"bgm.int"

// #VOICE
// copy	"pcm_a.int"				"pcm_a.int"
// copy	"pcm_b.int"				"pcm_b.int"
// copy	"pcm_c.int"				"pcm_c.int"
// copy	"pcm_d.int"				"pcm_d.int"
// copy	"pcm_e.int"				"pcm_e.int"
// copy	"pcm_f.int"				"pcm_f.int"
// copy	"pcm_g.int"				"pcm_g.int"
// copy	"pcm_h.int"				"pcm_h.int"
// copy	"pcm_i.int"				"pcm_i.int"

// #SE
// copy	"se.int"				"se.int"

// #MOVIE
// copy	"movie.int"				"movie.int"

// // 起動パスワードコメント
// // A:接続失敗時 B:直接入力時 C:パスワードミス時
// #PS_COMMENT_A
// 起動パスワードは<http://frontwing.jp/pass.html>を始め
// 公式サイト内サポート、グリザイアの果実公式サイトproductページ等に記載しております。
// パソコンを始め携帯でのアクセスも対応しております。

// #PS_COMMENT_B
// 起動パスワードは<http://frontwing.jp/pass.html>を始め
// 公式サイト内サポート、グリザイアの果実公式サイトproductページ等に記載しております。
// 携帯でのアクセスも対応しております。
// 一切のネットワーク環境がない場合はユーザー葉書を送付ください。
// 詳しくはパッケージ同梱の案内をご参照ください。

// #PS_COMMENT_C
// コピーペーストの際はスペースや抜けにご注意ください。
// 直接入力をしている際は再度文字をご確認ください。

// または、ユーザーサポートまでお問い合わせください。


// ... | HM3 Unknown Image File | *.hm3  (appears in late CatSystem1 games)
//
//   Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
// 00000000: 00 00 00 00 64 00 5A 02 10 13 0C 14 2E 0C 37 23    ....d.Z.......7#
// 00000010: 17 25 4A 19 0D 8A 0B 2F 5E 20 48 2F 76 3D 68 32    .%J..../^.H/v=h2
// 00000020: 12 D5 12 6A 41 22 4A 7D 3C 59 6C 5C 59 89 47 6B    .U.jA"J}<Yl\Y.Gk
// 00000030: 77 71 6A 9E 4F 94 53 43 7F 89 7A 6B D3 2E 79 AF    wqj.O.SC..zkS.y/
// 00000040: 5D 80 82 C0 A8 73 3F 84 BE 66 91 9E 8A 8E 8D C3    ]..@(s?.>f.....C
// 00000050: 81 EA 47 93 C1 7A 99 9B D1 A3 AB 94 99 D7 76 C8    .jG.Az..Q#+..WvH
// 00000060: 94 4C A7 A8 D9 AE B3 9E 9B F2 5D A3 D4 86 B2 C6    .L'(Y.3..r]#T.2F
// 00000070: 9E B1 B5 DF AD FB 5F BD C6 A8 B3 E7 90 BC C0 E7    .15_-{_=F(3g.<@g
// 00000080: E1 B2 5E BB FC 70 C5 CB C5 C6 D5 A6 BD EF A3 C5    a2^;|pEKEFU&=o#E
// 00000090: CA EE C6 FC 80 D2 DA B3 C8 F3 AB D3 D6 D2 CF D4    JnF|.RZ3Hs+SVROT
// 000000a0: F4 EF CB 7E D1 F7 B4 DC E2 D5 DF FE 8C DE DD F3    toK~Qw4\bU_~.^]s
// 000000b0: DA F9 C1 E6 E9 E5 E5 FA CA EE EF F1 EF FA D8 F4    ZyAfieezJnoqozXt
// 000000c0: F7 EF F9 FB F7 FE FE FE 00 00 00 00 00 00 00 00    woy{w~~~........
// 000000d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000110: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000190: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000001f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000210: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000220: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000230: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000240: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000250: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000260: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000270: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000290: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 000002f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
// 00000300: 00 00 00 00 00 00 00 00 41 F4 1F FB C8 17 C4 07    ........At.{H.D.
// 00000310: F8 41 FE 20 F2 03 FC 68 0F DA 0B 40 7E 72 10 72    xA~.r.|h.Z.@~r.r
// 00000320: 07 D8 6C 0D A9 36 66 C2 BF 7F 86 1A 4B 6B 08 3E    .Xl.)6fB?...Kk.>
// 00000330: 80 FF A9 1A A0 3F 94 14 CF 1E D5 9A 7B 6F 61 46    ..)..?..O.U.{oaF

// (plaintext) | Definition File | *.def  (used in early CatSystem1 and later m2config.int)
//
//EXAMPLE FILE:

// ClientWidth = 800
// ClientHeight = 600
// SpeedMenu = 1
// FontMenu = 1
// FullScreen = 0
// MsgSpeed = 3
// Font = "ＭＳ ゴシック"


// (plaintext) | CD Identifier File | *.id  (used for something with CDs, not important)


// 0x00000028 "(" | CatSystem (1) Script | *.csx
// signature is part of the file structure, but possibly always starts off the file.

// ... | MPG Video File | *.mpg
//   Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
// 00000000: 00 00 01 BA 21 00 01 00 03 80 70 D3 00 00 01 BB    ...:!.....pS...;
// 00000010: 00 0C 80 70 D3 06 E1 FF E0 E0 F0 C0 C0 20 00 00    ...pS.a.``p@@...
// 00000020: 01 BE 07 DC 0F FF FF FF FF FF FF FF FF FF FF FF    .>.\............
// 00000030: FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 

// ... | LV PCM File | *.lv
// Found in pcm_?.int archives, shares names with *.ogg files
//   Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
// 00000000: D3 A0 29 3D 08 09 D4 3D E0 3A 34 3E 22 DF 81 3E    S.)=..T=`:4>"_.>
// 00000010: 0E A7 A5 3E 11 25 C2 3E 1A B1 D5 3E 1E 77 DF 3E    .'%>.%B>.1U>.w_>
// 00000020: 5B 66 F0 3E 27 26 0A 3F 65 7D 44 3F 65 7D 44 3F    [fp>'&.?e}D?e}D?
// 00000030: 65 7D 44 3F 65 7D 44 3F 63 F9 1C 40 B9 18 42 40    e}D?e}D?cy.@9.B@
// 00000040: 82 F2 65 40 87 0B 83 40 F9 2F 90 40 35 30 99 40    .re@...@y/.@50.@
// 00000050: F9 08 9E 40 28 16 9F 40 B1 1B 9D 40 98 12 99 40    y..@(..@1..@...@
// 00000060: 45 DC 93 40 4E 72 8E 40 3D 71 89 40 3A 7E 85 40    E\.@Nr.@=q.@:~.@

// "01" | PCM Tag Data | *.tag
#define SIGNATURE_PCMTAG  "01"
// "01" | PCM Tag Data | *.tag
#define MAGIC_PCMTAG  0x3130

// "CatScene" | CatScene Script | *.cst
#define SIGNATURE_CATSCENE   "CatScene"
// "CatScene" | CatScene Script | *.cst
#define SIGNATURE_CST   "CST"

// "KCLZ" | KISS LZH Compression Tag (probably) | *.kcf
#define TAG_KCLZ      "KCLZ"
// "stdinfo" | HG-3 Standard Information Tag (always first) | *.hg3
#define TAG_STDINFO   "stdinfo"
// "img%04d" | HG-3 HG Image Slice Tag (numbered) | *.hg3
#define TAG_IMG_fmt_04d  "img%04d"
// "img_al" | HG-3 JPEG Image Alpha Tag (appears after "img_jpg" tag) | *.hg3
#define TAG_IMG_AL    "img_al"
// "img_jpg" | HG-3 JPEG Image Tag | *.hg3
#define TAG_IMG_JPG   "img_jpg"
// "img_wbp" | HG-3 WebP Image Tag | *.hg3
#define TAG_IMG_WBP   "img_wbp"
// "img_jpa" | HG-3 Unknown Image Tag | *.hg3
//#define TAG_IMG_JPA   "img_jpa"
// "ats%04d" | HG-3 Attribute Tag (numbered) | *.hg3
#define TAG_ATS_fmt_04d  "ats%04d"
// "imgmode" | HG-3 Image Mode Tag | *.hg3
#define TAG_IMGMODE   "imgmode"
// "cptype" | HG-3 CP Type Tag | *.hg3
#define TAG_CPTYPE    "cptype"

// "ANM" | Animation Script | *.anm
#define MAGIC_ANM   0x4D4E41
// "CSS" | CatScene Save Data | savegen.dat; save%04d.dat
#define MAGIC_CSS   0x535343
// "CSTL" | CatScene Language | *.cstl
#define MAGIC_CSTL  0x4C545343
// "FES" | Front End Script | *.fes
#define MAGIC_FES   0x534546
// "KCF" | KISS Cel-shaded Image | *.kcf
#define MAGIC_KCF   0x46434B
// "KCF" | KISS LZH Compression Tag (probably) | *.kcf
#define MAGIC_KCLZ  0x5A4C434B
// "KCS" | Kclib Script | *.kcs; *.mot
#define MAGIC_KCS   0x53434B
// "KIF" | Kclib Integrated File Archive | *.int
#define MAGIC_KIF   0x46494B
// "KX2" | Kclib Unknown 2D Script | *.kx2
#define MAGIC_KX2   0x32584B
// "KX3" | Kclib Unknown 3D Script | *.kx3
#define MAGIC_KX3   0x33584B
// "HG-2" | HG-2 Image | *.hg2
#define MAGIC_HG2   0x322D4748
// "HG-3" | HG-3 Image | *.hg3
#define MAGIC_HG3   0x332D4748
// "WPM" | WPM Image | *.wpm
#define MAGIC_WPM   0x4D5057

// "OggS" | Ogg Vorbis Sound | *.ogg
#define MAGIC_OGG   0x5367674F

// "CatScene" | CatScene Script | *.cst
#define MAGIC_CATSCENE    0x656E656353746143LL
// "CatS" | CatScene Script | *.cst
#define MAGIC_CATSCENE_1  0x53746143
// "cene" | CatScene Script | *.cst
#define MAGIC_CATSCENE_2  0x656E6563
// "CatScene" | CatScene Script | *.cst
#define MAGIC_CST    0x656E656353746143LL
// "CatS" | CatScene Script | *.cst
#define MAGIC_CST_1  0x53746143
// "cene" | CatScene Script | *.cst
#define MAGIC_CST_2  0x656E6563

// "CsPack" | CatSystem Pack Archive | *.dat; *.*; *.css (scene); *.csp (script)
// Not an actual format, use to check for both CsPack1 and CsPack2 archives simultaneously
#define SIGNATURE_CSPACK  "CsPack"


// "CsPack1" | CatSystem Pack v1 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define SIGNATURE_CSPACK1  "CsPack1"
// "CsPack2" | CatSystem Pack v2 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define SIGNATURE_CSPACK2  "CsPack2"

// "CsPack" | CatSystem Pack Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK  0x316b6361507343LL
// "CsPa" | CatSystem Pack Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK_1  0x61507343
// "ck" | CatSystem Pack Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK_2  0x6b63

// "CsPack1" | CatSystem Pack v1 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK1  0x316b6361507343LL
// "CsPa" | CatSystem Pack v1 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK1_1  0x61507343
// "ck1" | CatSystem Pack v1 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK1_2  0x316b63
// "CsPack2" | CatSystem Pack v2 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK2  0x326b6361507343LL
// "CsPack2" | CatSystem Pack v2 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK2_1  0x61507343LL
// "ck2" | CatSystem Pack v2 Archive | *.dat; *.*; *.css (scene); *.csp (script)
#define MAGIC_CSPACK2_2  0x326b63

//43 73 50 61 63 6b 31

//0x316b63 61507343
//0x326b63 61507343


// "HG2(%d) : No corresponding ID found"
#define LOG_HG2_NO_CORRESPONDING_ID_FOUND "HG2(%d) : 対応するIDが見つかりません"
// "HG2 : Unsupported version"
#define LOG_HG2_UNSUPPORTED_VERSION "HG2 : 未対応のバージョンです"
// "HG2 : Header is different"
#define LOG_HG2_HEADER_IS_DIFFERENT "HG2 : ヘッダが違います"

// "kcImageHg2 : Size of byte area is abnormal"
#define LOG_KCIMAGEHG2_UNEXPECTED_BYTEAREA_SIZE "kcImageHg2 : バイト領域のサイズが異常です"
// "kcImageHg2 : Bit area size is abnormal"
#define LOG_KCIMAGEHG2_UNEXPECTED_BITAREA_SIZE "kcImageHg2 : ビット領域のサイズが異常です"

// "Not compatible with this number of colors"
#define LOG_INCOMPATIBLE_NUMBER_OF_COLORS "この色数には対応していません"
// "Image type is different"
#define LOG_IMAGE_TYPE_IS_DIFFERENT "イメージのタイプが違います"

#define LOG_Video_Mixing_Renderer_9 "Video Mixing Renderer 9"


// "No more than 4 bpp supported"
#define LOG_NO_MORE_THAN_4_BPP_SUPPORTED "4 を超える bpp はサポートしていません"
// "Failed to get plain buffer : size = %d"
#define LOG_FAILED_TO_GET_PLANE_BUFSIZE "プレーンバッファの確保に失敗しました : size = %d"


#define LOG_StretchRect_Failed "StretchRect Failed"

// "It is an image that cannot be expanded"
#define LOG_THIS_IMAGE_CANNOT_BE_EXPANDED "展開できないイメージです"
// "Unsupported BPP  bpp=%d"
#define LOG_UNSUPPORTED_BPP_fmt_d "サポート外BPPです  bpp=%d"
// "Image copy failed:%s"
#define LOG_IMAGE_COPY_FAILED_fmt_s "イメージのコピーに失敗しました:%s"
// "Image save failed:%s"
#define LOG_IMAGE_SAVE_FAILED_fmt_s "イメージの保存に失敗しました:%s"
// "File open failed:%s"
#define LOG_FILE_OPEN_FAILED_fmt_s "ファイルのオープンに失敗しました:%s"
// "File write failed:%s"
#define LOG_FILE_WRITE_FAILED_fmt_s "ファイルの書き込みに失敗しました:%s"
// "Image copy failed"
#define LOG_FAILED_TO_COPY_IMAGE "イメージのコピーに失敗しました"
// "Image save failed"
#define LOG_FAILED_TO_SAVE_IMAGE "イメージの保存に失敗しました"
// "Illegal bpp"
#define LOG_ILLEGAL_BPP "不正な bpp です"
// "Failed to allocate pixel image buffer"
#define LOG_FAILED_TO_ALLOCATE_PIXEL_IMAGE_BUFFER "ピクセルイメージ用バッファの確保に失敗しました"


// "Failed to create heap area"
#define LOG_ "ヒープ領域の作成に失敗しました"

// "Heap created. size = %d (0x%x) byte (%fMB)"
#define LOG_HEAP_CREATED_fmt_d_x_f "ヒープを作成しました。 size = %d (0x%x) byte (%fMB)"
// "Heap destroyed."
#define LOG_DESTROYED_HEAP "ヒープを破棄しました。"

// "Memory could not be allocated  id = %d  size = %d(0x%x)"
#define LOG_MEMORY_COULD_NOT_BE_ALLOCATED_fmt_d_d_x "メモリが確保出来ませんでした  id = %d  size = %d(0x%x)"


// "kcProtect : Data size must be in 8-byte units"
#define LOG_KCPROTECT_DATA_SIZE_MUST_BE_IN_8BYTE_UNITS "kcProtect : データサイズは8バイト単位である必要があります"


// 00640094
#define LAST_REACHED_STRING "Copyright (c) 1992-2004 by P.J. Plauger, licensed by Dinkumware, Ltd. ALL RIGHTS RESERVED."

#define zlib_uncompress uncompress

// dummy
#define DAT_007a0c44 ((void*)0)

// Header of both HG-2 and HG-3 images.
typedef struct {
    /*$0,4*/  unsigned int Magic;
    /*$4,4*/  unsigned int HeaderSize;
    /*$8,2*/  short Version; // compared as short in HG-2
    /*A,2*/  short x_padding;
    /*C*/
} HGHEADER;



typedef struct {
    /*$0,4*/  int SliceIndex;
    /*$4,4*/  int SliceLength;
    /*$8,4*/  int DataPacked;
    /*C,4*/  int DataUnpacked;
    /*$10,4*/ int CtlPacked;
    /*$14,4*/ int CtlUnpacked;
    /*$18*/
} HGSLICE;

typedef struct {
    // Version 0x10:
    /*$0,4*/  int Width;
    /*$4,4*/  int Height;
    /*$8,2*/  short BPP;
    /*A,2*/  short Depth;
    /*C,18*/ HGSLICE Slice;
    //    /*C,4*/  int SliceIndex;
    //    /*$10,4*/ int SliceLength;
    //    /*$14,4*/ int DataPacked;
    //    /*$18,4*/ int DataUnpacked;
    //    /*$1C,4*/ int CtlPacked;
    //    /*$20,4*/ int CtlUnpacked;
    // Version 0x20:
    /*$24,4*/ int OffsetToData;
    /*$28,4*/ int ID;
    /*$2C,4*/ int CanvasWidth;
    /*$30,4*/ int CanvasHeight;
    /*$34,4*/ int OffsetX;
    /*$38,4*/ int OffsetY;
    /*$3C,4*/ int HasTransparency; //bool32
    /*$40,4*/ int OffsetToNext;
    // Version 0x25:
    /*$44,4*/ int OriginX;
    /*$48,4*/ int OriginY;
    /*$4C*/
} HG2METADATA;

typedef struct {
    /*$0,4*/  unsigned int Magic;
    /*$4,4*/  unsigned int HeaderSize;
    /*$8,2*/  short Version; // compared as short in HG-2
    /*A,2*/  short x_padding;
    /*C,4C*/ HG2METADATA Image;
} HG2FILEINFO;

typedef struct {
    /*$0,4*/  HG2FILEINFO *FileInfo;
    /*$4,4*/  undefined4 Unk4;
    /*$8,4*/  int Width;
    /*C,4*/  int Height;

} kcImageHg2;

typedef struct {
    /*$0,4*/  int ID;
    /*$4,4*/ int Width; //iVar6
    /*$8,4*/ int Height; //iVar4

    /*C,4*/ int UnkC;
    /*$10,4*/ int Unk10; //(unnassigned?)

    /*$14,4*/ int Stride; //iVar7 * iVar6
    /*$18,4*/ int ByteDepth; //iVar7
    /*$1C,4*/ int CanvasWidth; //local_30[2]
    /*$20,4*/ int CanvasHeight; //local_30[3]

    /*$24,4*/ int Unk24; //OffsetX?  local_20
    /*$28,4*/ int Unk28; //OffsetX?  local_1c

    /*$2C,4*/ int Unk2C; //HasTransparency?  0, 1, or 2?  local_18, 0 for version < 0x20
    //NOTE: Skips OffsetToNext field
    /*$30,4*/ int Unk30; //OriginX?  local_10
    /*$34,4*/ int Unk34; //OriginY?  local_c

    /*$38,4*/ int Unk38; //0
    /*$3C,4*/ int Unk3C; //0
    /*$40,4*/ int Width40; //iVar6
    /*$44,4*/ int Height44; //iVar4

    /*$48,4*/ int Unk48; //
    /*$4C,4*/ int Unk4C; //
    /*$50,4*/ int Unk50; //
    /*$54,4*/ int Unk54; //
    /*$58,4*/ int Unk58; //ptr
} kcImageSearch;
    // *(int *)((int)search + 0x14) = iVar7 * iVar6;
    // *(int *)((int)search + 4) = iVar6;
    // *(int *)((int)search + 8) = iVar4;
    // *(int *)((int)search + 0x18) = iVar7;
    // *(undefined4 *)((int)search + 0x38) = 0;
    // *(undefined4 *)((int)search + 0x3c) = 0;
    // *(int *)((int)search + 0x40) = iVar6;
    // *(int *)((int)search + 0x44) = iVar4;
    // iVar17 = *(int *)((int)search + 0x14);
    // pcVar14 = (char *)((*(int *)((int)search + 8) + -1) * iVar17 + *(int *)((int)search + 0xc));

    // *(undefined4 *)((int)search + 0x1c) = local_30[2];
    // *(undefined4 *)((int)search + 0x20) = local_30[3];
    // *(undefined4 *)((int)search + 0x24) = local_20;
    // *(undefined4 *)((int)search + 0x28) = local_1c;
    // //if (*(short *)(in_EAX + 8) < 0x20) {
    // if (hg2File->Version < 0x20) {
    //   *(undefined4 *)((int)search + 0x2c) = 0;
    // }
    // else {
    //   *(int *)((int)search + 0x2c) = (unsigned int)(local_18 != 0) + 1;
    // }
    // *(undefined4 *)((int)search + 0x30) = local_10;
    // *(undefined4 *)((int)search + 0x34) = local_c;
//hg2_unkfunc_2_004e7710
//kcImageHg2_ReadImageID

        // *puVar18 = *(unsigned int *)((int)search + 0xd74 + (unsigned int)pbVar13[iVar17] * 4) |
        //            *(unsigned int *)((int)search + 0x574 + (unsigned int)pbVar13[-iVar17] * 4) |
        //            *(unsigned int *)((int)search + 0x174 + (unsigned int)*local_4c * 4) |
        //            *(unsigned int *)((int)search + 0x974 + (unsigned int)*pbVar13 * 4);


    // iVar6 = *(int *)((int)search + 0x58);
    // if (iVar6 != 0) {
    //   if (*(int *)(iVar6 + -4) < 2) {
    //     HeapFree(DAT_007a0c44,0,(int *)(iVar6 + -4));
    //   }
    //   else {
    //     *(int *)(iVar6 + -4) = *(int *)(iVar6 + -4) + -1;
    //   }
    // }
    // *(undefined4 *)((int)search + 0x58) = 0;
    // if (iVar9 != 0) {
    //   if (*(int *)(iVar9 + -4) < 2) {
    //     HeapFree(DAT_007a0c44,0,(int *)(iVar9 + -4));
    //   }
    //   else {
    //     *(int *)(iVar9 + -4) = *(int *)(iVar9 + -4) + -1;
    //   }
    // }



//cs2_log_sendmessage_004b57b0
void kclib_logError(const char *cmsg, ...)

{
  // (dummy func)
}

unsigned int kcImageHg2_ReadImageAt(kcImageHg2 *this, unsigned int index, kcImageSearch *search)

{
    //undefined4 uVar1;
    //int iVar2;
    //unsigned int uVar3;
    //unsigned int result; //undefined4 uVar1;
    HG2METADATA *img; //int iVar2;
    unsigned int i; //unsigned int uVar3;
    

    if (this->FileInfo->Version < 0x25)
    {
        if (index == 0)
        {
            search->ID = 0;
            return kcImageHg2_ReadImageID(this, search);//, 0
        }
    }
    else
    {
        // Find ID of image at index
        img = &this->FileInfo->Image;
        for (i = 0; i < index && img->OffsetToNext; i++)
        {
            img = (HG2METADATA *)((int)img + img->OffsetToNext);
        }
        if (i == index)
        {
            search->ID = img->ID;
            return kcImageHg2_ReadImageID(this, search);//, search->ID
        }
    }
    return 0;
}



//undefined4 __thiscall hg2_unkfunc_2_004e7710(int param_1,int param_1_00)
undefined4 __thiscall kcImageHg2_ReadImageID(kcImageHg2 *this, kcImageSearch *search) //int param_1, int id)

{
  //in_<REG>
  //unaff_<REG>
  //out_<REG>?
  int in_EAX;
  HG2FILEINFO *hg2File;


  int *piVar1;
  char cVar2;
  short version; //short sVar3;
  int iVar4;
  unsigned char bVar5;
  int in_EAX;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  unsigned char *pbVar13;
  char *pcVar14;
  unsigned int uVar15;
  unsigned char bVar16;
  int iVar17;
  unsigned int *puVar18;
  char *pcVar19;
  int *piVar20;
  unsigned int *puVar21;
  int *piVar22;
  char *pcVar23;
  unsigned int uVar24;
  //int *local_60;
  HG2METADATA *img; //int *local_60;
  unsigned char *local_5c;
  int local_54;
  unsigned char *local_4c;
  int local_40;
  int local_30 [4];
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  undefined4 local_10;
  undefined4 local_c;
  
  iVar6 = memcmp(&hg2File->Magic, SIGNATURE_HG2, 4);
  if (iVar6 != 0) {
    kclib_logError(LOG_HG2_HEADER_IS_DIFFERENT);
    return 0;
  }
  version = hg2File->Version;//*(short *)(in_EAX + 8);
  if (version == 0x10) {
    local_30[2] = hg2File->Image.Width; //*(undefined4 *)(in_EAX + 0xc);
    local_30[3] = hg2File->Image.Height; //*(undefined4 *)(in_EAX + 0x10);
    //local_60 = &hg2File->Image; //(int *)(in_EAX + 0xc);
    img = &hg2File->Image; //(int *)(in_EAX + 0xc);
    local_20 = 0;
    local_1c = 0;
    local_18 = 0;
    // this is the start of the image for HG-2 ver 0x10
    local_30[0] = &hg2File->Image.OffsetToData; //in_EAX + 0x30;
  }
  else {
    if (version != 0x20) {
      if (version != 0x25) {
        kclib_logError(LOG_HG2_UNSUPPORTED_VERSION);
        return 0;
      }
      img = &hg2File->Image; //local_60 = (int *)(in_EAX + 0xc);
      piVar1 = &hg2File->Image.OffsetToData; //(int *)(in_EAX + 0x30);
      iVar6 = hg2File->Image.ID; //*(int *)(in_EAX + 0x34);
      //while (iVar6 != param_1_00) {
      while (iVar6 != search->ID) {
        if (img->OffsetToNext == 0) goto LAB_004e7809; //if (piVar1[7] == 0) goto LAB_004e7809;
        img = (HG2METADATA *)((int)img + img->OffsetToNext); //local_60 = (int *)((int)local_60 + piVar1[7]);
        iVar6 = hg2File->Image.ID; //local_60[10];
        piVar1 = &img->OffsetToData; //local_60 + 9;
      }
      // Copy next 10, 4-byte fields into ptr
      local_30[0] = &img->OffsetToData + img->OffsetToData;
      local_30[1] = img->ID;
      local_30[2] = img->CanvasWidth;
      local_30[3] = img->CanvasHeight;
      local_30[4] = img->OffsetX;
      local_30[5] = img->OffsetY;
      local_30[6] = img->HasTransparency;
      local_30[7] = img->OffsetToNext;
      local_30[8] = img->OriginX;
      local_30[9] = img->OriginY;
      // iVar6 = 10;
      // piVar20 = piVar1;
      // piVar22 = local_30;
      // for (int i = 0; i < 10; i++) {
      //   local_30[i] = piVar1[i];
      // }
      // while (iVar6 != 0) {
      //   iVar6 += -1;
      //   *piVar22 = *piVar20;
      //   piVar20 = piVar20 + 1;
      //   piVar22 = piVar22 + 1;
      // }
      // local_30[0] += (int)piVar1;
      goto LAB_004e7854;
    }
    img = &hg2File->Image; //local_60 = (int *)(in_EAX + 0xc);
    piVar1 = &hg2File->Image.OffsetToData; //(int *)(in_EAX + 0x30);
    iVar6 = hg2File->Image.ID; //*(int *)(in_EAX + 0x34);
    //while (iVar6 != param_1_00) {
    while (iVar6 != search->ID) {
      if (piVar1[7] == 0) {
LAB_004e7809:
        //kclib_logError(LOG_HG2_NO_CORRESPONDING_ID_FOUND,param_1_00);
        kclib_logError(LOG_HG2_NO_CORRESPONDING_ID_FOUND, search->ID);
        return 0;
      }
      img = (HG2METADATA *)((int)img + img->OffsetToNext); //local_60 = (int *)((int)local_60 + piVar1[7]);
      iVar6 = hg2File->Image.ID; //local_60[10];
      piVar1 = &img->OffsetToData; //local_60 + 9;
    }
    // Copy next 8, 4-byte fields into ptr
    iVar6 = 8;
    piVar20 = piVar1;
    piVar22 = local_30;
    while (iVar6 != 0) {
      iVar6 += -1;
      *piVar22 = *piVar20;
      piVar20 = piVar20 + 1;
      piVar22 = piVar22 + 1;
    }
    local_30[0] += (int)piVar1;
  }
  local_c = 0;
  local_10 = 0;
LAB_004e7854:
  iVar6 = img->Width; //*local_60;
  iVar4 = img->Height; //local_60[1];
  //WTF: Really funky byteDepth conversion
  iVar7 = (int)(
    (int)*(short *)(local_60 + 2)
  +
    ((int)*(short *)(local_60 + 2) >> 0x1f & 7U)
  ) >> 3;
  iVar7 = (int)((int)*(short *)(local_60 + 2) + ((int)*(short *)(local_60 + 2) >> 0x1f & 7U)) >> 3;
  iVar8 = iVar7 * iVar4 * iVar6;
  iVar17 = local_60[6] + 0x10;
  iVar9 = cs2_HeapAlloc_logged_004b02b0(0,local_60[6] + 0x2010 + iVar8 * 2);
  puVar21 = (unsigned int *)(iVar9 + iVar17);
  local_4c = (unsigned char *)(iVar9 + 0x1000 + iVar8 + iVar17);
  iVar10 = zlib_uncompress(iVar9,local_60[6] + 0x10,local_30[0],local_60[5]);
  iVar17 = local_60[5];
  if (iVar10 != local_60[6]) {
    kclib_logError(LOG_KCIMAGEHG2_UNEXPECTED_BYTEAREA_SIZE);
    if (iVar9 != 0) {
      if (1 < *(int *)(iVar9 + -4)) {
        *(int *)(iVar9 + -4) = *(int *)(iVar9 + -4) + -1;
        return 0;
      }
      HeapFree(DAT_007a0c44,0,(int *)(iVar9 + -4));
    }
    return 0;
  }
  FUN_004e94c0(search);
  iVar17 = zlib_uncompress(*(undefined4 *)((int)search + 0x58),local_60[8] + 0x10,local_30[0] + iVar17,
                        local_60[7]);
  if (iVar17 != local_60[8]) {
    cs2_HeapFree_004b0300(iVar9);
    FUN_004e82b0();
    kclib_logError(LOG_KCIMAGEHG2_UNEXPECTED_BITAREA_SIZE);
    return 0;
  }
  iVar11 = FUN_004e8520();
  iVar10 = FUN_004e83b0();
  std_vectorfunc_unk_004b17a0(local_4c,iVar10);
  local_5c = local_4c;
  iVar17 = iVar9;
  if (iVar11 != 0) {
    iVar11 = FUN_004e83b0();
    iVar10 -= iVar11;
    cs2_strmemcpy_004b1600(local_4c,iVar9,iVar11);
    iVar17 = iVar11 + iVar9;
    local_5c = local_4c + iVar11;
  }
  while (0 < iVar10) {
    iVar11 = FUN_004e83b0();
    local_5c = local_5c + iVar11;
    if (iVar10 - iVar11 < 1) break;
    iVar12 = FUN_004e83b0();
    iVar10 = (iVar10 - iVar11) - iVar12;
    cs2_strmemcpy_004b1600(local_5c,iVar17,iVar12);
    local_5c = local_5c + iVar12;
    iVar17 = iVar17 + iVar12;
  }
  if (((unsigned int)(local_5c + -(int)local_4c) & 3) == 0) {
    iVar17 = (int)(local_5c + -(int)local_4c) >> 2;
    if (0 < iVar17) {
      pbVar13 = local_4c + iVar17 * 2;
      puVar18 = puVar21;
      local_54 = iVar17;
      do {
        *puVar18 = *(unsigned int *)((int)search + 0xd74 + (unsigned int)pbVar13[iVar17] * 4) |
                   *(unsigned int *)((int)search + 0x574 + (unsigned int)pbVar13[-iVar17] * 4) |
                   *(unsigned int *)((int)search + 0x174 + (unsigned int)*local_4c * 4) |
                   *(unsigned int *)((int)search + 0x974 + (unsigned int)*pbVar13 * 4);
        local_4c = local_4c + 1;
        pbVar13 = pbVar13 + 1;
        local_54 += -1;
        puVar18 = puVar18 + 1;
      } while (local_54 != 0);
    }
    FUN_004db480(iVar8);
    *(int *)((int)search + 0x14) = iVar7 * iVar6;
    *(int *)((int)search + 4) = iVar6;
    *(int *)((int)search + 8) = iVar4;
    *(int *)((int)search + 0x18) = iVar7;
    *(undefined4 *)((int)search + 0x38) = 0;
    *(undefined4 *)((int)search + 0x3c) = 0;
    *(int *)((int)search + 0x40) = iVar6;
    *(int *)((int)search + 0x44) = iVar4;
    iVar17 = *(int *)((int)search + 0x14);
    pcVar14 = (char *)((*(int *)((int)search + 8) + -1) * iVar17 + *(int *)((int)search + 0xc));
    iVar8 = iVar7;
    pcVar23 = pcVar14;
    while (iVar8 != 0) {
      iVar8 += -1;
      *pcVar23 = *(char *)((unsigned int)*(unsigned char *)puVar21 + 0x74 + (int)search);
      pcVar23 = pcVar23 + 1;
      puVar21 = (unsigned int *)((int)puVar21 + 1);
    }
    local_5c = (unsigned char *)((iVar6 + -1) * iVar7);
    if (local_5c != (unsigned char *)0x0) {
      pcVar19 = pcVar23 + -iVar7;
      do {
        local_5c = (unsigned char *)((int)local_5c + -1);
        *pcVar23 = *(char *)((unsigned int)*(unsigned char *)puVar21 + 0x74 + (int)search) + *pcVar19;
        pcVar23 = pcVar23 + 1;
        pcVar19 = pcVar19 + 1;
        puVar21 = (unsigned int *)((int)puVar21 + 1);
      } while (local_5c != (unsigned char *)0x0);
    }
    pcVar14 = pcVar14 + -iVar17;
    if (1 < iVar4) {
      local_40 = iVar4 + -1;
      do {
        pcVar23 = pcVar14 + iVar17;
        iVar8 = iVar7 * iVar6;
        pcVar19 = pcVar14;
        while (iVar8 != 0) {
          cVar2 = *pcVar23;
          pcVar23 = pcVar23 + 1;
          iVar8 += -1;
          *pcVar19 = *(char *)((unsigned int)*(unsigned char *)puVar21 + 0x74 + (int)search) + cVar2;
          pcVar19 = pcVar19 + 1;
          puVar21 = (unsigned int *)((int)puVar21 + 1);
        }
        pcVar14 = pcVar14 + -iVar17;
        local_40 += -1;
      } while (local_40 != 0);
    }
    iVar6 = *(int *)((int)search + 0x58);
    if (iVar6 != 0) {
      if (*(int *)(iVar6 + -4) < 2) {
        HeapFree(DAT_007a0c44,0,(int *)(iVar6 + -4));
      }
      else {
        *(int *)(iVar6 + -4) = *(int *)(iVar6 + -4) + -1;
      }
    }
    *(undefined4 *)((int)search + 0x58) = 0;
    if (iVar9 != 0) {
      if (*(int *)(iVar9 + -4) < 2) {
        HeapFree(DAT_007a0c44,0,(int *)(iVar9 + -4));
      }
      else {
        *(int *)(iVar9 + -4) = *(int *)(iVar9 + -4) + -1;
      }
    }
    if (*(short *)((int)local_60 + 10) != 0) {
      puVar21 = *(unsigned int **)((int)search + 0xc);
      cVar2 = (char)*(undefined2 *)((int)local_60 + 10);
      bVar5 = 8 - cVar2;
      bVar16 = cVar2 - bVar5;
      uVar15 = (0xff << (bVar5 & 0x1f) & 0xffU) * 0x1010101;
      local_4c = (unsigned char *)iVar4;
      if (0 < iVar4) {
        do {
          uVar24 = *(unsigned int *)((int)search + 0x14) >> 2;
          puVar18 = puVar21;
          while (uVar24 != 0) {
            uVar24 -= 1;
            *puVar18 = *puVar18 >> (bVar16 & 0x1f) & ~uVar15 | *puVar18 << (bVar5 & 0x1f) & uVar15;
            puVar18 = puVar18 + 1;
          }
          uVar24 = *(unsigned int *)((int)search + 0x14) & 3;
          while (uVar24 != 0) {
            uVar24 -= 1;
            *(unsigned char *)puVar18 =
                 *(unsigned char *)puVar18 >> (bVar16 & 0x1f) | *(unsigned char *)puVar18 << (bVar5 & 0x1f);
            puVar18 = (unsigned int *)((int)puVar18 + 1);
          }
          puVar21 = (unsigned int *)((int)puVar21 + *(int *)((int)search + 0x14));
          local_4c = (unsigned char *)((int)local_4c + -1);
        } while (local_4c != (unsigned char *)0x0);
      }
    }
    *(undefined4 *)((int)search + 0x1c) = local_30[2];
    *(undefined4 *)((int)search + 0x20) = local_30[3];
    *(undefined4 *)((int)search + 0x24) = local_20;
    *(undefined4 *)((int)search + 0x28) = local_1c;
    //if (*(short *)(in_EAX + 8) < 0x20) {
    if (hg2File->Version < 0x20) {
      *(undefined4 *)((int)search + 0x2c) = 0;
    }
    else {
      *(int *)((int)search + 0x2c) = (unsigned int)(local_18 != 0) + 1;
    }
    *(undefined4 *)((int)search + 0x30) = local_10;
    *(undefined4 *)((int)search + 0x34) = local_c;
    return 1;
  }
  cs2_HeapFree_004b0300(iVar9);
  FUN_004e82b0();
  return 0;
}



//undefined4 kcImageHg2_virtfunc_2(int *param_1, unsigned int param_2, undefined4 *param_3)
//   for (i = 0; i <= index; i++) {
//       *param_3 = img->ID;
//       if (img->OffsetToNext == 0) {
//         if (i != index)
//           return 0;
//         break;
//       }
//       img = (HG2METADATA *)((int)img + img->OffsetToNext);
//   }
//   return hg2_unkfunc_2_004e7710(*param_3);
//   i = 0;
//   do {
//     //*param_3 = *(undefined4 *)(iVar2 + 0x28);
//     //if (*(int *)(iVar2 + 0x40) == 0) {
//     *param_3 = img->ID;
//     if (img->OffsetToNext == 0) {
//       // this is the last image
//       //if (uVar3 != param_2) {
//       if (i != index) {
//         return 0;
//       }
//       break;
//     }
//     //uVar3 += 1;
//     i++;
//     //iVar2 += *(int *)(iVar2 + 0x40);
//     img = (HG2METADATA *)((int)img + img->OffsetToNext);
//   } while (i <= index);
//   //} while (uVar3 <= param_2);
//   result = hg2_unkfunc_2_004e7710(*param_3);
//   return result;
// }


undefined4 * FUN_004db480(uint param_1)

{
  SIZE_T dwBytes;
  int iVar1;
  int unaff_EBX;
  undefined4 *puVar2;
  
  iVar1 = *(int *)(unaff_EBX + 0xc);
  if (iVar1 != 0) {
    if (param_1 <= *(uint *)(unaff_EBX + 0x10)) {
      return (undefined4 *)iVar1;
    }
    if (*(int *)(iVar1 + -4) < 2) {
      HeapFree(DAT_007a0c44,0,(int *)(iVar1 + -4));
    }
    else {
      *(int *)(iVar1 + -4) = *(int *)(iVar1 + -4) + -1;
    }
    *(undefined4 *)(unaff_EBX + 0xc) = 0;
  }
  dwBytes = param_1 + 4;
  puVar2 = (undefined4 *)HeapAlloc(DAT_007a0c44,0,dwBytes);
  if (puVar2 == (undefined4 *)0x0) {
    kclib_logError(s__id_=_%d_size_=_%d(0x%x)_006275d8,0,dwBytes,dwBytes);
  }
  else {
    *puVar2 = 1;
    puVar2 = puVar2 + 1;
  }
  *(undefined4 **)(unaff_EBX + 0xc) = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    *(uint *)(unaff_EBX + 0x10) = param_1;
    return puVar2;
  }
  kclib_logError(s__00628a10);
  return (undefined4 *)0;
}



