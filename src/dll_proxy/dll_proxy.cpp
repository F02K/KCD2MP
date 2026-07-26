#include "dll_proxy.hpp"

#include <array>

#include <Windows.h>

extern "C"
{
	FARPROC pD3D12CoreCreateLayeredDevice{};
	FARPROC pD3D12CoreGetLayeredDeviceSize{};
	FARPROC pD3D12CoreRegisterLayers{};
	FARPROC pD3D12CreateDevice{};
	FARPROC pD3D12CreateRootSignatureDeserializer{};
	FARPROC pD3D12CreateVersionedRootSignatureDeserializer{};
	FARPROC pD3D12DeviceRemovedExtendedData{};
	FARPROC pD3D12EnableExperimentalFeatures{};
	FARPROC pD3D12GetDebugInterface{};
	FARPROC pD3D12GetInterface{};
	FARPROC pD3D12PIXEventsReplaceBlock{};
	FARPROC pD3D12PIXGetThreadInfo{};
	FARPROC pD3D12PIXNotifyWakeFromFenceSignal{};
	FARPROC pD3D12PIXReportCounter{};
	FARPROC pD3D12SerializeRootSignature{};
	FARPROC pD3D12SerializeVersionedRootSignature{};
	FARPROC pGetBehaviorValue{};
	FARPROC pSetAppCompatStringPointer{};
}

namespace
{
	HMODULE g_system_d3d12{};

	bool setup_functions()
	{
		struct export_binding
		{
			const char *name;
			FARPROC *target;
		};

		const std::array bindings{
		    export_binding{"D3D12CoreCreateLayeredDevice", &pD3D12CoreCreateLayeredDevice},
		    export_binding{"D3D12CoreGetLayeredDeviceSize", &pD3D12CoreGetLayeredDeviceSize},
		    export_binding{"D3D12CoreRegisterLayers", &pD3D12CoreRegisterLayers},
		    export_binding{"D3D12CreateDevice", &pD3D12CreateDevice},
		    export_binding{"D3D12CreateRootSignatureDeserializer", &pD3D12CreateRootSignatureDeserializer},
		    export_binding{"D3D12CreateVersionedRootSignatureDeserializer", &pD3D12CreateVersionedRootSignatureDeserializer},
		    export_binding{"D3D12DeviceRemovedExtendedData", &pD3D12DeviceRemovedExtendedData},
		    export_binding{"D3D12EnableExperimentalFeatures", &pD3D12EnableExperimentalFeatures},
		    export_binding{"D3D12GetDebugInterface", &pD3D12GetDebugInterface},
		    export_binding{"D3D12GetInterface", &pD3D12GetInterface},
		    export_binding{"D3D12PIXEventsReplaceBlock", &pD3D12PIXEventsReplaceBlock},
		    export_binding{"D3D12PIXGetThreadInfo", &pD3D12PIXGetThreadInfo},
		    export_binding{"D3D12PIXNotifyWakeFromFenceSignal", &pD3D12PIXNotifyWakeFromFenceSignal},
		    export_binding{"D3D12PIXReportCounter", &pD3D12PIXReportCounter},
		    export_binding{"D3D12SerializeRootSignature", &pD3D12SerializeRootSignature},
		    export_binding{"D3D12SerializeVersionedRootSignature", &pD3D12SerializeVersionedRootSignature},
		    export_binding{"GetBehaviorValue", &pGetBehaviorValue},
		    export_binding{"SetAppCompatStringPointer", &pSetAppCompatStringPointer},
		};

		for (const auto &binding : bindings)
		{
			*binding.target = GetProcAddress(g_system_d3d12, binding.name);
		}

		// These exports are required by KCD2 on all supported Windows versions.
		return pD3D12CreateDevice && pD3D12CreateRootSignatureDeserializer &&
		       pD3D12CreateVersionedRootSignatureDeserializer && pD3D12SerializeRootSignature &&
		       pD3D12SerializeVersionedRootSignature;
	}
}

namespace big
{
	bool dll_proxy::init()
	{
		WCHAR system_directory[MAX_PATH]{};
		const auto length = GetSystemDirectoryW(system_directory, MAX_PATH);
		if (!length || length >= MAX_PATH)
		{
			return false;
		}

		std::wstring path(system_directory, length);
		path += L"\\D3D12.dll";
		g_system_d3d12 = LoadLibraryW(path.c_str());
		return g_system_d3d12 && setup_functions();
	}
} // namespace big
