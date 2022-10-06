function generate_dicom_series, directory = directory, filename = filename
;-----------------------------------------------------------------------
;Program generates a list of unique series in a directory of dicom files
;reply = generate_dicom_series(directory = directory)
;returns a structure with:
;	dcmseries : names of unique series
;	numseries : number of unique series
;	series_filter: string filter used
;	type  :	Dicom Manufacturer based on filename
;	directory : directory
;	file_lop : ending to chop off for creating image lists for a series
;
;J. Maldjian
;-------------------------------------------------
;Program has only been tested on these filetypes:
;GE
;1.2.840.2000000.1001521136.2598301.91.001
;-------------------------------------------------
;Phillips
;MR.9836.1
;--------------------------
;Siemens
;E00001S06I00001.dcm
;-------------------------

if keyword_set(filename) then extract_pname,filename,directory
filetypes = 	['GE',	'Philips',	'Siemens',	'GE_AW', 'Unknown']
series_filter = ['.001','*.1',		'*I0001*',	'*IM1' , '*']
file_lop = [3,4,8,strlen(directory) + 2,3]

;-----------------------------------------------
;Construc a list of the number of series
;-----------------------------------------------
numseries = -1
i = -1
while (numseries le 0) and (i lt n_elements(filetypes) - 1) do begin
	i = i + 1
	dcmseries = findfile(directory + series_filter(i),count = numseries)
endwhile

;-----------------------------------------------------------
;Failsafe: assume all files in this directory are 1 series
;-----------------------------------------------------------	
if i eq n_elements(filetypes) - 1 then begin
	numseries = numseries < 1
	dcmseries = dcmseries(0)
	;extract_pname,dcmseries,p
	;if strlen(p) le 1 then dcmseries = directory + dcmseries
	;print,dcmseries
endif
;-------------------------------------------------------------------------------------------------
;Philips fMRI series continue from 1st image, with same series number, but different directories
;-------------------------------------------------------------------------------------------------
extract_fname,dcmseries(0),fname
if strpos(fname,'MR') eq 0 then i = 1
	
reply = {dcmseries:dcmseries,numseries:numseries,series_filter:series_filter(i),type:filetypes(i),directory:directory,$
	file_lop:file_lop(i)}

return,reply
end


function generate_dicom_filelist,filename = filename,series_reply = series_reply
;------------------------------------------------------------------------
;Program generates a sorted list of files in a directory of dicom files
;dcmlist = generate_dicom_filelist(filename = filename)
;------------------------------------------------------------------------

series_reply = generate_dicom_series(filename = filename)
fsearch = strmid(filename,0,strlen(filename) - series_reply.file_lop)
if series_reply.type eq 'GE' or series_reply.type eq 'Philips' then begin
	dcmimages = findfile(fsearch + '*',count = numimages)
	dotpos = strpos(dcmimages(0),'.',/reverse_search)
	imnum = float(strmid(dcmimages,dotpos + 1,10))
	dcmimages = dcmimages(sort(imnum))
endif
if series_reply.type eq 'GE_AW'	then begin
		dcmimages = findfile(series_reply.directory + 'IM*',count = numimages)
		dcmimages = dcmimages(sort(long(strmid(dcmimages,series_reply.file_lop,5))))
endif
if not(keyword_set(dcmimages)) then dcmimages = findfile(fsearch + '*',count = numimages)

return,dcmimages
end








