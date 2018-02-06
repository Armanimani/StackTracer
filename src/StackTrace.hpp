#pragma once

#include <string>
#include <vector>
#include <mutex>

#ifdef _WIN32

#include <Windows.h>
#include <DbgHelp.h>
#pragma comment (lib, "imagehlp.lib")

std::vector<std::string> stackTrace()
{
	static std::mutex m;
	std::lock_guard<std::mutex> lock(m);

	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));

	#ifdef _M_IX86
	context.ContextFlags = CONTEXT_CONTROL;

	__asm
	{
	Label:
		mov[context.Ebp], ebp;
		mov[context.Esp], esp;
		mov eax, [Label];
		mov[context.Eip], eax;
	}
	#else
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);
	#endif

	SymInitialize(process, NULL, TRUE);

	DWORD image;
	STACKFRAME64 stackframe;
	ZeroMemory(&stackframe, sizeof(STACKFRAME64));

	#ifdef _M_IX86
	image = IMAGE_FILE_MACHINE_I386;
	stackframe.AddrPC.Offset = context.Eip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Ebp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Esp;
	stackframe.AddrStack.Mode = AddrModeFlat;
	#elif _M_X64
	image = IMAGE_FILE_MACHINE_AMD64;
	stackframe.AddrPC.Offset = context.Rip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Rsp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Rsp;
	stackframe.AddrStack.Mode = AddrModeFlat;
	#elif _M_IA64
	image = IMAGE_FILE_MACHINE_IA64;
	stackframe.AddrPC.Offset = context.StIIP;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.IntSp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrBStore.Offset = context.RsBSP;
	stackframe.AddrBStore.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.IntSp;
	stackframe.AddrStack.Mode = AddrModeFlat;
	#endif

	std::vector<std::string> ret;
	while (StackWalk64(image, process, thread, &stackframe, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
	{
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO) buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		DWORD64 displacement = 0;
		if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol))
		{
			ret.push_back(symbol->Name);
		}
		else
		{
			ret.push_back("?????");
		}
	}

	SymCleanup(process);
	return ret;
}
#elif
"NOT IMPLEMENTED YET"
#endif