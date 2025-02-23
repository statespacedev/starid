# vs = [u1 u2 u3 u4 u5 x1 x2 x3 x4 x5]
vts = [3 2 2 2 1 2 2 2 1 1];

function feas = isfeas(itr,vts)
  feas = true;
  for i = 1:5
    v = itr(i,11);
    t = vts(itr(i,12));
    if t == 1
      if v < 0 || v > 1, feas = false; end
    elseif t == 2
      if v < 0, feas = false; end
    end
  end
endfunction

itr0 = [
    1   0   0   0   0   -5.4    -7.3    -12.96  120     90      -800    1
    0   1   0   0   0   .5      0       .6      -20     0       80      2
    0   0   1   0   0   .25     .5      .6      0       -10     40      3
    0   0   0   1   0   -1      1       0       0       0       0       4
    0   0   0   0   1   .02     0       0       0       0       3       5
]
feas = isfeas(itr0,vts);
# infeas, pricing on 'w cff compfeasfunc' to choose incol
# incol is col x1
# ratio test on incol col x1 to choose pivot
# pivot is col x1 row 5
# transform to next tableau

 itr1 = [
    1   0   0   0   270     0    -7.3    -12.96  120     90      10     1
    0   1   0   0   -25     0      0      .6     -20     0       5      2
    0   0   1   0   -12.5   0     .5      .6      0      -10     2.5    3
    0   0   0   1   50      0      1       0      0      0       150    4
    0   0   0   0   50      1      0       0      0      0       150    6
]

itr2 = [
    1   0   21.6  0   0       0    3.5     0     120     -126    64     1
    0   1   -1    0   -12.5   0    -.5     0     -20     10      2.5    2
    0   0   1.67  0   -20.83  0    .83     1      0      -16.67  4.17   8
    0   0   0     1   50      0    1       0      0      0       150    4
    0   0   0     0   50      1    0       0      0      0       150    6
]




