#!/usr/bin/env python2


################################################################################
## Copyright (c) 2015-2016 by Silicon Laboratories Inc.  All rights reserved.
## The program contained in this listing is proprietary to Silicon Laboratories,
## headquartered in Austin, Texas, U.S.A. and is subject to worldwide copyright
## protection, including protection under the United States Copyright Act of 1976
## as an unpublished work, pursuant to Section 104 and Section 408 of Title XVII
## of the United States code.  Unauthorized copying, adaptation, distribution,
## use, or display is prohibited by this law.
################################################################################

# Python 3.4

import ctypes
import sys

#-------------------------------------------------------------------------------
if sys.platform == 'win32':
    g_DLL = ctypes.windll.LoadLibrary("SLAB_USB_SPI.dll")
    getattr(g_DLL, "CP213x_OpenByIndex").restype = ctypes.c_int
elif sys.platform.startswith('linux'):
    g_DLL = ctypes.cdll.LoadLibrary("./libslab_usb_spi.so.1.0")
    getattr(g_DLL, "CP213x_Open").restype = ctypes.c_int
elif sys.platform == 'darwin':
    g_DLL = ctypes.cdll.LoadLibrary("libSLAB_USB_SPI.dylib")
    getattr(g_DLL, "CP213x_Open").restype = ctypes.c_int

for cp210x_function in [
    "CP213x_GetNumDevices",
    "CP213x_GetLibraryVersion",
    "CP213x_GetDeviceVersion",
    "CP213x_SetChipSelect",
    "CP213x_TransferWrite",
    "CP213x_TransferWriteRead",
    "CP213x_SetSpiControlByte",
    "CP213x_GetGpioModeAndLevel",
    "CP213x_SetGpioModeAndLevel",
    "CP213x_SetEventCounter",
    "CP213x_GetEventCounter",
    "CP213x_SetFifoFullThreshold",
    "CP213x_SetGpioValues",
    "CP213x_TransferReadSync",
    "CP213x_SetSpiDelay",
    "CP213x_Close",
#    "CP213x_GetOpenedVidPid", replace with GetUsbConfig
    "CP213x_GetProductString",
    "CP213x_GetSerialString"]:
    fnc = getattr(g_DLL, cp210x_function)
    fnc.restype = ctypes.c_int

#-------------------------------------------------------------------------------
# Constant definitions copied from the public DLL header
STRING_DESCRIPTOR_SIZE = 256
# XXX GetProductString() function flags
CP210x_RETURN_SERIAL_NUMBER = 0
CP210x_RETURN_DESCRIPTION   = 1
CP210x_RETURN_FULL_PATH     = 2

# CP2130 General Errors
USB_SPI_ERRCODE_SUCCESS             = 0x00
USB_SPI_ERRCODE_DEVICE_NOT_FOUND    = 0x20
USB_SPI_ERROR_FUNCTION_NOT_SUPPORTED= 0xFF

# API Errors
USB_SPI_ERRCODE_INVALID_PARAMETER     = 0x10
USB_SPI_ERRCODE_INVALID_DEVICE_OBJECT = 0x11

# Device Hardware Interface Errors
USB_SPI_ERRCODE_HWIF_DEVICE_ERROR = 0x30

#-------------------------------------------------------------------------------
# This class wraps SLAB_USB_SPI.dll. Class methods have same names as DLL functions.
# Every DLL function and its wrapper return USB_SPI_STATUS
# The parameters are the same unless the DLL declaration is given in a comment above wrapper to show difference.
# Also, the device file handle is stored in the object, so wrappers don't have it as the first parameter.
#
# See Function prototypes in SLAB_USB_SPI.h and SLAB_USB_SPI.chm for functional description.
#
# Strings returned from SLAB_USB_SPI.dll functions can be as large as STRING_DESCRIPTOR_SIZE chars
# plus 0 terminator. That much pre-allocated storage must be passed to them, there is no way to pass less.

