#include "utility.h"

namespace fs = std::filesystem;

namespace utility {

	std::string GetFileName(std::string filename) {

		fs::path p = fs::u8path(filename.c_str());

		return p.filename().string();
	};

	std::string GetFileExt(std::string filepath) {

		fs::path p = fs::path(filepath.c_str());

		return p.extension().string();
	}

	std::wstring GetWideFileName(std::string filepath) {

		fs::path p = fs::path(filepath.c_str());

		const std::wstring wsfilename = p.filename().wstring();

		return wsfilename;
	}

	std::wstring GetParentDirectoryWide(std::string filepath) {

		fs::path p = fs::u8path(filepath.c_str());

		std::wstring wsparentdirectory;

		if (p.has_parent_path()) {
			wsparentdirectory = p.parent_path().wstring() + L"\\";
		}
		else {
			wsparentdirectory = L"\\";
		}

		return wsparentdirectory;
	}

	DirectX::XMMATRIX ConvertaiMtxTodxMtx(const aiMatrix4x4& aiMtx) {

		DirectX::XMMATRIX matrix = DirectX::XMMATRIX{
			aiMtx.a1, aiMtx.b1, aiMtx.c1, aiMtx.d1,
			aiMtx.a2, aiMtx.b2, aiMtx.c2, aiMtx.d2,
			aiMtx.a3, aiMtx.b3, aiMtx.c3, aiMtx.d3,
			aiMtx.a4, aiMtx.b4, aiMtx.c4, aiMtx.d4
		};

		return matrix;
	};
}
