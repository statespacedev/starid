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
        