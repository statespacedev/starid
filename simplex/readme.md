# project simplex

go backwards in time, starting from the mid sixties, and get hands on with a very small business problem using linear programming on an ibm 7094 computer. the widget problem of alp 3.5 seems ideal since it is explicitly solved there in tableau form. it only takes four iterations and every operation is easily checked against code implementations.

it can be implemented in python first. the trick is to make the python as relevant as possible to f66 on pdp-10 and 7094. the ultimate implementation in 7094 f66 should use simh emulated mag tape to store pfi product form of inverse eta columns for use in ftran and btran. but python is too annoying for linear algebra, for now a return to matlab makes sense.

# descending in time

- smpl2, 'revised' simplex, chapter four of alp
- smpl1, this is the alp chapter 3.5 example worked exactly as it appears in the book. pre 'revised' simplex

# widget problem

five activities x1 to x5 and five columns of the tableau

- activity x1 is production of unfinished widgets
- activity x2 is conversion of unfinished to finished widgets
- activity x3 is production of finished widgets from scratch
- activity x4 is the use of l1 overtime
- activity x5 is the use of l2 overtime

the five rows of the model and its tableau

- row 1 is the functional, costs are positive and revenues are negative. functional means objective recast as an equation including the values to be maximized
- row 2 is capacity constraint on l1 labor
- row 3 is capactiy constraint on l2 labor
- row 4 is used to insure that enough unfinished widgets are produced to allow some of them to be converted to finished ones
- row 5 is contractual and policy constraints on unfinshed widgets

row equations

- u1 - 5.4x1 - 7.3x2 -12.96x3 + 6x4 + 9x5 + 800 = 0
- .5x1 + .6x3 - x4 <= 80
- .25x1 + .5x2 + .6x3 - x5 <= 40
- -x1 + x2 <= 0
- 100 <= x1 <= 150

where all x >= 0, x4 <= 20, x5 <= 10, u1 is to be maximized

# code cold storage area

function feas = isfeas(itr, bh, vtyps)
  feas = true;
  for i = 1:5
    b = itr(i,11);
    vt = vtyps(bh(i));
    if vt == 1
      if b < 0 || b > 1, feas = false; end
    elseif vt == 2
      if b < 0, feas = false; end
    end
  end
end
