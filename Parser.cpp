#include "CParser.h"
#define PARSERAPI extern "C" __declspec(dllexport)
#include "Parser.h"

PARSERAPI PARSER CreateParser(LPCWSTR fileName)
{
    CParser* pParser = new CParser;
    pParser->LoadFile(fileName);
    return ((PARSER)(pParser));
}


PARSERAPI bool GetValue(PARSER parser, LPCWSTR key, PVOID* pStart, PVOID* pNextOfEnd)
{
    return ((CParser*)(parser))->GetValue(key, pStart, pNextOfEnd);
}

PARSERAPI VOID ReleaseParser(PARSER parser)
{
    delete ((CParser*)(parser));
}


