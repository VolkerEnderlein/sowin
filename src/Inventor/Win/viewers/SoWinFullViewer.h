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

#ifndef  SOWIN_FULLVIEWER_H
#define  SOWIN_FULLVIEWER_H

#include <Inventor/Win/viewers/SoWinViewer.h>
#include <Inventor/SbPList.h>
#include <Inventor/lists/SoCallbackList.h>

class SoFieldSensor;
class SoWinBitmapButton;
class SoWinThumbWheel;
class SoAnyPopupMenu;

typedef void PushAppButtonCB( HWND hwnd, int id, void * pushData, void * userData );
typedef void RedrawAppButtonCB( LPDRAWITEMSTRUCT lpdis, void * userData );

class SOWIN_DLL_API SoWinFullViewer : public SoWinViewer {
  SOWIN_OBJECT_ABSTRACT_HEADER(SoWinFullViewer, SoWinViewer);

  friend class SoAnyFullViewer;
public:

  enum BuildFlag {
    BUILD_NONE          = 0x00, 
    BUILD_DECORATION    = 0x01, 
    BUILD_POPUP         = 0x02, 
    BUILD_ALL           = 0xff, 
  };
    
  void setDecoration(SbBool set);
  SbBool isDecoration( void );

  void setPopupMenuEnabled( SbBool set );
  SbBool isPopupMenuEnabled( void );

  static void setDoButtonBar( SbBool set );
  static SbBool isDoButtonBar( void );

  SoCallbackList popupPostCallback;   // FIXME: is this needed? mariusbu 20010611.
  SoCallbackList popupPreCallback;

  void setClientPopupMenu( HMENU menu );
  SbBool isClientPopupMenuInstalled( void );
    
  HWND getAppPushButtonParent( void ) const;
  void addAppPushButton( HWND newButton );
  void insertAppPushButton( HWND newButton, int index );
  void removeAppPushButton( HWND oldButton );
  int findAppPushButton( HWND oldButton );
  int lengthAppPushButton( void );

  HWND getRenderAreaWidget( void );
    
  virtual void setViewing( SbBool set );
  virtual void setCamera( SoCamera * camera );
  virtual void hide( void );

  // SoWinStereoDialog * getStereoDialog( void );
  // void setStereoDialog( SoWinStereoDialog * newDialog );

  void addPushAppButtonCallback( PushAppButtonCB * callback,
                                 void * data = NULL );
  void addRedrawAppButtonCallback( RedrawAppButtonCB * callback,
                                   void * data = NULL);
        
  void selectedPrefs( void );
  void resetToHomePosition( void );
  void saveHomePosition( void );
  void viewAll( void );
  void seekbuttonClicked( void );
  void copyView( SbTime time );
  void pasteView( SbTime time );

  //protected:
  SoWinFullViewer( HWND parent,
                   const char * name, 
                   SbBool embedded, 
                   BuildFlag flag,
                   SoWinViewer::Type type, 
                   SbBool buildNow);
protected:
  ~SoWinFullViewer( void );
  HWND viewerWidget; // form which manages all other widgets
  HWND renderAreaWidget;  // render area HWND

  static SbBool doButtonBar;
    
  //HWND zoomForm;
  //HWND zoomField;
  //HWND zoomSlider;

  SoFieldSensor * zoomSensor;
    
  SoWinThumbWheel * rightWheel;
  SoWinThumbWheel * bottomWheel;
  SoWinThumbWheel * leftWheel;
  SoWinThumbWheel * extraWheel;

	// FIXME: remove ( let SoWinThumbWheel save the string and value )
  char * rightWheelStr;
  char * bottomWheelStr;
  char * leftWheelStr;
  char * extraWheelStr;

  float rightWheelVal;
  float bottomWheelVal;
  float leftWheelVal;
  float extraWheelVal;

  SbPList * viewerButtonWidgets;

  //HWND vwrButtonForm;
  int numFullVwrButtons;
  virtual void pushButtonCB( HWND, int id, void * );
  virtual void pushAppButtonCB( HWND hwnd, int id, void * data );
  virtual void redrawAppButtonCB( LPDRAWITEMSTRUCT lpdis );
 
  HWND getButtonWidget( void ) const;
    
  SbBool popupEnabled;
  //HWND popupWidget; // FIXME: not needed? mariusbu 20010611.
  //HWND * popupToggleWidgets;
  //HWND * drawStyleWidgets;
  //HWND bufferStyleWidgets[3];
  char * popupTitle;

  HWND buildWidget( HWND parent );

  virtual void buildDecoration( HWND parent );
  virtual HWND buildLeftWheel( HWND parent );
  virtual HWND buildBottomWheel( HWND parent );
  virtual HWND buildRightWheel( HWND parent );
  virtual HWND buildZoomSlider( HWND parent );

  HWND buildAppButtons( HWND parent );
  HWND buildViewerButtons( HWND parent );

  virtual void createViewerButtons( HWND parent );
    
  virtual void openStereoDialog( void );

  virtual void buildPopupMenu( void );
  virtual void setPopupMenuString( const char * title );
  virtual void openPopupMenu( const SbVec2s position );
  virtual void destroyPopupMenu( void );  
  virtual int displayPopupMenu( int x, int y, HWND owner );