class SLAB_USB_SPI:
    """Every DLL function returns USB_SPI_STATUS"""

    def __init__(self):
        self.hDev = ctypes.c_void_p(0)

    def CP213x_GetNumDevices(self):
        NumDevices = ctypes.c_ulong(0)
        if sys.platform == 'win32':
            err = g_DLL.CP213x_GetNumDevices(ctypes.byref(NumDevices));
        else:
            err = g_DLL.CP213x_GetNumDevices(ctypes.byref(NumDevices), 0x10C4, 0x87A0);
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetNumDevices err 0x%x" % err)
        return err, NumDevices.value

    def CP213x_Open(self, deviceIndex):
        try:
            if sys.platform == 'win32':
                err = g_DLL.CP213x_OpenByIndex(deviceIndex, ctypes.byref(self.hDev))
            else:
                err = g_DLL.CP213x_Open(deviceIndex, ctypes.byref(self.hDev), 0x10C4, 0x87A0)
        except:
            err = USB_SPI_ERROR_FUNCTION_NOT_SUPPORTED
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_Open err 0x%x" % err)
        return err

    def CP213x_Close(self):
        err = g_DLL.CP213x_Close(self.hDev)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_Close err 0x%x" % err)
        return err

    # CP213x_GetProductString( LPSTR productString, BYTE* strlen );
    def CP213x_GetProductString(self):
        cnt = ctypes.c_ubyte(0)
        buf = ctypes.create_string_buffer( STRING_DESCRIPTOR_SIZE)
        err = g_DLL.CP213x_GetProductString(self.hDev, buf, ctypes.byref(cnt))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP210x_GetProductString err %x" % err)
        return err, buf.value.decode()

    # CP213x_GetSerialString ( LPSTR serialString, BYTE* strlen );
    def CP213x_GetSerialString(self):
        cnt = ctypes.c_byte(0)
        buf = ctypes.create_string_buffer( STRING_DESCRIPTOR_SIZE)
        err = g_DLL.CP213x_GetSerialString(self.hDev, buf, ctypes.byref(cnt))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetSerialString err %x" % err)
        return err, buf.value.decode()

