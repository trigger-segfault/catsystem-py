
// functional-specific function definition for token_parse
//  (OOP equivalent: TokenParser:NextToken)

#include "common.h"
#ifndef KCLIB_OOP

#include "TokenParser_functional.h"
#include "kclib_common.h"
#include "ghidra_types_min.h"


//TokUnk0 values:
//3 = WHITESPACE ' ', '\t'
//5 = INTEGER \d+|0[xX][A-Fa-f0-9]+, FLOAT \d+\.\d*[fF]?|\d+[fF],
//    CHAR \'([^\']|\\\'|\\\\)*\', STRING "\([^\"]|\\\")*\"
//6 = SYMBOL '!', '!=', '|', ...
//7 = KEYWORD (TABLE_KEYWORDS)

void __invalid_parameter_none()
{
    // dummy func, MSVC
}

// dummy data pointer
#define _DAT_00000010 ((void*)0)

// undefined4 __fastcall FUN_00411750(int **param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411750
undefined4 __fastcall FUN_00411750(int **param_1)

{
    int *piVar1;
    int iVar2;
    int iVar3;
    int *piVar4;
    
    piVar1 = *param_1;
    piVar4 = param_1[1];
    if (piVar1 == (int *)0x0) {
        __invalid_parameter_none();
        piVar1 = *param_1;
        if (piVar1 == (int *)0x0) {
            iVar3 = 0;
            goto LAB_0041176e;
        }
    }
    iVar3 = *piVar1;
    LAB_0041176e:
    if (piVar1 == (int *)0x0) {
        iVar2 = 0;
    }
    else {
        iVar2 = *piVar1;
    }
    if ((int *)(*(int *)(iVar2 + 0x1c) + *(int *)(iVar3 + 0x18)) <= param_1[1]) {
        __invalid_parameter_none();
    }
    piVar1 = *param_1;
    if (piVar1 == (int *)0x0) {
        iVar3 = 0;
    }
    else {
        iVar3 = *piVar1;
    }
    if (*(int **)(iVar3 + 0x14) < piVar4 || *(int **)(iVar3 + 0x14) == piVar4) {
        if (piVar1 == (int *)0x0) {
            iVar3 = 0;
        }
        else {
            iVar3 = *piVar1;
        }
        piVar4 = (int *)((int)piVar4 - *(int *)(iVar3 + 0x14));
    }
    if (piVar1 != (int *)0x0) {
        return *(undefined4 *)(*(int *)(*piVar1 + 0x10) + (int)piVar4 * 4);
    }
    return *(undefined4 *)(_DAT_00000010 + (int)piVar4 * 4);
}


// this => SCRIPT_DECODER->ScrUnk71 (linked list?)
// undefined4 __thiscall FUN_00411890(void *this, char *str, undefined8 *outBuffer)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411890
BOOL __thiscall token_LookupIdentifier(void *this, IN const char *str, OUT unsigned char *outBuffer)

