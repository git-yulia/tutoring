function y = get_average(x)
if ~isvector(x)
    error('Input must be a vector, sils')
end

y = sum(x)/length(x); 
end