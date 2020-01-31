#include <Windows.h>
#include <stdio.h>
#include <intrin.h>

enum nvmlReturn_t {
	NVML_SUCCESS,
	NVML_ERROR_UNINITIALIZED,
	NVML_ERROR_INVALID_ARGUMENT,
	NVML_ERROR_NOT_SUPPORTED,
	NVML_ERROR_NO_PERMISSION,
	NVML_ERROR_ALREADY_INITIALIZED,
	NVML_ERROR_NOT_FOUND,
	NVML_ERROR_INSUFFICIENT_SIZE,
	NVML_ERROR_INSUFFICIENT_POWER,
	NVML_ERROR_DRIVER_NOT_LOADED,
	NVML_ERROR_TIMEOUT,
	NVML_ERROR_IRQ_ISSUE,
	NVML_ERROR_LIBRARY_NOT_FOUND,
	NVML_ERROR_FUNCTION_NOT_FOUND,
	NVML_ERROR_CORRUPTED_INFOROM,
	NVML_ERROR_GPU_IS_LOST,
	NVML_ERROR_RESET_REQUIRED,
	NVML_ERROR_OPERATING_SYSTEM,
	NVML_ERROR_LIB_RM_VERSION_MISMATCH,
	NVML_ERROR_IN_USE,
	NVML_ERROR_MEMORY,
	NVML_ERROR_NO_DATA,
	NVML_ERROR_VGPU_ECC_NOT_SUPPORTED,
	NVML_ERROR_UNKNOWN = 999
};

typedef nvmlReturn_t(*t_nvmlInit)();
typedef nvmlReturn_t(*t_nvmlShutdown)();
typedef nvmlReturn_t(*t_nvmlDeviceGetHandleByIndex)(unsigned int index, void* device);
typedef nvmlReturn_t(*t_nvmlDeviceGetCount)(unsigned int* deviceCount);
typedef nvmlReturn_t(*t_nvmlDeviceGetName)(void* device, char* name, unsigned int length);
typedef nvmlReturn_t(*t_nvmlDeviceGetUUID)(void* device, char* uuid, unsigned int length);

int main()
{
	HMODULE nvml = LoadLibraryA("nvml.dll");
	if (nvml != nullptr)
	{
		t_nvmlInit nvmlInit = (t_nvmlInit)GetProcAddress(nvml, "nvmlInit_v2");
		t_nvmlShutdown nvmlShutdown = (t_nvmlShutdown)GetProcAddress(nvml, "nvmlShutdown");
		t_nvmlDeviceGetHandleByIndex nvmlDeviceGetHandleByIndex = (t_nvmlDeviceGetHandleByIndex)GetProcAddress(nvml, "nvmlDeviceGetHandleByIndex_v2");
		t_nvmlDeviceGetCount nvmlDeviceGetCount = (t_nvmlDeviceGetCount)GetProcAddress(nvml, "nvmlDeviceGetCount_v2");
		t_nvmlDeviceGetName nvmlDeviceGetName = (t_nvmlDeviceGetName)GetProcAddress(nvml, "nvmlDeviceGetName");
		t_nvmlDeviceGetUUID nvmlDeviceGetUUID = (t_nvmlDeviceGetUUID)GetProcAddress(nvml, "nvmlDeviceGetUUID");

		printf("nvml: 0x%p\n", nvml);
		printf("nvmlInit: 0x%p\n", nvmlInit);
		printf("nvmlShutdown: 0x%p\n", nvmlShutdown);
		printf("nvmlDeviceGetHandleByIndex: 0x%p\n", nvmlDeviceGetHandleByIndex);
		printf("nvmlDeviceGetCount: 0x%p\n", nvmlDeviceGetCount);
		printf("nvmlDeviceGetName: 0x%p\n", nvmlDeviceGetName);
		printf("nvmlDeviceGetUUID: 0x%p\n", nvmlDeviceGetUUID);

		if (nvmlInit() == 0)
		{
			printf("\nInitialized successfully!\n");

			unsigned int device_count = 0;
			nvmlDeviceGetCount(&device_count);
			printf("Found: %i GPU!\nLooping now ...\n\n", device_count);

			for (int i = 0; i < device_count; ++i)
			{
				printf("Device ID: %i\n", i);

				void* device_handle = nullptr;
				nvmlDeviceGetHandleByIndex(i, &device_handle);
				printf("Device Handle: 0x%p\n", device_handle);

				char buffer[0x50] = {};
				nvmlDeviceGetName(device_handle, buffer, 0x50);
				printf("Device Name: %s\n", buffer);

				memset(buffer, 0, sizeof(buffer));
				nvmlDeviceGetUUID(device_handle, buffer, 0x50);
				printf("Device UUID: %s\n", buffer);
			}

			printf("\nExiting loop ... \n\n");
			nvmlShutdown();
		}
		else
		{
			printf("Failed initializing nvml ...\n");
		}

		FreeLibrary(nvml);
	}

	system("pause");

	return EXIT_SUCCESS;
}
