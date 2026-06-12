#include "Globals.h"

namespace
{
	uint8_t state_flags;
}

#ifdef _WIN64

#include <d3d11.h>
#include <dxgi1_3.h>

void OnDirect3D11DeviceCreated(ID3D11Device* device)
{
	if (state_flags & 2)
		return;

	state_flags |= 2;
}

VTABLE_HOOK(HRESULT, STDMETHODCALLTYPE, IDXGISwapChain, Present, 8, UINT SyncInterval, UINT Flags)
{
	g_loader->OnUpdate();
	return originalIDXGISwapChainPresent(This, SyncInterval, Flags);
}

void OnDXGISwapChainCreated(IDXGISwapChain* swapChain)
{
	if (swapChain)
	{
		INSTALL_VTABLE_HOOK(IDXGISwapChain, swapChain, Present);
	}
}

VTABLE_HOOK(HRESULT, STDMETHODCALLTYPE, IDXGIFactory, CreateSwapChain, 10, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	const auto result = originalIDXGIFactoryCreateSwapChain(This, pDevice, pDesc, ppSwapChain);
	if (SUCCEEDED(result) && ppSwapChain)
	{
		OnDXGISwapChainCreated(*ppSwapChain);
	}
	return result;
}

VTABLE_HOOK(HRESULT, STDMETHODCALLTYPE, IDXGIFactory2, CreateSwapChainForHwnd, 15, IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	const auto result = originalIDXGIFactory2CreateSwapChainForHwnd(This, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
	if (SUCCEEDED(result) && ppSwapChain)
	{
		OnDXGISwapChainCreated(*ppSwapChain);
	}
	return result;
}

void OnDXGIFactoryCreated(void* factory)
{
	if (state_flags & 1)
		return;

	state_flags |= 1;
	if (!factory)
	{
		LOG("CreateDXGIFactory2() returned nullptr");
		return;
	}

	INSTALL_VTABLE_HOOK(IDXGIFactory, factory, CreateSwapChain);
	INSTALL_VTABLE_HOOK(IDXGIFactory2, factory, CreateSwapChainForHwnd);
}

HOOK(HRESULT, WINAPI, D3D11CreateDeviceHook, PROC_ADDRESS("d3d11", "D3D11CreateDevice"), IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
	const auto result = originalD3D11CreateDeviceHook(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
	if (SUCCEEDED(result) && ppDevice)
	{
		OnDirect3D11DeviceCreated(*ppDevice);
	}
	return result;
}

HOOK(HRESULT, WINAPI, CreateDXGIFactory2Hook, PROC_ADDRESS("dxgi", "CreateDXGIFactory2"), UINT Flags, REFIID riid, void** ppFactory)
{
	const auto result = originalCreateDXGIFactory2Hook(Flags, riid, ppFactory);
	if (SUCCEEDED(result) && ppFactory)
	{
		OnDXGIFactoryCreated(*ppFactory);
	}
	return result;
}

void D3DHooks_Init()
{
	INSTALL_HOOK(D3D11CreateDeviceHook);
	INSTALL_HOOK(CreateDXGIFactory2Hook);
}

#else

#include <d3d9.h>

VTABLE_HOOK(HRESULT, WINAPI, IDirect3DDevice9, Present, 17, void* pSrcRect, void* pDestRect, HWND window, void* pDirtyRegion)
{
	g_loader->OnUpdate();
	return originalIDirect3DDevice9Present(This, pSrcRect, pDestRect, window, pDirtyRegion);
}

VTABLE_HOOK(HRESULT, WINAPI, IDirect3DDevice9Ex, PresentEx, 121, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	g_loader->OnUpdate();
	return originalIDirect3DDevice9ExPresentEx(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

void OnDirect3DDeviceCreated(IDirect3DDevice9* device, bool ex)
{
	if (state_flags & 2)
		return;

	state_flags |= 2;

	g_loader->update_info.device = device;
	INSTALL_VTABLE_HOOK(IDirect3DDevice9, device, Present);
	if (ex)
	{
		INSTALL_VTABLE_HOOK(IDirect3DDevice9Ex, device, PresentEx);
	}
}

VTABLE_HOOK(HRESULT, WINAPI, IDirect3D9, CreateDevice, 16, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	const auto result = originalIDirect3D9CreateDevice(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	if (*ppReturnedDeviceInterface)
	{
		OnDirect3DDeviceCreated(*ppReturnedDeviceInterface, false);
	}

	return result;
}

VTABLE_HOOK(HRESULT, WINAPI, IDirect3D9, CreateDeviceEx, 20, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
	const auto result = originalIDirect3D9CreateDeviceEx(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);

	if (*ppReturnedDeviceInterface)
	{
		OnDirect3DDeviceCreated(*ppReturnedDeviceInterface, true);
	}

	return result;
}

void OnDirect3DCreated(IDirect3D9* d3d, bool ex)
{
	if (state_flags & 1)
		return;

	state_flags |= 1;
	if (d3d == nullptr)
	{
		LOG("Direct3DCreate9() returned nullptr");
		return;
	}

	INSTALL_VTABLE_HOOK(IDirect3D9, d3d, CreateDevice);
	if (ex)
	{
		INSTALL_VTABLE_HOOK(IDirect3D9, d3d, CreateDeviceEx);
	}
}

HOOK(IDirect3D9*, WINAPI, Direct3DCreate9Hook, PROC_ADDRESS("d3d9", "Direct3DCreate9"), UINT SDK_VERSION)
{
	auto* d3d = originalDirect3DCreate9Hook(SDK_VERSION);
	OnDirect3DCreated(d3d, false);
	return d3d;
}

HOOK(HRESULT, WINAPI, Direct3DCreate9ExHook, PROC_ADDRESS("d3d9", "Direct3DCreate9Ex"), UINT SDK_VERSION, IDirect3D9Ex** ppD3d)
{
	const auto hr = originalDirect3DCreate9ExHook(SDK_VERSION, ppD3d);
	OnDirect3DCreated(*ppD3d, true);
	return hr;
}

void D3DHooks_Init()
{
	INSTALL_HOOK(Direct3DCreate9Hook);
	INSTALL_HOOK(Direct3DCreate9ExHook);
}

#endif
