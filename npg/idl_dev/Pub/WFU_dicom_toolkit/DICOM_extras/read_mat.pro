function read_mat,filename = filename
;---------------------------------------------------------------------------------
;This program will read SPM .mat file and return embedded any M, mat,  or landmark
;For use with read_analyze_header for dicom generated files and magnet transform
;---------------------------------------------------------------------------------
if not(keyword_set(filename)) then filename = pickfile(title = 'CHoose .mat file', filter = '*.mat')
filename = filename(0)
if strlen(filename) le 0 then return,{error:1}

M = fltarr(4,4)
mat = fltarr(4,4)
landmark = 0
pipe_open,u

engput,u,M,'M'
engput,u,mat,'mat'
engput,u,filename,'filename'

engEval,u,"load(filename);"
engBufget,u,buf
print,buf
engget,u,M,'M'
;engeval,u,"sizemat = size(mat);"
;engget,u,sizemat,'sizemat'
;k = size(sizemat)
;if k(1) eq 2 then engget,u,mat,'mat' else mat = M
engeval,u,"mat1 = mat(:,:,1);
engeval,u,"if ~exist('landmark','var'), landmark = 0; end;"
engeval,u,"if landmark ==0 & exist('wfu_fields'), landmark = wfu_fields.wfu_HorizFrameOfRef; end;"
engget,u,mat,'mat1'
engget,u,landmark,'landmark'
pipe_close,u
if total(mat) eq 0 then mat = M
if total(M) eq 0 then m = mat
reply = {M:M,mat:mat,landmark:landmark,filename:filename,error:0}
return,reply
end
