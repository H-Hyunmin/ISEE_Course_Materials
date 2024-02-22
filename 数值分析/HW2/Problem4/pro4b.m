% Define the vector function g(x)
g = @(x) [10*x(1) - 2*x(2)^2 + x(2) - 2*x(3) - 5;   
          8*x(2)^2 + 4*x(3)^2 - 9; 
          8*x(2)*x(3) + 4];

% Define the gradient of g(x)
grad_g = @(x) [10, -4*x(2) + 1, -2;
               0, 16*x(2), 8*x(3);
               0, 8*x(3), 8*x(2)];
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
g(x)