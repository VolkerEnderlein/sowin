/**************************************************************************
 *
 *  This file is part of the Coin SoWin GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin SoWin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

static const char rcsid[] =
"$Id$";

#include <Inventor/lists/SbPList.h>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>
#include <Inventor/Win/SoWin.h>
#include <sowindefs.h>

#include <assert.h>
#include <stdio.h>

// *************************************************************************

SoWinBitmapButton::SoWinBitmapButton( HWND parent,
	                                    int x,
                                      int y,
	                                    int width,
	                                    int height,
	                                    int depth,
	                                    const char * name,
																			void * bits )
{
  RECT rect = { x, y, width, height };

  this->constructor( );
  this->buildWidget( parent, rect );

	if( bits != NULL ) {
		this->addBitmap( width, height, depth, bits );
		this->setBitmap( 0 );
	}
	
} // SoWinBitmapButton()

SoWinBitmapButton::SoWinBitmapButton( HWND button )
{
	assert( IsWindow( button) );

	char name[8];
	GetClassName( button, name, 8 );
	assert( strcmp( name, "BUTTON" ) == 0 );
	
	this->constructor( );
	this->buttonWindow = button;
	this->addBitmap( ( HBITMAP ) (
		SendMessage( this->buttonWindow, BM_GETIMAGE, ( WPARAM ) IMAGE_BITMAP, 0 )
		) );
} // SoWinBitmapButton()

SoWinBitmapButton::~SoWinBitmapButton( void )
{
	// FIXME: cleanup resources
	this->destructor( );
} // ~SoWinBitmapButton()

SIZE
SoWinBitmapButton::sizeHint( void ) const
{
  SIZE size = { 30, 30 };
	return size;
} // sizeHint()

HWND
SoWinBitmapButton::getWidget( void )
{
  return this->buttonWindow;
}

int
SoWinBitmapButton::width( void )
{
	RECT rect;
	GetWindowRect( this->buttonWindow, & rect );
  return ( rect.right - rect.left ); //this->sizeHint( ).cx;
}

int
SoWinBitmapButton::height( void )
{	
	RECT rect;
	GetWindowRect( this->buttonWindow, & rect );
  return ( rect.bottom - rect.top ); //this->sizeHint( ).cy;
}

void
SoWinBitmapButton::move( int x, int y )
{
	this->move( x, y, this->width( ), this->height( ) );
}

void
SoWinBitmapButton::move( int x, int y, int width, int height )
{
	assert( IsWindow( this->buttonWindow ) );
  MoveWindow( this->buttonWindow, x, y, width, height, TRUE );
}

void
SoWinBitmapButton::show( void )
{
	ShowWindow( this->buttonWindow, SW_SHOW );
}

void
SoWinBitmapButton::hide( void )
{
	ShowWindow( this->buttonWindow, SW_HIDE );
}

void
SoWinBitmapButton::registerCallback( bitmapButtonCB * func )
{
  this->viewerCB = func;
}

void
SoWinBitmapButton::registerViewer( SoWinFullViewer * viewer )
{
	this->viewer = viewer;
}

void
SoWinBitmapButton::constructor( void )
{
	this->buttonWindow = NULL;
	this->viewerCB = NULL;
	this->viewer = NULL;
	this->bitmapList = new SbPList;
} // constructor()

void
SoWinBitmapButton::destructor( void )
{
	// FIXME: DeleteObject all bitmaps
	delete this->bitmapList;
} // destructor()

HWND
SoWinBitmapButton::buildWidget( HWND parent, RECT rect )
{
	
	assert( IsWindow( parent ) );

	this->buttonWindow = CreateWindow( "BUTTON",
		                                 NULL,
		                                 WS_VISIBLE | WS_CHILD |
		                                 BS_PUSHBUTTON | BS_BITMAP | BS_CENTER,
		                                 0, 0, // move later
		                                 rect.right - rect.left, rect.bottom - rect.top,
		                                 parent,
		                                 NULL,
		                                 SoWin::getInstance( ),
		                                 NULL );
	
  assert( IsWindow( this->buttonWindow ) );
	
  return this->buttonWindow;
}

void
SoWinBitmapButton::setId( long id )
{
	SetWindowLong( this->buttonWindow, GWL_ID, id );
}

long
SoWinBitmapButton::getId( void )
{
	return GetWindowLong( this->buttonWindow, GWL_ID );
}
	
void
SoWinBitmapButton::setState( SbBool pushed )
{
	SendMessage( this->buttonWindow, BM_SETSTATE, ( WPARAM ) pushed, 0 );
	InvalidateRect( this->buttonWindow, NULL, FALSE );
} // setState()

SbBool
SoWinBitmapButton::getState( void ) const
{
	return ( SendMessage( this->buttonWindow, BM_GETSTATE, 0, 0 ) & BST_PUSHED );
} // setState()

void
SoWinBitmapButton::setEnabled( SbBool enable )
{
  long style = GetWindowLong( this->buttonWindow, GWL_STYLE );

	if ( enable)
		style &= ~WS_DISABLED;
	else
		style |= WS_DISABLED;

	SetWindowLong( this->buttonWindow, GWL_STYLE, style );
	InvalidateRect( this->buttonWindow, NULL, FALSE );
} // setEnabled()

SbBool
SoWinBitmapButton::isEnabled( void ) const
{
  return ( ! ( GetWindowLong( buttonWindow, GWL_STYLE ) & WS_DISABLED ) );
} // isEnabled()

void
SoWinBitmapButton::addBitmap( HBITMAP hbmp )
{
	this->bitmapList->append( hbmp );
} // addBitmap()

void
SoWinBitmapButton::addBitmap( int width, int height, int bpp, void * src )
{
	void * dest;

	HBITMAP hbmp = this->createDIB( width, height, bpp, & dest );

	memcpy( dest, src, width * height * ( bpp / 8 ) );

	this->addBitmap( hbmp );
	
} // addBitmap()

void
SoWinBitmapButton::addBitmap( char ** xpm )
{
	this->addBitmap( this->parseXpm( xpm ) );	
} // addBitmap()

HBITMAP
SoWinBitmapButton::getBitmap( int index )
{
	return ( HBITMAP ) ( * this->bitmapList )[index];
} // getBitmap()

void
SoWinBitmapButton::setBitmap( int index )
{
	assert( IsWindow( this->buttonWindow ) );

	SendMessage( this->buttonWindow,
		BM_SETIMAGE,
		( WPARAM ) IMAGE_BITMAP,
		( LPARAM ) this->getBitmap( index) );

	InvalidateRect( this->buttonWindow, NULL, FALSE );
} // setBitmap()
/*
HBITMAP
SoWinBitmapButton::getCurrentBitmap( void ) const
{	
	assert( IsWindow( this->buttonWindow ) );
	
	return ( HBITMAP ) ( SendMessage( this->buttonWindow, BM_GETIMAGE, ( WPARAM ) IMAGE_BITMAP, 0 ) );
} // getCurrentBitmap()
*/
HBITMAP
SoWinBitmapButton::createDIB( int width, int height, int bpp, void ** bits ) // 16||24||32 bpp
{
  assert( bpp > 8 );

  HBITMAP bitmap = NULL;
  HDC hdc = CreateCompatibleDC( NULL );
  int heapspace = sizeof( BITMAPINFOHEADER );

  HANDLE heap = GetProcessHeap( );
  BITMAPINFO * format = ( BITMAPINFO * ) HeapAlloc( heap, 0, heapspace );
	
  BITMAPINFOHEADER * header = ( BITMAPINFOHEADER * ) format;
  header->biSize = sizeof( BITMAPINFOHEADER );
  header->biWidth = width;
  header->biHeight = -height;
  header->biPlanes = 1;
  header->biBitCount = bpp;
  header->biCompression = BI_RGB;
  header->biSizeImage = 0;
  header->biXPelsPerMeter = 0;
  header->biYPelsPerMeter = 0;
  header->biClrUsed = 0;
  header->biClrImportant = 0;

  UINT flag = DIB_RGB_COLORS;
  bitmap = CreateDIBSection( hdc, format, flag, ( void ** ) bits, NULL, 0 );

  assert( * bits );

  HeapFree( heap, 0, format );
  DeleteDC( hdc );

  return bitmap;
}

