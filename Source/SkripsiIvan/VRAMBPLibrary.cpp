#include "VRAMBPLibrary.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <dxgi1_4.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif


static bool PickBestAdapter(IDXGIFactory1* Factory, IDXGIAdapter1** OutAdapter)
{
    *OutAdapter = nullptr;

#if PLATFORM_WINDOWS
    SIZE_T BestVRAM = 0;
    IDXGIAdapter1* Best = nullptr;

    for (UINT i = 0;; ++i)
    {
        IDXGIAdapter1* Adapter = nullptr;
        if (Factory->EnumAdapters1(i, &Adapter) == DXGI_ERROR_NOT_FOUND)
            break;

        DXGI_ADAPTER_DESC1 Desc;
        Adapter->GetDesc1(&Desc);

        const bool bIsSoftware = (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
        if (!bIsSoftware && Desc.DedicatedVideoMemory > BestVRAM)
        {
            if (Best) Best->Release();
            Best = Adapter;
            BestVRAM = Desc.DedicatedVideoMemory;
        }
        else
        {
            Adapter->Release();
        }
    }

    if (Best)
    {
        *OutAdapter = Best;
        return true;
    }
#endif

    return false;
}

bool UVRAMBPLibrary::GetVRAMInfoMB(int32& TotalVRAM_MB, int32& UsedVRAM_MB, int32& BudgetVRAM_MB)
{
    TotalVRAM_MB = 0;
    UsedVRAM_MB = 0;
    BudgetVRAM_MB = 0;

#if PLATFORM_WINDOWS
    IDXGIFactory1* Factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&Factory))) || !Factory)
        return false;

    IDXGIAdapter1* Adapter = nullptr;
    if (!PickBestAdapter(Factory, &Adapter) || !Adapter)
    {
        Factory->Release();
        return false;
    }

    DXGI_ADAPTER_DESC1 Desc;
    Adapter->GetDesc1(&Desc);
    TotalVRAM_MB = int32(Desc.DedicatedVideoMemory / (1024ull * 1024ull));

    IDXGIAdapter3* Adapter3 = nullptr;
    if (FAILED(Adapter->QueryInterface(IID_PPV_ARGS(&Adapter3))) || !Adapter3)
    {
        Adapter->Release();
        Factory->Release();
        return false;
    }

    DXGI_QUERY_VIDEO_MEMORY_INFO Info = {};
    if (FAILED(Adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &Info)))
    {
        Adapter3->Release();
        Adapter->Release();
        Factory->Release();
        return false;
    }

    UsedVRAM_MB = int32(Info.CurrentUsage / (1024ull * 1024ull));
    BudgetVRAM_MB = int32(Info.Budget / (1024ull * 1024ull));

    Adapter3->Release();
    Adapter->Release();
    Factory->Release();
    return true;
#else
    return false;
#endif
}
