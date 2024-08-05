// For the "Intro to Windows Device Driver Development and Windows Kernel Debugger" tutorial.
// Tutorial video:  https://www.youtube.com/watch?v=5Zr_E3oJcxg
// Initial driver.

#include <ntddk.h>
#include <wdf.h>

#define MY_TAG 'wleh'

typedef enum {
    Nothing,
    AttemptUseAfterFree,
    AttemptWriteToAddressZero,
    AttemptReadInvalidNonPagedAddress
} WHAT_TO_DO;

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd;
 
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT     DriverObject,
    _In_ PUNICODE_STRING    RegistryPath
)
{
    // NTSTATUS variable to record success or failure
    NTSTATUS status = STATUS_SUCCESS;

    // Allocate the driver configuration object
    WDF_DRIVER_CONFIG config;

    // Print "Hello World" for DriverEntry
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: DriverEntry\n"));

    // Initialize the driver configuration object to register the
    // entry point for the EvtDeviceAdd callback, KmdfHelloWorldEvtDeviceAdd
    WDF_DRIVER_CONFIG_INIT(&config,
        KmdfHelloWorldEvtDeviceAdd
    );

    // Finally, create the driver object
    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &config,
        WDF_NO_HANDLE
    );
    return status;
}

WHAT_TO_DO gWhatToDo = Nothing;
 
NTSTATUS
KmdfHelloWorldEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    // We're not using the driver object,
    // so we need to mark it as unreferenced
    UNREFERENCED_PARAMETER(Driver);

    NTSTATUS status;

    // Allocate the device object
    WDFDEVICE hDevice;

    // Print "Hello World"
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: KmdfHelloWorldEvtDeviceAdd\n"));

    // Create the device object
    status = WdfDeviceCreate(&DeviceInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &hDevice
    );

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: exiting AddDevice.\n"));

    return status;
}