// FIXME: slow dog!
HBITMAP
SoWinBitmapButton::parseXpm( char ** xpm ) // convert from xpm to 24 bit DIB ( demands hex colors )
{
	int i, j, k, l, m, x, y;
	int width;
	int height;
	int depth;
	int numColors;
	int numChars;
	char * strStart;
	char * strEnd;
	char * line;
	char * charLookupTable;
	int * colorLookupTable;
	void * dest;
	HBITMAP hbmp;

	int colorValue;
	char pixelSize;

	depth = 24; // default depth is 24 bits per pixel
	pixelSize = depth / 8;

	// get width
	strStart = xpm[0];
	strEnd = strchr( strStart, ' ' );
	assert( strEnd );
	* strEnd = '\0';
	width = atoi( strStart );
	* strEnd = ' ';

	// get height
	strStart = strEnd + 1;
	strEnd = strchr( strStart, ' ' );
	assert( strEnd );
	* strEnd = '\0';
	height = atoi( strStart );
	* strEnd = ' ';

	// get number of colors
	strStart = strEnd + 1;
	strEnd = strchr( strStart, ' ' );
	assert( strEnd );
	* strEnd = '\0';
	numColors = atoi( strStart );
	* strEnd = ' ';

	// get number of chars per pixel
	strStart = strEnd + 1;
	strEnd = strchr( strStart, ' ' );
	if ( ! strEnd ) strEnd = strchr( strStart, '\0' );
	assert( strEnd );
	* strEnd = '\0';
	numChars = atoi( strStart );
	// * strEnd = ' '; FIXME
	
	// create color lookup table
	charLookupTable = new char[numColors * numChars];
	colorLookupTable = new int[numColors];
	
	// get colors
	for ( i = 0; i < numColors; i++ ) {
		
		for ( j = 0; j < numChars; j ++ )
			charLookupTable[( i * numChars ) + j] = * ( xpm[i + 1] + j );
		
		strStart = strchr( ( xpm[i + 1] + numChars ), 'c' ); // FIXME: make sure it is 'c '
		strEnd = strStart + 2;
		
		if ( * strEnd == '#' )
			colorLookupTable[i] = axtoi( strEnd + 1 );
		else
			colorLookupTable[i] = -1; // Parse string ( color name )
		
	}

	// create bitmap
	hbmp = this->createDIB( width, height, depth, & dest );
	
	// put pixels
	for ( i = 0; i < height; i++ ) {

		line = xpm[i + 1 + numColors];

		y = i * width * pixelSize;
		
		for ( j = 0; j < width; j++ ) {

			x = j * pixelSize;

			// for every color
			for ( k = 0; k < numColors; k++ ) {
				
				for ( l = 0; l < numChars; l++ )
					if ( charLookupTable[( k * numChars ) + l] != line[( j * numChars ) + l] )
						break;

				// if we found the char in the lookup table
				if ( l >= numChars ) {
					
					if ( colorLookupTable[k] == -1 )
						colorValue = GetSysColor( COLOR_3DFACE ); // FIXME: make param
					else
						colorValue = colorLookupTable[k];
					
					// FIXME: may not work with depth < 24
					// for each color byte in the pixel
					for ( m = 0; m < pixelSize; m++ ) {

						// put color byte ( and only one byte )
						( ( char * ) dest )[y + x + m] =
							( char ) ( ( colorValue & ( 0x000000FF << ( m << 3 ) ) ) >> ( m << 3 ) );	
						
					}

					// next pixel
					break;
					
				}
				
			}
			
		}
		
	}

	// cleanup
	delete charLookupTable;
	delete colorLookupTable;

	// return bitmap
	return hbmp;
}

int
SoWinBitmapButton::axtoi( char * str ) // convert from ASCII hex to int
{
	int x, i, n;
	char * c = str;

	n = ( strchr( c, '\0' ) - c );

	x = 0;
	
  // convert n nibbles
	for ( i = 0; i < n; i++ ) {

		// numbers 0 - 9
		if ( ( c[i] > 0x2F ) && ( c[i] < 0x3A ) )
			x += ( ( c[i] - 0x30 ) << ( ( n - i - 1 ) * 4 ) );

		// capital letters A - F
		if ( ( c[i] > 0x40 ) && ( c[i] < 0x47 ) )
			x += ( ( c[i] - 0x41 + 0x0A ) << ( ( n - i - 1 ) * 4 ) );

		// lower case letters a - f
		if ( ( c[i] > 0x60 ) && ( c[i] < 0x67 ) )
			x += ( ( c[i] - 0x61 + 0x0A ) << ( ( n - i - 1 ) * 4 ) );
	
	}
	
	return x;

}