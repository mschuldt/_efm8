/////////////////////////////////////////////////////////////////////////////
// SLABCP2110.h
// For SLABHIDtoUART.dll
// and Silicon Labs CP2110 HID to UART
/////////////////////////////////////////////////////////////////////////////
#ifndef SLAB_CP2110_H
#define SLAB_CP2110_H

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABHIDtoUART.h"
#include "silabs_sal.h"

/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

// Pin Config Mode Array Indices
#define CP2110_INDEX_GPIO_0_CLK			            0
#define CP2110_INDEX_GPIO_1_RTS			            1
#define CP2110_INDEX_GPIO_2_CTS			            2
#define CP2110_INDEX_GPIO_3_RS485			        3
#define CP2110_INDEX_GPIO_4_TX_TOGGLE		        4
#define CP2110_INDEX_GPIO_5_RX_TOGGLE		        5
#define CP2110_INDEX_GPIO_6				            6
#define CP2110_INDEX_GPIO_7				            7
#define CP2110_INDEX_GPIO_8				            8
#define CP2110_INDEX_GPIO_9				            9
#define CP2110_INDEX_TX					            10
#define CP2110_INDEX_SUSPEND				        11
#define CP2110_INDEX_SUSPEND_BAR			        12
// Size of the above array pointed to by pinConfig parameter in HidUart_SetPinConfig and HidUart_GetPinConfig
#define CP2110_PIN_CONFIG_SIZE					13

// Pin Bitmasks
#define CP2110_MASK_GPIO_0_CLK				        0x0001
#define CP2110_MASK_GPIO_1_RTS				        0x0002
#define CP2110_MASK_GPIO_2_CTS				        0x0004
#define CP2110_MASK_GPIO_3_RS485			        0x0008
#define CP2110_MASK_TX						        0x0010
#define CP2110_MASK_RX						        0x0020
#define CP2110_MASK_GPIO_4_TX_TOGGLE		        0x0040
#define CP2110_MASK_GPIO_5_RX_TOGGLE		        0x0080
#define CP2110_MASK_SUSPEND_BAR			            0x0100
// NA
#define CP2110_MASK_GPIO_6					        0x0400
#define CP2110_MASK_GPIO_7					        0x0800
#define CP2110_MASK_GPIO_8					        0x1000
#define CP2110_MASK_GPIO_9					        0x2000
#define CP2110_MASK_SUSPEND				            0x4000

/////////////////////////////////////////////////////////////////////////////
// Exported Library Functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// HidUart_SetPinConfig
/// @brief Sets the Pin Config to the device
/// @param device is an open handle to the device
/// @param pinConfig points at a buffer into which the port latch value will be written
/// @param useSuspendValues points at a buffer into which the port latch value will be written
/// @param suspendValue points at a buffer into which the port latch value will be written
/// @param suspendMode points at a buffer into which the port latch value will be written
/// @param rs485Level points at a buffer into which the port latch value will be written
/// @param clkDiv points at a buffer into which the port latch value will be written
/// @returns Returns HID_UART_SUCCESS on success, another HID_UART_STATUS if there is an error:
///			HID_UART_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_UART_DEVICE_NOT_SUPPORTED -- device is not a CP2110
///			HID_UART_INVALID_PARAMETER -- pinConfig is NULL
_Check_return_
_Ret_range_(HID_UART_SUCCESS, HID_UART_UNKNOWN_ERROR)
_Success_(return == HID_UART_SUCCESS)
HID_TO_UART_API HID_UART_STATUS WINAPI
HidUart_SetPinConfig(
	_In_ _Pre_defensive_ const HID_UART_DEVICE device,
	_In_reads_bytes_(CP2110_PIN_CONFIG_SIZE) _Pre_defensive_ BYTE* pinConfig,
	_In_ _Pre_defensive_ const BOOL useSuspendValues,
	_In_ _Pre_defensive_ const WORD suspendValue,
	_In_ _Pre_defensive_ const WORD suspendMode,
	_In_ _Pre_defensive_ const BYTE rs485Level,
	_In_ _Pre_defensive_ const BYTE clkDiv
);

// HidUart_GetPinConfig
/// @brief Reads and returns the Pin Config from the device
/// @param device is an open handle to the device
/// @param pinConfig points at a buffer into which the pin config will be written
/// @param useSuspendValues points at a BOOL into which the useSuspendValues value will be written
/// @param suspendValue points at a WORD into which the suspendValue value will be written
/// @param suspendMode points at a WORD into which the suspendMode value will be written
/// @param rs485Level points at a BYTE into which the rs485Level value will be written
/// @param clkDiv points at a BYTE into which the clkDiv value will be written
/// @returns Returns HID_UART_SUCCESS on success, another HID_UART_STATUS if there is an error:
///			HID_UART_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_UART_DEVICE_NOT_SUPPORTED -- device is not a CP2110
///			HID_UART_INVALID_PARAMETER -- pinConfig or useSuspendValues or suspendValue or suspendMode or rs485Level or clkDiv is NULL
_Check_return_
_Ret_range_(HID_UART_SUCCESS, HID_UART_UNKNOWN_ERROR)
_Success_(return == HID_UART_SUCCESS)
HID_TO_UART_API HID_UART_STATUS WINAPI
HidUart_GetPinConfig(
	_In_ _Pre_defensive_ const HID_UART_DEVICE device,
	_Out_writes_bytes_(CP2110_PIN_CONFIG_SIZE) _Pre_defensive_ BYTE* pinConfig,
	_Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* useSuspendValues,
	_Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* suspendValue,
	_Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* suspendMode,
	_Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* rs485Level,
	_Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* clkDiv
);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SLAB_CP2110_H
