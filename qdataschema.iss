; Inno Setup Installation Script.
; (c) Leader InfoTech, 2004-2007
; (c) Valery Grazdankin ( leader ), 2004-2007

[Languages]
Name: Russian; MessagesFile: "compiler:languages\Russian.isl"
Name: English; MessagesFile: "compiler:default.isl"

[Setup]
SourceDir=.\
AppName=QDataSchema
AppVerName=QDataSchema 1.2.0
AppPublisher=Leader IT
AppPublisherURL=http://www.leaderit.ru//files/qdataschema/
AppSupportURL=http://www.leaderit.ru/files/qdataschema/
AppUpdatesURL=http://www.leaderit.ru/files/qdataschema/updates
DefaultDirName={pf}\qdataschema
DefaultGroupName=QDdataSchema
AppCopyright=Copyright © 2002-2007 Leader InfoTech, Valery Grazdankin
DisableProgramGroupPage=yes
AllowNoIcons=yes
LicenseFile=COPYING
InfoBeforeFile=README
Compression=lzma
SolidCompression=yes
OutputDir=.\
OutputBaseFilename=qdataschema

[Components]
Name: "development"; Description: "library and header files"; Types: full compact custom
Name: "utilites"; Description: "utilites"; Types: full compact custom
Name: "doc"; Description: "Documentation"; Types: full compact custom

[Tasks]
;Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
;Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "setuplogo.bmp"; DestDir: {tmp}; Flags: dontcopy
;Source: "doc\*.pdf"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "src\lib\*.h";DestDir: "{app}\include"; Flags: ignoreversion
Source: "RELEASE"; DestDir: "{app}"; Flags: ignoreversion
Source: "README"; DestDir: "{app}"; Flags: ignoreversion
Source: "lib\*.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\example\qdsexample.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "src\utils\dump\qdsdump.exe"; DestDir: "{app}"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
[INI]
Filename: "{app}\lit.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.leaderit.ru"

[Icons]
;Name: "{group}\QDataSchema"; Filename: "{app}\ananas-engine.exe"
Name: "{group}\{cm:UninstallProgram,QDataSchema}"; Filename: "{uninstallexe}"; AfterInstall: qdataschemaAfterInstall
;Name: "{userdesktop}\Ananas Engine"; Filename: "{app}\ananas-engine.exe"; Tasks: desktopicon
;Name: "{userdesktop}\Ananas Administrator"; Filename: "{app}\ananas-administrator.exe"; Tasks: desktopicon
;Name: "{userdesktop}\Ananas Designer"; Filename: "{app}\ananas-designer.exe"; Tasks: desktopicon
;Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Ananas"; Filename: "{app}\ananas-designer.exe"; Tasks: quicklaunchicon
Name: "{group}\QDataSchema в интернете"; Filename: "{app}\lit.url"

[Run]
;Filename: "{app}\qds.exe"; Description: "{cm:LaunchProgram,Ananas}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\lit.url"

[Code]
//
procedure InitializeWizard();
var
  SplashImage: TBitmapImage;
  SplashForm: TForm;
  SplashFileName: String;
begin

  SplashFileName := ExpandConstant('{tmp}\setuplogo.bmp');
  ExtractTemporaryFile(ExtractFileName(SplashFileName));

  SplashForm := TForm.create(nil);
  with SplashForm do
  begin
    BorderStyle := bsNone;
    Position := poScreenCenter;
    // Adjust the height and width of the SplashForm
    // to the size of our splash image
    ClientWidth := 400;
    ClientHeight := 244;
  end;

  SplashImage := TBitmapImage.Create(SplashForm);
  with SplashImage do
  begin
    Bitmap.LoadFromFile(SplashFileName);
    Stretch := true;
    Align := alClient;
    Parent := SplashForm;
  end;

  with SplashForm do
  begin
     Show;
     Repaint;
     Sleep(3000);
     Close;
     Free;
  end;
end;


procedure qdataschemaAfterInstall();
begin
  { copy files }
//  FileCopy( ExpandConstant('{app}\designer\ananasplugin.dll'), ExpandConstant('{app}\ananasplugin.dll'), false);
end;

