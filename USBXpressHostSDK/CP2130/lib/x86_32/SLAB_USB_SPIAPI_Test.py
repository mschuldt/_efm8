#!/usr/bin/env python2

################################################################################
## Copyright (c) 2015 by Silicon Laboratories Inc.  All rights reserved.
## The program contained in this listing is proprietary to Silicon Laboratories,
## headquartered in Austin, Texas, U.S.A. and is subject to worldwide copyright
## protection, including protection under the United States Copyright Act of 1976
## as an unpublished work, pursuant to Section 104 and Section 408 of Title XVII
## of the United States code.  Unauthorized copying, adaptation, distribution,
## use, or display is prohibited by this law.
################################################################################

# Python 3.4

#-------------------------------------------------------------------------------
# USER GUIDE
# Tests all found CP213x devices
# Returns 0 to shell on success, non-zero on error

from SLAB_USB_SPI import *
from ComPortTestSuite import CByteGenerator

#------------------------------------------------------
def LoopbackTestCP213x(device, DevIndex):
    PRINTV("Connect")
    if device.CP213x_Open(DevIndex) != USB_SPI_ERRCODE_SUCCESS:
        return -1

    PRINTV("-- Loopback test")

    ReadByteGen   = CByteGenerator()
    WriteByteGen  = CByteGenerator()

    rc = 9

    # Write-read a chunk
    WriteBuf  = ctypes.create_string_buffer(256)
    CbToLoop = len(WriteBuf)
    for i in range(0, CbToLoop):
        WriteBuf[i] = WriteByteGen.Next()

    ReadBuf  = ctypes.create_string_buffer( CbToLoop)
    for i in range(0, CbToLoop):
        ReadBuf[ i] = 0
    
    (status, CbLooped) = device.CP213x_TransferWriteRead(WriteBuf, ReadBuf, CbToLoop, False, 500)
    if status == USB_SPI_ERRCODE_SUCCESS:
        # Verify read data
        if CbLooped == CbToLoop:
            PRINTV("verifying %d bytes" % CbLooped)
            for i in range(0, CbLooped):
                if ReadBuf.raw[ i] != ReadByteGen.Next():
                    print("data corrupt at index %x: %x" % (i, ReadBuf.raw[ i]))
                    rc = -1
                    break
                if ReadBuf.raw[i] != WriteBuf.raw[i]:
                    print("data corrupt at index %d: Wrote 0x%x, Read 0x%x" % (i, WriteBuf.raw[i], ReadBuf.raw[i]))
                    rc = -1
                    break
            else:
                PRINTV("verified %d bytes" % CbLooped)
                rc = 0
        else:
            print("not enough data read %d\n" % CbLooped)
            rc = -1
    else:
        rc = -1

    if device.CP213x_Close() != USB_SPI_ERRCODE_SUCCESS:
        return -1
    return rc

def LoopbackTestAll():
    import sys
    errorlevel = 1
    lib = SLAB_USB_SPI()
    try:
        SuccessCnt = 0
        (status, NumDevices) = lib.CP213x_GetNumDevices()
        if status == USB_SPI_ERRCODE_SUCCESS:
            if TestInvalDevIndex( lib, NumDevices) == 0:
                for i in range(0, NumDevices):
                    if LoopbackTestCP213x(lib, i) == 0:
                        SuccessCnt = SuccessCnt + 1
                if NumDevices == SuccessCnt:
                    errorlevel = 0 # let shell know that test PASSED
    except:
        print("SLAB_USB_SPIAPI_Test: Unhandled exception")
    finally:
        if 0 == errorlevel: print("PASS\n")
        else: print("FAIL\n")
        sys.exit(errorlevel)

if __name__ == "__main__":
    LoopbackTestAll()
