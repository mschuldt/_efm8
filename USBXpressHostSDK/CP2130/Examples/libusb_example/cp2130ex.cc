#include <iostream>
#include <libusb-1.0/libusb.h>

bool cp2130_libusb_gpio_example(libusb_device_handle* cp2130Handle);
bool cp2130_libusb_write_example(libusb_device_handle* cp2130Handle);
bool cp2130_libusb_read_example(libusb_device_handle* cp2130Handle);

int main(int argc, char**argv) {
    std::cout << "CP2130 LibUSB Example" << std::endl;

    libusb_context* context = NULL;
    libusb_device** deviceList = NULL;
    ssize_t deviceCount = 0;
    struct libusb_device_descriptor deviceDescriptor;
    libusb_device* device = NULL;
    libusb_device_handle* cp2130Handle = NULL;
    int kernelAttached = 0;
    
    // Initialize libusb
    if (libusb_init(&context) != 0)
        goto exit;
    
    // Search the connected devices to find and open a handle to the CP2130
    deviceCount = libusb_get_device_list(context, &deviceList);
    if (deviceCount <= 0)
        goto exit;
    for (int i = 0; i < deviceCount; i++)
    {
        if (libusb_get_device_descriptor(deviceList[i], &deviceDescriptor) == 0)
        {
            if ((deviceDescriptor.idVendor == 0x10C4) &&
                    (deviceDescriptor.idProduct == 0x87A0))
            {
                device = deviceList[i];
                break;
            }
        }
    }
    if (device == NULL)
    {
        std::cout << "ERROR: Device not found" << std::endl;
        goto exit;
    }
    
    // If a device is found, then open it
    if (libusb_open(device, &cp2130Handle) != 0)
    {
        std::cout << "ERROR: Could not open device" << std::endl;
        goto exit;
    }
    
    // See if a kernel driver is active already, if so detach it and store a
    // flag so we can reattach when we are done
    if (libusb_kernel_driver_active(cp2130Handle, 0) != 0)
    {
        libusb_detach_kernel_driver(cp2130Handle, 0); 
	kernelAttached = 1;
    }
 
    // Finally, claim the interface
    if (libusb_claim_interface(cp2130Handle, 0) != 0)
    {
        std::cout << "ERROR: Could not claim interface" << std::endl;
        goto exit;
    }
    
    // Example functions start here:
    if (cp2130_libusb_gpio_example(cp2130Handle) == false)
        goto exit;
    if (cp2130_libusb_write_example(cp2130Handle) == false)
        goto exit;
    // NOTE: this is disabled since we don't have anything that will return data
    //if (cp2130_libusb_read_example(cp2130Handle) == false)
    //    goto exit;

 exit:
    // Cleanup and deinitialize libusb
    if (cp2130Handle)
        libusb_release_interface(cp2130Handle, 0);
    if (kernelAttached)
        libusb_attach_kernel_driver(cp2130Handle, 0);
    if (cp2130Handle)
        libusb_close(cp2130Handle);
    if (deviceList)
        libusb_free_device_list(deviceList, 1);
    if (context)
        libusb_exit(context);
    
    return 0;
}

bool cp2130_libusb_gpio_example(libusb_device_handle* cp2130Handle)
{   
    // This example shows how to issue a control transfer over endpoint zero to
    // get the GPIO0.7 value, toggle the opposite value and get the value again
    unsigned char control_buf_in[2];
    unsigned char control_buf_out[4];
    int usbTimeout = 500;
    
    if (libusb_control_transfer(cp2130Handle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), usbTimeout) != sizeof(control_buf_in))
    {
        std::cout << "ERROR: Error in control transfer" << std::endl;
        return false;
    }
    std::cout << "Successfully read GPIO, GPIO0.7 = " << std::dec << ((control_buf_in[0] & 0x08) ? "1" : "0") << std::endl;
    control_buf_out[0] = ~control_buf_in[0] & 0x08;
    control_buf_out[1] = control_buf_in[1];
    control_buf_out[2] = 0x08;
    control_buf_out[3] = 0x00;
    if (libusb_control_transfer(cp2130Handle, 0x40, 0x21, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), usbTimeout) != sizeof(control_buf_out))
    {
        std::cout << "ERROR: Error in control transfer" << std::endl;
        return false;
    }
    std::cout << "Successfully set GPIO, GPIO0.7 = " << std::dec << ((control_buf_out[0] & 0x08) ? "1" : "0") << std::endl;
    if (libusb_control_transfer(cp2130Handle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), usbTimeout) != sizeof(control_buf_in))
    {
        std::cout << "ERROR: Error in control transfer" << std::endl;
        return false;
    }
    std::cout << "Successfully read GPIO again, GPIO0.7 = " << std::dec << ((control_buf_in[0] & 0x08) ? "1" : "0") << std::endl;
    return true;
}

bool cp2130_libusb_write_example(libusb_device_handle* cp2130Handle)
{   
    // This example shows how to issue a bulk write request to the SPI MOSI line
    unsigned char write_command_buf[14] = {
        0x00, 0x00, // Reserved
        0x01, // Write command
        0x00, // Reserved
        0x06, 0x00, 0x00, 0x00, // Write 6 bytes, little-endian
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55 // Test data, 6 bytes
    };
    int bytesWritten;
    int usbTimeout = 500;
    
    if (libusb_bulk_transfer(cp2130Handle, 0x01, write_command_buf, sizeof(write_command_buf), &bytesWritten, usbTimeout))
    {
        std::cout << "ERROR: Error in bulk transfer" << std::endl;
        return false;
    }
    std::cout << "Successfully wrote to SPI MOSI, number of bytes written = " << std::dec << bytesWritten << std::endl;
    return true;
}

bool cp2130_libusb_read_example(libusb_device_handle* cp2130Handle)
{
    // This example shows how to issue a bulk read request to the SPI MISO line
    unsigned char read_command_buf[14] = {
        0x00, 0x00, // Reserved
        0x00, // Read command
        0x00, // Reserved
        0x06, 0x00, 0x00, 0x00, // Read 6 bytes, little-endian
    };
    int bytesWritten;
    int bytesToRead;
    unsigned char read_input_buf[6];
    int bytesRead;
    int usbTimeout = 500;
    
    if (libusb_bulk_transfer(cp2130Handle, 0x01, read_command_buf, sizeof(read_command_buf), &bytesWritten, usbTimeout))
    {
        std::cout << "ERROR: Error in bulk transfer" << std::endl;
        return false;
    }
    if (bytesWritten != sizeof(read_command_buf))
    {
        std::cout << "ERROR: Error in bulk transfer write size" << std::endl;
        return false;
    }
    if (libusb_bulk_transfer(cp2130Handle, 0x01, read_input_buf, sizeof(read_input_buf), &bytesRead, usbTimeout))
    {
        std::cout << "ERROR: Error in bulk transfer" << std::endl;
        return false;
    }
    std::cout << "Successfully read from SPI MISO, number of bytes read = " << std::dec << bytesRead << std::endl; 
    return true;
}