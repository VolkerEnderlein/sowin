#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SbTime.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#if SOWIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <conio.h>
#endif // SOWIN_DEBUG

#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinComponent.h>
#include <Inventor/Win/SoWinMessageHandler.h>

// *************************************************************************

HINSTANCE SoWin::Instance = NULL;
HWND SoWin::mainWidget = NULL;
char * SoWin::appName = NULL;
char * SoWin::className = NULL;
SoWinMessageHandler * SoWin::messageHandler = NULL;
int SoWin::timerSensorId = 0;
SbBool SoWin::timerSensorActive = FALSE;
int SoWin::delaySensorId = 0;
SbBool SoWin::delaySensorActive = FALSE;
int SoWin::idleSensorId = 0;
SbBool SoWin::idleSensorActive = FALSE;

// *************************************************************************


///////////////////////////////////////////////////////////////////
//
//  (public)
//

HWND SoWin::init( const char * const appName,
                  const char * const className )
{
    int argc = 1;
    char * argv[] = { ( char * ) appName, NULL };
    return SoWin::init( argc, argv, appName, className );
}

HWND SoWin::init( int argc,
                  char ** argv,
                  const char * const appName,
                  const char * const className )
{
    if ( appName )
       SoWin::appName = strcpy( new char [strlen( appName ) + 1], appName );
    if ( className )
       SoWin::className = strcpy( new char [strlen( className ) + 1], className );

    messageHandler = new SoWinMessageHandler;
    SoWin::registerWindowClass( className );

    RECT rect = { 0, 0, SoWin_DefaultWidth, SoWin_DefaultHeight };  // default window size
    HWND toplevel =  SoWin::createWindow( ( char * ) appName, ( char * ) className, rect, NULL );
    SoWin::init( toplevel );
    return toplevel;
}

void SoWin::init( HWND const topLevelWidget )   // requires messageHandler to be initialized
{
    SoDB::init( );
    SoNodeKit::init( );
    SoInteraction::init( );

    SoDB::getSensorManager( )->setChangedCallback( SoWin::sensorQueueChanged, NULL );
    if(topLevelWidget)
        SoWin::mainWidget = topLevelWidget;
}

void SoWin::init( void )
{
    SoWin::init( "SoWin", "SoWin" );
}

void SoWin::mainLoop( void )
{
    messageHandler->messageLoop( );
}

void SoWin::doIdleTasks( void )
{
    static int idleCount = 0;
    _cprintf( "doIdleTasks %d\n", idleCount++ );
    // FIXME: tmp fix
    SoWin::idleSensorId = 0;
    SoWin::idleSensorActive = FALSE;
    SoDB::getSensorManager( )->processDelayQueue( TRUE );
    SoWin::sensorQueueChanged( NULL );
}

BOOL SoWin::dispatchEvent( MSG * msg )
{
    /*switch ( msg->message ) {
        case WM_TIMER: if( msg->... id = idleTimerId) retval = TRUE;
    }*/

    DispatchMessage( msg );
    return ( TRUE );
}

void SoWin::show( HWND const widget )
{
    ShowWindow( widget, SW_SHOW );
}

void SoWin::hide(HWND const widget)
{
    ShowWindow( widget, SW_HIDE );
}

void SoWin::setWidgetSize( HWND widget, const SbVec2s size )
{
    SIZE old_size;
    HDC hdc = GetDC( mainWidget );

    SetWindowExtEx( hdc, size[0], size[1], &old_size );
} 

SbVec2s SoWin::getWidgetSize( HWND widget )
{
    SIZE size;
    HDC hdc = GetDC(mainWidget);

    if ( ! GetWindowExtEx( hdc, & size ) )
    {
        size.cx = -1;
        size.cy = -1;
    }
    return SbVec2s( ( short ) size.cx, ( short ) size.cy );
}

HWND SoWin::getTopLevelWidget( void )
{
  return SoWin::mainWidget; // FIXME: ???
}

