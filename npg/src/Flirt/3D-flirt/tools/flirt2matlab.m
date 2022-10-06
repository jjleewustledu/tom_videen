function Tin = flirt2matlab(filenameIn,m)
% function Tin = flirt2matlab(filenameIn,m)
%
% (c) N.Papenberg, K.Ens, SAFIR, Luebeck, Germany, 2006
%
% Transforming data from the flirt-format to matlab
%
% input:
%	- filenameIn	:   the filename of the inputdata
%	- m				:	the image size
%
% output:
%	- Tin			: 	the data read from the file
%						and transposed to matlab's coordinate system
%
						
fid = fopen(filenameIn,'r');
Tin = fread(fid,'double');
fclose(fid);
Tin = reshape(Tin,m(3),m(2),m(1));
Tin = permute(Tin,[1,3,2]);
Tin = shiftdim(Tin,1);

