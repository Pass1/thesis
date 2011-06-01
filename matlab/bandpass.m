function data=bandpass(data,lcorner,hcorner,dt,N)
%
%  data = bandpass filters seismograms (2-pass)
%
%  The input data matrix has seismograms arranged as column
%  vectors, default values of lcorner = 1 Hz, hcorner = 9 Hz,
%  and dt = 0.01 are used as well as N =2.
%
%  data=bandpass(data,lcorner,hcorner,dt,N) is the complete 
%  function call to specify desired parameters.
%  To use default values for any argument input the empty matrix.
%
%  David Schaff 8-8-98	modified from Doug Dodge.
%  included option to handle order manually (mmai, 05/19/00)
%  ----------------------------------------------------------

if nargin == 1
   lcorner=1;
   hcorner=9;
   dt=0.01;
   N = 2;
elseif nargin < 5
   error('Need either 1 or 5 input arguments.')
else
   if isempty(lcorner), lcorner=1; end
   if isempty(hcorner), hcorner=9; end
   if isempty(dt), dt=0.01; end
   if isempty(N), N = 2; end
end

   [b,a]=butter(N,[lcorner*dt*2 hcorner*dt*2]);
   data=filtfilt(b,a,data);
