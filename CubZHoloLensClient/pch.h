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

#include "TCPClient.h"
#include "TCPPacket.h"

#include <collection.h>
#include <ppltasks.h>

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
