
# tableau
tablu = [
    1   0   0   0   0   -5.4    -7.3    -12.96  120     90      -800
    0   1   0   0   0   .5      0       .6      -20     0       80
    0   0   1   0   0   .25     .5      .6      0       -10     40
    0   0   0   1   0   -1      1       0       0       0       0
    0   0   0   0   1   .02     0       0       0       0       3];
vtyps = [3 2 2 2 1 2 2 2 1 1]; # variable types
atbou = [0 0 0 0 0 0 0 0 0 0]; # variables at bound
bahead = [1 2 3 4 5]; # basis heading
tapem = zeros(6,10); # tape matrix for columns left to right
tapeof = 0; # last column on tape
global tablu bahead tapem tapeof atbou vtyps;

# tape based backward transform of pricing vector pi
function pie = btran(fvec)
  global tapem tapeof tablu;
  pie = fvec; # pricing vector pi, symbol pi is reserved
  for i = tapeof:-1:1
    ei = eye(5);
    piv = tapem(1,i);
    eta = tapem(2:6,i);
    ei(:,piv) = eta;
    pie = pie*ei;
  end
end

# tape based forward transform
function alpha = ftran(alpha)
  global tapem tapeof;
  bi = eye(5); # basis inverse
  for i =1:tapeof
    ei = eye(5); # elementary inverse
    piv = tapem(1,i);
    eta = tapem(2:6,i);
    ei(:,piv) = eta;
    bi = ei*bi;
  end
  alpha = bi*alpha;
end

# tape write pivot value and eta column
function ei = tapew(piv, eta)
  global tapem tapeof;
  tapeof = tapeof + 1;
  tapem(1,tapeof) = piv;
  tapem(2:6,tapeof) = eta;
  ei = eye(5);
  ei(:,piv) = eta;
end

# pricing form. depends on feasibility state.
function fvec = prcfrm(beta)
  global bahead vtyps;
  fvec = [0 0 0 0 0]; # pricing form fvec
  for i = 1:5
    vtyp = vtyps(bahead(i));
    if vtyp == 3
      continue;
    elseif beta(i) < 0
      fvec(i) = 1; # set symbol m (for minus)
    elseif (vtyp == 1 && beta(i) > 1) || (vtyp == 0 && beta(i) > 0)
      fvec(i) = -1; # set symbol p (for plus)
    end
  end
  if !any(fvec), fvec(1) = 1; end
end

# update tableau
function beta = updtab(beta, col, piv)
  global tablu;
  alpha = ftran(tablu(:,col));
  eta = zeros(5,1);
  for i = 1:5
    if i != piv, eta(i) = -alpha(i) / alpha(piv);
    else, eta(i) = 1 / alpha(piv); end
  end
  ei = tapew(piv, eta);
  beta = ei*beta;
end

# iteration of the revised simplex method
function beta = itr(beta, col, piv)
  global tablu bahead atbout;
  # choose col for incoming
  fvec = prcfrm(beta); # pricing form f
  pie = btran(fvec); # pricing vector pi, symbol pi is reserved
  prices = pie*tablu;
  # choose piv for outgoing
  # handle at bounds
  if col == 5
    atbou(10) = 1;
    beta(2) = beta(2) - 1;
  endif  
  bahead(piv) = col # update basis heading
  beta = updtab(beta, col, piv) # update tableau
end

# main program
beta0 = tablu(:,11);
beta1 = itr(beta0, 6, 5);
beta2 = itr(beta1, 8, 3);
beta3 = itr(beta2, 10, 2);
beta4 = itr(beta3, 5, 2)
exit
