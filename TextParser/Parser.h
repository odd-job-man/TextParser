#pragma once
#ifdef PARSERAPI
#else
#define PARSERAPI extern "C" __declspec(dllimport)
#endif 

#include <windows.h>
using PARSER = PVOID;

PARSERAPI PARSER CreateParser(LPCWSTR fileName);
PARSERAPI bool GetValue(PARSER parser, LPCWSTR key, PVOID* pStart, PVOID* pNextOfEnd);
PARSERAPI VOID ReleaseParser(PARSER parser);

// �����ϸ� ���ۿ� �����ϰ� 0��ȯ �����ϸ� �������� �ʰ� ���̹�ȯ 
PARSERAPI int GetValueWSTR(PARSER parser, WCHAR* pOutBuf, int countOfBuf, LPCWSTR key);
PARSERAPI int GetValueINT(PARSER parser, LPCWSTR key);
PARSERAPI unsigned int GetValueUINT(PARSER parser, LPCWSTR key);
PARSERAPI long long GetValueLONGLONG(PARSER parser, LPCWSTR key);
PARSERAPI unsigned long long GetValueULONGLONG(PARSER parser, LPCWSTR key);
