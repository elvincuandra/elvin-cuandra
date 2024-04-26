#include "dx11helper.h"

#include <iostream>


namespace DirectXHelper {
	HRESULT CreateConstantBuffer(
		ID3D11Device* device,
		unsigned int byteSize,
		ID3D11Buffer** constantBuffer) {

		HRESULT hr = E_FAIL;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = byteSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		hr = device->CreateBuffer(&bd, nullptr, constantBuffer);

		if (FAILED(hr)) {
			std::cout << "Failed to create constant buffer" << std::endl;
			return hr;
		}

		return hr;
	};

	HRESULT CreateConstantBufferWrite(ID3D11Device* device, unsigned int byteSize, ID3D11Buffer** constantBuffer)
	{
		HRESULT hr = E_FAIL;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = byteSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = device->CreateBuffer(&bd, nullptr, constantBuffer);

		if (FAILED(hr)) {
			std::cout << "Failed to create constant buffer" << std::endl;
			return hr;
		}

		return hr;
	}

	HRESULT CreateVertexBufferWrite(ID3D11Device* device, unsigned int stride, unsigned int vertexNum, void* vertexData, ID3D11Buffer** vertexBuffer)
	{

		HRESULT hr = E_FAIL;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = stride * vertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexData;

		hr = device->CreateBuffer(&bd, &sd, vertexBuffer);

		if (FAILED(hr)) {
			MessageBox(nullptr, L"CreateBufer(vertex buffer) error", L"Error", MB_OK);
			return hr;
		}

		return hr;
	};

	HRESULT CreateIndexBuffer(ID3D11Device* device, unsigned int indexNum, void* indexData, ID3D11Buffer** indexBuffer)
	{
		HRESULT hr = E_FAIL;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * indexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = indexData;

		hr = device->CreateBuffer(&bd, &sd, indexBuffer);

		if (FAILED(hr)) {
			MessageBox(nullptr, L"CreateBufer(index buffer) error", L"Error", MB_OK);
			return hr;
		}

		return hr;
	}
	HRESULT CreateVertexShader(
		ID3D11Device* device,
		const char* fileName,
		LPCSTR entryPoint,
		LPCSTR shaderModel,
		D3D11_INPUT_ELEMENT_DESC* layout,
		unsigned int numElements,
		ID3D11VertexShader** ppVertexShader,
		ID3D11InputLayout** ppVertexLayout)
	{
		HRESULT hr = E_FAIL;

		ID3DBlob* pBlob = nullptr;

		void* shaderObj;
		size_t shaderObjSize;

		hr = CompileShader(fileName, entryPoint, shaderModel, &shaderObj, shaderObjSize, &pBlob);
		if (FAILED(hr)) {
			if (pBlob) {
				pBlob->Release();
				return hr;
			}
		}

		hr = device->CreateVertexShader(shaderObj, shaderObjSize, nullptr, ppVertexShader);
		if (FAILED(hr)) {
			if (pBlob) {
				pBlob->Release();
				return hr;
			}
		}

		hr = device->CreateInputLayout(layout, numElements, shaderObj, shaderObjSize, ppVertexLayout);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"CreateInputLayout error", L"Error", MB_OK);
			pBlob->Release();
			return hr;
		}

		return hr;
	}

	HRESULT CreatePixelShader(ID3D11Device* device, const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3D11PixelShader** pixelShader)
	{
		HRESULT hr = E_FAIL;

		ID3DBlob* pBlob = nullptr;

		void* shaderObj;
		size_t shaderObjSize;

		hr = CompileShader(fileName, entryPoint, shaderModel, &shaderObj, shaderObjSize, &pBlob);
		if (FAILED(hr)) {
			if (pBlob) {
				pBlob->Release();
				return hr;
			}
		}

		hr = device->CreatePixelShader(shaderObj, shaderObjSize, nullptr, pixelShader);
		if (FAILED(hr)) {
			if (pBlob) {
				pBlob->Release();
				return hr;
			}
		}

		return S_OK;
	}

	HRESULT CompileShader(const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, void** shaderObj, size_t& ShaderObjSize, ID3DBlob** ppBlob)
	{
		HRESULT hr = E_FAIL;
		static std::vector<unsigned char> byteArray;
		*ppBlob = nullptr;

		std::string extName = DirectXHelper::GetFileExtName(fileName);
		if (extName == "cso") {
			hr = ReadShader(fileName, byteArray);
			if (FAILED(hr)) {
				FILE* fp;
				fopen_s(&fp, "debug.txt", "a");
				fprintf_s(fp, "Error: Could not open file\n");
				fclose(fp);
				return hr;
			}
			*shaderObj = byteArray.data();
			ShaderObjSize = byteArray.size();
		}
		else {
			hr = CompileShaderFromFile(fileName, entryPoint, shaderModel, ppBlob);
			if (FAILED(hr)) {
				if (*ppBlob) {
					(*ppBlob)->Release();
					return hr;
				}
			}

			*shaderObj = (*ppBlob)->GetBufferPointer();
			ShaderObjSize = (*ppBlob)->GetBufferSize();
		}

		return S_OK;
	}
	HRESULT CompileShaderFromFile(const char* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlob)
	{
		HRESULT hr = E_FAIL;

		WCHAR filenameW[512];
		size_t wLen = 0;
		int err = 0;

		err = mbstowcs_s(&wLen, filenameW, fileName, _TRUNCATE);

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		
		hr = D3DCompileFromFile(
			filenameW,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			shaderModel,
			dwShaderFlags,
			0,
			ppBlob,
			&pErrorBlob
		);

		if (FAILED(hr)) {
			if (pErrorBlob) {
				MessageBox(NULL, (wchar_t*)pErrorBlob->GetBufferPointer(), L"Error", MB_OK);
				pErrorBlob->Release();
				return hr;
			}
		}
		return S_OK;
	}

	HRESULT ReadShader(const char* csoName, std::vector<unsigned char>& byteArray)
	{
		FILE* fp = nullptr;
		int ret = fopen_s(&fp, csoName, "rb");
		if (ret != 0) {
			return E_FAIL;
		}
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		byteArray.resize(size);
		fseek(fp, 0, SEEK_SET);

		fread(byteArray.data(), size, 1, fp);
		fclose(fp);

		return S_OK;
	}

	std::string GetFileExtName(const char* filename)
	{
		std::string extName;

		std::string fullpathstr(filename);
		size_t ext_i = fullpathstr.find_last_of(".");
		extName = fullpathstr.substr(ext_i + 1, fullpathstr.size() - ext_i);

		return extName;
	}
}