{
    //this fields
    //this -> 0x0 -> 0x1c (int) (val - 1, likely length of some sort, maybe kcFile?)
    //0x1c (-)
    // param_2 (local_16c)
    // undefined8 local_16c [40]; // sizeof => 320, 0x140, assumed size: 348, 0x15c
    // undefined4 local_28;
    // undefined4 local_24;
    // undefined4 local_20;
    // undefined4 local_1c;
    // undefined4 local_18;
    // undefined4 local_14;
    // undefined4 local_c;

	int **ppiVar1;
	int iVar2;
	undefined8 *puVar3;
	int *piVar4;
	int iVar5;
	int *piVar6;
	uint uVar7;
	int **local_18;
	int *local_14;
	int **local_10;
	int *local_c;
	int *local_4;
    // FILE
	
	iVar5 = *(int *)(*(int *)this + 0x1c) + -1;
	local_18 = (int **)this;
	if (iVar5 < 0) {
		return FALSE;
	}
	while (true) // do { } while (true);
    {
		ppiVar1 = (int **)*local_18;
		piVar6 = ppiVar1[6];
		if ((int *)((int)ppiVar1[7] + (int)piVar6) < piVar6) {
            //_invalid_parameter((wchar_t *)0x0,(wchar_t *)0x0,(wchar_t *)0x0,0,0);
			__invalid_parameter_none();
		}
		ppiVar1 = (int **)*ppiVar1;
		local_4 = piVar6;
		if (ppiVar1 == (int **)0x0) {
			__invalid_parameter_none();
			piVar4 = (int *)0x0;
		}
		else {
			piVar4 = *ppiVar1;
		}
		uVar7 = (int)piVar6 + iVar5;
		if ((uint)(piVar4[7] + piVar4[6]) < uVar7) {
LAB_004118fe:
			__invalid_parameter_none();
		}
		else {
			if (ppiVar1 == (int **)0x0) {
				piVar6 = (int *)0x0;
			}
			else {
				piVar6 = *ppiVar1;
			}
			if (uVar7 < (uint)piVar6[6]) goto LAB_004118fe;
		}
		local_10 = ppiVar1;
		local_c = (int *)uVar7;
		iVar2 = FUN_00411750((int **)&local_10);
		iVar2 = strcmp((char *)(iVar2 + 4), str);
		if (iVar2 == 0) {
			if (outBuffer != NULL) {
				ppiVar1 = (int **)*local_18;
				local_14 = ppiVar1[6];
				if ((int *)((int)ppiVar1[7] + (int)local_14) < local_14) {
					__invalid_parameter_none();
				}
				local_18 = (int **)*ppiVar1;

                ///TEMP: define dummy func for no errors
                #define FUN_004117c0(a,b) do {} while (false)

				FUN_004117c0(&local_18, iVar5);
                
                ///TEMP: undefine dummy func for no errors
                #undef FUN_004117c0

				local_10 = local_18;
				uVar7 = 0x168; // Exact size of stack from local_16c -> local_4 (exclusive)
				local_c = local_14;
				puVar3 = (undefined8 *)FUN_00411750((int **)&local_10);
				memcpy(outBuffer, puVar3, uVar7);
			}
			return TRUE;
		}
		iVar5--; //iVar5 += -1;
		if (iVar5 < 0) {
			return FALSE;
		}
	}
}


///FID:cs2_full_v401/tool/ac.exe: FUN_004112e0
//BOOL __thiscall foo_token_Next(SCRIPT_DECODER *this, IN const char *str, OUT TOKEN_RESULT *token, OUT undefined8 *param_1)
BOOL __thiscall token_Next(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token)

