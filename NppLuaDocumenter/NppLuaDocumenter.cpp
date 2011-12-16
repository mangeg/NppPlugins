#include "NppLuaDocumenter.h"
#include <regex>

using namespace std::	tr1;

NppLuaDocumenter::NppLuaDocumenter() :
	NppPlugin( 1 )
{
	m_Name = tstring( TEXT( "Lua Documenter\0" ) );
}

void NppLuaDocumenter::Init()
{
	for ( int i = 0; i < m_NrFunctions; i++ )
	{
		m_Functions.push_back( FuncItem() );
	}

	ShortcutKey*	sk = new ShortcutKey;
	sk->_isAlt = true;
	sk->_isCtrl = true;
	sk->_isShift = true;
	sk->_key = 'D';

	SetCommand( 0, TEXT( "Document" ), DocumentExt, sk, false );
}

void NppLuaDocumenter::Document()
{
	int which = -1;
	::SendMessage( m_NppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM) & which );
	if ( which == -1 )
	{
		return;
	}

	HWND	curScintilla = ( which == 0 ) ? m_NppData._scintillaMainHandle : m_NppData._scintillaSecondHandle;

	int		lineCount = ::SendMessage( curScintilla, SCI_GETLINECOUNT, 0, 0 );
	int		curPos = ::SendMessage( curScintilla, SCI_GETCURRENTPOS, 0, 0 );
	int		curLine = ::SendMessage( curScintilla, SCI_LINEFROMPOSITION, curPos, 0 );
	int		indentation = ::SendMessageA( curScintilla, SCI_GETLINEINDENTATION, curLine, 0 );

	int		currentParseLine = curLine;
	int		lineLength = 0;
	tstring line;

	while ( currentParseLine <= lineCount )
	{
		lineLength = ::SendMessage( curScintilla, SCI_LINELENGTH, currentParseLine, 0 );

		if ( lineLength <= 0 )
		{
			curLine++;
			continue;
		}

		char*	tmpLine = new char[lineLength + 1];
		ZeroMemory( tmpLine, sizeof(char) * (lineLength + 1) );
		::SendMessage( curScintilla, SCI_GETLINE, currentParseLine, (LPARAM) tmpLine );

		line = StringTools::ToUnicode( std::string( tmpLine ) );
		delete tmpLine;

		if ( FindFunction( line ) )
		{
			break;
		}

		if ( currentParseLine - curLine > 2 )
		{
			break;
		}

		currentParseLine++;
	}

	if ( line.length() > 0 )
	{
		int								lineStart = ::SendMessage( curScintilla, SCI_POSITIONFROMLINE, currentParseLine,
																   0 );
		int								startParan = line.find_first_of( TEXT( "(" ) );
		const tregex					pattern( TEXT( "[\\w\\.]+" ) );
		const tsregex_token_iterator	end;

		if ( startParan != -1 )
		{
			tstring					toParse = line.substr( startParan );
			std::vector<tstring>	params;

			for ( tsregex_token_iterator it( toParse.begin(), toParse.end(), pattern ); it != end; it++ )
			{
				params.push_back( tstring( *it ) );
			}

			if ( params.size() > 0 )
			{
				for ( std::vector<tstring>::reverse_iterator it = params.rbegin(); it != params.rend(); it++ )
				{
					std::string			ansiName = StringTools::ToAscii( *it );
					std::stringstream	s;
					s << "-- " << "@param " << ansiName << std::endl;
					::SendMessage( curScintilla, SCI_INSERTTEXT, lineStart, (LPARAM) s.str().c_str() );
					::SendMessage( curScintilla, SCI_SETLINEINDENTATION, currentParseLine, indentation );
				}

				::SendMessage( curScintilla, SCI_INSERTTEXT, lineStart, (LPARAM) "--- \n" );
				::SendMessage( curScintilla, SCI_SETLINEINDENTATION, currentParseLine, indentation );

				int lineEnd = ::SendMessage( curScintilla, SCI_GETLINEENDPOSITION, currentParseLine, 0 );
				::SendMessage( curScintilla, SCI_SETSELECTIONSTART, lineEnd, 0 );
				::SendMessage( curScintilla, SCI_SETSELECTIONEND, lineEnd, 0 );
			}
		}
	}
}

bool NppLuaDocumenter::FindFunction( tstring& str )
{
	int finding = str.find_first_of( TEXT( "function" ) );
	if ( finding > -1 )
	{
		return true;
	}

	return false;
}

NppLuaDocumenter plugin;

void DocumentExt()
{
	plugin.Document();
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved )
{
	switch ( reasonForCall )
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

extern "C"
__declspec( dllexport )
void setInfo( NppData notpadPlusData )
{
	plugin.SetData( notpadPlusData );
	plugin.Init();
}

extern "C"
__declspec( dllexport )
const TCHAR* getName()
{
	tstring name = plugin.GetName();
	int length = name.length();

	TCHAR*	cName = new TCHAR[length + 5];
	ZeroMemory( cName, sizeof(TCHAR) * length + 5 );

	for ( size_t i = 0; i < length; i++ )
	{
		cName[i] = name.at( i );
	}

	return cName;
}

extern "C"
__declspec( dllexport )
FuncItem* getFuncsArray( int* nbF )
{
	std::vector<FuncItem>	functions = plugin.GetFunctions();
	*nbF = functions.size();

	FuncItem*	funcList = new FuncItem[functions.size()];
	for ( size_t i = 0; i < functions.size(); i++ )
	{
		funcList[i] = functions[i];
	}

	return funcList;
}

extern "C"
__declspec( dllexport )
void beNotified( SCNotification* notifyCode )
{
	plugin.Notification( notifyCode );
}

extern "C"
__declspec( dllexport )
LRESULT messageProc( UINT Message, WPARAM wParam, LPARAM lParam )
{
	return plugin.MessageProc( Message, wParam, lParam );
}

#ifdef UNICODE
extern "C"
__declspec( dllexport )
BOOL isUnicode()
{
	return TRUE;
}
#endif /* UNICODE */
