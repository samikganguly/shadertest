#include <ST_WinError.h>

namespace shadertest {

	WError::WError(const std::wstring& msg) : m_msg(msg) {}

	const char *WError::what() const {
		return "WError: call wwhat() instead";
	}

	const wchar_t *WError::wwhat() const {
		return m_msg.c_str();
	}

	WinError::WinError(const std::wstring& usr_msg) : WError(usr_msg + L": " + get_last_error_str()) {}

	std::wstring WinError::get_last_error_str() {
		DWORD err_code = GetLastError();
		LPTSTR err_msg = nullptr;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			err_code,
			0,
			reinterpret_cast<LPTSTR>(&err_msg),
			0,
			nullptr
		);
		std::wstring ret;
		if(err_msg != nullptr) {
			ret += err_msg;
			LocalFree(err_msg);
		}
		return ret;
	}

}