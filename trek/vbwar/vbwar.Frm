VERSION 4.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   4335
   ClientLeft      =   7575
   ClientTop       =   1620
   ClientWidth     =   5445
   Height          =   4740
   Left            =   7515
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4335
   ScaleWidth      =   5445
   Top             =   1275
   Width           =   5565
   Begin VB.TextBox Text1 
      Height          =   2295
      Left            =   360
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Top             =   1920
      Width           =   4695
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Start VBWars"
      Height          =   375
      Left            =   2400
      TabIndex        =   0
      Top             =   1080
      Width           =   1575
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Stop VBWars"
      Height          =   375
      Left            =   2400
      TabIndex        =   1
      Top             =   1560
      Width           =   1455
   End
   Begin VB.Label Label3 
      Height          =   255
      Left            =   2760
      TabIndex        =   5
      Top             =   720
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "Host Name:"
      Height          =   255
      Left            =   1800
      TabIndex        =   4
      Top             =   720
      Width           =   855
   End
   Begin VB.Label Label1 
      Caption         =   "Visual Basic Wars"
      BeginProperty Font 
         name            =   "Times New Roman"
         charset         =   0
         weight          =   700
         size            =   14.25
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1560
      TabIndex        =   2
      Top             =   120
      Width           =   2775
   End
   Begin IPDaemonLib.IPDaemon IPDaemon1 
      Left            =   5520
      Top             =   120
      _version        =   131072
      _extentx        =   741
      _extenty        =   741
      _stockprops     =   0
      localport       =   1703
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False



Private Sub DSSocket1_Connect()
End Sub


Private Sub Form_Load()
Form1.Caption = "VBWars"
Text1.Text = ""
crlf$ = Chr$(13) + Chr$(10)

End Sub

Private Sub IPDaemon1_Connected(connectionid As Integer, StatusCode As Integer, Description As String)
Text1.Text = Text1.Text + "Connected to connection:" + Str(connectionid) + " status = " + Str(StatusCode) + " from Host:" + Form1.IPDaemon1.RemoteHost(connectionid) + " Port:" + Str(Form1.IPDaemon1.RemotePort(connectionid)) + crlf$
IPDaemon1.DataToSend(connectionid) = "Welcome to VBWar" + crlf$
IPDaemon1.DataToSend(connectionid) = "Copyright 1997 Harris S. Newman Consulting" + crlf$
IPDaemon1.DataToSend(connectionid) = "Connection id:" + Str$(connectionid) + crlf$
IPDaemon1.DataToSend(connectionid) = "Connected from host:" + Form1.IPDaemon1.RemoteHost(connectionid) + crlf$
IPDaemon1.DataToSend(connectionid) = "Connected from port:" + " Port:" + Str(Form1.IPDaemon1.RemotePort(connectionid)) + crlf$ + crlf$
IPDaemon1.DataToSend(connectionid) = "Pregame commands available:" + crlf$
IPDaemon1.DataToSend(connectionid) = "HElp      GRipe" + crlf$
IPDaemon1.DataToSend(connectionid) = "LOgin     NEws" + crlf$
IPDaemon1.DataToSend(connectionid) = "USers     Quit" + crlf$
IPDaemon1.DataToSend(connectionid) = "Visit" + crlf$ + crlf$

Rem
Rem Setup the guy in the objcon database
Rem
wait4lock (connectionid)
Objcon(connectionid, kvpos) = -1
Objcon(connectionid, khpos) = -1
Objcon(connectionid, knturn) = 0
Objcon(connectionid, kspcon) = 1
Objcon(connectionid, kntorp) = ntorp
Objcon(connectionid, kshcon) = 1
Objcon(connectionid, klfsup) = 2000
Objcon(connectionid, ksnrgy) = 5000
Objcon(connectionid, ksdam) = 0
Objcon(connectionid, ksshpc) = 100
Objcon(connectionid, ksttyf) = lng Xor kboth Xor pregme
endlock (connectionid)

Rem
Rem Print the first prompt
Rem
Module1.Prompt (connectionid)

End Sub

Private Sub IPDaemon1_DataIn(connectionid As Integer, Text As String, EOL As Integer)
Rem Text1.Text = Text1.Text + "Connection:" + Str(ConnectionID) + "sent:" + Text + " eol=" + Str(EOL) + crlf$
inpline$(connectionid) = inpline$(connectionid) + Text

Rem
Rem Try to echo
Rem
On Error GoTo FlowControl

IPDaemon1.DataToSend(connectionid) = Text

FlowControl:
If Err = 25036 Then

Text1.Text = Text1.Text + "Doing flow control on connection:" + Str(connectionid) + "sent:" + Text + " eol=" + Str(EOL) + crlf$

bs% = IPDaemon1.BytesSent(connectionid)
 If bs% > 0 Then
  Text$ = Mid$(Text$, BytesSent% + 1)
  End If
  DoEvents
  Resume
Else
  If Err <> 0 Then
    Text1.Text = Text1.Text + "Serious error on Connection:" + Str(connectionid) + " Error is:" + Str(Err) + crlf$
    Beep
  End If
End If
  
Rem
Rem Did we get a line full?
Rem

If (Asc(Mid(inpline$(connectionid), Len(inpline$(connectionid)), 1)) = 10) Then
    thelth = Len(inpline$(connectionid))
    bgptr = 1


    Rem
    Rem First parse the line a delete backspace chars and their pred chars
    Rem step 1: Move 1st non bs to leftmost
    For i = 1 To thelth
        If Mid$(inpline$(connectionid), i, 1) = Chr$(8) Then
           Mid$(inpline$(connectionid), i, 1) = " "
        Else
            i = thelth
        End If
    Next i
            
    If Mid$(inpline$(connectionid), 1, 1) = Chr$(8) Then
        Mid$(inpline$(connectionid), 1, 1) = " "
    End If
        
    For i = 2 To thelth
        If Mid$(inpline$(connectionid), i, 1) = Chr$(8) Then
            Mid$(inpline$(connectionid), i - 1, thelth) = Mid$(inpline$(connectionid), i + 1, thelth)
            i = i - 2
        End If
    Next i
    
    Rem
    Rem 1st lower case everything
    Rem 2nd trim to eliminate spaces
    Rem
    inpline$(connectionid) = LCase(inpline$(connectionid))
    inpline$(connectionid) = Trim(inpline$(connectionid))
    
    Rem
    Rem Parse A COMMAND LINE Function
    Rem Then find the end of line: either physical or logical '/'
    Rem

    Rem parse for a '/'

    thelth = Len(inpline$(connectionid))
    bgptr = 1
    While (bgptr < thelth)
        ab = InStr(bgptr, inpline$(connectionid), "/", 0)
        If ab <> 0 Then
            If ab <> bgptr Then
                cmd$ = Mid$(inpline$(connectionid), bgptr, ab - bgptr)
                Call Module1.ParseCmd(connectionid, cmd$)
            End If
        Else
                If thelth <> bgptr + 1 Then
                    cmd$ = Mid$(inpline$(connectionid), bgptr, thelth - bgptr + 1)
                    Call Module1.ParseCmd(connectionid, cmd$)
                End If
        End If
        
        If ab = 0 Then
            bgptr = thelth
            inpline$(connectionid) = ""
        Else
            bgptr = ab + 1
        End If
    Wend
    
End If
End Sub


Private Sub IPDaemon1_Disconnected(connectionid As Integer, StatusCode As Integer, Description As String)
Text1.Text = Text1.Text + "Disconnected from connection:" + Str(connectionid) + " status = " + Str(StatusCode) + crlf$
End Sub


Private Sub IPDaemon1_Error(ErrorCode As Integer, Description As String)
Text1.Text = Text1.Text + "Error:" + Str(ErrorCode) + " Text = " + Description + crlf$
End Sub

Private Sub Option1_Click()
Text1.Text = ""
Text1.Text = Text1.Text + "Starting VBWar Server" + crlf$
Form1.IPDaemon1.WinsockLoaded = True
Form1.IPDaemon1.LocalPort = 1701
Form1.IPDaemon1.Linger = True
Form1.IPDaemon1.KeepAlive = True
Form1.IPDaemon1.Listening = True
Label3.Caption = Form1.IPDaemon1.LocalHost
Text1.Text = Text1.Text + "Winsock status:" + crlf$
Text1.Text = Text1.Text + "     " + Form1.IPDaemon1.WinsockInfo + crlf$
Text1.Text = Text1.Text + "     Sockets loaded = " + Str(Form1.IPDaemon1.WinsockLoaded) + crlf$
Text1.Text = Text1.Text + "     Max Datagram size = " + Str(Form1.IPDaemon1.WinsockMaxDatagramSize) + crlf$
Text1.Text = Text1.Text + "     Max # Sockets = " + Str(Form1.IPDaemon1.WinsockMaxSockets) + crlf$
Text1.Text = Text1.Text + "     " + Form1.IPDaemon1.WinsockStatus + crlf$

Rem
Rem Force the database to be unlocked
Rem
semephore = False

Rem
Rem Here is where the batch commands would go for setup of the universe
Rem
Call Module1.Startup

End Sub


Private Sub StartDaemon_KeyDown(KeyCode As Integer, Shift As Integer)
End Sub



Private Sub Check1_Click()
End Sub


Private Sub Check1_KeyDown(KeyCode As Integer, Shift As Integer)
End Sub


Private Sub Check1_KeyPress(KeyAscii As Integer)
End Sub

Private Sub Check1_KeyUp(KeyCode As Integer, Shift As Integer)
End Sub


Private Sub Command1_KeyPress(KeyAscii As Integer)
End Sub

Private Sub Option2_Click()
Text1.Text = Text1.Text + "Shutting down VBWar Server" + crlf$
Form1.IPDaemon1.Listening = False
Form1.IPDaemon1.WinsockLoaded = False
End
End Sub




