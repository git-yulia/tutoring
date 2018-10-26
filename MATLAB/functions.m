% syntax of a function:
% function [out1,out2, ..., outN] = myfun(in1,in2,in3, ..., inN)

% example: 
function max = functions(n1, n2, n3, n4, n5)

max =  n1;
if(n2 > max)
   max = n2;
end
if(n3 > max)
   max = n3;
end
if(n4 > max)
   max = n4;
end
if(n5 > max)
   max = n5;
end

% anonymous functions example
% btw this is the VECTOR TO A POWER operator
power = @(x, n) x.^n;
result1 = power([1,2,3,4,5], 3);
disp(result1)