#    def CP213x_GetOpenedVidPid(self):
#        vid = ctypes.c_ushort(0)
#        pid = ctypes.c_ushort(0)
#        try:
#            err = g_DLL.CP213x_GetOpenedVidPid(self.hDev, ctypes.byref(vid), ctypes.byref(pid))
#        except:
#            err = USB_SPI_ERROR_FUNCTION_NOT_SUPPORTED
#        if err != USB_SPI_ERRCODE_SUCCESS :
#            print("CP213x_GetOpenedVidPid err %x" % err)
#        return err, vid.value, pid.value

    def CP213x_GetLibraryVersion(self):
        MajVer  = ctypes.c_ubyte(0)
        MinVer  = ctypes.c_ubyte(0)
        Release = ctypes.c_ulong(0)
        err = g_DLL.CP213x_GetLibraryVersion(ctypes.byref(MajVer), ctypes.byref(MinVer), ctypes.byref(Release))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetLibraryVersion err 0x%x" % err)
        return err, MajVer.value, MinVer.value, Release.value

    def CP213x_GetDeviceVersion(self):
        MajVer  = ctypes.c_ubyte(0)
        MinVer  = ctypes.c_ubyte(0)
        err = g_DLL.CP213x_GetDeviceVersion(self.hDev, ctypes.byref(MajVer), ctypes.byref(MinVer))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetDeviceVersion err %x" % err)
        return err, MajVer.value, MinVer.value

    def CP213x_SetChipSelect(self, channel, mode ):
        err = g_DLL.CP213x_SetChipSelect(self.hDev, channel, mode)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetChipSelect err %x" % err)
        return err

    def CP213x_TransferWrite(self, pWriteBuf, length, releaseBusAfterTransfer, timeoutMs, pBytesActuallyWritten ):
        err = g_DLL.CP213x_TransferWrite(self.hDev, pWriteBuf, length, releaseBusAfterTransfer, timeoutMs, ctypes.byref(pBytesActuallyWritten))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_TransferWrite err %x" % err)
        return err

    def CP213x_TransferWriteRead(self, pWriteBuf, pReadBuf, length, releaseBusAfterTransfer, timeoutMs):
        BytesActuallyTransferred = ctypes.c_ulong(0)
        err = g_DLL.CP213x_TransferWriteRead(self.hDev, pWriteBuf, pReadBuf, length, releaseBusAfterTransfer, timeoutMs, ctypes.byref( BytesActuallyTransferred))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_TransferWriteRead err %x" % err)
        return err, BytesActuallyTransferred.value

    def CP213x_SetSpiControlByte(self, channel, controlByte ):
        err = g_DLL.CP213x_SetSpiControlByte(self.hDev, channel, controlByte)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetSpiControlByte err %x" % err)
        return err

    def CP213x_GetGpioModeAndLevel(self, channel):
        mode  = ctypes.c_ubyte(0)
        level = ctypes.c_ubyte(0)
        err = g_DLL.CP213x_GetGpioModeAndLevel(self.hDev, channel, ctypes.byref(mode), ctypes.byref(level))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetGpioModeAndLevel err 0x%X" % err)
        return err, mode.value, level.value

    def CP213x_SetGpioModeAndLevel(self, channel, mode, level ):
        err = g_DLL.CP213x_SetGpioModeAndLevel(self.hDev, channel, mode, level)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetGpioModeAndLevel err %x" % err)
        return err

    def CP213x_SetEventCounter(self, mode, eventCount ):
        err = g_DLL.CP213x_SetEventCounter(self.hDev, mode, eventCount)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetEventCounter err %x" % err)
        return err

    def CP213x_GetEventCounter(self):
        mode  = ctypes.c_ubyte(0)
        count = ctypes.c_ushort(0)
        err = g_DLL.CP213x_GetEventCounter(self.hDev, ctypes.byref(mode), ctypes.byref(count))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_GetEventCounter err %x" % err)
        return err, mode.value, count.value

    def CP213x_SetFifoFullThreshold(self, fifoFullThreshold ):
        err = g_DLL.CP213x_SetFifoFullThreshold(self.hDev, fifoFullThreshold)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetFifoFullThreshold err %x" % err)
        return err

    def CP213x_SetGpioValues(self, mask, gpioValues ):
        err = g_DLL.CP213x_SetGpioValues(self.hDev, mask, gpioValues)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetGpioValues err %x" % err)
        return err

    def CP213x_TransferReadSync(self, pReadBuf, length, releaseBusAfterTransfer, timeoutMs, pBytesActuallyRead ):
        err = g_DLL.CP213x_TransferReadSync(self.hDev, pReadBuf, length, releaseBusAfterTransfer, timeoutMs, ctypes.byref(pBytesActuallyRead))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_TransferReadSync err %x" % err)
        return err

    def CP213x_TransferWrite(self, pWriteBuf, length, releaseBusAfterTransfer, timeoutMs, pBytesActuallyWritten ):
        err = g_DLL.CP213x_TransferWrite(self.hDev, pWriteBuf, length, releaseBusAfterTransfer, timeoutMs, ctypes.byref(pBytesActuallyWritten))
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_TransferWrite err %x" % err)
        return err

    def CP213x_SetSpiDelay(self, channel, delayMode, interByteDelay, postAssertDelay, preDeassertDelay):
        err = g_DLL.CP213x_SetSpiDelay(self.hDev, channel, delayMode, interByteDelay, postAssertDelay, preDeassertDelay)
        if err != USB_SPI_ERRCODE_SUCCESS :
            print("CP213x_SetSpiDelay err %x" % err)
        return err

#-------------------------------------------------------------------------------
# Functions yet to be wrapped

