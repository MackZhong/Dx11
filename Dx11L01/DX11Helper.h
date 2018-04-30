//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

inline void ThrowIfFailed(HRESULT hr, LPCWSTR szMsg = nullptr)
{
	if (FAILED(hr))
	{
		char* pMsg = nullptr;
		if (szMsg) {
			OutputDebugStringW(szMsg);
			size_t numChar, len;
			len = wcslen(szMsg);
			errno_t err = wcstombs_s(&numChar, nullptr, 0, szMsg, len);
			pMsg = new char[numChar + 1];
			err = wcstombs_s(&numChar, pMsg, numChar, szMsg, len);
		}
		throw std::exception(pMsg);
		if (pMsg)
			delete[] pMsg;
	}
}

inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
{
	if (path == nullptr)
	{
		throw std::exception();
	}

	//DWORD size = GetCurrentDirectoryW(pathSize, path);
	//path[size + 1] = L'\0';
	//path[size] = L'\\';
	//return;
	DWORD size = GetModuleFileNameW(nullptr, path, pathSize);
	if (size == 0 || size == pathSize)
	{
		// Method failed or path was truncated.
		throw std::exception();
	}

	WCHAR* lastSlash = wcsrchr(path, L'\\bin\\');
	if (lastSlash)
	{
		*(lastSlash - 3) = L'\0';
		return;
	}
	lastSlash = wcsrchr(path, L'\\');
	if (lastSlash)
	{
		*(lastSlash + 1) = L'\0';
	}
}

inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
{
	using namespace Microsoft::WRL;

	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
	if (file.Get() == INVALID_HANDLE_VALUE)
	{
		throw std::exception();
	}

	FILE_STANDARD_INFO fileInfo = {};
	if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
	{
		throw std::exception();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw std::exception();
	}

	*data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
	*size = fileInfo.EndOfFile.LowPart;

	if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
	{
		throw std::exception();
	}

	return S_OK;
}

// Assign a name to the object to aid with debugging.
//#if defined(_DEBUG)
//inline void SetName(ID3D11Object* pObject, LPCWSTR name)
//{
//	pObject->SetName(name);
//}
//inline void SetNameIndexed(ID3D11Object* pObject, LPCWSTR name, UINT index)
//{
//	WCHAR fullName[50];
//	if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)
//	{
//		pObject->SetName(fullName);
//	}
//}
//
//#else
//inline void SetName(ID3D11Object*, LPCWSTR)
//{
//}
//inline void SetNameIndexed(ID3D11Object*, LPCWSTR, UINT)
//{
//}
//#endif

// Naming helper for ComPtr<T>.
// Assigns the name of the variable as the name of the object.
// The indexed variant will include the index in the name of the object.
#define NAME_D3D11_OBJECT(x) SetName(x.Get(), L#x)
#define NAME_D3D11_OBJECT_INDEXED(x, n) SetNameIndexed(x[n].Get(), L#x, n)
