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

template<typename T> inline void SafeRelease(T& ptr) {
	if (ptr)
		ptr->Release();
	ptr = nullptr;
}

#define ReturnIfFailed(hr, msg, result) \
	if ((HRESULT)(hr)<0) {\
		OutputDebugString(msg);\
		return result;}

void OutputErrorMsgs(ID3DBlob* pMsgs, LPCWSTR cause);

bool CompileShaderFromFile(LPCWSTR szFile, LPCSTR pEntryPoint, UINT shaderFlags, ID3DBlob** ppCode);

bool CompileVertexShaderFromFile(ID3D11Device* pDevice, LPCWSTR szFile, LPCSTR pEntryPoint, UINT shaderFlags, ID3D11VertexShader** ppVertexShader,
	const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElements, ID3D11InputLayout** ppInputLayout);

bool CompilePixelShaderFromFile(ID3D11Device* pDevice, LPCWSTR szFile, LPCSTR pEntryPoint, UINT shaderFlags, ID3D11PixelShader** ppPixelShader);

void ThrowIfFailed(HRESULT hr, LPCWSTR szMsg = nullptr);

namespace DX {
	class Config {
	private:
		static Config* instance;
	public:
		WCHAR basePath[512 * sizeof(WCHAR)];
		Config();
		static Config* Instance() {
			if (!instance)
				instance = new Config;
			return instance;
		}
		LPCWSTR BasePath() { return basePath; }
	};

	inline std::wstring GetFullPathAssets(LPCWSTR szFile) {
		std::wstring path(Config::Instance()->BasePath());
		return path + L"\\Assets\\" + szFile;
	}

	inline std::wstring GetFullPathShaders(LPCWSTR szShaderFile) {
		std::wstring path(Config::Instance()->BasePath());
		return path + L"\\Shaders\\" + szShaderFile;
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
