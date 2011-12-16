#include "StringTools.h"

std::string StringTools::ToAscii( std::wstring& txt )
{
	std::string temp( txt.length(), ' ' );
	std::copy( txt.begin(), txt.end(), temp.begin() );
	return temp;
}

std::wstring StringTools::ToUnicode( std::string& txt )
{
	std::wstring temp( txt.length(), L' ' );
	std::copy( txt.begin(), txt.end(), temp.begin() );
	return temp;
}

void StringTools::StringSplit( tstring& str, tstring& delim, std::vector<tstring>& result )
{
	int start = 0;
	int end = str.find_first_of( delim );
	if ( end != -1 )
	{
		while ( end != -1 )
		{
			result.push_back( str.substr( start, end - start ) );
			start = end + 1;
			end = str.find_first_of( delim, start );
			if ( end == -1 )
			{
				result.push_back( str.substr( start ) );
			}
		}
	}
	else
	{
		result.push_back( str.substr( start ) );
	}
}

tstring StringTools::Trim( tstring& str )
{
	tstring ret( str );

	ret = Trim( ret, ' ' );
	ret = Trim( ret, '\t' );

	return ret;
}

tstring StringTools::Trim( tstring& str, TCHAR toTrim )
{
	tstring ret( str );

	for ( size_t i = 0; i < ret.length(); i++ )
	{
		if ( ret.at( i ) == toTrim )
		{
			ret.erase( i, 1 );
		}
		else
		{
			break;
		}
	}

	for ( size_t i = ret.length(); i > 0; i-- )
	{
		if ( ret.at( i ) == toTrim )
		{
			ret.erase( i, 1 );
		}
		else
		{
			break;
		}
	}

	return ret;
}

tstring StringTools::Trim( tstring& str, std::vector<TCHAR> toTrim )
{
	tstring ret( str );
	for ( std::vector<TCHAR>::iterator it = toTrim.begin(); it != toTrim.end(); it++ )
	{
		ret = Trim( ret, (*it) );
	}

	return ret;
}
