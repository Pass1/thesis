%A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-03898-00052.dat', ' ', 5);
%B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-04240-00041.dat', ' ', 5);
% 
% subplot(2,3,1), plot(A.data(:,2)) 
% title('U for pickpoint 03898')
% subplot(2,3,4), plot(B.data(:,2))
% title('U for pickpoint 04240')
% subplot(2,3,2), plot(A.data(:,3))
% title('V for pickpoint 03898')
% subplot(2,3,5), plot(B.data(:,3))
% title('V for pickpoint 04240')
% subplot(2,3,3), plot(A.data(:,4))
% title('W for pickpoint 03898')
% subplot(2,3,6), plot(B.data(:,4))
% title('W for pickpoint 04240')



% A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-05860-00045.dat', ' ', 5);
% B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-05866-00049.dat', ' ', 5);
% C = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-06010-00042.dat', ' ', 5);
% D = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-06016-00037.dat', ' ', 5);
% subplot(3,1,1), plot(A.data(:,2))
% title('U component')
% hold all
% subplot(3,1,2), plot(A.data(:,3))
% title('V component')
% hold all
% subplot(3,1,3), plot(A.data(:,4))
% title('W component')
% hold all
% 
% subplot(3,1,1), plot(B.data(:,2))
% hold off
% subplot(3,1,2), plot(-1.*B.data(:,3))
% hold off
% subplot(3,1,3), plot(-1.*B.data(:,4))
% hold off


%1250 away start
A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-05860-00045.dat', ' ', 5);
B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-05866-00049.dat', ' ', 5);
C = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-06010-00042.dat', ' ', 5);
D = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-06016-00037.dat', ' ', 5);

E = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-09610-00455.dat', ' ', 5);
F = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-09616-00452.dat', ' ', 5);
G = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-09760-00453.dat', ' ', 5);
H = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-09766-00453.dat', ' ', 5);
%endtime = 1000;
%starttime = 1;
%1250 away end

%2500 away start
% A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-03907-00048.dat', ' ', 5);
% B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-03919-00053.dat', ' ', 5);
% C = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-04207-00043.dat', ' ', 5);
% D = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-04219-00053.dat', ' ', 5);
% 
% E = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-11407-00461.dat', ' ', 5);
% F = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-11419-00456.dat', ' ', 5);
% G = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-11707-00449.dat', ' ', 5);
% H = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-11719-00456.dat', ' ', 5);
% endtime = 800;
% starttime = 1;
%2500 away end

%3750 away start
% A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-01954-00054.dat', ' ', 5);
% B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-01972-00052.dat', ' ', 5);
% C = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-02404-00004.dat', ' ', 5);
% D = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-02422-00062.dat', ' ', 5);
% 
% E = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-13204-00478.dat', ' ', 5);
% F = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-13222-00460.dat', ' ', 5);
% G = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-13654-00487.dat', ' ', 5);
% H = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-13672-00456.dat', ' ', 5);
% endtime = 1000;
% starttime = 1;
%3750 away end

figure(1);
%5000 away start
A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-00001-00054.dat', ' ', 5);
B = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-00025-00052.dat', ' ', 5);
C = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-00601-00105.dat', ' ', 5);
D = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-00625-00062.dat', ' ', 5);

E = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-15001-00478.dat', ' ', 5);
F = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-15025-00460.dat', ' ', 5);
G = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-15601-00488.dat', ' ', 5);
H = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-15625-00481.dat', ' ', 5);
% endtime = 1000;
% starttime = 1;
%5000 away end

%apply bandpass filterfunction data=
% A.data = bandpass(A.data, 0.001, 10, 0.01, 1);
% B.data = bandpass(B.data, 0.001, 10, 0.01, 1);
% C.data = bandpass(C.data, 0.001, 10, 0.01, 1);
% D.data = bandpass(D.data, 0.001, 10, 0.01, 1);
% E.data = bandpass(E.data, 0.001, 10, 0.01, 1);
% F.data = bandpass(F.data, 0.001, 10, 0.01, 1);
% G.data = bandpass(G.data, 0.001, 10, 0.01, 1);
% H.data = bandpass(H.data, 0.001, 10, 0.01, 1);
%end of bandpass filter

starttime = 1;
endtime = size(A.data,1);

