{*******************************************************************************
// It's An Unofficial Version XEngine v.2.0.0.1
// XRenderClass.pas
// It's A XEngine 3D Renderer Control Unit.
// Programming By: Max Tantser (Verg)
// Date Creation: 22 April 2014
// Last Changing: 28 April 2014
// Author Rights By: Native Soft Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2015
*******************************************************************************}
unit XRenderClass;
interface uses WinTypes,Variants,Tsa_Helper;

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
const XEngineLib = 'XEngine.dll';

////////////////////////////////////////////////////////////////////////////////
// Structures And Prototypes
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Name: Struct TxRender
// Desc:
//------------------------------------------------------------------------------
type
  TxRender = class
  public
    {Public Directives}
    constructor Create(); overload;
    destructor Destroy(); overload;
    procedure Release();

    function Init3D(): Bool;
  private
    //Private Directives
    FWndOutput: HWnd;      //Graphics Output Window Handle
    FWndInput: HWnd;       //Input Devices Window Handle
    Tmr_Update: TTsaTimer; //Render Update Timer
    FWidth: Integer;       //Screen Width
    FHeight: Integer;      //Screen Height
    FWindowed: Bool;       //Windowed Mode
    FUseCamera: Bool;      //Use Camera State

    procedure OnRenderUpdate(Sender: TObject);
    procedure SetWindowed(const Value: Bool);
    procedure SetUseCamera(const Value: Bool);
  protected
    //Protected Directives
  published
    //Published Directives
    property Output: HWnd read FWndOutput write FWndOutput;
    property Input: HWnd read FWndInput write FWndInput;
    property Height: Integer read FHeight write FHeight;
    property Width: Integer read FWidth write FWidth;
    property Windowed: Bool read FWindowed write SetWindowed;
    property UseCamera: Bool read FUseCamera write SetUseCamera;
end;

//------------------------------------------------------------------------------
//XEngine Directives
procedure XEngineInit(WndOut,WndInput: HWND) cdecl; external XEngineLib;
function AcceptDirect3D(): HResult; cdecl; external XEngineLib;
function AcceptOpenGL(): HResult; cdecl; external XEngineLib;
procedure RenderDirect3D(); cdecl; external XEngineLib;
procedure RenderOpenGL(); cdecl; external XEngineLib;
procedure ReleaseDirect3D(); cdecl; external XEngineLib;
procedure SetWindowedMode(Windowed: Bool); cdecl; external XEngineLib;
procedure SetScreenResolution(Width,Height: Integer); cdecl; external XEngineLib;
procedure ConnectCamera(State: Bool); cdecl; external XEngineLib;
//------------------------------------------------------------------------------

implementation

////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TxRender Class
////////////////////////////////////////////////////////////////////////////////
constructor TxRender.Create();
begin
  //Set Default Parameters
  Tmr_Update:= nil; //Null Render Update Timer By Default
  FWidth:= 640;     //Set Screen Width By Default
  FHeight:= 480;    //Set Screen Height By Default
  FWindowed:= True; //Set Windowed Mode By Default

end; //EndConstructorDirectives

destructor TxRender.Destroy();
begin

end; //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: TxRender.Release()
// Desc:
//------------------------------------------------------------------------------
procedure TxRender.Release();
begin
  //Release Render Update Timer
  if (Tmr_Update <> nil) then begin
  Tmr_Update.Enabled:= False;
  Tmr_Update.Free();
  Tmr_Update:= nil; end;
  
  //Release Direct3D
  ReleaseDirect3D();
end; //EndReleaseProcedure

//------------------------------------------------------------------------------
// Name: TxRender.SetWindowed()
// Desc:
//------------------------------------------------------------------------------
procedure TxRender.SetWindowed(const Value: Bool);
begin
  FWindowed:= Value;
  if (not FWindowed) then begin
  if (FWndInput <> 0) then FWndOutput:= FWndInput; end;
end; //EndSetWindowedProcedure

//------------------------------------------------------------------------------
// Name: TxRender.SetUseCamera()
// Desc:
//------------------------------------------------------------------------------
procedure TxRender.SetUseCamera(const Value: Bool);
begin
  //Connect To Camera Object
  ConnectCamera(Value);
end; //EndSetUseCameraProcedure

//------------------------------------------------------------------------------
// Name: TxRender.OnRenderUpdate()
// Desc:
//------------------------------------------------------------------------------
procedure TxRender.OnRenderUpdate(Sender: TObject);
begin
  //Pool Render Scene Update
  RenderDirect3D();
end; //EndOnRenderUpdateProcedure

//------------------------------------------------------------------------------
// Name: TxRender.Init3D()
// Desc:
//------------------------------------------------------------------------------
function TxRender.Init3D(): Bool;
begin
  //Set Default Parameters
  Result:= False;

  //Before Release Direct3D If It's Startup
  ReleaseDirect3D();

  if (FWndOutput <> 0) then begin
  //XEngine Interface Initialization
  XEngineInit(FWndOutput,FWndInput);

  //Set Direct3D Parameters
  SetScreenResolution(FWidth,FHeight);
  SetWindowedMode(FWindowed);

  //Create Direct3D Interface
  if (AcceptDirect3D() <> NULL) then begin

  //Create Render Update Timer
  if (Tmr_Update = nil) then begin
  Tmr_Update:= TTsaTimer.Create(nil);
  Tmr_Update.Interval:= 1;
  Tmr_Update.OnTimer:= OnRenderUpdate; end;

  //Start Direct3D Rendering Process
  if (Tmr_Update <> nil) then Tmr_Update.Enabled:= True;

  Result:= True; end; end;
end; //EndInit3DFunction

//------------------------------------------------------------------------------

end.
