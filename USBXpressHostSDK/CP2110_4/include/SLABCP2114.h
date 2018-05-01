/////////////////////////////////////////////////////////////////////////////
// SLABCP2114.h
// For SLABHIDtoUART.dll
// and Silicon Labs CP2114 HID to UART
/////////////////////////////////////////////////////////////////////////////
#ifndef SLAB_CP2114_H
#define SLAB_CP2114_H

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABHIDtoUART.h"
#include "CP2114_Common.h"

/////////////////////////////////////////////////////////////////////////////
// Return Code Definitions
/////////////////////////////////////////////////////////////////////////////

// Errors associated with Config
#define HID_UART_INVALID_CONFIG_NUMBER              kInvalidConfigNumber        // Requested config number >= MAX_CONFIGS
#define HID_UART_BOOT_INDEXES_DEPLETED              kBootIndicesDepleted        // All boot indices have been used
#define HID_UART_REQUESTED_CONFIG_NOT_PRESENT       kRequestedConfigNotPresent  // Pointer to requested config is 0xFFFF
#define HID_UART_CONFIG_INVALID                     kRequestedConfigInvalid     // Specified config consists of invalid parameters
#define HID_UART_CONFIG_POINTERS_DEPLETED           kConfigPointersDepleted     // All Config pointer slots have been used
#define HID_UART_CONFIG_SPACE_DEPLETED              kConfigSpaceDepleted        // Not enough space to save the new config
#define HID_UART_BOOT_INDEX_UNCHANGED               kBootIndexUnchanged         // The user-specified boot index is already the current boot index
#define HID_UART_CONFIG_UNCHANGED                   kConfigUnchanged            // The Config is already as the user requested
#define HID_UART_INVALID_CONFIG_SEQUENCE_IDENTIFIER kConfigInvalidConfigIdentifier  // Invalid Config string ID
#define HID_UART_INVALID_CONFIG_SETTINGS            kConfigSettingsInvalid      // Configuration contains invalid setting(s)
#define HID_UART_UNSUPPORTED_CONFIG_FORMAT          kUnsupportedConfigFormat    // The config format version is not supported by library/firmware

// 'Set Parameters for Next Get' errors
#define HID_UART_INVALID_NUMBER_OF_CACHED_PARAMS    kInvalidNumberOfCachedParams// Specified tSetParamsForNextGet.params > MAX_CACHED_PARAMS
#define HID_UART_UNEXPECTED_CACHE_DATA              kUnexpectedCacheData        // Something in tSetParamsForNextGet was not as expected

// I2C Errors
#define HID_UART_I2C_BUSY                           kI2cBusy                    // I2C bus is busy
#define HID_UART_I2C_TIMEOUT                        kI2cTimeout                 // Timeout waiting for I2C event (start, ACK, etc.)
#define HID_UART_I2C_INVALID_TOKEN                  kI2cInvalidToken            // I2C interpreter detected invalid token
#define HID_UART_I2C_INVALID_WRITE_LENGTH           kI2cInvalidWriteLength      // Specified number of bytes to write is 0 or too large
#define HID_UART_I2C_INVALID_CONFIG_LENGTH          kI2cInvalidConfigLength     // Specified configuration length is invalid
#define HID_UART_I2C_SCL_STUCK_LOW                  kI2cSclStuckLow             // SCL line is stuck low
#define HID_UART_I2C_SDA_STUCK_LOW                  kI2cSdaStuckLow             // SDA line is stuck low


/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

// Pin Config Mode Array Indices
#define CP2114_INDEX_GPIO_0                         0
#define CP2114_INDEX_GPIO_1                         1
#define CP2114_INDEX_GPIO_2                         2
#define CP2114_INDEX_GPIO_3                         3
#define CP2114_INDEX_GPIO_4                         4
#define CP2114_INDEX_GPIO_5                         5
#define CP2114_INDEX_GPIO_6                         6
#define CP2114_INDEX_GPIO_7                         7
#define CP2114_INDEX_GPIO_8                         8
#define CP2114_INDEX_GPIO_9                         9
#define CP2114_INDEX_TX                             10
#define CP2114_INDEX_RX                             11
#define CP2114_INDEX_SUSPEND                        12
#define CP2114_INDEX_SUSPEND_BAR                    13
// Size of the above array pointed to by pinConfig parameter in CP2114_SetPinConfig and CP2114_GetPinConfig
#define CP2114_PIN_CONFIG_SIZE                      14