void SoWin::createSimpleErrorDialog( HWND const widget, const char * const dialogTitle, const char * const errorStr1, const char * const errorStr2)
{
    MessageBox( widget, errorStr1, dialogTitle, MB_OK | MB_ICONERROR );  
}

// TGS sh*t - not implemented

void SoWin::terminate( long terminateSyncTime )
{
    // FIXME: not implemented
}

SbBool SoWin::PreTranslateMessage( MSG * msg )
{
    // FIXME: not implemented
    return FALSE;
}

SbBool SoWin::nextEvent(int appContext, MSG * msg)
{
    // FIXME: not implemented
    return PeekMessage( msg, NULL, 0, 0, PM_NOREMOVE );
}

int SoWin::getAppContext( void )
{
    // FIXME: not implemented
    return 0;
}

int * SoWin::getDisplay( void )
{
    // FIXME: not implemented
    return NULL;
}

HWND SoWin::getShellWidget( HWND hwnd )
{
    assert( IsWindow( hwnd ) );

    // FIXME: ???

    HWND parent = NULL;
    HWND ancestor = NULL;
    
    parent = GetParent( hwnd );

    while ( parent )
    {
        ancestor = parent;
        parent = GetParent( ancestor );
    }

    return ancestor;
}

void SoWin::getPopupArgs( int * display, int screen, char ** args, int * n )
{
    // FIXME: not implemented
}

void SoWin::addColormapToShell( HWND widget, HWND shell )
{
    // FIXME: not implemented
}

LRESULT SoWin::isInventorMessage( HWND hwnd,
                                  UINT message,
                                  WPARAM wParam,
                                  LPARAM lParam )
{
    // FIXME: not implemented
    return 0;
}

char * SoWin::encodeString( char *string )
{
    // FIXME: not implemented
    return NULL;
}

char * SoWin::decodeString( char * wstring )
{
    // FIXME: not implemented
    return NULL;
}

// Coin spesific functions - not in oiv

void SoWin::setInstance( HINSTANCE instance )
{
    SoWin::Instance = instance;
}

HINSTANCE SoWin::getInstance( void )
{
    return SoWin::Instance;
}
/*
void SoWin::errorHandlerCB( const SoError *error, void *data )
{
   // FIXME: not implemented
}*/

void SoWin::addMessageHook( HWND hwnd, UINT message )
{
//    messageHandler.addMessageHook( hwnd, message );
}

void SoWin::removeMessageHook( HWND hwnd, UINT message )
{
//    messageHandler.removeMessageHook( hwnd, message );
}

// Coin spesific

void SoWin::deInit( void )  // cleanup
{
    delete SoWin::messageHandler;
    SoWin::messageHandler = NULL;

    SoWin::unRegisterWindowClass( SoWin::className );
}

void SoWin::addProcessEventFunction( processEventFunction * function )
{
    SoWin::messageHandler->addProcessEventFunction( function );
}

void SoWin::removeProcessEventFunction( processEventFunction * function )
{
    SoWin::messageHandler->removeProcessEventFunction( function );
}

///////////////////////////////////////////////////////////////////
//
//  (protected)
//

void SoWin::registerWindowClass( const char * const className )
{
    WNDCLASS windowclass;
	LPCTSTR icon,cursor;
	HBRUSH brush;

	icon = MAKEINTRESOURCE( IDI_APPLICATION );
	cursor = MAKEINTRESOURCE( IDC_ARROW );
	brush = ( HBRUSH ) GetStockObject( COLOR_BACKGROUND );

    windowclass.lpszClassName = className;
    windowclass.hInstance = SoWin::Instance;
    windowclass.lpfnWndProc = SoWin::windowCB;
	windowclass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	windowclass.lpszMenuName = NULL;
	windowclass.hIcon = LoadIcon( NULL, icon );
	windowclass.hCursor = LoadCursor( NULL, cursor );
	windowclass.hbrBackground = brush;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 4;

	RegisterClass( &windowclass );
}

