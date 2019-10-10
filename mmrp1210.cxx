
#include <ctype.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include "mmrp1210.h"

#define NO_ERRORS  0
#define ERR_DEVICE_NOT_SUPPORTED 207

typedef short (__stdcall *fxRP1210_ClientConnect)       ( HWND, short, char*, long, long, short );
typedef short (__stdcall *fxRP1210_ClientDisconnect)    ( short );
typedef short (__stdcall *fxRP1210_SendMessage)         ( short, char*, short, short, short );
typedef short (__stdcall *fxRP1210_ReadMessage)         ( short, char*, short, short );
typedef short (__stdcall *fxRP1210_SendCommand)         ( short, short, char*, short );
typedef short (__stdcall *fxRP1210_ReadVersion)         ( char*, char*, char*, char* );
typedef short (__stdcall *fxRP1210_ReadDetailedVersion) ( short, char*, char*, char* );
typedef short (__stdcall *fxRP1210_GetHardwareStatus)   ( short, char*, short, short );
typedef short (__stdcall *fxRP1210_GetLastErrorMsg)     ( short, int *, char*, short );
#ifdef RP1210B
typedef short (__stdcall *fxRP1210_GetErrorMsg)         ( short, char*, short);
#else
typedef short (__stdcall *fxRP1210_GetErrorMsg)         ( short, char* );
#endif


fxRP1210_ClientConnect           pRP1210_ClientConnect           = NULL;
fxRP1210_ClientDisconnect        pRP1210_ClientDisconnect        = NULL;
fxRP1210_ReadMessage             pRP1210_ReadMessage             = NULL;
fxRP1210_SendMessage             pRP1210_SendMessage             = NULL;
fxRP1210_SendCommand             pRP1210_SendCommand             = NULL;
fxRP1210_ReadVersion             pRP1210_ReadVersion             = NULL;
#ifdef RP1210B
fxRP1210_ReadDetailedVersion     pRP1210_ReadDetailedVersion     = NULL;
#endif
fxRP1210_GetErrorMsg             pRP1210_GetErrorMsg             = NULL;
#ifdef RP1210B
fxRP1210_GetLastErrorMsg         pRP1210_GetLastErrorMsg         = NULL;
#endif
fxRP1210_GetHardwareStatus       pRP1210_GetHardwareStatus       = NULL;

HINSTANCE hPCAN1210DLL = NULL;
HANDLE hDebugFile = NULL;
char sMsgBuf[64];

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{

  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      break;
    case DLL_PROCESS_DETACH:
      if(hDebugFile)
      {
        CloseHandle(hDebugFile);
        hDebugFile = 0;
      }
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
  }

  return TRUE;
}


