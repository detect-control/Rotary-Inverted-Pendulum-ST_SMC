% Rotating Inverted Pendulum Control with STA-SMC
% Control goal: Stabilize theta to 0 radians
% Controller: Super-Twisting SMC with tanh and integral error

clc;
clear;
close all;

%% System Parameters
% Physical parameters
params.ma = 0.21;      % Arm mass (kg)
params.mp = 0.156;    % Pendulum mass (kg)
params.la = 0.155;    % Arm length (m)
params.lp = 0.155;    % Pendulum length (m)
params.ba = 0.005;    % Arm damping coefficient (N·m·s/rad)
params.bp = 0.0007;   % Pendulum damping coefficient (N·m·s/rad)
params.g  = 9.81;     % Gravity (m/s²)

% Derived parameters (p1 to p5)
params.p1 = (1/3 * params.ma + params.mp) * params.la^2;
params.p2 = (1/4 * params.mp) * params.lp^2;
params.p3 = (1/2 * params.mp) * params.lp * params.la;
params.p4 = (1/2 * params.mp) * params.lp * params.g;
params.p5 = (1/12 * params.mp) * params.lp^2;

%% Controller Parameters
controller.c1 = 16;   % Sliding surface coefficient (proportional)
controller.c2 = 79;   % Sliding surface coefficient (integral)
controller.k = 370;  
controller.n = 0.1;     % tanh boundary layer parameter

%% Simulation Settings with RK4
h = 0.01;            % Step size
t = 0:h:10;           % Time vector
nSteps = length(t);   % Number of steps
X = zeros(nSteps, 5); % State matrix [phi, theta, phi_dot, theta_dot, int_e, int_tanh]
tauPhi = zeros(nSteps, 1); % Control input vector

% Initial state
X(1,:) = [0; 0.2; 0; 0; 0]; 

%% RK4 Integration Loop
for i = 1:nSteps-1
    current_X = X(i,:)';
    
    % Step 1: Compute k1 and control input
    [k1, tau_phi1] = pendulumDynamics(t(i), current_X, params, controller);
    tauPhi(i) = tau_phi1; % Record tauPhi at current time
    
    % Step 2: Compute k2
    [k2, ~] = pendulumDynamics(t(i) + h/2, current_X + h/2 * k1, params, controller);
    
    % Step 3: Compute k3
    [k3, ~] = pendulumDynamics(t(i) + h/2, current_X + h/2 * k2, params, controller);
    
    % Step 4: Compute k4
    [k4, ~] = pendulumDynamics(t(i) + h, current_X + h * k3, params, controller);
    
    % Update state
    X(i+1,:) = current_X' + (h/6) * (k1 + 2*k2 + 2*k3 + k4)';
end

% Record control input for the last time step
[~, tauPhi(nSteps)] = pendulumDynamics(t(end), X(nSteps,:)', params, controller);

%% Extract States (same as before)
phi = X(:, 1);        % Arm angle (rad)
theta = X(:, 2);      % Pendulum angle (rad)
phiDot = X(:, 3);     % Arm angular velocity (rad/s)
thetaDot = X(:, 4);   % Pendulum angular velocity (rad/s)
intE = X(:, 5);       % Integral of theta

% Calculate sliding surface s and control input tau_phi
s = thetaDot + controller.c1 * theta + controller.c2 * intE;

%% Plot Results
figure;
subplot(2,2,1);
plot(t, theta, 'b', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\theta (rad)');
title('Pendulum Angle \theta');
grid on;

subplot(2,2,2);
plot(t, phi, 'g', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\phi (rad)');
title('Arm Angle \phi');
grid on;

subplot(2,2,3);
plot(t, tauPhi, 'r', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\tau_\phi (N\cdot m)');
title('Control Input');
grid on;

subplot(2,2,4);
plot(t, s, 'm', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('s');
title('Sliding Surface');
grid on;

%% Dynamics Function (Revised for clarity)
function [dXdt, tauPhi] = pendulumDynamics(t, X, params, ctrl)
    % Extract states
    phi = X(1);
    theta = X(2);
    phiDot = X(3);
    thetaDot = X(4);
    intE = X(5);
    
    % Calculate sliding surface s
    e = theta; 
    s = thetaDot + ctrl.c1 * e + ctrl.c2 * intE;
    
    % Mass matrix elements
    m11 = params.p1 + params.p2 * sin(theta)^2;
    m12 = params.p3 * cos(theta);
    m22 = params.p2 + params.p5;
    M = [m11, m12; m12, m22];
    
    % Nonlinear terms (Coriolis, centrifugal, gravity, friction)
    H1 = params.p2 * sin(2*theta) * thetaDot * phiDot ...
         - params.p3 * sin(theta) * thetaDot^2 ...      
         + params.ba * phiDot;                           
    
    H2 = -0.5 * params.p2 * sin(2*theta) * phiDot^2 ...  
          - params.p4 * sin(theta) ...                  
          + params.bp * thetaDot;                         
    
    % Dynamics equation: M*[phiDDot; thetaDDot] = [tauPhi - H1; -H2]
    % We'll solve for accelerations after computing tauPhi
    
    % Avoid division by zero
    threshold = 1e-5;
    if abs(m12) < threshold
         m12 = threshold * sign(m12);  % Preserves sign
    end

    % Compute terms for thetaDDot expression
    detM = m11*m22 - m12^2;
   
    % Effect of tauPhi on thetaDDot
    g = m12 / detM;
    
    % Drift dynamics (effect of nonlinear terms on thetaDDot)
    f = (m12*H1 - m11*H2) / detM;
    
    % Super-Twisting control law
    tauPhi = (1/g) * ( (f + ctrl.c1 * thetaDot + ctrl.c2 * e) ...
        + ctrl.k * sqrt(abs(s)) * tanh(s / ctrl.n));

    tauPhi = max(min(tauPhi, 2), -2);

    % Compute accelerations by solving matrix equation
    rhs = [tauPhi - H1;  % Arm dynamics: M_row1*accel = tauPhi - H1
           -H2];        % Pendulum dynamics: M_row2*accel = -H2
    accel = M \ rhs;
    phiDDot = accel(1);
    thetaDDot = accel(2);
    
    % State derivatives
    dIntE = theta;                  % d/dt(∫e) = theta
    dXdt = [phiDot; thetaDot; phiDDot; thetaDDot; dIntE];
end