%START common code
subplot(3,4,1), plot(A.data(starttime:endtime,2), 'Color', [0, 0, 0])
title('Top plane U component comparison')
hold all
subplot(3,4,5), plot(A.data(starttime:endtime,3), 'Color', [0, 0, 0])
title('Top plane V component comparison')
hold all
subplot(3,4,9), plot(A.data(starttime:endtime,4), 'Color', [0, 0, 0])
title('Top plane W component comparison')
hold all

subplot(3,4,1), plot(B.data(starttime:endtime,2), 'Color', [0, 0, 1])
subplot(3,4,5), plot(-1.*B.data(starttime:endtime,3), 'Color', [0, 0, 1])
subplot(3,4,9), plot(-1.*B.data(starttime:endtime,4), 'Color', [0, 0, 1])

subplot(3,4,1), plot(-1.*C.data(starttime:endtime,2), 'Color', [0, 1, 0])
subplot(3,4,5), plot(C.data(starttime:endtime,3), 'Color', [0, 1, 0])
subplot(3,4,9), plot(-1.*C.data(starttime:endtime,4), 'Color', [0, 1, 0])

subplot(3,4,1), plot(-1.*D.data(starttime:endtime,2), 'Color', [0 1 1])
hold off
subplot(3,4,5), plot(-1.*D.data(starttime:endtime,3), 'Color', [0 1 1])
hold off
subplot(3,4,9), plot(D.data(starttime:endtime,4), 'Color', [0 1 1])
hold off


subplot(3,4,2), plot(E.data(starttime:endtime,2), 'Color', [1 0 0])
title('Bottom plane U component comparison')
hold all
subplot(3,4,6), plot(E.data(starttime:endtime,3), 'Color', [1 0 0])
title('Bottom plane V component comparison')
hold all
subplot(3,4,10), plot(E.data(starttime:endtime,4), 'Color', [1 0 0])
title('Bottom plane W component comparison')
hold all

subplot(3,4,2), plot(F.data(starttime:endtime,2), 'Color', [1 0 1])
subplot(3,4,6), plot(-1.*F.data(starttime:endtime,3), 'Color', [1 0 1])
subplot(3,4,10), plot(-1.*F.data(starttime:endtime,4), 'Color', [1 0 1])

subplot(3,4,2), plot(-1.*G.data(starttime:endtime,2), 'Color', [1 1 0])
subplot(3,4,6), plot(G.data(starttime:endtime,3), 'Color', [1 1 0])
subplot(3,4,10), plot(-1.*G.data(starttime:endtime,4), 'Color', [1 1 0])

subplot(3,4,2), plot(-1.*H.data(starttime:endtime,2), 'Color', [0.5 0.5 0.5])
hold off
subplot(3,4,6), plot(-1.*H.data(starttime:endtime,3), 'Color', [0.5 0.5 0.5])
hold off
subplot(3,4,10), plot(H.data(starttime:endtime,4), 'Color', [0.5 0.5 0.5])
hold off

subplot(3,4,3), plot(C.data(starttime:endtime,2), 'Color', [0, 1, 0])
title('Front plane U component comparison')
hold all
subplot(3,4,7), plot(C.data(starttime:endtime,3), 'Color', [0, 1, 0])
title('Front plane V component comparison')
hold all
subplot(3,4,11), plot(C.data(starttime:endtime,4), 'Color', [0, 1, 0])
title('Front plane W component comparison')
hold all

subplot(3,4,3), plot(D.data(starttime:endtime,2), 'Color', [0 1 1])
subplot(3,4,7), plot(-1.*D.data(starttime:endtime,3), 'Color', [0 1 1])
subplot(3,4,11), plot(-1.*D.data(starttime:endtime,4), 'Color', [0 1 1])

subplot(3,4,3), plot(G.data(starttime:endtime,2), 'Color', [1 1 0])
subplot(3,4,7), plot(G.data(starttime:endtime,3), 'Color', [1 1 0])
subplot(3,4,11), plot(-1.*G.data(starttime:endtime,4), 'Color', [1 1 0])

subplot(3,4,3), plot(H.data(starttime:endtime,2), 'Color', [0.5 0.5 0.5])
hold off
subplot(3,4,7), plot(-1.*H.data(starttime:endtime,3), 'Color', [0.5 0.5 0.5])
hold off
subplot(3,4,11), plot(H.data(starttime:endtime,4), 'Color', [0.5 0.5 0.5])
hold off


