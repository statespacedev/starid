
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
im = eye(5);
global tab bh im;

# update tableau
function beta = updt(beta, col, piv)
  global tab bh im;
  bh(piv) = col
  alpha = im*tab(:,col); # this is effectively ftran
  eta = zeros(5,1);
  for i = 1:5
    if i != piv, eta(i) = -alpha(i) / alpha(piv);
    else, eta(i) = 1 / alpha(piv); end
  end
  eim = eye(5);
  eim(:,piv) = eta;
  im = eim*im;
  beta = eim*beta;
end

# main program
beta0 = tab(:,11);
beta1 = updt(beta0, 6, 5);
beta2 = updt(beta1, 8, 3);
beta3 = updt(beta2, 10, 2);
atbou(10) = 1;
beta3(2) = beta3(2) - 1;
beta4 = updt(beta3, 5, 2)

