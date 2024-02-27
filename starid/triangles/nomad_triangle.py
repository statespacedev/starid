
class NOMADTriangle:
    """NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides,
    each increasing the constraints on the preceding basestars. first constructor here is for the initial triangle
    and has the target star as basestar. second constructor is for following triangles. each takes side2 from its
    predecessor and uses that as its baseside. the chain of triangles is a train of baseides - side2 of each triangle
    is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for
    information to flow backwards, all the way backward from latest triangle to the initial triangle - increasing the
    constraints on the initial triangle baseside and basestar."""

    def __init__(self):
        pass

    def constrain(self):
        """in each of the three sides, there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means
        drop this pair. here we will mark pairs to keep by setting them to 1, all others will be dropped."""

    def feedback(self):
        """increase the constraints on the baseside in the prev triangle, using the baseside of the following triangle
        in the chain. as triangles are added, constraints flow backwards through preceding basesides and basestars.
        the chain of triangles is a train of baseides - side2 of each triangle is the baseside of the following
        triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow
        backwards, all the way backward from latest triangle to the initial triangle - increasing the constraints on
        the initial triangle baseside and basestar."""

    def stop(self):
        """stopping condition. true if basestars and basesides have been constrained to the point where only one
        possible basestar remains."""