  // FIXME: not needed? mariusbu 20010611.
  //HMENU buildFunctionsSubmenu( HMENU popup );
  //HMENU buildDrawStyleSubmenu( HMENU popup );

  void setPrefSheetString( const char * name );
  virtual void createPrefSheet( void );
  void createPrefSheetShellAndForm( HWND shell, HWND form );
  void createDefaultPrefSheetParts( HWND widgetList[], 
                                    int num,
                                    HWND form );
  void layoutPartsAndMapPrefSheet( HWND widgetList[], 
                                   int num,
                                   HWND form,
                                   HWND shell );

  HWND createSeekPrefSheetGuts( HWND parent );
  HWND createSeekDistPrefSheetGuts( HWND parent );
  HWND createZoomPrefSheetGuts( HWND parent );
  HWND createClippingPrefSheetGuts( HWND parent );
  HWND createStereoPrefSheetGuts( HWND parent );
    
  float getLeftWheelValue( void ) const;
  void setLeftWheelValue( const float value );

  float getBottomWheelValue( void ) const;
  void setBottomWheelValue( const float value );

  float getRightWheelValue( void ) const;
  void setRightWheelValue( const float value );

  virtual void rightWheelMotion( float );
  virtual void bottomWheelMotion( float );
  virtual void leftWheelMotion( float );

  virtual void rightWheelStart( void );
  virtual void bottomWheelStart( void );
  virtual void leftWheelStart( void );

  virtual void rightWheelFinish( void );
  virtual void bottomWheelFinish( void );
  virtual void leftWheelFinish( void );
    
  void setBottomWheelString( const char * name );
  void setLeftWheelString( const char * name );
  void setRightWheelString( const char * name );
    
  virtual void openViewerHelpCard( void );
  virtual void afterRealizeHook( void );

  HWND getViewerWidget( void );

  virtual SbBool processSoEvent( const SoEvent * const event );

private:
  void doAppButtonLayout( int start );

  SbString menutitle; // really needed? mariusbu 20010611.

  SoAnyPopupMenu * prefmenu;
  SoAnyFullViewer * const common; 

  SbBool menuenabled;
  SbBool decorations;
  SbBool firstBuild;

  HWND appButtonForm;
  SbPList * appButtonList;

	//SbPList * viewerButtonList;
	HWND viewerButtons[8];

  //SoWinStereoDialog * stereoDialogBox ; // FIXME: not implemented yet
    
  SbVec2f zoomSldRange;

  void setCameraZoom( float zoom );
  float getCameraZoom( void );
  void setZoomSliderPosition( float zoom );
  void setZoomFieldString( float zoom );

  static void zoomSliderCB( HWND, SoWinFullViewer *, void ** );
  static void zoomFieldCB( HWND, SoWinFullViewer *, void ** );
  static void zoomSensorCB( void *, SoSensor *);
    
  //SoWinBitmapButton * buttonList[10];

  HWND prefSheetShellWidget;
  const char * prefSheetStr;  // - const

  int seekDistWheelVal;
  HWND seekDistField;
    
  HWND clipWheelForm;
  int clipNearWheelVal, clipFarWheelVal;
  HWND clipNearField, clipFarField;
    
  HWND stereoWheelForm;
  HWND stereoField;
  HWND stereoLabel;
  int stereoWheelVal;

  static void drawDecorations( SoWinFullViewer * viewer, HWND hwnd, HDC hdc );

  static void visibilityChangeCB( void * pt, SbBool visible );
    
  static void rightWheelCB ( SoWinFullViewer * viewer, void ** data );
  static void bottomWheelCB( SoWinFullViewer * viewer, void ** data );
  static void leftWheelCB  ( SoWinFullViewer * viewer, void ** data );

  // Window proc for SoWinFullViewer "manager HWND" windows
  static LRESULT CALLBACK mgrWindowProc( HWND window, UINT message,
                                         WPARAM wparam, LPARAM lparam );

  // Window proc for SoWinFullViewer "button container" windows
  static LRESULT CALLBACK btnWindowProc( HWND window, UINT message,
                                         WPARAM wparam, LPARAM lparam );

  // Window proc for SoWinFullViewer "application button container" windows
  static LRESULT CALLBACK appBtnWindowProc( HWND window, UINT message,
                                            WPARAM wparam, LPARAM lparam );

  // Window proc for SoWinFullViewer text entry window (zoomfield)
  static LRESULT CALLBACK txtWindowProc( HWND window, UINT message,
                                         WPARAM wparam, LPARAM lparam );

  LRESULT onCreate( HWND window, UINT message, WPARAM wparam, LPARAM lparam );
  LRESULT onPaint( HWND window, UINT message, WPARAM wparam, LPARAM lparam );
  LRESULT onSize( HWND window, UINT message, WPARAM wparam, LPARAM lparam );
  LRESULT onDestroy( HWND window, UINT message, WPARAM wparam, LPARAM lparam );

  PushAppButtonCB * customPushBtnCB ; // FIXME: really needed? mariusbu 20010611.
  RedrawAppButtonCB * customRedrawBtnCB ;
  void * customPushBtnData, * customRedrawBtnData ;
  
  //WNDPROC origBtnWndProc;
  //WNDPROC origAppBtnWndProc;
  //WNDPROC origTxtWndProc;

  RECT renderAreaOffset; // RenderArea position
};

#endif  // SOWIN_FULLVIEWER_H














