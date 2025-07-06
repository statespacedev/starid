---
layout: post
title: "sixties assembly"
---

up to eighteen decwar players use assembly calls to manipulate shared memory. to be specific, up to eighteen tops10 jobs (processes) have fortran10v6/fortraniv/f66 code calling macro10 code that manipulates a single set of words (addresses) in dec10 core memory. 

- there are some crashes to deal with
- what's the most effective way for a beginner to approach this situation? 
- one possibility is to simplify things as much as possible

replace the players with robots. no drops, hangups, or sudden exits from the game. where it makes sense, limit or deactivate code for handling connectivity and continuity, of which there is plenty from compuserve for their dialup customers. then base everything around a smooth and friendly stream of commands from a limited vocabulary. 'move' and 'tell' primarily. some 'list' and 'time'. a bit of randomness around the exact timing of the commands, the 'move' direction, the 'tell' messages.

at the moment things work well enough if the robots don't use the 'tell' command. this suggests, focus on what's happening inside the 'tell' command and try to make things work well enough with it active as well. things have been simplified to the point where they can work ok, but it's on the edge. in fact, the 'tell' command can be active as long as there's not 'many' robots. it's both the 'tell' commmand and many robots that push it over the edge. this suggests that 'system load' is involved, whether via number of robots, frequency at which they send commands, types of commands, etc.

meanwhile, the locking code is fundamental, so also simplify there and make it as stripped down and well understood as possible. if locking can possibly work without a particular line of code, comment it out. if not forced to bring it back, leave it out.
