#include "NppPlugin.h"

NppPlugin::NppPlugin( int nrFunctions )
{
	m_NrFunctions = nrFunctions;
	m_Functions = std::vector<FuncItem> ( nrFunctions );
}

bool NppPlugin::SetCommand( int index, TCHAR*  cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey*	sk /* = NULL */,
							bool checkOnInit	/* = false */
						   )
{
	if ( index > m_NrFunctions - 1 )
	{
		return false;
	}

	if ( !pFunc )
	{
		return false;
	}

	lstrcpy( m_Functions[index]._itemName, cmdName );

	m_Functions[index]._pFunc = pFunc;
	m_Functions[index]._init2Check = checkOnInit;
	m_Functions[index]._pShKey = sk;

	return true;
}

void NppPlugin::MessageBox( const TCHAR* caption, const TCHAR* body ) const
{
	::MessageBox( NULL, (LPCTSTR) body, (LPCTSTR) caption, MB_OK );
}