{
    ///TODO: needs return values identified

    // Stack written to from token_LookupIdentifier
    unsigned char local_16c[320]; //undefined8 local_16c [40];
    TOKEN_GROUP  local_28; //assigned to token->TokenGroup
    TOKEN_TYPE   local_24; //assigned to token->TokenType
    int          local_20; //assigned to token->TokUnk4
    unsigned int local_1c; //assigned to token->TokUnk3
    unsigned int local_18; //assigned to token->TokUnk5
    unsigned int local_14; //assigned to token->TokUnk6
    TOKEN_TYPE   local_c;  //assigned to token->LiteralType


	//undefined8 *outBuffer; //to be removed
	//uint uVar1; // has tokenLength value after parse
    
    int tokenLength; //uint local_174;
    TOKEN_VALUE tokenValue; //undefined4 tokenValue; //float local_170;
    memset(token, 0, sizeof(TOKEN_RESULT));

    const char *initialStart = &this->Buffer[this->Position];
    if (this->Position >= this->Length || initialStart[0] == '\x1a')
        return FALSE;
    
    int linesRead = 0;
    const char *str = token_SkipComments(initialStart, &linesRead);
    if (str == NULL)
        return FALSE;
        
    this->LineNumber += linesRead;
    this->Position = (unsigned int)(str - this->Buffer); // how its done in assembly
                                                              // SkipComments returns no other info
    // *(int *)((int)this + 0x10) = *(int *)((int)this + 0x10) + linesRead;
    // *(undefined8 **)((int)this + 0xc) = (undefined8 *)((int)str - *(int *)this);

    if (str[0] == ' ' || str[0] == '\t') {
        token->TokenGroup   = assert_enum(3, GROUP_WHITESPACE);
        token->TokenType    = assert_enum(6, TOKEN_WHITESPACE);
        token->TokenValue.Int32 = 0;

        //memcpy(&token->TokenText[0], str, 1); //FUN_004126c0
        token->TokenText[0] = str[0];
        token->TokenText[1] = '\0';
        
        this->Position++;


        // *(undefined4 *)param_1 = 3;
        // *(undefined4 *)((int)param_1 + 4) = 6;
        // *(undefined4 *)(param_1 + 1) = 0;
        // FUN_004126c0(param_1 + 4,str,1);
        // *(undefined *)((int)param_1 + 0x21) = 0;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + 1;
    }
	else if (token_ParseUnsignedInteger(str, &tokenLength, &tokenValue.Int32))
    {
        token->TokenGroup   = assert_enum(5, GROUP_LITERAL);
        token->TokenType    = assert_enum(0xd, TOKEN_INTEGER_LITERAL);
        token->TokenValue.Int32 = tokenValue.Int32;
        token->TokUnk5      = 4;
        token->TokUnk6      = 4;
        token->LiteralType  = assert_enum(0x29, TOKEN_VOID_KEYWORD); // strange, not DWORD/LONG
        
        memcpy(&token->TokenText[0], str, tokenLength); //FUN_004126c0
        token->TokenText[tokenLength] = '\0';
        
        this->Position += tokenLength;
        this->LastTokenType = token->TokenType;


        // *(undefined4 *)param_1 = 5;
        // *(undefined4 *)((int)param_1 + 4) = 0xd;
        // *(float *)(param_1 + 1) = local_170;
        // FUN_004126c0(param_1 + 4,str,local_174);
        // *(undefined *)((int)param_1 + uVar1 + 0x20) = 0;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + uVar1;
        // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
        // *(undefined4 *)((int)param_1 + 0x1c) = 0x29;
        // *(undefined4 *)((int)param_1 + 0x14) = 4;
        // *(undefined4 *)(param_1 + 3) = 4;
    }
    else if (token_ParseFloat(str, &tokenLength, &tokenValue.Single))
    {
        token->TokenGroup   = assert_enum(5, GROUP_LITERAL);
        token->TokenType    = assert_enum(0xe, TOKEN_FLOAT_LITERAL);
        token->TokenValue.Single = tokenValue.Single;
        token->TokUnk5      = 4;
        token->TokUnk6      = 4;
        token->LiteralType  = assert_enum(0x30, TOKEN_FLOAT_KEYWORD);
        
        memcpy(&token->TokenText[0], str, tokenLength); //FUN_004126c0
        token->TokenText[tokenLength] = '\0';
        
        this->Position += tokenLength;
        this->LastTokenType = token->TokenType;


        // *(float *)(param_1 + 1) = local_170;
        // *(undefined4 *)param_1 = 5;
        // *(undefined4 *)((int)param_1 + 4) = 0xe;
        // FUN_004126c0(param_1 + 4,str,local_174);
        // *(undefined *)((int)param_1 + uVar1 + 0x20) = 0;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + uVar1;
        // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
        // *(undefined4 *)((int)param_1 + 0x1c) = 0x30;
        // *(undefined4 *)((int)param_1 + 0x14) = 4;
        // *(undefined4 *)(param_1 + 3) = 4;
    }
    else if (token_ParseChar(str, &tokenLength, &tokenValue.UInt16))
    {
        token->TokenGroup   = assert_enum(5, GROUP_LITERAL);
        token->TokenType    = assert_enum(0xd, TOKEN_INTEGER_LITERAL);
        token->TokenValue.Int32 = (int)tokenValue.UInt16 & 0xffff;
        token->LiteralType  = assert_enum(0x2a, TOKEN_CHAR_KEYWORD);
        
        memcpy(&token->TokenText[0], str, tokenLength); //FUN_004126c0
        token->TokenText[tokenLength] = '\0';
        
        this->Position += tokenLength;
        this->LastTokenType = token->TokenType;
        

        // *(undefined4 *)param_1 = 5;
        // *(undefined4 *)((int)param_1 + 4) = 0xd;
        // *(uint *)(param_1 + 1) = (uint)local_170 & 0xffff;
        // FUN_004126c0(param_1 + 4,str,local_174);
        // *(undefined *)((int)param_1 + uVar1 + 0x20) = 0;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + uVar1;
        // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
        // *(undefined4 *)((int)param_1 + 0x1c) = 0x2a;
    }
    else if (token_ParseString(str, &tokenLength, &token->TokenText[0], sizeof(TOKEN_RESULT::TokenText)))
    {
        token->TokenGroup   = assert_enum(5, GROUP_LITERAL);
        token->TokenType    = assert_enum(0xf, TOKEN_STRING_LITERAL);
        token->TokUnk4      = 1; // BOOL?
        token->LiteralType  = assert_enum(0xf, TOKEN_STRING_LITERAL);

        // already copied directly into token->TokenText
        
        this->Position += tokenLength;
        this->LastTokenType = token->TokenType;


        // *(undefined4 *)((int)param_1 + 4) = 0xf;
        // *(undefined4 *)param_1 = 5;
        // *(undefined4 *)(param_1 + 2) = 1;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + local_174;
        // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
        // *(undefined4 *)((int)param_1 + 0x1c) = 0xf;
    }
    else if (token_ParseSymbol(this, str, &tokenLength, &tokenValue.Symbol))
    {
        token->TokenGroup   = assert_enum(6, GROUP_SYMBOL);
        token->TokenValue.Symbol = tokenValue.Symbol;
        
        memcpy(&token->TokenText[0], str, tokenLength); //FUN_004126c0
        token->TokenText[tokenLength] = '\0';
        
        this->Position += tokenLength;
        this->LastTokenType = token->TokenType;
        

        // *(undefined4 *)param_1 = 6;
        // *(float *)((int)param_1 + 4) = local_170;
        // FUN_004126c0(outBuffer,str,local_174);
        // *(undefined *)((int)param_1 + local_174 + 0x20) = 0;
        // *(int *)((int)this + 0xc) = *(int *)((int)this + 0xc) + local_174;
        // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
    }
    else if (token_ParseIdentifier(str, &tokenLength, &token->TokenText[0]))
    {
        // copied directly into token->TokenText
        
        this->Position += tokenLength;

        if (this->EnableKeywords == 1)
        {
            // BOOL __stdcall token_ParseKeyword(IN const char *str, OUT int *outValue);
            ///NOTE: tokenLength used here in assembly, but we know its use is as tokenValue/Type
            if (token_ParseKeyword(&token->TokenText[0], &tokenValue.Symbol))
            {
                token->TokenGroup   = assert_enum(7, GROUP_KEYWORD);
                token->TokenType    = tokenValue.Symbol;

                this->LastTokenType = token->TokenType;
                

                // *(undefined4 *)param_1 = 7;
                // *(uint *)((int)param_1 + 4) = local_174;
                // *(uint *)((int)this + 0x118) = local_174;
                return;
            }
            else if (strcmp(&token->TokenText[0], "sizeof") == 0)
            {
                token->TokenType    = assert_enum(0xb4, TOKEN_SIZEOF_OPERATOR);
                token->TokenGroup   = assert_enum(6, GROUP_SYMBOL);

                this->LastTokenType = token->TokenType;


                // *(undefined4 *)((int)param_1 + 4) = 0xb4;
                // *(undefined4 *)param_1 = 6;
                // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
                return;
            }
            else
            {
                ///TODO: what are these?
                token->TokenGroup   = assert_enum(4, GROUP_IDENTIFIER);
                token->TokenType    = assert_enum(7, TOKEN_IDENTIFIER);

                // *(undefined4 *)param_1 = 4;
                // *(undefined4 *)((int)param_1 + 4) = 7;

                //iVar3 = token_LookupIdentifier(*(void **)((int)this + 0x11c),(char *)outBuffer, local_16c);
                BOOL unkres = token_LookupIdentifier((void *)this->Identifiers, &token->TokenText[0], local_16c);
                if (unkres) {
                    // assign everything but token->TokenValue
                    // //ORIGINAL ORDER:
                    // token->TokUnk3      = local_1c; // (int)param_1 + 0xc
                    // token->TokUnk4      = local_20; // param_1 + 2
                    // token->TokenType    = local_24; // (int)param_1 + 4
                    // token->TokenGroup   = local_28; // param_1
                    // token->TokUnk5      = local_18; // (int)param_1 + 0x14
                    // token->TokUnk6      = local_14; // param_1 + 3
                    // token->LiteralType  = local_c; // (int)param_1 + 0x1c

                    //TOKEN_RESULT ORDER:
                    token->TokenGroup   = local_28; // param_1
                    token->TokenType    = local_24; // (int)param_1 + 4

                    token->TokUnk3      = local_1c; // (int)param_1 + 0xc
                    token->TokUnk4      = local_20; // param_1 + 2
                    token->TokUnk5      = local_18; // (int)param_1 + 0x14
                    token->TokUnk6      = local_14; // param_1 + 3
                    token->LiteralType  = local_c; // (int)param_1 + 0x1c

                    // //LOCAL STACK ORDER:
                    // token->TokenGroup   = local_28; // param_1
                    // token->TokenType    = local_24; // (int)param_1 + 4
                    // token->TokUnk4      = local_20; // param_1 + 2
                    // token->TokUnk3      = local_1c; // (int)param_1 + 0xc
                    // token->TokUnk5      = local_18; // (int)param_1 + 0x14
                    // token->TokUnk6      = local_14; // param_1 + 3

                    // token->LiteralType  = local_c; // (int)param_1 + 0x1c
                    

                    // *(undefined4 *)((int)param_1 + 0xc) = local_1c;
                    // *(undefined4 *)(param_1 + 2) = local_20;
                    // *(undefined4 *)((int)param_1 + 4) = local_24;
                    // *(undefined4 *)param_1 = local_28;
                    // *(undefined4 *)((int)param_1 + 0x14) = local_18;
                    // *(undefined4 *)(param_1 + 3) = local_14;
                    // *(undefined4 *)((int)param_1 + 0x1c) = local_c;
                }
            }
        }
        else
        {
            if (strcmp(&token->TokenText[0], "@") == 0) {
                token ->TokenType   = assert_enum(0x27, TOKEN_ATSIGN_KEYWORD);
                token->TokenGroup   = assert_enum(7, GROUP_KEYWORD);

                this->LastTokenType = token->TokenType;


                // *(undefined4 *)((int)param_1 + 4) = 0x27;
                // *(undefined4 *)param_1 = 7;
                // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
            }
            else
            {
                ///TODO: what are these?
                token->TokenGroup   = assert_enum(4, GROUP_IDENTIFIER);
                token->TokenType    = assert_enum(7, TOKEN_IDENTIFIER);
                BOOL unkres = token_LookupIdentifier(this->Identifiers, &token->TokenText[0], local_16c);
                if (unkres) {
                    // assign everything but token->TokenValue
                    //ORIGINAL ORDER:
                    token->TokUnk3      = local_1c;
                    token->TokenType    = local_24;
                    token->TokUnk4      = local_20;
                    token->TokenGroup   = local_28;
                    token->TokUnk6      = local_14;
                    token->TokUnk5      = local_18;
                    token->LiteralType  = local_c;

                    this->LastTokenType = token->TokenType;


                    // *(undefined4 *)((int)param_1 + 0xc) = local_1c;
                    // *(undefined4 *)((int)param_1 + 4) = local_24;
                    // *(undefined4 *)(param_1 + 2) = local_20;
                    // *(undefined4 *)param_1 = local_28;
                    // *(undefined4 *)(param_1 + 3) = local_14;
                    // *(undefined4 *)((int)param_1 + 0x14) = local_18;
                    // *(undefined4 *)((int)param_1 + 0x1c) = local_c;
                    // *(undefined4 *)((int)this + 0x118) = *(undefined4 *)((int)param_1 + 4);
                }
            }
            this->LastTokenType = token->TokenType;
        }
    }
    else
    {
        token->TokenGroup   = assert_enum(0, GROUP_NONE);
        token->TokenType    = assert_enum(0, TOKEN_NONE);
        if (shiftjis_IsCharDoubleByte(str))
        {
            token->TokenText[0] = str[0];
            token->TokenText[1] = str[1];
            token->TokenText[2] = '\0';

            this->Position += 2;
            this->LastTokenType = token->TokenType;
        }
        else
        {
            token->TokenText[0] = str[0];
            token->TokenText[1] = '\0';

            this->Position += 1;
            this->LastTokenType = token->TokenType;
        }
    }

    return TRUE;
}

#endif
