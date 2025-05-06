## decwar introduction

historic fortran and assembly code running on simh pdp10 with standard tops10. a time capsule of how things were done in the fifties and sixties. assembly and fortran fused together. here's [june 3, 1980 v2.2 utexas](decwar22-utexas.md) and [november 20, 1981 v2.3 compuserve](decwar23-compuserve.md) player instructions, the excellent [decwar wikipedia page](https://en.wikipedia.org/wiki/Decwar), the [utexas center for american history](https://briscoecenter.org/) catalog items [v2.2](https://repositories.lib.utexas.edu/items/1aa48343-09ab-4b3b-a9f2-e2e525074a4d) and [v2.3](https://repositories.lib.utexas.edu/items/7e2ccf50-e814-4bce-91d2-a7f6440eabe4), and a 1982 letter from utexas to compuserve, before utexas sent the primordial decwar tape.

_I have received your request for sources to the DECWAR program. We will be pleased to send you the sources, but first we must ask you to sign and return the enclosed license agreement. DECWAR was developed at the University of Texas at Austin, and it is being maintained. Hence our interest in using the agreement to record and control who gets the source to it. DECWAR is a sophisticated real-time space battle game designed to be played by from 1 to 18 people. It was written at the University of Texas at Austin, primarily by Jeff Potter and Bob Hysick. The game was originally based on a very limited, single job, single terminal, two player Star Trek type game known as WAR. This game came from the CDC-6600/6400 system at the University of Texas at Austin, author unknown. Robert Schneider re-wrote the original source on the CDC and started the transfer to the DEC-10. In the transfer process, the game was renamed to DECWAR, largely re-designed, and almost entirely re-written, so that the current version bears little resemblance to the original. Almost all the commands were added once the game was on the DEC-10, as well as the basic concept of separate jobs controlling each ship, and most of the other features that make the game challenging, exciting, and enjoyable. The first version was installed on the DEC-10 in August 1978. After several revisions, a greatly enhanced and improved game, version 2.0, was installed in July 1979._

## background

decwar evolved across the seventies. eric raymond has done lots of excellent work around the single player side of that whole story, http://www.catb.org/~esr/super-star-trek/ and https://gitlab.com/esr/super-star-trek. nutshell summary for decwar context, rough timeline am detecting so far, thanks to feedbacks from merlyn, ron, etc.

- 1971, original single player star trek, written in basic for ucal irvine sds sigma 7, mike mayfield
- 1972, rewrite of original, basic for hp 2000c at hp office near ucal irvine, mike mayfield
- 1973, ut trek, written in basic, grady hicks and jim korp at utexas
- 1974, ut fortran, aka 'super star trek', single player fortran on utexas cdc 6600, dave matuszek and paul reynolds
- mid 70s, war, fortran two player version written for utexas cdc 6600, author unknown, rewritten by robert schneider
- 1978, decwar, assembly and fortran, eighteen player version written for utexas pdp-10, bob hysick and jeff potter

https://en.wikipedia.org/wiki/Star_Trek_(1971_video_game) In 1971, Mike Mayfield, then in his final year of high school, frequented a computer lab at the [University of California, Irvine](https://en.wikipedia.org/wiki/University_of_California,_Irvine), while teaching himself how to program. The lab operated both a [SDS Sigma 7](https://en.wikipedia.org/wiki/SDS_Sigma_series) and a [DEC PDP-10](https://en.wikipedia.org/wiki/PDP-10) [mainframe computer](https://en.wikipedia.org/wiki/Mainframe_computer). The PDP-10 hosted a copy of [Spacewar!](https://en.wikipedia.org/wiki/Spacewar!), a multiplayer space combat video game developed in 1962 in the [early history of video games](https://en.wikipedia.org/wiki/Early_history_of_video_games). Mayfield had gained illicit access to the Sigma 7 at the lab and wanted to create his own version of the game for the system. Spacewar! required a [vector graphics](https://en.wikipedia.org/wiki/Vector_graphics) display, however, and the Sigma 7 only had access to a non-graphical [Teletype Model 33](https://en.wikipedia.org/wiki/Teletype_Model_33) ASR [teleprinter](https://en.wikipedia.org/wiki/Teleprinter).

https://en.wikipedia.org/wiki/Decwar#Original_versions Version 1.0 of DECWAR was released in August 1978. The university would make copies available on tape for the nominal fee of $50, and it soon appeared on PDP-10s around the world. The greatly updated 2.0 was released in July 1979, and another major version, 2.3, on 20 November 1981. concerning the [utexas center for american history](https://briscoecenter.org/) catalog items and 
 
