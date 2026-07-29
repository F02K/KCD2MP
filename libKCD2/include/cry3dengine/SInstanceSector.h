#pragma once
#include <cstdint>

// -----------------------------------------------
// SInstanceSector -- compiled merged-mesh sector record, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Element of CMergedMeshesManager's DynArray at +0xC080, stride 0x10 (GetCompiledSector:
// `shl rax,4` @0x181AADCB0). Built by CompileSectors (0x1834E007C): the data DynArray is
// grown from the empty sentinel 0x185665CC0 and the id string from the shared CryStringT
// empty singleton (sub_1804FD80C()+12). Matches the stock IMergedMeshesManager::SInstanceSector
// shape {DynArray<uint8> data; string id;}.

struct SInstanceSector
{
    DynArray<uint8_t> data;    // +0x00  compiled instance stream
    CryStringT<char>  id;      // +0x08  unique sector id
};
static_assert(sizeof(SInstanceSector) == 0x10, "stride proven by GetCompiledSector (shl rax,4)");
