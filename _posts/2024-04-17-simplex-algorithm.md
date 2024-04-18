---
layout: post
title: "simplex algorithm"
categories: aerospace simplex
---
in the early days of digital computing, what were some of the motivations for industry to invest in computers? what exactly could computers do? they could perform numerical simulations. in the manhattan project, they replaced rooms of humans operating mechanical calculators. and they could collate data. in the sage continental air defence system, they replaced humans in low level command and control roles.

another motivation was logistics and resource allocation. the b-29 program during the war was an early example of something new that soon became associated with the terms 'operations research', 'systems analysis', and 'aerospace'. at the time, the term 'programming' referred to defense programs such as the b-29 program or the icbm program, rather than writing computer code.

it's curious to note the evolution in terminology from 'projects' to 'programs'. a few years later, the manhattan project would probably have been the manhattan program. this change from projects to programs marks the appearance of digital computers, linear programming, and the simplex algorithm. 

the simplex algorithm and digital computers appeared as an organic pair because of their affinity for floating-point vector and matrix operations. large scale matrix inversion hadn't been practical before, but was what the simplex algorithm required, and what the adaptability of digital computers provided. software implementations of the operations needed for matrix inversion fused simplex and computers together, and took on definite shape as fortran. 

not surprisingly, dantzig and von neumann wrote about matrix inversion and computing as early as 1950, likely based on work underway for the eniac from 1946 forward. and there's another curiosity regarding terminology. the term 'simplex' reappears later, in the seemingly different context of sigma point kalman filters and state space sampling algorithms. this is a topic of interest for project statespacedev.