// Pin Bitmasks
#define CP2114_MASK_GPIO_0                          0x0001
#define CP2114_MASK_GPIO_1                          0x0002
#define CP2114_MASK_GPIO_2                          0x0004
#define CP2114_MASK_GPIO_3                          0x0008
#define CP2114_MASK_GPIO_4                          0x0010
#define CP2114_MASK_GPIO_5                          0x0020
#define CP2114_MASK_GPIO_6                          0x0040
#define CP2114_MASK_GPIO_7                          0x0080
#define CP2114_MASK_GPIO_8                          0x0100
#define CP2114_MASK_GPIO_9                          0x0200
#define CP2114_MASK_TX                              0x0400
#define CP2114_MASK_RX                              0x0800
#define CP2114_MASK_SUSPEND                         0x1000
#define CP2114_MASK_SUSPEND_BAR                     0x2000

/////////////////////////////////////////////////////////////////////////////
// DAC Definitions
/////////////////////////////////////////////////////////////////////////////

#define MAX_DAC_CONFIG_SIZE (2*512 - sizeof(CP2114_RAM_CONFIG_STRUCT))

// 60 is the largest common multiple of 2, 3 and 4. This ensures that 
// an I2C transactions contained in the SET/GET_DAC_REGISTERS payload 
// will not be split across multiple frames.
#define DAC_REGISTERS_PAYLOAD_MAX_LEN               60

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef tDeviceCaps CP2114_CAPS_STRUCT, *PCP2114_CAPS_STRUCT;
typedef _RAM_CONFIG_STRUCT CP2114_RAM_CONFIG_STRUCT, *PCP2114_RAM_CONFIG_STRUCT;

/////////////////////////////////////////////////////////////////////////////
// DAC Structures
/////////////////////////////////////////////////////////////////////////////

typedef union _CP2114_OTP_CONFIG
{
    struct // if config_version == CP2114_CONFIG_VERSION_B01
    {
        BYTE RemConfig[ RAM_CONFIG_SIZE_B01];
        BYTE DacConfig[ MAX_DAC_CONFIG_SIZE];
    } CP2114_B01;
    struct // if config_version == CP2114_CONFIG_VERSION_B02
    {
        BYTE PemConfig[ RAM_CONFIG_SIZE_B02];
        BYTE DacConfig[ MAX_DAC_CONFIG_SIZE];
    } CP2114_B02;
    BYTE Other[ 0xffff]; // Max size that can be specified in 2 bytes
} CP2114_OTP_CONFIG, *PCP2114_OTP_CONFIG;

typedef struct _CP2114_OTP_CONFIG_GET
{
    U16 Length;
    CP2114_OTP_CONFIG OtpConfig;
} CP2114_OTP_CONFIG_GET, *PCP2114_OTP_CONFIG_GET;

/////////////////////////////////////////////////////////////////////////////
// Exported Library Functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// CP2114_GetVersions
_Check_return_
_Ret_range_(HID_UART_SUCCESS, HID_UART_UNKNOWN_ERROR)
_Success_(return == HID_UART_SUCCESS)
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetVersions(_In_ _Pre_defensive_  HID_UART_DEVICE device, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* api_version, _Out_writes_bytes_(1) BYTE* fw_version, _Out_writes_bytes_(1) BYTE* config_version);

// CP2114_SetPinConfig
/// @brief Sets the Pin Config to the device
/// @param device is an open handle to the device
/// @param pinConfig points at a buffer into which the port latch value will be written
/// @param useSuspendValues points at a buffer into which the port latch value will be written
/// @param suspendValue points at a buffer into which the port latch value will be written
/// @param suspendMode points at a buffer into which the port latch value will be written
/// @param clkDiv points at a buffer into which the port latch value will be written
/// @returns Returns HID_UART_SUCCESS on success, another HID_UART_STATUS if there is an error:
///			HID_UART_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_UART_DEVICE_NOT_SUPPORTED -- device is not a CP2114
///			HID_UART_INVALID_PARAMETER -- pinConfig is NULL
_Check_return_
_Ret_range_(HID_UART_SUCCESS, HID_UART_UNKNOWN_ERROR)
_Success_(return == HID_UART_SUCCESS)
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_SetPinConfig(
	_In_ _Pre_defensive_ const HID_UART_DEVICE device,
	_In_reads_bytes_(CP2114_PIN_CONFIG_SIZE) _Pre_defensive_ BYTE* pinConfig,
	_In_ _Pre_defensive_ const BOOL useSuspendValues,
	_In_ _Pre_defensive_ const WORD suspendValue,
	_In_ _Pre_defensive_ const WORD suspendMode,
	_In_ _Pre_defensive_ const BYTE clkDiv
);

