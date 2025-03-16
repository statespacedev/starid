
# tableau
tab = [
    1   0   0   0   0   -5.4    -7.3    -12.96  120     90      -800
    0   1   0   0   0   .5      0       .6      -20     0       80
    0   0   1   0   0   .25     .5      .6      0       -10     40
    0   0   0   1   0   -1      1       0       0       0       0
    0   0   0   0   1   .02     0       0       0       0       3];
vtyps = [3 2 2 2 1 2 2 2 1 1]; # variable types
atbou = [0 0 0 0 0 0 0 0 0 0]; # variables at bound
bh = [1 2 3 4 5]; # basis heading
tap = zeros(6,10); # tape storage for columns left to right
tapeof = 0; # last column on tape
global tab bh tap tapeof;

# tape based forward transform
function alpha = ftran(alpha)
  global tap tapeof;
  bi = eye(5); # basis inverse
  for i =1:tapeof
    ei = eye(5); # elementary inverse
    piv = tap(1,i);
    eta = tap(2:6,i);
    ei(:,piv) = eta;
    bi = ei*bi;
  end
  alpha = bi*alpha;
end

# tape write pivot value and eta column
function ei = tapw(piv, eta)
  global tap tapeof;
  tapeof = tapeof + 1;
  tap(1,tapeof) = piv;
  tap(2:6,tapeof) = eta;
  ei = eye(5);
  ei(:,piv) = eta;
end

# tape based backward transform
function py = btran()
  global tap tapeof tab;
  f = [1 0 0 0 0]; # pricing form
  for i = tapeof:-1:1
    ei = eye(5);
    piv = tap(1,i);
    eta = tap(2:6,i);
    ei(:,piv) = eta;
    f = f*ei;
  end
  py = f; # pricing vector
end

# update tableau
function beta = updt(beta, col, piv)
  global tab bh bi;
  py = btran();
  test = py*tab(:,7);
  bh(piv) = col
  alpha = ftran(tab(:,col));
  eta = zeros(5,1);
  for i = 1:5
    if i != piv, eta(i) = -alpha(i) / alpha(piv);
    else, eta(i) = 1 / alpha(piv); end
  end
  ei = tapw(piv, eta);
  beta = ei*beta;
end

# main program
beta0 = tab(:,11);
beta1 = updt(beta0, 6, 5);
beta2 = updt(beta1, 8, 3);
beta3 = updt(beta2, 10, 2);
atbou(10) = 1;
beta3(2) = beta3(2) - 1;
beta4 = updt(beta3, 5, 2)