subplot(3,4,4), plot(A.data(starttime:endtime,2), 'Color', [0, 0, 0])
title('Rear plane U component comparison')
hold all
subplot(3,4,8), plot(A.data(starttime:endtime,3), 'Color', [0, 0, 0])
title('Rear plane V component comparison')
hold all
subplot(3,4,12), plot(A.data(starttime:endtime,4), 'Color', [0, 0, 0])
title('Rear plane W component comparison')
hold all

subplot(3,4,4), plot(B.data(starttime:endtime,2), 'Color', [0, 0, 1])
subplot(3,4,8), plot(-1.*B.data(starttime:endtime,3), 'Color', [0, 0, 1])
subplot(3,4,12), plot(-1.*B.data(starttime:endtime,4), 'Color', [0, 0, 1])

subplot(3,4,4), plot(F.data(starttime:endtime,2), 'Color', [1 0 1])
subplot(3,4,8), plot(-1.*F.data(starttime:endtime,3), 'Color', [1 0 1])
subplot(3,4,12), plot(F.data(starttime:endtime,4), 'Color', [1 0 1])

subplot(3,4,4), plot(E.data(starttime:endtime,2), 'Color', [1 0 0])
hold off
subplot(3,4,8), plot(E.data(starttime:endtime,3), 'Color', [1 0 0])
hold off
subplot(3,4,12), plot(-1.*E.data(starttime:endtime,4), 'Color', [1 0 0])
hold off
%END COMMON CODE


figure(2)
set(gca, 'XDir', 'reverse');
set(gca, 'YDir', 'reverse');
x = [-1250 1250 -1250 1250 -1250 1250 -1250 1250 -2500 2500 -2500 2500 -2500 2500 -2500 2500];
y = [-1250 -1250 1250 1250 -1250 -1250 1250 1250 -2500 -2500 2500 2500 -2500 -2500 2500 2500];
z = [-7500 -7500 -7500 -7500 -12500 -12500 -12500 -12500 -5000 -5000 -5000 -5000 -15000 -15000 -15000 -15000];
C = [0 0 0; 0 0 1; 0 1 0; 0 1 1; 1 0 0; 1 0 1; 1 1 0; 0.5 0.5 0.5; 0 0 0; 0 0 1; 0 1 0; 0 1 1; 1 0 0; 1 0 1; 1 1 0; 0.5 0.5 0.5];

scatter3(x,y ,z,80, C, 'filled');
hold on
text(0, 0, -10000, 'X', 'FontSize', 18, 'HorizontalAlignment','center');
line([-2500, 0],[-2500, 0], [-5000, -10000], 'Color', [0 0 0]);
line([2500, 0],[-2500, 0], [-5000, -10000], 'Color', [0 0 1]);
line([-2500, 0],[2500, 0], [-5000, -10000], 'Color', [0 1 0]);
line([2500, 0],[2500, 0], [-5000, -10000], 'Color', [0 1 1]);
line([-2500, 0],[-2500, 0], [-15000, -10000], 'Color', [1 0 0]);
line([2500, 0],[-2500, 0], [-15000, -10000], 'Color', [1 0 1]);
line([-2500, 0],[2500, 0], [-15000, -10000], 'Color', [1 1 0]);
line([2500, 0],[2500, 0], [-15000, -10000], 'Color', [0.5 0.5 0.5]);

%draw the cube
%front and top pannel connected by 1 line
x = [-2500  2500  2500 -2500 -2500 -2500   2500   2500  -2500  -2500  ];
y = [-2500 -2500  2500  2500 -2500 -2500  -2500   2500   2500  -2500  ];
z = [-5000 -5000 -5000 -5000 -5000 -15000 -15000 -15000 -15000 -15000 ];
line(x,y,z, 'Color', [0 0.5 0.5]);
line([-2500 -2500], [2500 2500], [-5000 -15000], 'Color', [0 0.5 0.5]);
line([2500 2500], [2500 2500], [-5000 -15000], 'Color', [0 0.5 0.5]);
line([2500 2500], [-2500 -2500], [-5000 -15000], 'Color', [0 0.5 0.5]);

xlabel('x');
ylabel('y');
zlabel('z');
title('Reference grid for point orientation');


% title('U for pickpoint 03898')
% 
% title('U for pickpoint 04240')
% 
% title('V for pickpoint 03898')
% 
% title('V for pickpoint 04240')
% 
% title('W for pickpoint 03898')
% 
% title('W for pickpoint 04240')