// CP2114_GetPinConfig
/// @brief Reads and returns the Pin Config from the device
/// @param device is an open handle to the device
/// @param pinConfig points at a buffer into which the pin config will be written
/// @param useSuspendValues points at a BOOL into which the useSuspendValues value will be written
/// @param suspendValue points at a WORD into which the suspendValue value will be written
/// @param suspendMode points at a WORD into which the suspendMode value will be written
/// @param clkDiv points at a BYTE into which the clkDiv value will be written
/// @returns Returns HID_UART_SUCCESS on success, another HID_UART_STATUS if there is an error:
///			HID_UART_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_UART_DEVICE_NOT_SUPPORTED -- device is not a CP2110
///			HID_UART_INVALID_PARAMETER -- pinConfig or useSuspendValues or suspendValue or suspendMode or clkDiv is NULL
_Check_return_
_Ret_range_(HID_UART_SUCCESS, HID_UART_UNKNOWN_ERROR)
_Success_(return == HID_UART_SUCCESS)
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetPinConfig(
	_In_ _Pre_defensive_ const HID_UART_DEVICE device,
	_Out_writes_bytes_(CP2114_PIN_CONFIG_SIZE) _Pre_defensive_ BYTE* pinConfig,
	_Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* useSuspendValues,
	_Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* suspendValue,
	_Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* suspendMode,
	_Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* clkDiv
);

// CP2114_GetDeviceStatus
// This function shouldn't be called in normal conditions. The other APIs call this function.
// However, due to possible reenumeration after HidUart_SetRamConfig, getCP2114Status can't be called
// immediately after HidUart_SetRamConfig, this can be used to clear possible ConfigUnchanged status
// prior to the next command.

HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetDeviceStatus(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE *pCP2114Status);

// CP2114_GetDeviceCaps
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetDeviceCaps(_In_ _Pre_defensive_ HID_UART_DEVICE device, PCP2114_CAPS_STRUCT pCP2114CapsStruct);

// CP2114_SetRamConfig
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_SetRamConfig(_In_ _Pre_defensive_ HID_UART_DEVICE device, PCP2114_RAM_CONFIG_STRUCT pCP2114RamConfigStruct);

// CP2114_GetRamConfig
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetRamConfig(_In_ _Pre_defensive_ HID_UART_DEVICE device, PCP2114_RAM_CONFIG_STRUCT pCP2114RamConfigStruct);

// CP2114_SetDacRegisters
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_SetDacRegisters(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE* pDacConfigBuffer, BYTE dacConfigBufferLength);

// CP2114_GetDacRegisters
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetDacRegisters(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE dacStartAddress, BYTE dacRegistersToRead, BYTE* pDacConfigBuffer);

// CP2114_GetOtpConfig
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_GetOtpConfig(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE cp2114ConfigNumber, PCP2114_OTP_CONFIG_GET pCP2114ConfigStruct);

// CP2114_CreateOtpConfig
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_CreateOtpConfig(_In_ _Pre_defensive_ HID_UART_DEVICE device, WORD configBufferLength, BYTE* pConfigBuffer);

// CP2114_SetBootConfig
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_SetBootConfig(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE cp2114ConfigNumber);

// CP2114_ReadOTP
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_ReadOTP(_In_ _Pre_defensive_ HID_UART_DEVICE device, UINT cp2114Address ,BYTE* pReadBuffer, UINT ReadLength);

// CP2114_WriteOTP
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_WriteOTP(_In_ _Pre_defensive_ HID_UART_DEVICE device, UINT cp2114Address ,BYTE* pWriteBuffer, UINT writeLength);

// CP2114_I2cWriteData
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_I2cWriteData(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE slaveAddress, BYTE* pWriteBuffer, BYTE writeLength);

// CP2114_I2cReadData
HID_TO_UART_API HID_UART_STATUS WINAPI
CP2114_I2cReadData(_In_ _Pre_defensive_ HID_UART_DEVICE device, BYTE slaveAddress, BYTE* pWriteBuffer, BYTE WriteLength, BYTE* pReadBuffer, BYTE ReadLength);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SLAB_CP2114_H
