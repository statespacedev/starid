---
layout: post
title: "utexas robots"
---

robots in the utexas folder are a 'simple as possible' minimalist skeleton. the concept is that descendant code can spin off and explore various other directions. for example it's natural to picture bringing in some machine learning, but this should be in a separate 'dedicated repo' project because it's something beyond 'absolutely essential'. another example is usage with decwar variants where the idea is to spin off a dedicated repo, make the minimalist skeleton go, and then extend with nice stuff only possible with the variant.  

hopefully the robots themselves can become a real frontier for exploration. on top of still very much alive hardware and code coming directly from the seventies, whole new types of descendants could evolve fifty years later that would have been pure science fiction to the original coders and players. few would have been able to realistically contemplate using machine learning with decwar fifty years ago. today it's already simplistic and obvious. 

boring 'infrastructure' stuff is corralled into main. fun 'decwar specific' stuff begins with the brain. the brain kicks in and takes over once the decwar game command prompt is available, and only knows about giving decwar commands and reacting to decwar output.

the 'secret' here is that there's a child thread with a robot instance spinning slowly and handling telnet, tops10, and pregame topics. the main thread is meanwhile listening to the keyboard. the idea is that the main thread always has to ask and wait for the child thread to take any actions, hopefully in a calm, reasonable, clean way. for shutdown, the child should quit decwar, logout of tops10, and close the telnet connection, at which point finally both threads can exit.

a brain object is effectively a 'ship in the game'. it knows how to give commands and handle the resulting output. on top of that, it adds basic concepts of 'what to do'. this is where things get interesting. without getting too complex and going beyond minimalist approaches, the tougher, longer surviving, and harder to beat the robots can become, the better. 

