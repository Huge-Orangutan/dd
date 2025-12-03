#include <windows.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <iostream>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

int main() {
    HRESULT hr;

    std::cout << "DXGI Capture Test Starting...\n";

    // Create D3D11 device
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &device,
        nullptr,
        &context
    );

    if (FAILED(hr)) {
        std::cout << "Failed to create D3D11 device, hr=" << std::hex << hr << "\n";
        return -1;
    }

    IDXGIDevice* dxgiDevice = nullptr;
    hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if (FAILED(hr)) {
        std::cout << "Failed to get DXGI device, hr=" << std::hex << hr << "\n";
        return -2;
    }

    IDXGIAdapter* adapter = nullptr;
    hr = dxgiDevice->GetAdapter(&adapter);
    if (FAILED(hr)) {
        std::cout << "Failed to get adapter, hr=" << std::hex << hr << "\n";
        return -3;
    }

    IDXGIOutput* output = nullptr;
    hr = adapter->EnumOutputs(0, &output);
    if (FAILED(hr)) {
        std::cout << "Failed to get output, hr=" << std::hex << hr << "\n";
        return -4;
    }

    IDXGIOutput1* output1 = nullptr;
    hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
    if (FAILED(hr)) {
        std::cout << "Failed to get IDXGIOutput1, hr=" << std::hex << hr << "\n";
        return -5;
    }

    IDXGIOutputDuplication* duplication = nullptr;
    hr = output1->DuplicateOutput(device, &duplication);

    if (FAILED(hr)) {
        std::cout << "DuplicateOutput FAILED, hr=" << std::hex << hr << "\n";
        return -6;
    }

    std::cout << "DuplicateOutput SUCCESS\n";

    DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
    IDXGIResource* desktopResource = nullptr;

    hr = duplication->AcquireNextFrame(1000, &frameInfo, &desktopResource);

    if (FAILED(hr)) {
        std::cout << "AcquireNextFrame FAILED, hr=" << std::hex << hr << "\n";
        return -7;
    }

    std::cout << "AcquireNextFrame SUCCESS\n";

    duplication->ReleaseFrame();
    if (desktopResource) desktopResource->Release();
    if (duplication) duplication->Release();
    if (output1) output1->Release();
    if (output) output->Release();
    if (adapter) adapter->Release();
    if (dxgiDevice) dxgiDevice->Release();
    if (context) context->Release();
    if (device) device->Release();

    std::cout << "DXGI Test Finished.\n";
    return 0;
}
