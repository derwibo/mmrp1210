

#ifndef MMRP1210_H
#define MMRP1210_H


extern "C" {

#define DLLEXPORT __declspec(dllexport)

BOOL WINAPI DLLEXPORT DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

short __stdcall DLLEXPORT RP1210_ClientConnect(HWND hwndClient, short nDeviceId, char* fpchProtocol,
                                     long lSendBuffer, long lReceiveBuffer, short nIsAppPacketizingIncomingMsgs);

short __stdcall DLLEXPORT RP1210_ClientDisconnect(short nClientID);

short __stdcall DLLEXPORT RP1210_SendMessage(short nClientID, char* fpchClientMessage, short nMessageSize, short nNotifyStatusOnTx, short nBlockOnSend);

short __stdcall DLLEXPORT RP1210_ReadMessage(short nClientID, char* fpchAPIMessage, short nBufferSize, short nBlockOnSend);

short __stdcall DLLEXPORT RP1210_SendCommand(short nCommandNumber, short nClientID, char* fpchClientCommand, short nMessageSize);

void __stdcall DLLEXPORT RP1210_ReadVersion(char* fpchDLLMajorVersion, char* fpchDLLMinorVersion, char* fpchAPIMajorVersion, char* fpchAPIMinorVersion);

#ifdef RP1210B
short __stdcall DLLEXPORT RP1210_ReadDetailedVersion(short nClientID, char* fpchAPIVersionInfo, char* fpchDLLVersionInfo, char* fpchFWVersionInfo);
#endif

short __stdcall DLLEXPORT RP1210_GetHardwareStatus(short nClientID, char* fpchClientInfo, short nInfoSize, short nBlockOnRequest);

#ifdef RP1210B
short __stdcall DLLEXPORT RP1210_GetErrorMsg(short err_code, char* fpchMessage, short nClientID);
#else
short __stdcall DLLEXPORT RP1210_GetErrorMsg(short err_code, char* fpchMessage);
#endif

#ifdef RP1210B
short __stdcall DLLEXPORT RP1210_GetLastErrorMsg(short err_code, int* SubErrorCode, char* fpchMessage)
#endif

}

#endif // MMRP1210_H
