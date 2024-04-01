python oriented code
------------------------

in NOMAD star recognition, there's a chain of triangles and basesides. side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 and baseside pairs are the path for information to flow backwards, increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how the chain of triangles wanders away from the target star and initial triangle. 

in SETTLER star recognition, the target star is always star a. star b is a neighbor star. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that we never move away the target star, we're settling around it.
