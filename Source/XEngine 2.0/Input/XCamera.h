/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XCamera.h Visual Studio 2012 Version.
// This XEngine Camera Header File.
// Date Creation: 21 April 2014
// Last Changing: 11 May 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>
#include <XMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXCamera;

//-----------------------------------------------------------------------------
// Name: Struct CXCamera
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CXCamera_H__)
#define AFX_CXCamera_H__
class CXCamera
{	
public:
  //Public Directives
  CXCamera(ID3D11Device* pDevice,HWND WndHndl,int WndWidth,int WndHeight);
  virtual ~CXCamera();
  void Release();
  
  HRESULT CreateInputDevices(HWND WndLayer);
  void UpdateCamera();
  void UseCamera(bool UseConnect);
  
  //Установка начального расположения камеры в игровом мире или
  //вообще установка "глаз" в нужное местоположение.
  void SetCameraPos(D3DXVECTOR3 Pos_XYZ) { Position = Pos_XYZ; }

  //Скорость передвижения камеры, т.е фактически здесь
  //мы задаём скорость для движения камеры в стороны при управлении с клавиатуры.
  void SetCameraSpeed(float Cam_Speed) { m_fCameraSpeed = Cam_Speed; } 

  //Здесь задаётся скорость вращения и обзора мыши.
  void SetMouseSpeed(float Mouse_Speed) { m_fMouseSpeed = Mouse_Speed; }
 
  //Функция возвращающая местоположение камеры в игровом мире.
  D3DXVECTOR3 Pos() { return Position; }

  //Return Camera Projection Martix
  D3DXMATRIX ProjMatrix() { return m_pProjection; }

  //Return Camera View Martix
  D3DXMATRIX ViewMatrix() { return m_pView; }

  //Return Camera World Martix
  D3DXMATRIX WorldMatrix() { return m_pWorld; }

  //Return Camera Orthographic Martix
  D3DXMATRIX OrthoMatrix() { return m_pOrthoMatrix; }

  //Return Camera Dispose
  float CamPosX() { return PosX; }
  float CamPosY() { return PosY; }
  float CamPosZ() { return PosZ; }

  //Return Camera Button States
  bool SpaceDown() { return m_bSpaceDown; }
  bool MbLeftDown() { return m_bMbLeftDown; }
private:
  //Private Directives 
  ID3D11Device*        d_pDevice;      //Pointer On Direct3D Device Interface
  IDirectInput8*       m_pDirectInput; //Base DirectInput Device
  IDirectInputDevice8* m_pMouse;       //Mouse Input Device
  IDirectInputDevice8* m_pKeyboard;    //Keyboard Input Device
  CXDeltaTimer*        m_pDeltaTimer;  //Camera Delta Timer
  HWND m_pHWnd;                        //Window Handle

  //Camera View Directives
  D3DXMATRIX m_pProjection;
  D3DXMATRIX m_pView;
  D3DXMATRIX m_pWorld;
  D3DXMATRIX m_pOrthoMatrix;

  //Base Camera Directives
  bool m_nMbLDownOnce; //Use For Update Single Left Mouse Button Press Method
  bool m_bSpaceDown;   //Space Down State
  bool m_bMbLeftDown;  //Mouse Button Left Down State

  float Pitch,Yaw;
  float PosX,PosY,PosZ; //Camera Position
  float m_fCameraSpeed; //Camera Speed
  float m_fMouseSpeed;  //Mouse Speed
public:
  //Public Directives

  //Camera Vectors
  D3DXVECTOR3 Position;
  D3DXVECTOR3 Target;
  D3DXVECTOR3 Up;
  D3DXVECTOR3 Velocity;
  D3DXVECTOR3 Look_Vector; //Base Look Vector
  
};
#endif //!defined(AFX_CXCamera_H__)

//-----------------------------------------------------------------------------