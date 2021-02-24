#pragma once

constexpr const char*	kWindowTitle = "D3D12";
constexpr unsigned int	kWindowWidth = 1280;
constexpr unsigned int	kWindowHeight = 720;
constexpr bool				kShowCursor = true;
constexpr unsigned int	kFrameBufferCount = 2;
constexpr unsigned int	kMaxDescriptorCountRTV = 100;
constexpr unsigned int	kMaxDescriptorCountDSV = 100;
constexpr unsigned int	kMaxDescriptorCountCBV_SRV_UAV = 2048;
constexpr float			kClearColor[] = { 0,0,0,1 };