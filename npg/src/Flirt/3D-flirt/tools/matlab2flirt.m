function matlab2flirt(T,R,filenameT,filenameR);
% function matlab2flirt(T,R,filenameT,filenameR);
%
% (c) N.Papenberg, K.Ens, SAFIR, Luebeck, Germany, 2006
%
% Transforming Matlab 3D data to a format
% confirm with the flirt 3D-interface
%
% input:
%	- T,R 			: 	3D matlab image data
%	- filenameT,
%	  filenameR		:   name of inputfiles for the flirt-toolbox
%
		

m = size(T);

T = shiftdim(T,2);
T = permute(T,[1 3 2]);

R = shiftdim(R,2);
R = permute(R,[1 3 2]);

fid = fopen(filenameT,'w');
fwrite(fid,T(:),'double');
fclose(fid);
fid = fopen(filenameR,'w');
fwrite(fid,R(:),'double');
fclose(fid);

