#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

typedef std::basic_string<TCHAR>								tstring;
typedef std::basic_string<TCHAR>								ctstring;
typedef std::basic_stringstream<TCHAR>							tsstream;
typedef std::tr1::basic_regex<TCHAR>							tregex;
typedef std::tr1::regex_token_iterator<tstring::const_iterator> tsregex_token_iterator;

class StringTools
{

public:

	static std::string	ToAscii( std::wstring & txt );
	static std::wstring ToUnicode( std::string & txt );
	static void			StringSplit( tstring & str, tstring & delim, std::vector<tstring> &result );
	static tstring		Trim( tstring& str );
	static tstring		Trim( tstring& str, TCHAR toTrim );
	static tstring		Trim( tstring & str, std::vector<TCHAR> toTrim );
};
