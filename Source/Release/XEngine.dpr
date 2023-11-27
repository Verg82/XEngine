program XEngine;

uses
  Forms,
  unXGUI in 'Methods\unXGUI.pas' {FRM_XGUI},
  XRenderClass in 'Methods\XRenderClass.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFRM_XGUI, FRM_XGUI);
  Application.Run;
end.
