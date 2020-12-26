#ifndef WIN_ERROR_H
#define WIN_ERROR_H

#include <string>
#include <stdexcept>
#include <ST_common.h>

namespace shadertest {
	
	class WError : public std::exception {
	public:
		WError(const std::wstring& msg);

		const char *what() const override;

		virtual const wchar_t *wwhat() const;
	private:
		std::wstring	m_msg;
	};

	class WinError : public WError {
	public:
		WinError(const std::wstring& user_msg);
	private:
		static std::wstring get_last_error_str();
	};
	
}

#endif