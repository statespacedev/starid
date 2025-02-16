# imaginary powers of 10, starting with 10**(i/1024), and squaring successively ten times. 
# this matches with feynman's table 22-3. 
y = .00225;
x = sqrt(1. - y^2);
for i = 1:11
    sprintf('%10.5f %10.5f\n', x, y)
    x2 = x^2 - y^2;
    y2 = 2 * x * y;
    x = x2;
    y = y2;
end

