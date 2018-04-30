#include "pch.h"
#include "DXHelper.h"

namespace DX {
	Config* Config::instance = nullptr;

	Config::Config() {
		DWORD size = GetModuleFileNameW(nullptr, basePath, 512);
		if (size == 0 || size == 512)
		{
			throw std::exception("GetModuleFileNameW method failed or basePath was truncated");
		}
		PWSTR pFind = StrRStrIW(basePath, NULL, L"\\bin\\");
		if (NULL == pFind)
		{
			pFind = StrRChrW(basePath, NULL, L'\\');
			if (NULL == pFind)
			{
				throw std::exception("Directory struct invalid, executable should in \"bin\" subdirectory.");
			}
		}
		pFind[0] = L'\0';
	}

}


void OutputErrorMsgs(ID3DBlob* pMsgs, LPCWSTR cause) {
	char* pBuff = (char*)pMsgs->GetBufferPointer();
	SIZE_T bufSize = pMsgs->GetBufferSize();
	const size_t strLen = 256;
	static wchar_t swBuff[strLen * sizeof(wchar_t)];
	int count = swprintf_s(swBuff, strLen - 1, L"%hS\tFrom: %s.\n", pBuff, cause);
	OutputDebugString(swBuff);
}

bool CompileShaderFromFile(LPCWSTR szFile, LPCSTR pEntryPoint, LPCSTR pTarget, UINT shaderFlags, ID3DBlob** ppCode) {
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	ID3DBlob* error{ nullptr };
	HRESULT hr = D3DCompileFromFile(szFile, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pTarget, shaderFlags | compileFlags, 0, ppCode, &error);
	if (FAILED(hr)) {
		if (error) {
			OutputErrorMsgs(error, szFile);
			error->Release();
			error = nullptr;
		}
		else {
			std::wostringstream msgbuf;
			msgbuf << L"CompileFromFile:\"" << szFile << L"\" failed.";
			OutputDebugStringW(msgbuf.str().c_str());
		}
		return false;
	}

	return true;
}

bool CompileVertexShaderFromFile(ID3D11Device* pDevice, LPCWSTR szFile, LPCSTR pEntryPoint, UINT shaderFlags, ID3D11VertexShader** ppVertexShader,
	const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElements, ID3D11InputLayout** ppInputLayout) {
	// 编译顶点着色器到内存
	ID3DBlob* vertexShaderBuffer{ nullptr };
	if (!CompileShaderFromFile(szFile, pEntryPoint, "vs_5_0", shaderFlags, &vertexShaderBuffer)) {
		return false;
	}

	// 从内存创建顶点着色器
	HRESULT hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, ppVertexShader);
	if (FAILED(hr)) {
		SafeRelease(vertexShaderBuffer);
		OutputDebugStringW(L"CreateVertexShader failed.\n");
		return false;
	}

	//Create the vertex input layout.
	hr = pDevice->CreateInputLayout(pInputElementDesc, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), ppInputLayout);
	if (FAILED(hr)) {
		SafeRelease(vertexShaderBuffer);
		OutputDebugStringW(L"CreateInputLayout failed.\n");
		return false;
	}
	SafeRelease(vertexShaderBuffer);

	return true;
}

bool CompilePixelShaderFromFile(ID3D11Device* pDevice, LPCWSTR szFile, LPCSTR pEntryPoint, UINT shaderFlags, ID3D11PixelShader** ppPixelShader) {
	// 编译像素着色器到内存
	ID3DBlob* pixelShaderBuffer{ nullptr };
	if (!CompileShaderFromFile(szFile, pEntryPoint, "ps_5_0", shaderFlags, &pixelShaderBuffer)) {
		SafeRelease(pixelShaderBuffer);
		return false;
	}

	// 从内存创建像素着色器
	HRESULT hr = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, ppPixelShader);
	if (FAILED(hr)) {
		SafeRelease(pixelShaderBuffer);
		OutputDebugStringW(L"CreatePixelShader failed.\n");
		return false;
	}
	SafeRelease(pixelShaderBuffer);

	return true;
}


void ThrowIfFailed(HRESULT hr, LPCWSTR szMsg)
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