void SoWin::unRegisterWindowClass( const char * const className )
{
    UnregisterClass( className, SoWin::getInstance( ) );
}

HWND SoWin::createWindow( char * title, char * className, RECT rect, HWND parent, HMENU menu )
{
	DWORD style = WS_OVERLAPPEDWINDOW;
    LPVOID params = NULL;

    SoWin::mainWidget = CreateWindow( className,
						title,
						style,
						rect.left,
                        rect.top,
                        rect.right - rect.left,
                        rect.bottom - rect.top,
						parent,
						menu,
						SoWin::Instance,
						params );


    //messageHandler->addCB( SoWin::windowCB );

    return SoWin::mainWidget;
}

LRESULT CALLBACK SoWin::windowCB( HWND window, UINT message, WPARAM wparam, LPARAM lparam )
{
    switch( message )
    {
        /*case WM_ACTIVATE:
            return 0;*/

        /*case WM_SIZE:     
            return 0;*/

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
            
        case WM_QUIT:
            SoWin::deInit( );
            return 0;
    }
//    return object->dispatchMessage( window, message, wparam, lparam );
    return DefWindowProc( window, message, wparam, lparam );
}

///////////////////////////////////////////////////////////////////
//
//  (private)
//

void CALLBACK SoWin::timerSensorCB( HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
  SoDebugError::postInfo( "SoWin::timerSensorCB", "called" );
#endif // SOWIN_DEBUG
  _cprintf( "SoWin::timerSensorCB\n" );
  SoWin::timerSensorId = 0;
  SoWin::timerSensorActive = FALSE;
  SoDB::getSensorManager( )->processTimerQueue( );
  SoWin::sensorQueueChanged( NULL );
}

void CALLBACK SoWin::delaySensorCB( HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
  SoDebugError::postInfo( "SoWin::delaySensorCB", "called" );
#endif // SOWIN_DEBUG
  SoWin::delaySensorId = 0;
  SoWin::delaySensorActive = FALSE;
  SoDB::getSensorManager( )->processDelayQueue( FALSE );
  SoWin::sensorQueueChanged( NULL );
}

void CALLBACK SoWin::idleSensorCB( HWND window, UINT message, UINT idevent, DWORD dwtime)
{
    #if SOWIN_DEBUG && 0
    SoDebugError::postInfo( "SoWin::idleSensorCB", "called" );
    #endif // SOWIN_DEBUG
    _cprintf( "idleSensorCB\n" );
/*  SoWin::idleSensorId = 0;
    SoWin::idleSensorActive = FALSE;
    SoDB::getSensorManager( )->processDelayQueue( TRUE );
    SoWin::sensorQueueChanged( NULL );*/
    SoWin::doIdleTasks( );
}

void SoWin::sensorQueueChanged( void * cbdata )
{
   SoSensorManager * sensormanager = SoDB::getSensorManager( );

    SbTime timevalue;
    if ( sensormanager->isTimerSensorPending( timevalue ) ) {
        SbTime interval = timevalue - SbTime::getTimeOfDay( );

        if ( interval.getValue( ) < 0.0 ) interval.setValue( 0.0 );
        if ( SoWin::timerSensorActive ) KillTimer( NULL, SoWin::timerSensorId );
    
        SoWin::timerSensorId = SetTimer( NULL,
                                         0,
                                         interval.getMsecValue( ),
                                         SoWin::timerSensorCB );
        SoWin::timerSensorActive = TRUE;
    } else if ( SoWin::timerSensorActive ) {
        KillTimer( NULL, SoWin::timerSensorId );
        SoWin::timerSensorId = 0;
        SoWin::timerSensorActive = FALSE;
    }

    if ( sensormanager->isDelaySensorPending( ) ) {
        /* fix - remove                             
        if ( ! SoWin::idleSensorActive ) {
            SoWin::idleSensorId = SetTimer( NULL,
                                        0,
                                        1000,
                                        SoWin::idleSensorCB );
            SoWin::idleSensorActive = TRUE;
        }*/
        /*if ( ! SoXt::idleSensorActive ) {
            SoWin::idleSensorId =
                XtAppAddWorkProc( SoXt::getAppContext( ), SoXt::idleSensorCB, NULL );
            SoWin::idleSensorActive = TRUE;
        }*/

        if ( ! SoWin::delaySensorActive ) {
            unsigned long timeout = SoDB::getDelaySensorTimeout( ).getMsecValue( );
            SoWin::delaySensorId = SetTimer( NULL,
                                             0,
                                             timeout,
                                             SoWin::delaySensorCB );
            SoWin::delaySensorActive = TRUE;
        }
    } else {
        /* fix - remove                             
        if ( SoWin::idleSensorActive ) {
            KillTimer( NULL, SoWin::idleSensorId );
            SoWin::idleSensorId = 0;
            SoWin::idleSensorActive = FALSE;
        } */
        /*if ( SoXt::idleSensorActive ) {
            XtRemoveWorkProc( SoXt::idleSensorId );
            SoXt::idleSensorId = 0;
            SoXt::idleSensorActive = FALSE;
        }*/
        if ( SoWin::delaySensorActive ) {
            KillTimer( NULL, SoWin::delaySensorId );
            SoWin::delaySensorId = 0;
            SoWin::delaySensorActive = FALSE;
        }
    }
}

