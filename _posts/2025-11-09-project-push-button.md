---
layout: post
title: "project push button"
---

the objective here is a simple push button 'turn key and go' way to run the primordial utexas decwar and have as much fun as possible. that means a pdp10-kl simulator running all of the original sofware from tape images, and able to build decwar on the fly at every boot with modifications to the original fortran and mac10 flowing in smoothly. to really have fun there have to be other players. the more the better. robots have to be telnetting in and playing. that's done with python. and then, even more fun, is a graphical display of the whole galaxy. ideally with sounds as well. that's done via webbrowser using p5js javascript. here's where things are at the moment.

    https://gitlab.com/decwar/utexas - pure stone age
    https://gitlab.com/decwar/merely-players - pure python robots and 'cic' api
    https://gitlab.com/decwar/galaxy - pure p5js display calling 'cic' api

how to unify these three forces? doco. [docker compose](https://docs.docker.com/compose/)

the project push button concept is that doco is the unifier, controller, automator, and interface for casual play. everything should be reduced as much as possible to a matter of doco usage. of course in reality there will always be more frictions and bumps to smooth out. this is not a simple system, and on top of the three forces shown above, doco is effectively a fourth force. why all the complexity.

for one thing, the system should transparently move across intel, amd, arm, cloud, not to mention future hardware. and the usual software permutations of linux, windows, mac. in particular, at the moment the system is being used on a raspi and a macbook. the only thing the two have in common is the three gitlab repos. doco is available on both and used to run the game. it's now standard to have two simultaneous decwar games going on with ten robot players in each. one on the raspi, the other on the macbook.

for another thing, keeping all the forces separate is better for advancing and maintaining the code. stone age, python, and p5js are completely different planets, if not even different galaxies. there's effectively zero overlap between the three, and zero need to know about the details within the other two in order to work on one. someone interested in the original fortran and assembly language can put all their effort into the utexas repo. another person interested in visual displays can put all their effort into the galaxy repo.
