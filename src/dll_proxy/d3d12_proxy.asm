.data
extern pD3D12CoreCreateLayeredDevice : qword
extern pD3D12CoreGetLayeredDeviceSize : qword
extern pD3D12CoreRegisterLayers : qword
extern pD3D12CreateDevice : qword
extern pD3D12CreateRootSignatureDeserializer : qword
extern pD3D12CreateVersionedRootSignatureDeserializer : qword
extern pD3D12DeviceRemovedExtendedData : qword
extern pD3D12EnableExperimentalFeatures : qword
extern pD3D12GetDebugInterface : qword
extern pD3D12GetInterface : qword
extern pD3D12PIXEventsReplaceBlock : qword
extern pD3D12PIXGetThreadInfo : qword
extern pD3D12PIXNotifyWakeFromFenceSignal : qword
extern pD3D12PIXReportCounter : qword
extern pD3D12SerializeRootSignature : qword
extern pD3D12SerializeVersionedRootSignature : qword
extern pGetBehaviorValue : qword
extern pSetAppCompatStringPointer : qword

.code

fD3D12CoreCreateLayeredDevice proc
	jmp qword ptr [pD3D12CoreCreateLayeredDevice]
fD3D12CoreCreateLayeredDevice endp

fD3D12CoreGetLayeredDeviceSize proc
	jmp qword ptr [pD3D12CoreGetLayeredDeviceSize]
fD3D12CoreGetLayeredDeviceSize endp

fD3D12CoreRegisterLayers proc
	jmp qword ptr [pD3D12CoreRegisterLayers]
fD3D12CoreRegisterLayers endp

fD3D12CreateDevice proc
	jmp qword ptr [pD3D12CreateDevice]
fD3D12CreateDevice endp

fD3D12CreateRootSignatureDeserializer proc
	jmp qword ptr [pD3D12CreateRootSignatureDeserializer]
fD3D12CreateRootSignatureDeserializer endp

fD3D12CreateVersionedRootSignatureDeserializer proc
	jmp qword ptr [pD3D12CreateVersionedRootSignatureDeserializer]
fD3D12CreateVersionedRootSignatureDeserializer endp

fD3D12DeviceRemovedExtendedData proc
	jmp qword ptr [pD3D12DeviceRemovedExtendedData]
fD3D12DeviceRemovedExtendedData endp

fD3D12EnableExperimentalFeatures proc
	jmp qword ptr [pD3D12EnableExperimentalFeatures]
fD3D12EnableExperimentalFeatures endp

fD3D12GetDebugInterface proc
	jmp qword ptr [pD3D12GetDebugInterface]
fD3D12GetDebugInterface endp

fD3D12GetInterface proc
	jmp qword ptr [pD3D12GetInterface]
fD3D12GetInterface endp

fD3D12PIXEventsReplaceBlock proc
	jmp qword ptr [pD3D12PIXEventsReplaceBlock]
fD3D12PIXEventsReplaceBlock endp

fD3D12PIXGetThreadInfo proc
	jmp qword ptr [pD3D12PIXGetThreadInfo]
fD3D12PIXGetThreadInfo endp

fD3D12PIXNotifyWakeFromFenceSignal proc
	jmp qword ptr [pD3D12PIXNotifyWakeFromFenceSignal]
fD3D12PIXNotifyWakeFromFenceSignal endp

fD3D12PIXReportCounter proc
	jmp qword ptr [pD3D12PIXReportCounter]
fD3D12PIXReportCounter endp

fD3D12SerializeRootSignature proc
	jmp qword ptr [pD3D12SerializeRootSignature]
fD3D12SerializeRootSignature endp

fD3D12SerializeVersionedRootSignature proc
	jmp qword ptr [pD3D12SerializeVersionedRootSignature]
fD3D12SerializeVersionedRootSignature endp

fGetBehaviorValue proc
	jmp qword ptr [pGetBehaviorValue]
fGetBehaviorValue endp

fSetAppCompatStringPointer proc
	jmp qword ptr [pSetAppCompatStringPointer]
fSetAppCompatStringPointer endp

end
