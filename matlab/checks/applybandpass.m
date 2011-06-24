%read the pickpoint limatlabpool(3)


root = '/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/';
pickpoints = dir(strcat(root, '*.dat'));

matlabpool(8)
parfor i = 1:numel(pickpoints)
    RAW = importdata(strcat(root, pickpoints(i).name), ' ', 5);
    RAW.data = bandpass(RAW.data(:,2:4), 0.001, 10, 0.01, 1);
    dlmwrite(strcat(root, int2str(i), '.bps'), RAW.data, ' ');
    if (mod(i, 10) == 0)
        disp(i);
    end;
end
matlabpool close;
%import the data.
%A = importdata('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/out_ssgf_big-pickpoint-05860-00045.dat', ' ', 5);
%apply bandpass filter
%A.data(:,2:4) = bandpass(A.data(:,2:4), 0.01, 1, 0.01, 2);
%dlmwrite('/home/passone/Documents/directed/raw/SIVssp_15k_3_uniform/edited',A.data,' ');
%A.data(1:10,1:4)