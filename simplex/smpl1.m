
# initial
vtyps = [3 2 2 2 1 2 2 2 1 1];
atbou = [0 0 0 0 0 0 0 0 0 0];
tab = [
    1   0   0   0   0   -5.4    -7.3    -12.96  120     90      -800
    0   1   0   0   0   .5      0       .6      -20     0       80
    0   0   1   0   0   .25     .5      .6      0       -10     40
    0   0   0   1   0   -1      1       0       0       0       0
    0   0   0   0   1   .02     0       0       0       0       3];
bh = [1 2 3 4 5];

# update tableau
function [tab, bh] = updt(tab, bh, col, piv)
  bh(piv) = col
  eim = eye(5);
  eta = zeros(5,1);
  for i = 1:5
    if i != piv, eta(i) = -tab(i,col) / tab(piv,col);
    else, eta(i) = 1 / tab(piv,col); end
  end
  eim(:,piv) = eta;
  tab = eim*tab;
end

# main program
[tab, bh] = updt(tab, bh, 6, 5);
[tab, bh] = updt(tab, bh, 8, 3);
[tab, bh] = updt(tab, bh, 10, 2);
atbou(10) = 1;
tab(2,11) = tab(2,11) - 1;
[tab, bh] = updt(tab, bh, 5, 2)

