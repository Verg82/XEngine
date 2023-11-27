{*******************************************************************************
FR_NetworkList.pas It's A FRAGMA Network List Module.
General Development: Max Tantser (Native Soft Inc.)
Date Creation: 7 March 2011
Last Changing Date: 7 March 2011
All Rights Reserved By: (C)Native Soft Inc.,
Licensed: (C)DDD Digital Dreams Development Inc., 2011
*******************************************************************************}
unit FR_NetworkList;
interface uses WinTypes,Forms,Controls,Graphics,ComCtrls,TsaPanel2;

////////////////////////////////////////////////////////////////////////////////
// Structures And Prototypes
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Name: Struct TNetworkList
// Desc:
//------------------------------------------------------------------------------
type
  TNetworkList = class
  public
    {Public Directives}
    constructor Create(Control: TWinControl); overload;
    destructor Destroy(); overload;
    procedure Release();


  private
    {Private Directives}
    m_pPanel: TTsaPanel; //TSA Layer Panel
    m_pList: TListView; //Network Socket Connection List

    Img_Path: string; //Images Folder Path

  protected
    {Protected Directives}
  published
    {Published Directives}
end;

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
var NetworkList: TNetworkList = nil; //Global Network List Interface

implementation uses SkinEngine_dll,Tsa_API,FR_NetworkMap,   FR_ServMethods;

////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TNetworkList Class
////////////////////////////////////////////////////////////////////////////////
constructor TNetworkList.Create(Control: TWinControl);
begin
  //Set Default Parameters
  m_pPanel:= nil;
  m_pList:= nil;

  Img_Path:= ExtractFilePath(ParamStr(0))+'Profiles\Images\';

  //Create TSA Layer Panel
  m_pPanel:= TTsaPanel.Create(nil);
  m_pPanel.Parent:= Control;
  m_pPanel.Animated:= False;
  m_pPanel.BorderColor:= $006A595A;
  m_pPanel.HeaderColor:= $0089502E;
  m_pPanel.Color:= clBtnFace; //$00303030;
  //m_pPanel.HeaderFont.Name:= 'Lucida Console';
  m_pPanel.HeaderFont.Color:= clWhite;
  m_pPanel.HeaderFont.Style:= [];
  m_pPanel.HeaderCaption:= 'Network sockets connection list:';
  m_pPanel.HeaderHeight:= 16;
  m_pPanel.HeaderMinColor:= $0089502E;
  m_pPanel.HeaderVisible:= True;
  m_pPanel.Moveable:= True;
  m_pPanel.Sizeable:= True;
  m_pPanel.Transparent:= False;
  m_pPanel.Top:= 100;
  m_pPanel.Left:= 100;
  m_pPanel.Height:= 300;
  m_pPanel.Width:= 600;

  //Set Header Gradient
  m_pPanel.GradientFrom:= $006A6A6A;
  m_pPanel.GradientTo:= $00303030;
  m_pPanel.GradientStyle:= gsTop;
  m_pPanel.UseGradient:= True;

  m_pPanel.HeaderIcon:= Img_Path+'ConRDP.bmp';

  //Create Close Button
  m_pPanel.CloNormal:= Img_Path+'Btn_UpCloseN.bmp';
  m_pPanel.CloSelect:= Img_Path+'Btn_UpCloseS.bmp';
  m_pPanel.CloPress:= Img_Path+'Btn_UpCloseP.bmp';
  m_pPanel.CloseButton:= True;

  //Set Mover
  m_pPanel.PicMover:= Img_Path+'Mover.bmp';
  m_pPanel.UseMover:= True;

  //Show Bottom Line
  m_pPanel.BottomLine:= False;
  m_pPanel.Visible:= True;


  //Create Network Socket Connection List
  m_pList:= TListView.Create(nil);
  m_pList.Parent:= m_pPanel;
  //m_pList.Align:= alClient;
  m_pList.ViewStyle:= vsReport;
  m_pList.BorderStyle:= bsNone;
  m_pList.Ctl3D:= False;
  m_pList.FlatScrollBars:= True;
  m_pList.ColumnClick:= False;
  m_pList.GridLines:= False;
  m_pList.ReadOnly:= True;
  m_pList.RowSelect:= True;
  m_pList.Font.Color:= clSkyBlue;
  m_pList.Font.Name:= 'Arial';
  m_pList.Font.Size:= 7;
  m_pList.Color:= //$00F3F1F2; //$00585858; //
  $003E1100;
  m_pList.Top:= m_pPanel.HeaderHeight+1;
  m_pList.Left:= 2;
  m_pList.Height:= m_pPanel.Height-m_pPanel.HeaderHeight-4-12;
  m_pList.Width:= m_pPanel.Width-4;
  m_pList.Anchors:= [akLeft,akTop,akRight,akBottom];

//  m_pList.SmallImages:= m_pImgList;
//  m_pRDPList.OnDblClick:= RDPDblClick;

  m_pList.Columns.Add();
  m_pList.Columns[0].Caption:= 'Index:';
//  m_pList.Columns[0].Width:= ColWidth[0];
  m_pList.Columns.Add();
  m_pList.Columns[1].Caption:= 'Connect Index:';
//  m_pList.Columns[1].Width:= ColWidth[1];
  m_pList.Columns.Add();
  m_pList.Columns[2].Caption:= 'Map Number:';
//  m_pList.Columns[2].Width:= ColWidth[2];
  m_pList.Columns.Add();
  m_pList.Columns[3].Caption:= 'Map Object:';
//  m_pList.Columns[3].Width:= ColWidth[3];
  m_pList.Columns.Add();
  m_pList.Columns[4].Caption:= 'Internal Name:';
//  m_pList.Columns[4].Width:= ColWidth[4];
  m_pList.Columns.Add();
  m_pList.Columns[5].Caption:= 'Route IP:';
//  m_pList.Columns[5].Width:= ColWidth[5];
  m_pList.Columns.Add();
  m_pList.Columns[6].Caption:= 'Route Host:';
//  m_pList.Columns[6].Width:= ColWidth[6];
  m_pList.Columns.Add();
  m_pList.Columns[7].Caption:= 'Local IP:';
//  m_pList.Columns[7].Width:= ColWidth[7];
  m_pList.Columns.Add();
  m_pList.Columns[8].Caption:= 'Local Host:';
//  m_pList.Columns[8].Width:= ColWidth[8];
  m_pList.Columns.Add();
  m_pList.Columns[9].Caption:= 'MAC:';
//  m_pList.Columns[9].Width:= ColWidth[9];
  m_pList.Columns.Add();
  m_pList.Columns[10].Caption:= 'Connect State:';
//  m_pList.Columns[10].Width:= ColWidth[10];
  m_pList.Columns.Add();
  m_pList.Columns[11].Caption:= 'Socket Handle:';
//  m_pList.Columns[11].Width:= ColWidth[11];

  m_pList.Visible:= True;
  
end; //EndConstructorDirectives

destructor TNetworkList.Destroy();
begin

end; //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: TNetworkList.Release()
// Desc:
//------------------------------------------------------------------------------
procedure TNetworkList.Release();
begin
  //Release Socket Connection List
  if (m_pList <> nil) then begin
  m_pList.Free();
  m_pList:= nil; end;

  //Release TSA Layer Panel
  if (m_pPanel <> nil) then begin
  m_pPanel.Free();
  m_pPanel:= nil; end;
end; //EndReleaseProcedure






//------------------------------------------------------------------------------

end.
 