### utexas library catalog item v2.2

[v2.2](https://repositories.lib.utexas.edu/items/1aa48343-09ab-4b3b-a9f2-e2e525074a4d), files migrated from a Decus magnetic tape, and include only binary executables. [june 3, 1980 v2.2 utexas](decwar22-utexas.md) player instructions. eighteen playable ships, what this correspondent experienced at the southwest texas state university computation center, san marcos texas, circa 1983 and 1984

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

### utexas library catalog item v2.3

[v2.3](https://repositories.lib.utexas.edu/items/7e2ccf50-e814-4bce-91d2-a7f6440eabe4), files ported by Merlyn Cousins from Harris Newman's Decwar 2.3 source code to a Simh/Tops 10 emulator environment. Newman's 2.3 version reflected changes made to the UT-developed Decwar, which were made so the game could be installed on a Compuserve (CIS) system, circa 1981. [november 20, 1981 v2.3 compuserve](decwar23-compuserve.md) player instructions. compuserve modified the code, limiting to ten playable ships

                 Federation ships               Empire ships
                 ----------------               ------------
                    Lexington                     Cobra
                    Nimitz                        Demon
                    Savannah                      Hawk
                    Vulcan                        Jackal
                    Yorktown                      Wolf

## local project structure

- in the root folder are four important files from the primordial tape sent from utexas to compuserve. these give a good picture of what the primordial tape and original utexas decwar2.3 were like.
  - decwar.ltr 'letter' from utexas concerning the distribution tape
  - decwar.com 'comments' around individual files
  - decwar.imp 'important' additional comments
  - decwar.tap 'tape' listing
- step1 is what merlyn got working on simh pdp10, _all changes commented and additions noted by DrForbin AKA Merlyn Cousins. search for (drforbin), all things yet to be fixed, search (tofix)_ here's [more info](docs/app0-foundations.md) from merlyn around that.
  - utexas folder is migrated up to step2 and backward in time towards the primordial tape 
  - compuserve folder is fortran and macro files likely associated with compuserve, or at least not definitely shown as part of the primordial tape. they're not shown in decwar.imp and decwar.tap!
    - compuserve/docs folder is files associated with compuserve documenting the primordial tape's arrival from utexas. also some related 'operational' files. decwar.map has logging from a compuserve ki10 dated 19820519.
    - scripts folder is build automations
- step2 is just one way to go backwards. take what we have in step1, strip it down to bare metal, !always still working!, then stepbystep reintroduce things so we end up with something like v2.2 with eighteen ships. at least on the surface layers, the primordial utexas tape?
  - hlp
  - msc
  - tec folder for teco files to autogen fortran files contained in unhistorical tecout folder
  - tecout folder is unhistorical container for the output files generated by the teco scripts in tec

there's seven 'core' code files.
    
    DECWAR.FOR	FORTRAN source.
    WARMAC.MAC	MACRO source.
    SETUP.FOR	Once only code, deleted from core after initialization.
    HIGH.FOR	Used to force common block into high segment.
    LOW.FOR	    Used to force common block into low segment.
    MSG.MAC	    Output text source.
    SETMSG.MAC	Equivalent to MSG.MAC for SETUP text strings.

the high.for and low.for are stubs. somewhat likewise for msg.mac and setmsg.mac. teco scripts are associated with include files contained in unhistorical 'tecout' folder 

    lowseg.tec -> lowseg.for
    hiseg.tec  -> hiseg.for
    param.tec  -> param.for
    extern.tec -> extern.for
    setext.tec -> setext.for
    ???        -> lstvar.for

probably the scripts simply search through warmac.mac and pick out 'variables' that need to appear in the include for files. this can be done manually. if there's an update to warmac.mac, include for files can be manually updated. the teco scripts probably just automate this, generating the include for 'from scratch'. so it's not a 'must have'. can be done by hand. but possibly nice. and an interesting usage of teco.

## notes

v2.2 31 commands - bases, build, capture, damages, dock, energy, gripe, help, impulse, list, move, news, planets, points, quit, radio, repair, scan, set, shields, srscan, status, summary, targets, tell, time, torpedoes, tractor, type, users

v2.3 31 commands - apparently, zeroth order, same as v2.2