"""
CP213x_GetLibraryVersion        ( BYTE* major, BYTE* minor, BOOL* release );

// Device Management

CP213x_GetGuid                  ( LPGUID guid );
CP213x_SetGuid                  ( LPGUID guid );
CP213x_GetDevicePath            ( DWORD deviceIndex, LPSTR path );
CP213x_OpenByDevicePath         ( LPCSTR devicePath, CP213x_DEVICE* phDevice );
CP213x_GetVidPid                ( DWORD deviceIndex, WORD* vid, WORD* pid );

// following functions all have CP213x_DEVICE as parm1

CP213x_GetOpenedDevicePath      ( LPSTR path );
BOOL CP213x_IsOpened                 ( );
CP213x_Reset                    ( );
CP213x_GetDeviceVersion         ( BYTE* majorVersion, BYTE* minorVersion );
CP213x_GetDeviceDescriptor      ( PDEVICE_DESCRIPTOR pDescriptor );
CP213x_GetStringDescriptor      ( BYTE index, BYTE stringDescriptor[STRING_DESCRIPTOR_SIZE] );
CP213x_GetUsbConfig             ( WORD* vid, WORD* pid, BYTE* power, BYTE* powerMode, WORD* releaseVersion, BYTE* transferPriority );
CP213x_SetUsbConfig             ( WORD vid, WORD pid, BYTE power, BYTE powerMode, WORD releaseVersion, BYTE transferPriority, BYTE mask );
CP213x_GetManufacturingString   ( LPSTR manufacturingString, BYTE* strlen );
CP213x_SetManufacturingString   ( LPCSTR manufacturingString, BYTE strlen );
CP213x_SetProductString         ( LPCSTR productString, BYTE strlen );
CP213x_SetSerialString          ( LPCSTR serialString, BYTE strlen );
CP213x_GetPinConfig             ( BYTE pinConfig[SIZE_PIN_CONFIG] );
CP213x_SetPinConfig             ( BYTE pinConfig[SIZE_PIN_CONFIG] );
CP213x_GetLock                  ( WORD* lockValue );
CP213x_SetLock                  ( WORD lockValue );
CP213x_ReadProm                 ( BYTE pReadBuf[] );
CP213x_WriteProm                ( BYTE pWriteBuf[] );

// SPI Cfg and Transfers

CP213x_GetSpiControlBytes       ( BYTE controlBytes[CP213x_NUM_GPIO] );
CP213x_GetSpiDelay              ( BYTE channel, BYTE* delayMode, WORD* interByteDelay, WORD* postAssertDelay,  WORD* preDeassertDelay );
CP213x_GetChipSelect            ( WORD* channelCsEnable, WORD* pinCsEnable );
CP213x_TransferReadAsync        ( DWORD totalSize, DWORD blockSize, BOOL releaseBusAfterTransfer );
CP213x_TransferReadRtrAsync     ( DWORD totalSize, DWORD blockSize, BOOL releaseBusAfterTransfer );
CP213x_TransferReadRtrSync      ( BYTE pReadBuf[], DWORD totalSize, DWORD blockSize, BOOL releaseBusAfterTransfer,
                                  DWORD timeoutMs, DWORD* pBytesActuallyRead );
CP213x_GetRtrState              ( BYTE* isStopped );
CP213x_SetRtrStop               ( BYTE stopRtr );
CP213x_ReadPoll                 ( BYTE pReadBuf[], DWORD maxLength, DWORD* pBytesActuallyRead );
CP213x_ReadAbort                ( );
CP213x_AbortInputPipe           ( );
CP213x_FlushInputPipe           ( );
CP213x_AbortOutputPipe          ( );
CP213x_FlushOutputPipe          ( );
CP213x_GetFifoFullThreshold     ( BYTE* pFifoFullThreshold );

//GPIO and Aux-Func Pins

CP213x_GetGpioValues            ( WORD* gpioValues );
CP213x_GetClockDivider          ( BYTE* clockDivider );
CP213x_SetClockDivider          ( BYTE clockDivider );
"""

def PRINTV(*arg):
#    print(arg)
    pass

#------------------------------------------------------
# Open and dump info about device #DevIndex
def TestCP213x(device, DevIndex):
    status = device.CP213x_Open(DevIndex)
    if USB_SPI_ERRCODE_SUCCESS != status:
        return -1

