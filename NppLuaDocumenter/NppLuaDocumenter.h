#pragma once
#include "NppPlugin.h"

class NppLuaDocumenter :
	public NppPlugin
{

	public:

		NppLuaDocumenter();
		virtual~NppLuaDocumenter()
		{ }

		virtual void  Init();
		void			Document();
		bool			FindFunction( tstring& str );

	protected:

};

void	DocumentExt();

/* xcopy "$(TargetPath)" "C:\Program Files (x86)\Notepad++\plugins\" /y */
