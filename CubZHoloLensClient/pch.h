//
// pch.h
// Header for standard system include files.
//

#pragma once

//Disable the generation of a XAML main file
#define  DISABLE_XAML_GENERATED_MAIN

#define TRACE(x) \
			{std::stringstream s;  s << x;     \
            ::OutputDebugStringA(s.str().c_str());}

#include <libnetwork\TCPClient.h>
#include <libnetwork\TCPPacket.h>

#include "Utility\Utility.h"

#include <collection.h>
#include <ppltasks.h>

//DirectX TK
//https://github.com/Microsoft/DirectXTK
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

//Include for HoloLens
#include <ppltasks.h>
#include <chrono>
#include <agile.h>
#include <array>
#include <d2d1_2.h>
#include <d3d11_4.h>
#include <DirectXColors.h>
#include <dwrite_2.h>
#include <map>
#include <mutex>
#include <wincodec.h>
#include <WindowsNumerics.h>

#include "App.xaml.h"
