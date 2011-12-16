#pragma once

#include "StringTools.h"
#include "Scintilla.h"
#include "Notepad_plus_msgs.h"

const int	nbChar = 64;

typedef const TCHAR*  ( __cdecl * PFUNCGETNAME ) ();

struct NppData
{
	HWND	_nppHandle;
	HWND	_scintillaMainHandle;
	HWND	_scintillaSecondHandle;
};

typedef void ( __cdecl *PFUNCSETINFO ) ( NppData );
typedef void ( __cdecl *PFUNCPLUGINCMD ) ();
typedef void ( __cdecl *PBENOTIFIED ) (SCNotification*);
typedef LRESULT ( __cdecl *PMESSAGEPROC ) ( UINT Message, WPARAM wParam, LPARAM lParam );

struct ShortcutKey
{
	bool	_isCtrl;
	bool	_isAlt;
	bool	_isShift;
	UCHAR	_key;
};

struct FuncItem
{
	TCHAR			_itemName[nbChar];
	PFUNCPLUGINCMD	_pFunc;
	int				_cmdID;
	bool			_init2Check;
	ShortcutKey*	_pShKey;
};

typedef FuncItem*  ( __cdecl * PFUNCGETFUNCSARRAY ) (int*);

class NppPlugin
{

	public:

		NppPlugin( int nrFunctions );
		virtual~NppPlugin()
		{ }

		virtual void  Init() = 0;
		virtual void SetData( NppData data )
		{
			m_NppData = data;
		}

		virtual std::vector<FuncItem> GetFunctions()
		{
			return m_Functions;
		}

		virtual void Notification( SCNotification* notifyCode )
		{ }

		virtual LRESULT MessageProc( UINT Message, WPARAM wParam, LPARAM lParam )
		{
			return TRUE;
		}

		virtual tstring GetName()
		{
			return m_Name;
		}

		void	MessageBox( const TCHAR* caption, const TCHAR* body ) const;

	protected:

		bool					SetCommand( int index, TCHAR* cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey* sk = NULL,
											bool checkOnInit = false );

		std::vector<FuncItem>	m_Functions;
		NppData m_NppData;
		int m_NrFunctions;
		tstring m_Name;

	private:

};
