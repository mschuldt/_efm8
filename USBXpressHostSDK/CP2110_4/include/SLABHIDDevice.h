////////////////////////////////////////////////////////////////////////////////
// SLABHIDDevice.h
////////////////////////////////////////////////////////////////////////////////

#ifndef HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA
#define HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////


#include	"silabs_defs.h"
#include	"silabs_sal.h"
#ifdef _WIN32
#include <windows.h>
#else // !_WIN32
#endif // !_WIN32
#include "Types.h"

#ifdef _WIN32
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CP210xDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CP210xDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SLAB_HID_DEVICE_EXPORTS
#define SLAB_HID_DEVICE_API
#elif defined(SLAB_HID_DEVICE_BUILD_SOURCE)
#define SLAB_HID_DEVICE_API
#else
#define SLAB_HID_DEVICE_API __declspec(dllimport)
#pragma comment (lib, "SLABHIDDevice.lib")
#endif
#else // !_WIN32
#define SLAB_HID_DEVICE_API
#define WINAPI
#endif // !_WIN32


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

// Return Codes
typedef BYTE SLAB_HID_DEVICE_STATUS;

#define HID_DEVICE_SUCCESS					SILABS_STATUS_SUCCESS
#define HID_DEVICE_NOT_FOUND				0x01
#define HID_DEVICE_NOT_OPENED				0x02
#define HID_DEVICE_ALREADY_OPENED			0x03
#define	HID_DEVICE_TRANSFER_TIMEOUT			0x04
#define HID_DEVICE_TRANSFER_FAILED			0x05
#define HID_DEVICE_CANNOT_GET_HID_INFO		0x06
#define HID_DEVICE_HANDLE_ERROR				0x07
#define HID_DEVICE_INVALID_BUFFER_SIZE		0x08
#define HID_DEVICE_SYSTEM_CODE				0x09
#define HID_DEVICE_UNSUPPORTED_FUNCTION		0x0A
#define HID_DEVICE_UNKNOWN_ERROR			SILABS_STATUS_UNKNOWN_ERROR

// Max number of USB Devices allowed
#define MAX_USB_DEVICES					64

// Max number of reports that can be requested at time
#define MAX_REPORT_REQUEST_XP			512
#define MAX_REPORT_REQUEST_2K			200

#define DEFAULT_REPORT_INPUT_BUFFERS	0

// String Types
#define HID_VID_STRING					0x01
#define HID_PID_STRING					0x02
#define HID_PATH_STRING					0x03
#define HID_SERIAL_STRING				0x04
#define HID_MANUFACTURER_STRING			0x05
#define HID_PRODUCT_STRING				0x06

// String Lengths	// TODO: Length in bytes, or length in characters? NOTE: These are device-side string descriptor lengths, see host_common/include/silabs_usb.h
#define MAX_VID_LENGTH					5
#define MAX_PID_LENGTH					5
#define MAX_PATH_LENGTH                         260
#define MAX_SERIAL_STRING_LENGTH		256
#define MAX_MANUFACTURER_STRING_LENGTH	256
#define MAX_PRODUCT_STRING_LENGTH		256
#define MAX_INDEXED_STRING_LENGTH		256
#define MAX_STRING_LENGTH				260

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef void* HID_DEVICE;

/////////////////////////////////////////////////////////////////////////////
// Exported Functions
/////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

SLAB_HID_DEVICE_API	DWORD WINAPI
HidDevice_GetNumHidDevices(_In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidString(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _In_ _Pre_defensive_ const BYTE hidStringType, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI	HidDevice_GetHidIndexedString(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI HidDevice_GetHidAttributes(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ const WORD vid, _In_ const WORD pid, _Out_writes_bytes_(2) WORD* deviceVid, _Out_writes_bytes_(2) WORD* devicePid, _Out_writes_bytes_(2) WORD* deviceReleaseNumber);

SLAB_HID_DEVICE_API	void	WINAPI	HidDevice_GetHidGuid(void* hidGuid);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidLibraryVersion(_Out_writes_bytes_(1) _Pre_defensive_ BYTE* major, _Out_writes_bytes_(1) _Pre_defensive_ BYTE* minor, _Out_writes_bytes_(4) BOOL* release);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_Open(HID_DEVICE* device, _In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _In_ _Pre_defensive_ const DWORD numInputBuffers);

_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_IsOpened(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	HANDLE WINAPI
HidDevice_GetHandle(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_GetString(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const BYTE hidStringType, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_GetIndexedString(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_GetAttributes(_In_ _Pre_defensive_ const HID_DEVICE device, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_SetFeatureReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetFeatureReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize);

SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_SetOutputReport_Interrupt(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_GetInputReport_Interrupt(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize, _In_ _Pre_defensive_ const DWORD numReports, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ DWORD* bytesReturned);

_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	BYTE WINAPI
HidDevice_GetInputReport_Interrupt_WithTimeout(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize, _In_ _Pre_defensive_ const DWORD numReports, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ DWORD* bytesReturned, DWORD TimeoutMSec);

SLAB_HID_DEVICE_API BYTE WINAPI
HidDevice_SetOutputReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize);

SLAB_HID_DEVICE_API BYTE WINAPI
HidDevice_GetInputReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(bufferSize) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const DWORD bufferSize);

SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetInputReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetOutputReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetFeatureReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	DWORD WINAPI
HidDevice_GetMaxReportRequest(_In_ _Pre_defensive_ const HID_DEVICE device);

_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_FlushBuffers(_In_ _Pre_defensive_ const HID_DEVICE device);

_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_CancelIo(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	void WINAPI
HidDevice_GetTimeouts(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(4) _Pre_defensive_ DWORD* getReportTimeout, _Out_writes_bytes_(4) _Pre_defensive_ DWORD* setReportTimeout);
SLAB_HID_DEVICE_API	void WINAPI
HidDevice_SetTimeouts(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const DWORD getReportTimeout, _In_ _Pre_defensive_ const DWORD setReportTimeout);

_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_Close(_In_ _Pre_defensive_ const HID_DEVICE device);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA
