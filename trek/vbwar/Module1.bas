Attribute VB_Name = "Module1"
Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Global inpline$(1024)
Global crlf$
Global semephore

Rem
Rem Calc:   DW was 79*79 = 6241 spaces
Rem         DW had 16 users = 6241/16 = 390 per ship
Rem         This game can have 1024 players = 390*1024 = 399360
Rem         So X, Y = square root of 399360 = 632 * 632
Rem
Rem         DW was 16 players for 10 bases = 10/16 =  0.625
Rem         This game has 1024 players = 1024 *  0.625 = 640 bases!
Rem
Rem         DW had 20-80 planets for 6241 spaces
Rem
Rem     Constants
Rem
Global Const knplay = 1024:     Rem max # players
Global Const knbase = 640:      Rem initial number of bases
Global Const knplnt = 3262:     Rem number of planets
Global Const nroms = 64:        Rem number of romulans
Global Const nborg = 64:        Rem number of borg in the game
Global Const nstars = 3262:     Rem number of stars
Global Const nholes = 3262:     Rem Number of bholes
Global Const kndev = 9:         Rem number of devices
Global Const kgalv = 632:       Rem max vertical coordinates
Global Const kgalh = 632:       Rem max horizontal coordinates
Global Const knrange = 10:      Rem max range for scan, phasers and torps
Global Const kcrit = 3000:      Rem amount of device damage deemed critical
Global Const kendam = 25000:    Rem amount of ship damage deemed fatal

Rem ****************************
Rem obj # 1 thru 1024 = Human ships     qty 1024
Rem obj # 1025 thru 1665 = bases        qty 640
Rem obj #1666 thru 4928 = planets       qty 3262
Rem obj #4929 thru 8191 = black holes   qty 3262
Rem obj #8192 thru 8256 = roms          qty 64
Rem obj #8257 thru 8321 = borg          qty 64
Rem obj #8322 thru 11584 = stars        qty 3262
Rem
Rem Counters/databases
Rem
Global nbase(2):                Rem Number of bases for each side
Global Univxref(632, 632):      Rem Universe database
Global Objcon(11584, 13):       Rem Object database

