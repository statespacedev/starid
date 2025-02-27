# vlbls = [u1 u2 u3 u4 u5 x1 x2 x3 x4 x5] # var labels
vtyps = [3 2 2 2 1 2 2 2 1 1]; # var types
atbou = [0 0 0 0 0 0 0 0 0 0]; # at bound
tab0 = [
    1   0   0   0   0   -5.4    -7.3    -12.96  120     90      -800
    0   1   0   0   0   .5      0       .6      -20     0       80
    0   0   1   0   0   .25     .5      .6      0       -10     40
    0   0   0   1   0   -1      1       0       0       0       0
    0   0   0   0   1   .02     0       0       0       0       3
];
bh0 = [1 2 3 4 5];

# eim elementary inverse matrix
function ei = eimat(tab, incol, pivot)
  ei = eye(5);
  eta = zeros(5,1);
  for i = 1:5
    if i != pivot, eta(i) = -tab(i,incol) / tab(pivot,incol);
    else, eta(i) = 1 / tab(pivot,incol); end
  end
  ei(:,pivot) = eta;
end

# feas1 = isfeas(tab0,bh0,vtyps); # incol 6, piv 5
ei1 = eimat(tab0, 6, 5);
tab1 = ei1*tab0; 
bh1 = [1 2 3 4 6];

# feas2 = isfeas(tab1,bh1,vtyps); # incol 8, piv 3
ei2 = eimat(tab1, 8, 3);
tab2 = ei2*tab1;
bh2 = [1 2 8 4 6];

# feas3 = isfeas(tab2,bh2,vtyps); # incol 10, piv 2
ei3 = eimat(tab2, 10, 2);
tab3 = ei3*tab2;
bh3 = [1 10 8 4 6];

# feas4 = isfeas(tab3,bh3,vtyps); # incol 5, piv 2
tabadj = tab3; 
tabadj(2,11) = tabadj(2,11) - 1; 
atbou(10) = 1;
ei4 = eimat(tabadj, 5, 2);
tab4 = ei4*tabadj;
bh4 = [1 5 8 4 6];

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
