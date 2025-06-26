---
layout: post
title: "utexas decwar"
---
thanks to merlyn cousins, there's a [working decwar](https://github.com/drforbin/decwar). it's a little different than the primordial utexas decwar however. [project utexas](https://github.com/drforbin/decwar/tree/utexas/utexas) is about reconstruction of the original, converging nearer and nearer towards the primordial code and effectively going backwards in time. if a copy of the original code is discovered, it will hopefully match closely with the utexas reconstruction.

the first reconstruction concerns where the game is installed. at utexas the players typed 'r gam:decwar' and the virtual device 'gam:' was assigned to ppn [5,30]. the install involved commands like 'copy [5,30]decwar.exe<055>=[10,7]decwar.exe' and 'assign gam: dskb:[5,30]'. the tape contents were originally copied into [10,30] and compiled and linked there, you can see that in this [primordial tape listing](https://github.com/drforbin/decwar/blob/master/utexas/utexas23-reconstruction/hlp/DECWAR.TAP). for now, project utexas is using [10,7] as it matches with install scripts for fortran, kermit, etc in the utexas simh folder.

compuserve changed the target install directory. they had players coming from various host machines across their network. they brought the players to a central machine with the game installed in [1,27] using commands like 'copy [1,27]decwar.exe<055>=[10,7]decwar.exe'. the players effectively did 'r [1,27]decwar', without a virtual device like 'gam:'.

it's a bit amazing and not completely clear yet why changing this back was so straightforward. problems were expected because compuserve additions to the source code do in fact reference [1,27]. it's possible that the current tops-10 configuration in project utexas has something to do with it. it's suspected that the monitor security system is in some sense not fully activated. parts of rcornwell's baseline simh scripts were intentionally left off for a more 'tmp/dev' environment. the objective is not a production tops-10 environment, rather a more loose and free sandbox. maybe that's why install in [5,30] 'just works'?

the second reconstruction is even more amazing. at utexas there were up to eighteen players. this is also what players at the southwest texas state university computation center, san marcos texas, saw circa 1983 and 1984

                 Federation ships               Empire ships
                 ----------------               ------------
                    Excalibur                     Buzzard
                    Farragut                      Cobra
                    Intrepid                      Demon
                    Lexington                     Goblin
                    Nimitz                        Hawk
                    Savannah                      Jackal
                    Trenton                       Manta
                    Vulcan                        Panther
                    Yorktown                      Wolf

compuserve changed that to ten players, effectively halving the resources used. since decwar lives in both high and low segment core memory, that was probably a serious concern for their commercial production environment.

the reconstruction was straightforward again, and worked on basically the first try. utexas decwar has eighteen ships and players can again captain 'the buzzard'!
