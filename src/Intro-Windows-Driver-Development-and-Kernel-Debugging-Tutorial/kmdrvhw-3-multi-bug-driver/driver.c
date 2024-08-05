// For the "Intro to Windows Device Driver Development and Windows Kernel Debugger" tutorial.
// Tutorial video:  https://www.youtube.com/watch?v=5Zr_E3oJcxg
// Multi-bug driver: Can be used for all 3 bugs discussed:
//  * use-after-free
//  * null pointer dereference (read/write to address 0)
//  * read invalid non-paged address (i.e., driver goes endlessly past its valid allocation).

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

    if (!NT_SUCCESS(status))
        return status;

    try
    {
        switch (gWhatToDo)
        {
            case AttemptUseAfterFree:
            {
                char* pMem;
                pMem = (char*)ExAllocatePool2(POOL_FLAG_NON_PAGED, 1024, MY_TAG);
                if (!pMem)
                    return STATUS_INSUFFICIENT_RESOURCES;
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: pMem=%p\n", pMem));
                *pMem = 'A'; // OK while pMem is valid.
                ExFreePool(pMem); // Free the memory.
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: before invalid assignment.\n"));
                *pMem = 'B'; // Bad: use after free.
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: after invalid assignment.\n"));
                break;
            }

            case AttemptWriteToAddressZero:
            {
                char* p = 0;
                (*p) = 'Z';
                break;
            }

            case AttemptReadInvalidNonPagedAddress:
            {
                char* pMem;
                pMem = (char*)ExAllocatePool2(POOL_FLAG_NON_PAGED, 1024, MY_TAG);
                if (!pMem)
                    return STATUS_INSUFFICIENT_RESOURCES;
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: pMem=%p\n", pMem));
                *pMem = 'A'; // OK

                // Find invalid non-paged address...
                // Imagine this is a driver bug, where the driver is navigating its
                // own allocated block but erroneously performs operations beyond it.
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: looking for an invalid non-paged address.\n"));
                while (*pMem++ != 256);

                // Never reached given endless loop above which should touch an
                // invalid non-paged address which then leads to a Bug Check 0x50.
                ExFreePool(pMem);
                *pMem = 'B';
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: invalid assignment complete.\n"));
                break;
            }
        }
    } // end try block
    except(EXCEPTION_EXECUTE_HANDLER)
    {
        // IMPORTANT: This is for exercise-illustrative purposes only. Generally, never create
        // a "catch all" SEH handler, and never ignore exceptions without purpose.

        __debugbreak();
        status = GetExceptionCode();
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "kmdrvhw: exception detected: status=0X%08X\n", status));
        status = STATUS_SUCCESS;
    }
    
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "kmdrvhw: exiting AddDevice.\n"));

    return status;
}