short __stdcall RP1210_ClientConnect(HWND hwndClient, short nDeviceId, char* fpchProtocol,
                                     long lSendBuffer, long lReceiveBuffer, short nIsAppPacketizingIncomingMsgs)
{
  short nClientID;
  int iMsgLen;
  DWORD dwBytesWritten;

  hDebugFile = CreateFile("C:\\rp1210debug.log", GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if(hDebugFile == INVALID_HANDLE_VALUE)
  {
    MessageBox(0, "Error creating file", "RP1210_ClientConnect", MB_OK);
    hDebugFile = 0;
    return ERR_DEVICE_NOT_SUPPORTED;
  }

  iMsgLen = sprintf(sMsgBuf, "RP1210_ClientConnect nDeviceId=%d\r\n", nDeviceId);
  WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);

  if ( ( hPCAN1210DLL = LoadLibrary("pcanrp32.dll") ) == NULL )   
    {
      DWORD errcode;
      errcode = GetLastError();
      iMsgLen = sprintf(sMsgBuf, "Load Library Error %d\r\n", errcode);
      WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
      return ERR_DEVICE_NOT_SUPPORTED;
    }
  
  pRP1210_ClientConnect = (fxRP1210_ClientConnect)(GetProcAddress(hPCAN1210DLL, "RP1210_ClientConnect"));
  
  if ( pRP1210_ClientConnect == NULL ) 
    {
      pRP1210_ClientConnect = (fxRP1210_ClientConnect)(GetProcAddress(hPCAN1210DLL, "_RP1210_ClientConnect@24"));
      
      if ( pRP1210_ClientConnect == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_ClientDisconnect = (fxRP1210_ClientDisconnect)(GetProcAddress(hPCAN1210DLL, "RP1210_ClientDisconnect"));
  
  if ( pRP1210_ClientDisconnect == NULL ) 
    {
      pRP1210_ClientDisconnect = (fxRP1210_ClientDisconnect)(GetProcAddress(hPCAN1210DLL, "_RP1210_ClientDisconnect@4"));
      
      if ( pRP1210_ClientDisconnect == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_ReadMessage = (fxRP1210_ReadMessage)(GetProcAddress(hPCAN1210DLL, "RP1210_ReadMessage"));
  
  if ( pRP1210_ReadMessage == NULL ) 
    {
      pRP1210_ReadMessage = (fxRP1210_ReadMessage)(GetProcAddress(hPCAN1210DLL,"_RP1210_ReadMessage@16"));
      
      if ( pRP1210_ReadMessage == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_SendMessage = (fxRP1210_SendMessage)(GetProcAddress(hPCAN1210DLL, "RP1210_SendMessage"));
  
  if ( pRP1210_SendMessage == NULL ) 
    {
      pRP1210_SendMessage = (fxRP1210_SendMessage)(GetProcAddress(hPCAN1210DLL, "_RP1210_SendMessage@20"));
      
      if ( pRP1210_SendMessage == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_SendCommand = (fxRP1210_SendCommand)(GetProcAddress(hPCAN1210DLL, "RP1210_SendCommand"));
  
  if ( pRP1210_SendCommand == NULL ) 
    {
      pRP1210_SendCommand = (fxRP1210_SendCommand)(GetProcAddress(hPCAN1210DLL, "_RP1210_SendCommand@16"));
      
      if ( pRP1210_SendCommand == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_ReadVersion = (fxRP1210_ReadVersion)(GetProcAddress(hPCAN1210DLL, "RP1210_ReadVersion"));
  
  if ( pRP1210_ReadVersion == NULL ) 
    {
      pRP1210_ReadVersion = (fxRP1210_ReadVersion)(GetProcAddress(hPCAN1210DLL, "_RP1210_ReadVersion@16"));
      
      if ( pRP1210_ReadVersion == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  
#ifdef RP1210B
  pRP1210_ReadDetailedVersion = (fxRP1210_ReadDetailedVersion)(GetProcAddress(hPCAN1210DLL, "RP1210_ReadDetailedVersion"));
  
  if ( pRP1210_ReadDetailedVersion == NULL ) 
    {
      pRP1210_ReadDetailedVersion = (fxRP1210_ReadDetailedVersion)(GetProcAddress(hPCAN1210DLL, "_RP1210_ReadDetailedVersion@16"));
      
      if ( pRP1210_ReadDetailedVersion == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
#endif
  
  pRP1210_GetHardwareStatus = (fxRP1210_GetHardwareStatus)(GetProcAddress(hPCAN1210DLL, "RP1210_GetHardwareStatus"));
  
  if ( pRP1210_GetHardwareStatus == NULL ) 
    {
      pRP1210_GetHardwareStatus = (fxRP1210_GetHardwareStatus)(GetProcAddress(hPCAN1210DLL, "_RP1210_GetHardwareStatus@16") );
      
      if ( pRP1210_GetHardwareStatus == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
  pRP1210_GetErrorMsg = (fxRP1210_GetErrorMsg)(GetProcAddress(hPCAN1210DLL, "RP1210_GetErrorMsg"));
  
  if ( pRP1210_GetErrorMsg == NULL ) 
    {
      pRP1210_GetErrorMsg = (fxRP1210_GetErrorMsg)(GetProcAddress(hPCAN1210DLL, "_RP1210_GetErrorMsg@16"));
      
      if ( pRP1210_GetErrorMsg == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
  
#ifdef RP1210B  
  pRP1210_GetLastErrorMsg = (fxRP1210_GetLastErrorMsg)(GetProcAddress(hPCAN1210DLL, "RP1210_GetLastErrorMsg"));
  
  if ( pRP1210_GetLastErrorMsg == NULL ) 
    {
      pRP1210_GetLastErrorMsg= (fxRP1210_GetLastErrorMsg)(GetProcAddress(hPCAN1210DLL, "_RP1210_GetLastErrorMsg@20"));
      if ( pRP1210_GetLastErrorMsg == NULL ) 
        {
          return ERR_DEVICE_NOT_SUPPORTED;
        }
    }
#endif

  iMsgLen = sprintf(sMsgBuf, "Protocol = %s\r\n", fpchProtocol);
  WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);

  nClientID = pRP1210_ClientConnect(0, 261, "CAN", 0, 0, 0);
//  nClientID = pRP1210_ClientConnect(0, 261, fpchProtocol, lSendBuffer, lReceiveBuffer, nIsAppPacketizingIncomingMsgs);

  iMsgLen = sprintf(sMsgBuf, "RP1210_ClientConnect ID=%d\r\n", nClientID);
  WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);

  return nClientID;
}

short __stdcall RP1210_ClientDisconnect(short nClientID)
{
  short iRet;
  iRet = pRP1210_ClientDisconnect(nClientID);

  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_ClientDisconnect\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);

    CloseHandle(hDebugFile);
    hDebugFile = 0;
  }

  FreeLibrary( hPCAN1210DLL );

  return iRet;
}

short __stdcall RP1210_SendMessage(short nClientID, char* fpchClientMessage, short nMessageSize, short nNotifyStatusOnTx, short nBlockOnSend)
{
  short iRet;
/*
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_SendMessage\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }
*/
  iRet = pRP1210_SendMessage(nClientID, fpchClientMessage, nMessageSize, nNotifyStatusOnTx, nBlockOnSend);

  if(iRet != NO_ERRORS)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_SendMessage returned %d\r\n", iRet);
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  return iRet;
}

short __stdcall RP1210_ReadMessage(short nClientID, char* fpchAPIMessage, short nBufferSize, short nBlockOnSend)
{
  short iRet;
/*
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_ReadMessage\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }
*/
  iRet = pRP1210_ReadMessage(nClientID, fpchAPIMessage, nBufferSize, nBlockOnSend);

  if(iRet <= 0)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_ReadMessage returned %d\r\n", iRet);
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  return iRet;
}

short __stdcall RP1210_SendCommand(short nCommandNumber, short nClientID, char* fpchClientCommand, short nMessageSize)
{
  short iRet;
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_SendCommand %d\r\n", nCommandNumber);
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  iRet = pRP1210_SendCommand(nCommandNumber, nClientID, fpchClientCommand, nMessageSize);

  if(iRet != NO_ERRORS)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_SendCommand returned %d\r\n", iRet);
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  return iRet;
}

void __stdcall RP1210_ReadVersion(char* fpchDLLMajorVersion, char* fpchDLLMinorVersion, char* fpchAPIMajorVersion, char* fpchAPIMinorVersion)
{
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_ReadVersion\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  pRP1210_ReadVersion(fpchDLLMajorVersion, fpchDLLMinorVersion, fpchAPIMajorVersion, fpchAPIMinorVersion);
}

#ifdef RP1210B

short __stdcall RP1210_ReadDetailedVersion(short nClientID, char* fpchAPIVersionInfo, char* fpchDLLVersionInfo, char* fpchFWVersionInfo)
{
  return pRP1210_ReadDetailedVersion(nClientID, fpchAPIVersionInfo, fpchDLLVersionInfo, fpchFWVersionInfo);
}

#endif

short __stdcall RP1210_GetHardwareStatus(short nClientID, char* fpchClientInfo, short nInfoSize, short nBlockOnRequest)
{
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_GetHardwareStatus\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  return pRP1210_GetHardwareStatus(nClientID, fpchClientInfo, nInfoSize, nBlockOnRequest);
}

#ifdef RP1210B

short __stdcall RP1210_GetErrorMsg(short err_code, char* fpchMessage, short nClientID)
{
  return pRP1210_GetErrorMsg(err_code, fpchMessage, nClientID);
}

#else

short __stdcall RP1210_GetErrorMsg(short err_code, char* fpchMessage)
{
  if(hDebugFile)
  {
    int iMsgLen;
    DWORD dwBytesWritten;
    iMsgLen = sprintf(sMsgBuf, "RP1210_GetErrorMsg\r\n");
    WriteFile(hDebugFile, sMsgBuf, iMsgLen, &dwBytesWritten, 0);
  }

  return pRP1210_GetErrorMsg(err_code, fpchMessage);
}

#endif


#ifdef RP1210B

short __stdcall RP1210_GetLastErrorMsg(short err_code, int* SubErrorCode, char* fpchMessage)
{
  return pRP1210_GetLastErrorMsg(err_code, SubErrorCode, fpchMessage);
}

#endif
