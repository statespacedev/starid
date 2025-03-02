# ibm 7090/7094

target here is to get the f66 code in projects algebra and inverse running on 7094, as well as pdp10 and linux. and then hopefully add a project simplex with early lp code as well, again on all three 7094, pdp10, linux.

# notes

clone https://github.com/rcornwell/I7000tools.git
clone https://github.com/rcornwell/sims.git

    noah@raspberrypi:~/sims $ gmake i7090
    noah@raspberrypi:~/sims $ cp BIN/i7090 ~/starid/ibm7094/
    noah@raspberrypi:~/starid/ibm7094 $ ./i7090
    IBM 7090 simulator Open SIMH V4.1-0 Current        git commit id: 86bfb945
    sim> set cpu 7094
    sim> show config
    IBM 7090 simulator configuration
    CPU 7094, 32K

file job1.job example

    $JOB           LIST CONFIGURATION OF SYSTEM
    $UNITS
    $IBSYS
    $IBEDT
          *EDIT    MAP
    ~
    $IBSYS
    $EXECUTE       IBJOB
    $IBJOB IBLIB   NOGO,LOGIC,MAP
    $EDIT          LOGIC
    ~
    $IBSYS
    $STOP

tape

    mkbcdtape -o sysin.tp job1.job

file base.ini for running tape job

    set cpu 7090
    set cpu 32k
    at mta1 stage11.tp
    set mta1 locked
    at mta2 sysin.tp
    set mta2 locked
    at mta3 sysut1.tp
    at mtb3 sysut2.tp
    at mta4 sysut3.tp
    at mtb4 sysut4.tp
    at mtb1 sysou1.tp
    at mta5 sysck2.tp
    at mtb2 syspp1.tp
    at lp0 sys.log
    at cdp0 syspun.crd
    dep sw1 0

    i7090 base.ini
    ...snip...
    sim> boot mta1
    ...snip... job runs ...
    END OF JOBS
    HALT instruction, IC: 05370 ( -077400200144   AXC 00144,2)
    sim> quit

tape output

    listtape sysou1.tp

submit job via card reader

    attach cdr0 cardjob.job
    dep sw1 1
    boot mta1 or continue

submit via tape

    dep sw1 0
    boot mta1 or continue

file test.job to see tape drive assignments

 	$UNITS
    $PAUSE

'dep sw1 1' is 'deposit to sense switch 1' type of job, 1 for card job, 0 for tape job. 'bo mta1' is boot from tape drive mta1.

    at mta1 ../stage11.tp
    dep sw1 1
    at cdr0 test.job
    set lp0 echo
    at mtb1 junk.tp
    bo mta1
