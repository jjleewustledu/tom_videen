% (c) N.Papenberg, K.Ens, SAFIR, Luebeck, Germany, 2006
%
% a matlab script that produces trivial test data 

m = zeros(1,3);

m(1) = 128;
m(2) = 128;
m(3) = 128;

nameT = 'imageDataT.dat';
nameR = 'imageDataR.dat';

% first test
% alle Richtungen gleich stark
T = zeros(m);
R = zeros(m);

T(16:17,16:17,16:17) = 10;
R(13:20,13:20,13:20) = 10;

% x1 test
T = zeros(m);
R = zeros(m);

T(16:23,13:20,13:20) = 10;
R(13:20,13:20,13:20) = 10;

% x2 test
% T = zeros(m);
% R = zeros(m);
% 
% T(13:20,16:23,13:20) = 10;
% R(13:20,13:20,13:20) = 10;
% 
% % x3 test
% T = zeros(m);
% R = zeros(m);
% 
% T(13:20,13:20,16:23) = 10;
% R(13:20,13:20,13:20) = 10;
% 
% 
% %T(4,4,4) = 1;
% %R(3:5,3:5,3:5) = 1;
% 

matlab2flirt(T,R,nameT,nameR);
