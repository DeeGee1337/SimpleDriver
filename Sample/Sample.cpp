#include <ntddk.h>

#define DRIVER_TAG '7331'

//Strings Global
UNICODE_STRING g_RegistryPath;

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	//Free all located memory
	ExFreePool(g_RegistryPath.Buffer);
	KdPrint(("[DEBUG] DeeGee Sample - [FREE MEMORY POOL PAGED] g_RegistryPath.Buffer\n"));

	//KdPrint only in debug builds
	KdPrint(("[DEBUG] DeeGee Sample - driver unload called\n"));
}

//Main func = DriverEntry

extern "C" //needed for C-Linker Error
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	//Remove "threat warnings as errors" with macro
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = SampleUnload;
	KdPrint(("[DEBUG] DeeGee Sample - driver initialized successfully\n"));

	//Get windows version
	RTL_OSVERSIONINFOW info = { sizeof(info) };
	RtlGetVersion(&info);
	KdPrint(("[DEBUG] DeeGee Sample - Windows Version: %d.%d.%d\n", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber));

	//Allocate memory and string copying for registry path
	g_RegistryPath.Buffer = (WCHAR*)ExAllocatePoolWithTag(PagedPool, RegistryPath->Length, DRIVER_TAG);

	if (g_RegistryPath.Buffer == nullptr)
	{
		KdPrint(("[DEBUG] DeeGee Sample - Failed to allocate memory\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	KdPrint(("[DEBUG] DeeGee Sample - [ALLOCATED MEMORY POOL PAGED] g_RegistryPath.Buffer\n"));

	g_RegistryPath.MaximumLength = RegistryPath->Length;
	RtlCopyUnicodeString(&g_RegistryPath, (PCUNICODE_STRING)RegistryPath);

	//zW for UNICODE_STRING objects
	KdPrint(("[DEBUG] DeeGee Sample - Copied registry path: %wZ\n", &g_RegistryPath));

	return STATUS_SUCCESS;
}