Rem
Rem OBJCON indeces
Rem
Global Const kvpos = 1:     Rem 1=V coordinate of ships, etc.
Global Const khpos = 2:     Rem 2=H coordinate of ship, etc.
Global Const knturn = 3:    Rem 3=number of turns player has taken
Global Const kspcon = 4:    Rem 4=condition of ship (1=green, 2 = yellow,3=red
Global Const kntorp = 5:    Rem 5=number of torps remaining
Global Const kshcon = 6:    Rem 6=condition of shields (1=up, -1=down)
Global Const klfsup = 7:    Rem 7=life support reserves in stardates
Global Const ksnrgy = 8:    Rem 8=units of ship energy remaining
Global Const ksdam = 9:     Rem 9=amount of ship damage
Global Const ksshpc = 10:   Rem 10=shield %
Global Const ksttyf = 11:   Rem 11=ICDEF/OCDEF/SUPERUSER/side/logged on/visitor flags
Global Const kshdam = 12:   Rem 12 = ship damage
Global Const ksseen = 13:   Rem 13 = who has seen this obj

Rem
Rem Score indeces
Rem
Global Const kpedam = 1:    Rem 1=points for damage to enemy
Global Const kpekil = 2:    Rem 2=points for enemies killed
Global Const kpbdam = 3:    Rem 3=points for damage to bases
Global Const kppcap = 4:    Rem 4=points for planets captured
Global Const kpbbas = 5:    Rem 5=points for building bases
Global Const kprkil = 6:    Rem 6=points for Romulans killed
Global Const knsdes = 7:    Rem 7=numbers of stars destroyed
Global Const knpdes = 8:    Rem 8=number of planets destroyed

Rem
Rem SHPDAM (objcon 12) indeces
Rem
Global Const kdshld = 1:    Rem 1=damage to shields
Global Const kdwarp = 2:    Rem 2=damage to warp engines
Global Const kdimp = 3:     Rem 3=damage to impulse engines
Global Const kdlife = 4:    Rem 4=damage to life support
Global Const kdtorp = 5:    Rem 5=damage to photon torpedo tubes
Global Const kdphas = 6:    Rem 6=damage to phasers
Global Const kdcomp = 7:    Rem 7=damage to computer
Global Const kdrad = 8:     Rem 8=damage to radio

Rem Bits used by list
Global Const namebit = 1:       Rem ship name
Global Const portbit = 2:       Rem port
Global Const shipbit = 4:       Rem ship
Global Const basebit = 8:       Rem base
Global Const plntbit = 16:      Rem planet
Global Const coorbit = 32:      Rem coordinate
Global Const fedbit = 64:       Rem federation
Global Const empbit = 128:      Rem empire
Global Const rombit = 256:      Rem romulan
Global Const neubit = 512:      Rem neutral
Global Const capbit = 1024:     Rem captured
Global Const allbit = 2048:     Rem all
Global Const rngbit = 4096:     Rem range
Global Const clsbit = 8192:     Rem closest
Global Const sumbit = 16384:    Rem summary
Global Const lstbit = 32768:    Rem list
Global Const outmsk = 65536:    Rem summary list
Global Const irnbit = 1:        Rem in range
Global Const isrbit = 2:        Rem in specified range
Global Const igmbit = 4:        Rem in game
Global Const ornbit = 8:        Rem out of range
Global Const pasbit = 16:       Rem selected only because password is set
Global Const knobit = 32:       Rem only known about because it's been scanned
Global Const sidmsk = 1984:     Rem sidmsk fed emp neu rom and cap

Rem
Rem ship conditions
Rem
Global Const green = 1
Global Const yellow = 2
Global Const red = 3

Rem
Rem OBJCON Flag 11
Rem
Global Const Short = 1:         Rem oflg flag
Global Const medium = 2:        Rem oflg flag
Global Const lng = 4:           Rem oflg flag
Global Const krel = 8:          Rem icflg/ocflg flag
Global Const kabs = 16:         Rem icflg/ocflg flag
Global Const kboth = 32:        Rem icflg/ocflg flag
Global Const Suprusron = 64:    Rem Superuser flag
Global Const pregme = 128:      Rem Pregame flag
Global Const lgged = 256:       Rem Logged on (paying) customer
Global Const visiting = 512:    Rem visiting only (non-paying) customer
Global Const neutral = 1024:    Rem Player is neutral
Global Const fed = 2048:        Rem Object is federation
Global Const emp = 4096:        Rem object is empire
Global Const rom = 8192:        Rem obj is romulan
Global Const borg = 16384:      Rem obj is borg

Rem
Rem list command
Rem
Global Const lstcmd = 0
Global Const sumcmd = 1
Global Const bascmd = 2
Global Const plncmd = 3
Global Const tarcmd = 4

Rem
Rem constants for display
Rem
Global Const dxmpty = 0
Global Const dxfshp = 1
Global Const dxeshp = 2
Global Const dxfbas = 3
Global Const dxebas = 4
Global Const dxrom = 5
Global Const dxnpln = 6
Global Const dxfpln = 7
Global Const dxepln = 8
Global Const dxstar = 9
Global Const dxbhol = 10


Public Sub endlock(connectionid)
semephore = False
End Sub

Public Sub help(connectionid, Cmdprm$)
Form1.Text1.Text = Form1.Text1.Text + "Got into help:" + Cmdprm$ + crlf$
     
Rem
Rem Do this for all parms supplied
Rem

Call Module1.Parse(Cmdprm$, Linelesscmd$, Thecmd$)
cmdprm1$ = Thecmd$
Cmdprm$ = Linelesscmd$
lprm = Len(cmdprm1$)
gotone = 0

Rem
Rem handle no parm
Rem
If (cmdprm1$ = "") Then
    cmdprm1$ = "commands"
    lprm = Len(cmdprm1$)
End If

While (cmdprm1$ <> "")

If Left$(cmdprm1$, lprm) = Left$("bases", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a bases in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("build", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a build in help1" + crlf$
gotone = 1

End If

If Left$(cmdprm1$, lprm) = Left$("capture", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a capture in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("damages", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a damages in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("dock", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a dock in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("energy", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a energy in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("gripe", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a gripe in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("help", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a help in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("impulse", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a impulse in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("list", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a list in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("move", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a move in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("news", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a news in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("phasers", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a phasers in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("planets", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a planets in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("points", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a points in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("quit", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a quit in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("radio", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a radio in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("repair", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a repair in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("scan", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a scan in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("set", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a set in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("shields", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a shields in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("srscan", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a srscan in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("status", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a status in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("summary", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a summary in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("targets", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a targets in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("tell", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a tell in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("time", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a time in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("torpedoes", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a torpedoes in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("tractor", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a tractor in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("type", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a type in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("users", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a users in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("login", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a login in help1" + crlf$
gotone = 1
End If

If Left$(cmdprm1$, lprm) = Left$("visit", lprm) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Got a visit in help1" + crlf$
gotone = 1
End If

If (Left$(cmdprm1$, lprm) = Left$("commands", lprm)) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Valid COmmands are:" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "BAses       HElp        QUit        STatus      TRactor" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "BUild       IMpulse     RAdio       SUmmary     TYpe" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "CApture     LIst        REpair      TArgets     USers" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "DAmages     NEws        SCan        TEll        COmmands" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "DOck        PHasers     SEt         TIme        LOgin" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "ENergy      PLanets     SHields     TOrpedoes   VIsit" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = "GRipe       POints      SRscan" + crlf$
Form1.IPDaemon1.DataToSend(connectionid) = crlf$
gotone = 1
End If

If (gotone = 0) Then
Form1.IPDaemon1.DataToSend(connectionid) = "Invalid parameter:" + cmdprm1$ + crlf$
gotone = 1
End If

Call Module1.Parse(Cmdprm$, Linelesscmd$, Thecmd$)
cmdprm1$ = Thecmd$
Cmdprm$ = Linelesscmd$
lprm = Len(cmdprm1$)
gotone = 0
Wend

Rem
Rem This is the prompt routine, needs to come after all messages are handled for a command
Rem
Module1.Prompt (connectionid)

End Sub
Public Sub login(connectionid)
Rem
Rem This should add stuff for a pay version eventually
Rem
If (pregme = (Objcon(connectionid, ksttyf) And pregme)) Then
    wait4lock (connectionid)
    Objcon(connectionid, ksttyf) = Objcon(connectionid, ksttyf) Xor pregme Xor lgged
    endlock (connectionid)
End If

Rem
Rem This is the prompt routine, needs to come after all messages are handled for a command
Rem
Module1.Prompt (connectionid)

End Sub

Public Sub Parse(Linesent$, Linelesscmd$, Thecmd$)
a = InStr(1, Linesent$, " ")
If (a > 0) Then
    Thecmd$ = Mid$(Linesent$, 1, a - 1)
    Linelesscmd$ = Mid$(Linesent$, a + 1)
Else
    If Linesent$ <> "" Then
        If (Len(Mid$(Linesent$, 1, Len(Linesent$))) > 2) Then
            Thecmd$ = Mid$(Linesent$, 1, Len(Linesent$) - 2)
        Else
            Thecmd$ = Mid$(Linesent$, 1, Len(Linesent$))
        End If
            
        Linelesscmd$ = ""
    Else
        Thecmd$ = ""
    End If
End If

End Sub

Public Sub ParseCmd(connectionid, Cmdline$)

Rem
Rem Check for command valadity
Rem First find the first space in the commandline
Rem Then see if it matches (to uniqueness) a command
Rem

Rem
Rem Find the first space, if there are parms, pull them out
Rem

lcmd = InStr(1, Cmdline$, " ")

If lcmd = 0 Then
    lcmd = Len(Cmdline$) - 2
    Cmdprm$ = ""
Else
    lcmd = lcmd - 1
    Cmdprm$ = Mid$(Cmdline$, lcmd + 2)
End If

If lcmd < 1 Then
    lcmd = 1
    End If
    
Rem
Rem Here is where we parse
Rem

If (Left$(Cmdline$, lcmd) = Left$("bases", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a bases in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("build", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a build in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("capture", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a capture in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("damages", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a damages in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("dock", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a dock in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("energy", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a energy in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("gripe", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a gripe in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("help", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a help in parse" + crlf$
Call Module1.help(connectionid, Cmdprm$)
End If

If (Left$(Cmdline$, lcmd) = Left$("impulse", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a impulse in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("list", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a list in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("move", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a move in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("news", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a news in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("phasers", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a phaser in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("planets", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a planets in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("points", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a points in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("quit", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a quit in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("radio", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a radio in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("repair", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a repair in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("scan", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a scan in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("set", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a set in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("shields", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a shields in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("srscan", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a srscan in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("status", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a status in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("summary", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a summary in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("targets", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a targets in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("tell", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a tell in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("time", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a time in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("torpedoes", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a torpedoes in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("tractor", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a tractor in parse" + crlf$
End If

If (Left$(Cmdline$, lcmd) = Left$("type", lcmd) And (pregme <> (Objcon(connectionid, ksttyf) And pregme))) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a type in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("users", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a users in parse" + crlf$
End If

If Left$(Cmdline$, lcmd) = Left$("login", lcmd) Then
login (connectionid)
End If

If Left$(Cmdline$, lcmd) = Left$("visit", lcmd) Then
Form1.Text1.Text = Form1.Text1.Text + "Got a visit in parse" + crlf$
End If

End Sub



Public Sub place(Obj, v, h)
Rem   This routine is used to RANDOMLY position objects in the
Rem   universe.  It can position one or many such objects in a
Rem   single call.  It is called by SETUP and ROMDRV.

Rem
Rem Fist lock the database
Rem
Call wait4lock(connectionid)

Rem
Rem Find a spot to place it
Rem
k = False
While (Not k)
    v = Int((kgalv * Rnd) + 1)
    h = Int((kgalh * Rnd) + 1)
    If (Univxref(v, h) = 0) Then
        Univxref(v, h) = Obj
        Objcon(Obj, kvpos) = v
        Objcon(Obj, khpos) = h
        k = True
    End If
Wend
    
Rem
Rem Clear the lock
Rem
Call endlock(connectionid)
End Sub

Public Sub Prompt(connectionid)
Rem
Rem Prompting: Normal = Command: or Pregame:
Rem Prompting: Informative =    S> shields down
Rem                             E> energy is less than 1000
Rem                             D> Ship damage > 2000 units
Rem                             nL> life support is damaged with n stardates reserve

If (pregme = (Objcon(connectionid, ksttyf) And pregme)) Then
    Form1.IPDaemon1.DataToSend(connectionid) = "Pregame: "
Else
    Form1.IPDaemon1.DataToSend(connectionid) = "Command: "
End If
End Sub



Public Sub Startup()
Rem
Rem Initialize the bases
Rem
For j = knplay + 1 To knplay + 1 + knbase
    Call place(j, Objcon(j, kvpos), Objcon(j, khpos))
    Call wait4lock(connectionid)
    Objcon(j, knturn) = 0
    Objcon(j, kspcon) = 1
    Objcon(j, kntorp) = 0
    Objcon(j, kshcon) = 1
    Objcon(j, klfsup) = 0
    Objcon(j, ksnrgy) = 5000
    Objcon(j, ksdam) = 0
    Objcon(j, ksshpc) = 100
    If (j > (knplay + 1 + (knbase / 2))) Then
        Objcon(j, ksttyf) = fed
    Else
        Objcon(j, ksttyf) = emp
    End If
    Call endlock(connectionid)
Next j

Rem
Rem Setup # of bases for fed and empire at startup
Rem
nbase(1) = knbase / 2
nbase(2) = knbase / 2

Rem
Rem Place stars out there
Rem
For j = knplay + knbase + 1 To knplay + knbase + nstars + 1
    Call place(j, Objcon(j, kvpos), Objcon(j, khpos))
    Call wait4lock(connectionid)
    Objcon(j, knturn) = 0
    Objcon(j, kspcon) = 1
    Objcon(j, kntorp) = 0
    Objcon(j, kshcon) = 1
    Objcon(j, klfsup) = 0
    Objcon(j, ksnrgy) = 5000
    Objcon(j, ksdam) = 0
    Objcon(j, ksshpc) = 100
    Objcon(j, ksttyf) = neu
    Call endlock(connectionid)
Next j

Rem
Rem place the planets out there
Rem
For j = knplay + knbase + nstars + 1 To knplay + knbase + nstars + knplnt + 1
    Call place(j, Objcon(j, kvpos), Objcon(j, khpos))
    Call wait4lock(connectionid)
    Objcon(j, knturn) = 0
    Objcon(j, kspcon) = 1
    Objcon(j, kntorp) = 0
    Objcon(j, kshcon) = 1
    Objcon(j, klfsup) = 0
    Objcon(j, ksnrgy) = 5000
    Objcon(j, ksdam) = 0
    Objcon(j, ksshpc) = 100
    Objcon(j, ksttyf) = neu
    Call endlock(connectionid)
Next j


Rem
Rem place black holes out there
Rem

End Sub
Public Sub visit(connectionid)
Rem
Rem This should add stuff for a nonpay version eventually
Rem
If (pregme = (Objcon(connectionid, ksttyf) And pregme)) Then
    wait4lock (connectionid)
    Objcon(connectionid, ksttyf) = Objcon(connectionid, ksttyf) Xor pregme Xor visiting
    endlock (connectionid)
End If

Rem
Rem This is the prompt routine, needs to come after all messages are handled for a command
Rem
Module1.Prompt (connectionid)

End Sub

Public Sub wait4lock(connectionid)
While (semephore = True)
Rem
Rem This seems that it is not required as a result
Rem of the sync nature of the ipdaemon
Rem program - only 1 command will actually be run at once
Rem    Call Sleep(10)
Rem
Wend
semephore = True
End Sub