///////////////////////////////////////////////////////////////////
//
//  (globals)
//

#ifdef _DEBUG

void WinDisplayLastError( void )
{
    LPSTR lpMsgBuf;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                   FORMAT_MESSAGE_FROM_SYSTEM | 
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   GetLastError( ),
                   MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                   ( LPTSTR ) & lpMsgBuf,
                   0,
                   NULL );
    MessageBox( NULL,
                ( LPCTSTR ) lpMsgBuf,
                "Error",
                MB_OK |
                MB_ICONINFORMATION );
    LocalFree( lpMsgBuf );
}

#endif // _DEBUG

#ifndef SOWIN_INTERNAL

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    char ** argv;
    int argc = 0;
    int i = 0;
    int retval, strsize;
    char * chrptr; // string pointer to first instance of character
    char * argstr; // string pointer to argument
    char * argptr;
    char * cmdline;

    SoWin::setInstance( instance );

    // count args
    if( cmdline = GetCommandLine( ) )  // get entire command line (including program name)
        argc++;
    while( chrptr = strchr( cmdline, ' ' ) )   // search for ' '
    {
        argc++;
        cmdline = chrptr + 1;
    }
    argv = ( char ** ) malloc( sizeof( char * ) * argc );

    // get arguments
    i = strlen( GetCommandLine( ) );
    cmdline = ( char * ) malloc( i + 1 );
    strcpy( cmdline, GetCommandLine( ) );
    cmdline[ i ] = ' ';
    cmdline[ i + 1] = '\0';

    argptr = cmdline;
    i = 0;
    while(chrptr = strchr( argptr, ' ' ) )   // search for ' '
    {
        strsize = chrptr - argptr;         // get string size
        argstr = ( char *) malloc( strsize + 1 ); 
        memcpy( argstr, argptr, strsize );
        argstr[strsize] = '\0'; // add '\0' to string
        argv[i++] = argstr; // add string to argv
        argptr = chrptr + 1;   // next argument ( + skip the ' ' )
    }
    free( cmdline );

    #ifdef _DEBUG
    AllocConsole( );
    _cprintf( "Console open...\n" );
    for (i = 0; i < argc; i++ )
        _cprintf( "Argv[%d] = %s\n", i, argv[i] ); 
    #endif  // _DEBUG

    //retval = ivMain( argc, argv );
    ivMain( argc, argv );
    retval = 0;

    #ifdef _DEBUG
    _cprintf( "Closing console...\n" );
    FreeConsole( );
    #endif // _DEBUG

    // free allocated memory
    for( i = 0; i < argc; i++)
        free( argv[i] );
    free( argv );

	return retval;
}

#endif // !SOWIN_INTERNAL

// Globals