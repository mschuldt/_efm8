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
# Tests all found CP2110 devices
# Returns 0 to shell on success, non-zero on error

import os
from ComPortTestSuite import *
from SLABHIDtoUART import *

if __name__ == "__main__":
    import sys
   
    errorlevel = 1
    lib = HidUartDevice()
    try:
        SuccessCnt = 0
        NumDevices = GetNumDevices()

        if TestInvalDevIndex( NumDevices) == 0:
            for i in range(0, NumDevices):
                if TestSuite().Test( lib, i) == 0:
                    SuccessCnt = SuccessCnt + 1
            if NumDevices == SuccessCnt:
                errorlevel = 0 # let shell know that test PASSED
    except:
        print("SLABHIDtoUARTAPI_Test: Unhandled exception")
    finally:
        if errorlevel:
            print("FAIL\n")
        else:
            print("PASS\n")
        sys.exit(errorlevel)
