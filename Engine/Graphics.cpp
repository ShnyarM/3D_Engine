﻿/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include "TexVertex.h"
#include <assert.h>
#include <string>
#include <array>
#include <functional>
#include <sstream>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
#include "TexVertex.h"
}

#pragma comment( lib,"d3d11.lib" )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
	:
	sysBuffer( ScreenWidth,ScreenHeight )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		&featureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		&featureLevelsSupported,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}
}

Graphics::~Graphics()
{
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// perform the copy line-by-line
	sysBuffer.Present( mappedSysBufferTexture.RowPitch,
		reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData) );
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
	}
}

void Graphics::BeginFrame()
{
	sysBuffer.Clear( Colors::Red );
}

//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}

void Graphics::DrawLine( float x1,float y1,float x2,float y2,Color c )
{
	const float dx = x2 - x1;
	const float dy = y2 - y1;

	if( dy == 0.0f && dx == 0.0f )
	{
		PutPixel( int( x1 ),int( y1 ),c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dx / dy;
		float y = y1;
		int lastIntY;
		for( float x = x1; y < y2; y += 1.0f,x += m )
		{
			lastIntY = int( y );
			PutPixel( int( x ),lastIntY,c );
		}
		if( int( y2 ) > lastIntY )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
	else
	{
		if( dx < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dy / dx;
		float x = x1;
		int lastIntX;
		for( float y = y1; x < x2; x += 1.0f,y += m )
		{
			lastIntX = int( x );
			PutPixel( lastIntX,int( y ),c );
		}
		if( int( x2 ) > lastIntX )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
}

void Graphics::DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	//use pointers wo we can sort them
	const Vec2* pv0 = &v0;
	const Vec2* pv1 = &v1;
	const Vec2* pv2 = &v2;

	//Sort so pv0 is at the top
	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	//Check if flat top
	if (pv0->y == pv1->y)
	{
		if (pv0->x > pv1->x) std::swap(pv0, pv1); //Sort so pv0 is left
		DrawFlatTopTriangle(*pv0, *pv1, *pv2, c);
		return;
	}

	//Check if flat bottom
	if (pv1->y == pv2->y)
	{
		if (pv2->x < pv1->x) std::swap(pv1, pv2); //Sort so pv1 is left
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
		return;
	}

	//Split into flat top and flat bottom triangle
	float alpha = (pv1->y - pv0->y) / (pv2->y - pv0->y);
	Vec2 vi = pv0->InterpolateTo(*pv2, alpha);

	if (vi.x > pv1->x) //major right
	{
		DrawFlatBottomTriangle(*pv0, *pv1, vi, c);
		DrawFlatTopTriangle(*pv1, vi, *pv2, c);
	}
	else //Major left
	{
		DrawFlatBottomTriangle(*pv0, vi, *pv1, c);
		DrawFlatTopTriangle(vi, *pv1, *pv2, c);
	}
}

void Graphics::DrawFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	//Calculate slopes
	float m0 = (v0.x - v2.x) / (v0.y - v2.y);
	float m1 = (v1.x - v2.x) / (v1.y - v2.y);

	// calculate start and end scanlines
	int startY = ceil(v0.y - 0.5f);
	int endY = ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = startY; y < endY; y++)
	{
		//Find left starting point
		float deltaY = (y + 0.5f) - v0.y;
		float deltaXLeft = m0 * deltaY;
		int startX = ceil((v0.x + deltaXLeft) - 0.5f);

		//Find right end point (not included)
		float deltaXRight = m1 * deltaY;
		int endX = ceil((v1.x + deltaXRight) - 0.5f);

		for (int x = startX; x < endX; x++)
		{
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	// calulcate slopes in screen space
	float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	//Find start and end point (end point not included)
	int startY = ceil(v0.y - 0.5f);
	int endY = ceil(v2.y - 0.5f);

	for (int y = startY; y < endY; y++)
	{
		float deltaY = y + 0.5f - v0.y;
		float deltaXLeft = m0 * deltaY;
		int startX = ceil((v0.x + deltaXLeft) - 0.5f);

		float deltaXRight = m1 * deltaY;
		int endX = ceil((v0.x + deltaXRight) - 0.5f);

		for (int x = startX; x < endX; x++)
		{
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& texture)
{
	//Start without wrap
	StartDrawTriangleTex(v0, v1, v2, texture, false);
}

void Graphics::DrawTriangleTexWrap(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& texture)
{
	//Start with wrap
	StartDrawTriangleTex(v0, v1, v2, texture, true);
}

void Graphics::StartDrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& texture, bool wrap)
{
	//use pointers wo we can sort them
	const TexVertex* pv0 = &v0;
	const TexVertex* pv1 = &v1;
	const TexVertex* pv2 = &v2;

	//Sort so pv0 is at the top
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
	if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

	//Check if flat top
	if (pv0->pos.y == pv1->pos.y)
	{
		if (pv0->pos.x > pv1->pos.x) std::swap(pv0, pv1); //Sort so pv0 is left
		DrawFlatTopTriangleTex(*pv0, *pv1, *pv2, texture, wrap);
		return;
	}

	//Check if flat bottom
	if (pv1->pos.y == pv2->pos.y)
	{
		if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2); //Sort so pv1 is left
		DrawFlatBottomTriangleTex(*pv0, *pv1, *pv2, texture, wrap);
		return;
	}

	//Split into flat top and flat bottom triangle
	float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
	TexVertex vi = pv0->InterpolateTo(*pv2, alpha);

	if (vi.pos.x > pv1->pos.x) //major right
	{
		DrawFlatBottomTriangleTex(*pv0, *pv1, vi, texture, wrap);
		DrawFlatTopTriangleTex(*pv1, vi, *pv2, texture, wrap);
	}
	else //Major left
	{
		DrawFlatBottomTriangleTex(*pv0, vi, *pv1, texture, wrap);
		DrawFlatTopTriangleTex(vi, *pv1, *pv2, texture, wrap);
	}
}

void Graphics::DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& texture, bool wrap)
{
	//Calculate slopes
	TexVertex tDelta0 = (v2 - v0) / (v2.pos.y - v0.pos.y);
	TexVertex tDelta1 = (v2 - v1) / (v2.pos.y - v1.pos.y);

	// calculate start and end scanlines
	int startY = ceil(v0.pos.y - 0.5f);
	int endY = ceil(v2.pos.y - 0.5f); // the scanline AFTER the last line drawn

	//Start function with given information
	DrawFlatTriangleTex(startY, endY, tDelta0, tDelta1, v0, v1, texture, wrap);
}

void Graphics::DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& texture, bool wrap)
{
	//Calculate slopes
	TexVertex tDelta0 = (v1 - v0) / (v1.pos.y - v0.pos.y);
	TexVertex tDelta1 = (v2 - v0) / (v2.pos.y - v0.pos.y);

	// calculate start and end scanlines
	int startY = ceil(v0.pos.y - 0.5f);
	int endY = ceil(v1.pos.y - 0.5f); // the scanline AFTER the last line drawn

	DrawFlatTriangleTex(startY, endY, tDelta0, tDelta1, v0, v0, texture, wrap);
}

// Draw triangle Flat triangle with texture, that is Flat Bottom or Flat Top
// int startY and endY are screen coordinates where the triangle should be drawn, startY included and endY excluded
// TexVertex tDelta says how much TexVertex changes per Y step, 0 for left side and 1 for right side
// TexVertex v0 and v1 define the two top points for the triangle, for Flatbottom we have v0 == v1 
void Graphics::DrawFlatTriangleTex(int startY, int endY, const TexVertex tDelta0, const TexVertex tDelta1, 
	const TexVertex v0, const TexVertex v1,  const Surface& texture, bool wrap)
{
	//stores current position on the lines of the triangle (moves with increasing y)
	TexVertex tCurrentY0 = v0 + tDelta0 * ((startY + 0.5f) - (v0.pos.y));
	TexVertex tCurrentY1 = v1 + tDelta1 * ((startY + 0.5f) - (v0.pos.y));

	for (int y = startY; y < endY; y++, tCurrentY0 += tDelta0, tCurrentY1 += tDelta1)
	{
		//Find start and end points
		int startX = ceil(tCurrentY0.pos.x - 0.5f);
		int endX = ceil(tCurrentY1.pos.x - 0.5f);

		//Get delta per x step and calculate start position for texture map only
		Vec2 tDeltaX = (tCurrentY1.tPos - tCurrentY0.tPos) / (tCurrentY1.pos.x - tCurrentY0.pos.x);
		Vec2 tCurrentX = tCurrentY0.tPos + tDeltaX * ((startX + 0.5f) - (tCurrentY0.pos.x));

		//Helper variables
		const float tWidth = texture.GetWidth();
		const float tHeight = texture.GetHeight();
		const float tWidthMax = tWidth - 1.0f;
		const float tHeightMax = tHeight - 1.0f;

		for (int x = startX; x < endX; x++, tCurrentX += tDeltaX)
		{
			float tx = 0.0f, ty = 0.0f;

			if (!wrap) //clamp when not wrapped
			{
				const float topClampedX = std::min(tCurrentX.x * tWidth, tWidthMax);
				const float topClampedY = std::min(tCurrentX.y * tHeight, tHeightMax);
				tx = std::max(0.0f, topClampedX);
				ty = std::max(0.0f, topClampedY);
			}
			else //modulus when wrapped
			{
				tx = std::fmod(tCurrentX.x * tWidth, tWidthMax);
				ty = std::fmod(tCurrentX.y * tHeight, tHeightMax);
			}

			PutPixel(x, y, texture.GetPixel(tx, ty));
		}
	}
}