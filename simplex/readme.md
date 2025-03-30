# project simplex

go backwards in time, starting from the mid sixties, and get hands on with a very small business problem using linear programming on an ibm 7094 computer. the [widget problem](https://github.com/statespacedev/starid/discussions/4) of alp chapter three seems ideal since it's explicitly solved in tableau form, only takes four iterations, and every operation is easily checked against code. the target is 7094 f66 using btran, ftran, and simh emulated mag tape to maintain the basis inverse as pfi product form of inverse eta columns. for more modern code, python linear algebra is simply annoying, while matlab's 'fortran-osity' provokes spontaneous bouts of smiling. 

at first, the incoming column and pivot row numbers for each iteration are simply taken 'as is' from alp chapter three for each iteration. this makes the skeletal mechanism of the revised simplex, ftran, and btran more apparent.

## smpl1, original simplex

alp chapter three example, worked exactly as it appears in the book. the entire tableau is updated in each iteration. in this particular case, columns one and four of the tableau are unit vectors that don't play a direct role, but are touched regardless. in the revised simplex, they're left untouched.

## smpl2, revised simplex 

alp chapter four. the full tableau is not updated, the original serves as a kind of constant. instead, the beta column 'right hand side' is updated using the basis inverse. new eta vectors are generated and ready for 'storage on tape'. each eta is a column of the basis inverse. the basis inverse is also used to transform incoming columns, which has the same end effect as the 'tape based' ftran routine.

## smpl3, tape based ftran for transforming columns

tape is emulated by storing pivot value and eta column pairs left to right in a matrix. this revised simplex mechanism is approaching what was used in the late fifties and early sixties. only the 'right hand side' beta vector is maintained across iterations. the basis inverse is stored as eta vectors 'on tape'. the tape and eta columns are read forwards.

## smpl4, tape based btran for transforming columns into prices

early in each revised simplex iteration, the updated first row values at particular columns are needed. these are the 'prices' of those particular columns. profit is increased by bringing a column with a negative price into the basis. for a particular column, the updated price is the dot product with the first row of the basis inverse. btran is a mechanism to perform these dot products. the first row of the basis inverse comes from the eta columns on tape. the tape and eta columns are read backwards, conveniently reversing back through the previous iteration's ftran.

# widget problem of alp chapter three

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
