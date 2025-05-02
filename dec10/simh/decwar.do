expect "\r\n." send -t after=1000k "login 10,7\r"; continue
;expect "\r\n." send "r credir\r"; continue
;expect "Create directory:" send "[,,decwar]/prot:755\r"; continue
;expect "Create directory:" send "\3"; continue

expect "\r\n." send "r backup\r"; at %TAP%0 -r tapes/decwar.tap;  continue
expect "\r\n/" send "tape mta0:\r"; continue
expect "\r\n/" send "rewind\r"; continue
expect "\r\n/" send "inter\r"; continue
expect "\r\n/" send "files\r"; continue
;expect "\r\n/" send "rest [,,decwar]*.*=*.*\r"; continue
expect "\r\n/" send "rest [,]*.*=*.*\r"; continue
expect "\r\n/" send "exit\r"; continue

;expect "\r\n." send "r setsrc\r"; continue
;expect "\r\n*" send "cp [10,7,decwar]\r"; continue
;expect "\r\n*" send "\3"; continue

expect "\r\n." send "compile @com1\r"; continue
expect "\r\n." send "compile @com2\r"; continue
expect "\r\n." send "r link\r"; continue
expect "\r\n*" send "@can1\r"; continue

expect "\r\n." send ""; continue ; workaround to make prev line ok
; may hang here, hit return

expect "\r\n." send "get decwar\r"; continue
expect "\r\n." send "ssave\r"; continue
expect "\r\n." send "k/f\r"; continue

expect "\r\n." send -t after=1000k "login 1,2\r"; continue
expect "\r\n." send "r credir\r"; continue
expect "Create directory:" send "[1,27]/prot:777\r"; continue
expect "Create directory:" send "\3"; continue
;expect "\r\n." send "copy sys:<055>=dskb:[10,7,DECWAR]*.exe\r"; continue
;expect "\r\n." send "copy hlp:<055>=dskb:[10,7,DECWAR]*.hlp\r"; continue
expect "\r\n." send "k/f\r"; continue

