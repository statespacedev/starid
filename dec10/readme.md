# dec10

target here is to get the f66 code in projects algebra and inverse running on dec10 in an 'ephemeral' way. the dec10 should boot, run the f66 code, and exit out.

# to make decwar go

uncomment 'do decwar.do' in disk-boot.ini.

it wants decwar.tap file to be in the tapes folder. can also generate that fresh using tape-decwar.sh

    noah@raspberrypi:~/starid/dec10 $ telnet localhost 2030
    Trying ::1...
    Connected to localhost.
    Escape character is '^]'.
    Connected to the KL-10 simulator TTY device, line 0
    KL703 11:01:21 TTY4 system 1025
    Connected to Node KL10(1) Line # 4
    Please LOGIN
    .login dist
    Job 3  KL703  TTY4
    11:01   19-Mar-86   Wednesday
    .r [10,7]decwar
    DECWAR, Edit    16
    [DECWAR Version 2.3, 20-Nov-81]
    Are you:  
    1 Beginner
    2 Intermediate 
    3 Expert  

# useful passwords

1,2 FAILSA
6,6 MAINT
7,7 OPER
10,* DIST
100,100 DEMO1
100,101 DEMO2

