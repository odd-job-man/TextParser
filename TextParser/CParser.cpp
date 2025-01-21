#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <wchar.h>
#include "CParser.h"


void CParser::LoadFile(const wchar_t* p_file_name)
{
	FILE* pFile;
	_wfopen_s(&pFile, p_file_name, L"rb, ccs = UTF-16LE");
	if (!pFile)	__debugbreak();

	fseek(pFile, 0, SEEK_END);
	long file_size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	pFileBuffer_ = (wchar_t*)malloc(file_size + sizeof(wchar_t));
	if (!pFileBuffer_)	__debugbreak();
	bufferLength_ = (file_size + sizeof(wchar_t)) / sizeof(wchar_t);
	if (fread(pFileBuffer_, file_size, 1, pFile) != 1)	__debugbreak();
	pFileBuffer_[file_size / sizeof(wchar_t)] = '\0'; // ���� �ι��ڷ� ����
	fclose(pFile);
}

bool CParser::GetValue(const wchar_t* key, void** pStart, void** pNextOfEnd)
{
	wchar_t* pValueBuffer;
	if (!GetValueHelper(key, &pValueBuffer))
	{
		return false;
	}
	*pStart = pValueBuffer;


	// ���ڿ��� �ƴ� ��쿡�� NULL�� ����ڰ� ��������
	if (pNextOfEnd)
	{
		while (*pValueBuffer != L';')
		{
			++pValueBuffer;
		}
		*pNextOfEnd = pValueBuffer;
	}
	return true;
}

bool CParser::GetValueHelper(const wchar_t* key, wchar_t** ppValueBuffer)
{
	wchar_t* pBuffer = pFileBuffer_;
	++pBuffer; // BOM �ǳʶ�
	while (FindKey(key, &pBuffer))
	{
		do
		{
			if (!FindKey(L":", &pBuffer))
				goto jump;
			++pBuffer;
			if (*pBuffer == L' ')
				++pBuffer;

			*ppValueBuffer = pBuffer;
			return true;
		} while (0);
	}
jump:
	return false;
}



// �ּ� ������ �� \n Ȥ�� /�� *chppBuffer�� ����Ű�� ���¿��� ��ȯ
bool CParser::SkipComment(wchar_t** chppBuffer)
{
	++(*chppBuffer);
	if (**chppBuffer == L'/') // //�ּ�
	{
		while (**chppBuffer != L'\n')
			++(*chppBuffer);
	}
	else if (**chppBuffer == L'*')// /*�ּ�
	{
		++(*chppBuffer);
		while (**chppBuffer != L'*')
			++(*chppBuffer);

		++(*chppBuffer);
		if (**chppBuffer != L'/') // */�� �����°� Ȯ��
			__debugbreak();
	}
	else
		return false;

	return true;
}

// ���� �ܾ��� ù��° ���ڸ� *ppBuffer�� ����Ű����
bool CParser::FindKey(const wchar_t* key, wchar_t** ppBuffer)
{
	wchar_t* pBuffer = *ppBuffer;
	wchar_t* endOfFile = pFileBuffer_ + bufferLength_ - 1;
	int keyLength = (int)wcslen(key);
	int i;
	while (pBuffer < endOfFile)
	{
		// Ű�� ù���ڰ� ��ġ�ϰ� ù���� �ٷ� �����ڰ� �����̰ų� ; Ȥ�� �����̸� Ű �������� ����
		if (*pBuffer == key[0] && (pBuffer[-1] == L'\n' || pBuffer[-1] == L';' || pBuffer[-1] == L' ' || pBuffer[-1] == L'/' || pBuffer[-1] == L'\t'))
		{
			for (i = 1; i < keyLength; ++i)
			{
				if (key[i] != pBuffer[i])
					break;
			}

			// Ű�� ���̸�ŭ ��ġ�ϰ� Ű �������� �������ڰ� �����̸� Ű ������ ����
			if (i == keyLength && pBuffer[i] == L' ')
			{
				*ppBuffer = pBuffer;
				return true;
			}
			// ��ġ���� ������ ó������ Ʋ���κ����� �����͸� �̵���Ŵ
			else
			{
				pBuffer += i;
			}
		}
		// �ּ� �ǳʶ�
		if (*pBuffer == L'/')
			SkipComment(&pBuffer); // \n ���� �����ų� /�� ������ �����͸� pBuffer�� ����Ŵ
		++pBuffer;
	}
	return false;
}

CParser::~CParser()
{
	delete[] pFileBuffer_;
	pFileBuffer_ = nullptr;
}