#    (status, devVID, devPID) = device.CP213x_GetOpenedVidPid()
#    if USB_SPI_ERRCODE_SUCCESS == status:
#        PRINTV("devVID 0x%X devPID 0x%X" % (devVID, devPID))
#    elif USB_SPI_ERROR_FUNCTION_NOT_SUPPORTED != status:
#        return -1

    (status, MajVer, MinVer, Release) = device.CP213x_GetLibraryVersion()
    if status != USB_SPI_ERRCODE_SUCCESS:
        return -1
    PRINTV("LibraryVersion maj %d min %d IsRelease %d" % (MajVer, MinVer, Release))

    (status, MajVer, MinVer) = device.CP213x_GetDeviceVersion()
    if status != USB_SPI_ERRCODE_SUCCESS:
        return -1
    PRINTV("DeviceVersion maj %d min %d" % (MajVer, MinVer))

    (status, str) = device.CP213x_GetProductString()
    if status != USB_SPI_ERRCODE_SUCCESS :
        return -1
    PRINTV("ProductString: %s" % str)

    (status, str) = device.CP213x_GetSerialString()
    if status != USB_SPI_ERRCODE_SUCCESS :
        return -1
    PRINTV("SerialString: %s" % str)

    for channel in range(0, 43):
        (retval, mode, level) = device.CP213x_GetGpioModeAndLevel(channel)
        if retval != USB_SPI_ERRCODE_SUCCESS:
            # DLL used to succeed these calls for any GPIO index. It was a bug.
            # It doesn't anymore, so try and quit on the proper error
            if (USB_SPI_ERRCODE_INVALID_PARAMETER == retval and channel > 10):
                break
            return -1
    PRINTV("GetGpioModeAndLevel: upto channel #%d mode %d level %d" % (channel-1, mode, level))

    (status, mode, count) = device.CP213x_GetEventCounter()
    if status != USB_SPI_ERRCODE_SUCCESS:
        return -1
    PRINTV("GetEventCounter: event mode %d count %d" % (mode, count))

#    ReadBuf  = ctypes.create_string_buffer(256)
#    CbToRead = len(ReadBuf)
#    for i in range(0, CbToRead-1):
#        ReadBuf[i] = 0x33
#    CbRead = ctypes.c_ulong(0)
#    if device.CP213x_TransferReadSync(ReadBuf, CbToRead, False, 1000, CbRead ) != USB_SPI_ERRCODE_SUCCESS:
#        return -1
#    PRINTV("read %d bytes" % CbRead.value)

#    WriteBuf  = ctypes.create_string_buffer( 256)
#    CbToWrite = len( WriteBuf)
#    for i in range(0, CbToWrite-1):
#        WriteBuf[i] = 0x33
#    CbWritten = ctypes.c_ulong(0)
#    if device.CP213x_TransferWrite(WriteBuf, CbToWrite, False, 1000, CbWritten ) != USB_SPI_ERRCODE_SUCCESS:
#        return -1
#    PRINTV("written %d bytes" % CbWritten.value)

    if device.CP213x_Close() != USB_SPI_ERRCODE_SUCCESS:
        return -1

    return 0


def TestInvalDevIndex(device, NumDevices):
    status = device.CP213x_Open(NumDevices)
    # 2 errors documented in CP2130_SDK\Software\Library\Documentation\SLAB_USB_SPI.chm
    # However the func returns a different one - CP213XFW-58
    #if status not in (USB_SPI_ERRCODE_INVALID_DEVICE_OBJECT, USB_SPI_ERRCODE_INVALID_PARAMETER):
    if status != USB_SPI_ERRCODE_HWIF_DEVICE_ERROR and USB_SPI_ERRCODE_DEVICE_NOT_FOUND != status:
        print("TestInvalDevIndex: Unexpected error 0x%x\n" % status)
        return -1
    return 0


def TestAll():
    import sys
    errorlevel = 1
    lib = SLAB_USB_SPI()
    try:
        SuccessCnt = 0
        (status, NumDevices) = lib.CP213x_GetNumDevices()
        if status == USB_SPI_ERRCODE_SUCCESS:
            print("NumDevices = %d\n" % NumDevices)
            if TestInvalDevIndex(lib, NumDevices) == 0:
                for i in range(0, NumDevices):
                    if TestCP213x(lib, i) == 0:
                        SuccessCnt = SuccessCnt + 1
                if NumDevices == SuccessCnt:
                    errorlevel = 0 # let shell know that test PASSED
    except:
        print("SLAB_USB_SPI: TestAll: Unhandled exception")
    finally:
        if 0 == errorlevel: print("PASS")
        else: print("FAIL")
        sys.exit(errorlevel)

#------------------------------------------------------
# This main does no harm, just gets/dumps info about all found devices
# Its purpose is to call all non-destructive wrappers as unit-testing.
#
if __name__ == "__main__":
    TestAll()
