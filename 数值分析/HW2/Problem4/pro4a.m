% Define the vector function g(x1, x2, x3)
g = @(x) [15*x(1) + x(2)^2 - 4*x(3) - 13; x(1)^2 + 10*x(2) - x(3)-11; x(2)^3-25*x(3)+22];

% Define the gradient of g(x1, x2, x3)
grad_g = @(x) [15, 2*x(2), -4;
               2*x(1), 10,    -1;
               0,      3*x(2)^2, -25];
% Define the initial guess x0 and the maximum number of iterations
x0 = [0; 0; 0];
max_iter = 1000000;

% Perform the Steepest Descent method
x = x0;
for k = 1:max_iter
    % Compute the gradient of g at x
    grad = grad_g(x);
    
    % Compute the search direction
    d = -grad' * g(x);
    
    % Compute the step size using a line search
    alpha = fminbnd(@(a) norm(g(x + a*d))^2, 0, 1);
    
    % Update x
    x = x + alpha*d;
    
    % Check for convergence
    if norm(g(x)) < 0.05
        break;
    end
end
x