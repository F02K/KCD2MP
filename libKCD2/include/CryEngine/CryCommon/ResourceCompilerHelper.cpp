#include "StdAfx.h"

#if defined(CRY_ENABLE_RC_HELPER)

#undef RC_EXECUTABLE
#define RC_EXECUTABLE "rc.exe"

#include "ResourceCompilerHelper.h"
#include "EngineSettingsManager.h"
#include "LineStreamBuffer.h"

#pragma warning (disable:4312)

#include <windows.h>		
#include <shellapi.h> //ShellExecuteW()
#include <assert.h>


// pseudo-variable that represents the DOS header of the module
EXTERN_C IMAGE_DOS_HEADER __ImageBase;


//////////////////////////////////////////////////////////////////////////
// Modified version of of FileUtil::DirectoryExists() from Code/Tools/CryCommonTools/FileUtil.h
//
// Examples of paths that will return true:
//   "existing_dir", "existing_dir/", "existing_dir/subdir", "e:", "e://", "e:.", "e:/a", ".", "..", "//storage/builds"
// Examples of paths that will return false:
//   "", "//storage", "//storage/.", "nonexisting_dir", "f:/" (if f: drive doesn't exist)
static bool DirectoryExists(const wchar_t* szPathPart0, const wchar_t* szPathPart1 = 0)
{
	SettingsManagerHelpers::CFixedString<wchar_t, MAX_PATH * 2> dir;

	if (szPathPart0 && szPathPart0[0])
	{
		dir.append(szPathPart0);
	}
	if (szPathPart1 && szPathPart1[0])
	{
		dir.append(szPathPart1);
	}

	const DWORD dwAttr = GetFileAttributesW(dir.c_str());
	return (dwAttr != INVALID_FILE_ATTRIBUTES) && ((dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}


//////////////////////////////////////////////////////////////////////////
static void ShowMessageBoxRcNotFound(const wchar_t* const szCmdLine, const wchar_t* const szDir)
{
	SettingsManagerHelpers::CFixedString<wchar_t, MAX_PATH * 4 + 150> tmp;

	if (szCmdLine && szCmdLine[0])
	{
		tmp.append(szCmdLine);
		tmp.append(L"\n\n");
	}
	if (szDir && szDir[0])
	{
		tmp.append(szDir);
		tmp.append(L"\n\n");
	}
	tmp.append(L"\n\n");
	tmp.append(L"ResourceCompiler was not found.\n\nPlease verify CryENGINE RootPath.");

	MessageBoxW(0, tmp.c_str(), L"Error", MB_ICONERROR | MB_OK);
}


//////////////////////////////////////////////////////////////////////////
namespace
{
class ResourceCompilerLineHandler
{
public:
	ResourceCompilerLineHandler(IResourceCompilerListener* listener)
		: m_listener(listener)
	{
	}

	void HandleLine(const char* line)
	{
		if (!m_listener || !line)
		{
			return;
		}

		// check the first three characters to see if it's a warning or error.
		bool bHasPrefix;
		IResourceCompilerListener::MessageSeverity severity;
		if ((line[0] == 'E') && (line[1] == ':') && (line[2] == ' '))
		{
			bHasPrefix = true;
			severity = IResourceCompilerListener::MessageSeverity_Error;
			line += 3;  // skip the prefix
		}
		else if ((line[0] == 'W') && (line[1] == ':') && (line[2] == ' '))
		{
			bHasPrefix = true;
			severity = IResourceCompilerListener::MessageSeverity_Warning;
			line += 3;  // skip the prefix
		}
		else if ((line[0] == ' ') && (line[1] == ' ') && (line[2] == ' '))
		{
			bHasPrefix = true;
			severity = IResourceCompilerListener::MessageSeverity_Info;
			line += 3;  // skip the prefix
		}
		else
		{
			bHasPrefix = false;
			severity = IResourceCompilerListener::MessageSeverity_Info;
		}

		if (bHasPrefix)
		{
			// skip thread info "%d>", if present
			{
				const char* p = line;
				while (*p == ' ')
				{
					++p;
				}
				if (isdigit(*p)) 
				{
					while (isdigit(*p))
					{
						++p;
					}
					if (*p == '>')
					{
						line = p + 1;
					}
				}
			}

			// skip time info "%d:%d", if present
			{
				const char* p = line;
				while (*p == ' ')
				{
					++p;
				}
				if (isdigit(*p))
				{
					while (isdigit(*p))
					{
						++p;
					}
					if (*p == ':')
					{
						++p;
						if (isdigit(*p))
						{
							while (isdigit(*p))
							{
								++p;
							}
							while (*p == ' ')
							{
								++p;
							}
							line = p;
						}
					}
				}
			}
		}

		m_listener->OnRCMessage(severity, line);		
	}

private:
	IResourceCompilerListener* m_listener;
};
}


//////////////////////////////////////////////////////////////////////////
CResourceCompilerHelper::ERcCallResult CResourceCompilerHelper::CallResourceCompiler(
	const char* szFileName, 
	const char* szAdditionalSettings, 
	IResourceCompilerListener* listener, 
	bool bMayShowWindow, 
	CResourceCompilerHelper::ERcExePath rcExePath, 
	bool bSilent,
	bool bNoUserDialog,
	const wchar_t* szWorkingDirectory,
	const wchar_t* szRootPath)
{
	// make command for execution
	SettingsManagerHelpers::CFixedString<wchar_t, MAX_PATH * 3> wRemoteCmdLine;
	CSettingsManagerTools smTools = CSettingsManagerTools();

	if (!szAdditionalSettings)
	{
		szAdditionalSettings = "";
	}

	wchar_t szRcDirectory[512];
	{
		wchar_t pathBuffer[512];
		switch (rcExePath)
		{
		case eRcExePath_registry:
			smTools.GetRootPathUtf16(true, SettingsManagerHelpers::CWCharBuffer(pathBuffer, sizeof(pathBuffer)));
			break;
		case eRcExePath_settingsManager:
			smTools.GetRootPathUtf16(false, SettingsManagerHelpers::CWCharBuffer(pathBuffer, sizeof(pathBuffer)));
			break;
		case eRcExePath_currentFolder:
			wcscpy(pathBuffer, L".");
			break;
		case eRcExePath_customPath:
			wcscpy(pathBuffer, szRootPath);
			break;
		default:
			return eRcCallResult_notFound;
		}

		if (!pathBuffer[0])
		{
			wcscpy(pathBuffer, L".");
		}

		if (smTools.Is64bitWindows() && (DirectoryExists(pathBuffer, L"/Bin64/rc") || !DirectoryExists(pathBuffer, L"/Bin32/rc")))
		{
			swprintf_s(szRcDirectory, L"%s/Bin64/rc", pathBuffer);
		}
		else
		{
			swprintf_s(szRcDirectory, L"%s/Bin32/rc", pathBuffer);
		}
	}

	wchar_t szRegSettingsBuffer[1024];
	smTools.GetEngineSettingsManager()->GetValueByRef("RC_Parameters", SettingsManagerHelpers::CWCharBuffer(szRegSettingsBuffer, sizeof(szRegSettingsBuffer)));

	wRemoteCmdLine.appendAscii("\"");
	wRemoteCmdLine.append(szRcDirectory);
	wRemoteCmdLine.appendAscii("/");
	wRemoteCmdLine.appendAscii(RC_EXECUTABLE);	
	wRemoteCmdLine.appendAscii("\"");

	if (!szFileName)
	{
		wRemoteCmdLine.appendAscii(" /userdialog=0 ");
		wRemoteCmdLine.appendAscii(szAdditionalSettings);
		wRemoteCmdLine.appendAscii(" ");
		wRemoteCmdLine.append(szRegSettingsBuffer);
	}
	else
	{
		wRemoteCmdLine.appendAscii(" \"");
		wRemoteCmdLine.appendAscii(szFileName);
		wRemoteCmdLine.appendAscii("\"");
		wRemoteCmdLine.appendAscii(bNoUserDialog ? " /userdialog=0 " : " /userdialog=1 ");
		wRemoteCmdLine.appendAscii(szAdditionalSettings);
		wRemoteCmdLine.appendAscii(" ");
		wRemoteCmdLine.append(szRegSettingsBuffer);
	}

	// Create a pipe to read the stdout of the RC.
	SECURITY_ATTRIBUTES saAttr;
	HANDLE hChildStdOutRd, hChildStdOutWr;
	HANDLE hChildStdInRd, hChildStdInWr;
	if (listener)
	{
		ZeroMemory(&saAttr, sizeof(saAttr));
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = 0;
		CreatePipe(&hChildStdOutRd, &hChildStdOutWr, &saAttr, 0);
		SetHandleInformation(hChildStdOutRd, HANDLE_FLAG_INHERIT, 0); // Need to do this according to MSDN
		CreatePipe(&hChildStdInRd, &hChildStdInWr, &saAttr, 0);
		SetHandleInformation(hChildStdInWr, HANDLE_FLAG_INHERIT, 0); // Need to do this according to MSDN
	}

	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwX = 100;
	si.dwY = 100;
	if (listener)
	{
		si.hStdError = hChildStdOutWr;
		si.hStdOutput = hChildStdOutWr;
		si.hStdInput = hChildStdInRd;
		si.dwFlags = STARTF_USEPOSITION | STARTF_USESTDHANDLES;
	}
	else
	{
		si.dwFlags = STARTF_USEPOSITION;
	}

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	bool bShowWindow;
	if (bMayShowWindow)
	{
		wchar_t buffer[20];
		smTools.GetEngineSettingsManager()->GetValueByRef("ShowWindow", SettingsManagerHelpers::CWCharBuffer(buffer, sizeof(buffer)));
		bShowWindow = (wcscmp(buffer, L"true") == 0);
	}
	else
	{
		bShowWindow = false;
	}

	const wchar_t* const szStartingDirectory = szWorkingDirectory ? szWorkingDirectory : szRcDirectory;

	if (!CreateProcessW(
		NULL,                   // No module name (use command line).
		const_cast<wchar_t*>(wRemoteCmdLine.c_str()), // Command line.
		NULL,                   // Process handle not inheritable.
		NULL,                   // Thread handle not inheritable.
		TRUE,                   // Set handle inheritance to TRUE.
		bShowWindow ? 0 : CREATE_NO_WINDOW, // creation flags.
		NULL,                   // Use parent's environment block.
		szStartingDirectory,    // Set starting directory.
		&si,                    // Pointer to STARTUPINFO structure.
		&pi))                   // Pointer to PROCESS_INFORMATION structure.
	{
		// The following  code block is commented out instead of being deleted 
		// because it's good to have at hand for a debugging session.







		if (!bSilent)
		{
			ShowMessageBoxRcNotFound(wRemoteCmdLine.c_str(), szStartingDirectory);
		}

		return eRcCallResult_notFound;
	}

	bool bFailedToReadOutput = false;

	if (listener)
	{
		// Close the pipe that writes to the child process, since we don't actually have any input for it.
		CloseHandle(hChildStdInWr);

		// Read all the output from the child process.
		CloseHandle(hChildStdOutWr);
		ResourceCompilerLineHandler lineHandler(listener);
		LineStreamBuffer lineBuffer(&lineHandler, &ResourceCompilerLineHandler::HandleLine);
		for (;;)
		{
			char buffer[2048];
			DWORD bytesRead;
			if (!ReadFile(hChildStdOutRd, buffer, sizeof(buffer), &bytesRead, NULL) || (bytesRead == 0))
			{
				break;
			}
			lineBuffer.HandleText(buffer, bytesRead);
		} 

		bFailedToReadOutput = lineBuffer.IsTruncated();
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode = eRcExitCode_Error;
	if (bFailedToReadOutput || GetExitCodeProcess(pi.hProcess, &exitCode) == 0)
	{
		exitCode = eRcExitCode_Error;
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	switch (exitCode)
	{
	case eRcExitCode_Success:
	case eRcExitCode_UserFixing:
		return eRcCallResult_success;
	case eRcExitCode_Crash:
		return eRcCallResult_crash;
	default:
		return eRcCallResult_error;
	}
}


//////////////////////////////////////////////////////////////////////////
void* CResourceCompilerHelper::AsyncCallResourceCompiler(
	const char* szFileName, 
	const char* szAdditionalSettings, 
	bool bMayShowWindow, 
	CResourceCompilerHelper::ERcExePath rcExePath, 
	bool bSilent,
	bool bNoUserDialog,
	const wchar_t* szWorkingDirectory,
	const wchar_t* szRootPath)
{
	// make command for execution
	SettingsManagerHelpers::CFixedString<wchar_t, MAX_PATH * 3> wRemoteCmdLine;
	CSettingsManagerTools smTools = CSettingsManagerTools();

	if (!szAdditionalSettings)
	{
		szAdditionalSettings = "";
	}

	wchar_t szRcDirectory[512];
	{
		wchar_t pathBuffer[512];
		switch (rcExePath)
		{
		case eRcExePath_registry:
			smTools.GetRootPathUtf16(true, SettingsManagerHelpers::CWCharBuffer(pathBuffer, sizeof(pathBuffer)));
			break;
		case eRcExePath_settingsManager:
			smTools.GetRootPathUtf16(false, SettingsManagerHelpers::CWCharBuffer(pathBuffer, sizeof(pathBuffer)));
			break;
		case eRcExePath_currentFolder:
			wcscpy(pathBuffer, L".");
			break;
		case eRcExePath_customPath:
			wcscpy(pathBuffer, szRootPath);
			break;
		default:
			return NULL;
		}

		if (!pathBuffer[0])
		{
			wcscpy(pathBuffer, L".");
		}

		if (smTools.Is64bitWindows() && (DirectoryExists(pathBuffer, L"/Bin64/rc") || !DirectoryExists(pathBuffer, L"/Bin32/rc")))
		{
			swprintf_s(szRcDirectory, L"%s/Bin64/rc", pathBuffer);
		}
		else
		{
			swprintf_s(szRcDirectory, L"%s/Bin32/rc", pathBuffer);
		}
	}

	wchar_t szRegSettingsBuffer[1024];
	smTools.GetEngineSettingsManager()->GetValueByRef("RC_Parameters", SettingsManagerHelpers::CWCharBuffer(szRegSettingsBuffer, sizeof(szRegSettingsBuffer)));

	wRemoteCmdLine.appendAscii("\"");
	wRemoteCmdLine.append(szRcDirectory);
	wRemoteCmdLine.appendAscii("/");
	wRemoteCmdLine.appendAscii(RC_EXECUTABLE);	
	wRemoteCmdLine.appendAscii("\"");

	if (!szFileName)
	{
		wRemoteCmdLine.appendAscii(" /userdialog=0 ");
		wRemoteCmdLine.appendAscii(szAdditionalSettings);
		wRemoteCmdLine.appendAscii(" ");
		wRemoteCmdLine.append(szRegSettingsBuffer);
	}
	else
	{
		wRemoteCmdLine.appendAscii(" \"");
		wRemoteCmdLine.appendAscii(szFileName);
		wRemoteCmdLine.appendAscii("\"");
		wRemoteCmdLine.appendAscii(bNoUserDialog ? " /userdialog=0 " : " /userdialog=1 ");
		wRemoteCmdLine.appendAscii(szAdditionalSettings);
		wRemoteCmdLine.appendAscii(" ");
		wRemoteCmdLine.append(szRegSettingsBuffer);
	}

	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwX = 100;
	si.dwY = 100;
	si.dwFlags = STARTF_USEPOSITION;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	bool bShowWindow;
	if (bMayShowWindow)
	{
		wchar_t buffer[20];
		smTools.GetEngineSettingsManager()->GetValueByRef("ShowWindow", SettingsManagerHelpers::CWCharBuffer(buffer, sizeof(buffer)));
		bShowWindow = (wcscmp(buffer, L"true") == 0);
	}
	else
	{
		bShowWindow = false;
	}

	const wchar_t* const szStartingDirectory = szWorkingDirectory ? szWorkingDirectory : szRcDirectory;

	if (!CreateProcessW(
		NULL,                   // No module name (use command line).
		const_cast<wchar_t*>(wRemoteCmdLine.c_str()), // Command line.
		NULL,                   // Process handle not inheritable.
		NULL,                   // Thread handle not inheritable.
		TRUE,                   // Set handle inheritance to TRUE.
		bShowWindow ? 0 : CREATE_NO_WINDOW, // creation flags.
		NULL,                   // Use parent's environment block.
		szStartingDirectory,    // Set starting directory.
		&si,                    // Pointer to STARTUPINFO structure.
		&pi))                   // Pointer to PROCESS_INFORMATION structure.
	{
		if (!bSilent)
		{
			ShowMessageBoxRcNotFound(wRemoteCmdLine.c_str(), szStartingDirectory);
		}
		return NULL;
	}

	return pi.hProcess;
}


//////////////////////////////////////////////////////////////////////////
ERcExitCode CResourceCompilerHelper::InvokeResourceCompiler(const char* szSrcFilePath, const char* szDstFilePath, const bool bUserDialog) 
{
	const char* szAdjustedFilename = szSrcFilePath;
#if defined(USE_GAMESTREAM)
	char szFullSrcPathBuf[ICryPak::g_nMaxPath];
	if (gEnv && gEnv->pGameStream)
	{
		szAdjustedFilename = gEnv->pCryPak->AdjustFileName(szSrcFilePath, szFullSrcPathBuf, ICryPak::FLAGS_RESOLVE_TO_CACHE);
	}
#endif

	ERcExitCode eRet = eRcExitCode_Pending;

	// make command for execution
	wchar_t szProjectDir[512];
	GetCurrentDirectoryW(sizeof(szProjectDir) / sizeof(szProjectDir[0]), szProjectDir);

	SettingsManagerHelpers::CFixedString<wchar_t, 512> wRemoteCmdLine;
	SettingsManagerHelpers::CFixedString<wchar_t, 512> wDir;
	CSettingsManagerTools smTools = CSettingsManagerTools();

	const char* const szRcParentDir =
		(smTools.Is64bitWindows() && (DirectoryExists(L"Bin64/rc") || !DirectoryExists(L"Bin32/rc")))
		? "Bin64" 
		: "Bin32";

	wchar_t szRegSettingsBuffer[1024];
	smTools.GetEngineSettingsManager()->GetValueByRef("RC_Parameters", SettingsManagerHelpers::CWCharBuffer(szRegSettingsBuffer, sizeof(szRegSettingsBuffer)));

	wRemoteCmdLine.appendAscii(szRcParentDir);
	wRemoteCmdLine.appendAscii("/rc/");
	wRemoteCmdLine.appendAscii(RC_EXECUTABLE);
	wRemoteCmdLine.appendAscii(" \"");
	wRemoteCmdLine.append(szProjectDir);
	wRemoteCmdLine.appendAscii("\\");
	wRemoteCmdLine.appendAscii(szAdjustedFilename);
	wRemoteCmdLine.appendAscii("\" /userdialog=0 ");
	wRemoteCmdLine.append(szRegSettingsBuffer);

	// make it write to a filename of our choice
	char szDstFilename[512];
	char szDstPath[512];

	RemovePath(szDstFilePath, szDstFilename, 512);
	RemoveFilename(szDstFilePath, szDstPath, 512);

	wRemoteCmdLine.appendAscii(" /overwritefilename=\"");
	wRemoteCmdLine.appendAscii(szDstFilename);
	wRemoteCmdLine.appendAscii("\"");
	wRemoteCmdLine.appendAscii(" /targetroot=\"");
	wRemoteCmdLine.append(szProjectDir);
	wRemoteCmdLine.appendAscii("\\");
	wRemoteCmdLine.appendAscii(szDstPath);
	wRemoteCmdLine.appendAscii("\"");

	wDir.append(szProjectDir);
	wDir.appendAscii("\\");
	wDir.appendAscii(szRcParentDir);
	wDir.appendAscii("\\rc");

	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwX = 100;
	si.dwY = 100;
	si.dwFlags = STARTF_USEPOSITION;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

#if defined(DEBUG) && !defined(NDEBUG) && defined(_RENDERER)
	extern ILog *iLog;

	char tmp1[512];
	char tmp2[512];
	SettingsManagerHelpers::CCharBuffer dst1(tmp1, 512); SettingsManagerHelpers::ConvertUtf16ToUtf8(wDir.c_str(), dst1);
	SettingsManagerHelpers::CCharBuffer dst2(tmp2, 512); SettingsManagerHelpers::ConvertUtf16ToUtf8(wRemoteCmdLine.c_str(), dst2);

	iLog->Log("Debug: RC: dir \"%s\", cmd \"%s\"\n", tmp1, tmp2);
#endif

	if (!CreateProcessW( 
		NULL,     // No module name (use command line). 
		const_cast<wchar_t*>(wRemoteCmdLine.c_str()), // Command line. 
		NULL,     // Process handle not inheritable. 
		NULL,     // Thread handle not inheritable. 
		FALSE,    // Set handle inheritance to FALSE. 
		BELOW_NORMAL_PRIORITY_CLASS + (bUserDialog ? 0 : CREATE_NO_WINDOW),	// creation flags. 
		NULL,     // Use parent's environment block. 
		wDir.c_str(),  // Set starting directory. 
		&si,      // Pointer to STARTUPINFO structure.
		&pi))     // Pointer to PROCESS_INFORMATION structure.
	{
		eRet = eRcExitCode_FatalError;
	}
	else
	{
		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exitCode;
		if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0)
		{
			eRet = eRcExitCode_Error;
		}
		else
		{
			eRet = (ERcExitCode)exitCode;
		}
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return eRet;
}


//////////////////////////////////////////////////////////////////////////
const char* CResourceCompilerHelper::GetCallResultDescription(ERcCallResult result)
{
	switch (result)
	{
	case eRcCallResult_success:
		return "Success.";
	case eRcCallResult_notFound:
		return "ResourceCompiler executable was not found.";
	case eRcCallResult_error:
		return "ResourceCompiler exited with an error.";
	case eRcCallResult_crash:
		return "ResourceCompiler crashed! Please report this. Include source asset and this log in the report.";
	case eRcCallResult_queued:
		return "ResourceCompiler added to the background-processing queue.";
	default: 
		return "Unexpected failure in ResultCompilerHelper.";
	}
}
#endif //(CRY_ENABLE_RC_HELPER)














































































