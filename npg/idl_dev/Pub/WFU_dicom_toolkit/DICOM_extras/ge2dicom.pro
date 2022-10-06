function ge2dicom,rh

;--------------------------------
;Converts GE Raw header to Dicom
;--------------------------------


dh = make_dicom_header()


;------------------------------------------------------------------------------------
;Length_to_End_RET		={group:'0008',element:'0001',value:'Unimplemented'}
;Specific_Character_Set		={group:'0008',element:'0005',value:'ISO_IR 100'}
;Image_Type		={group:'0008',element:'0008',value:'DERIVED\PRIMARY'}
;----------------------------------------------------------------------------------
;--------------------------------------------------------------------------------------
;Do not change SOP_Class_UID : indicates an MR image,1.2.840 indicates DICOM
;113619 = GE
;FakeHUPID = 200000
;Format will be dicomroot.idlspmid.timestamp.ptid.accession_number.series_number.image_number
;---------------------------------------------------------------------------------------
dh.SOP_Class_UID.value	=   '1.2.840.10008.5.1.4.1.1.4'
;dicomroot = '1.2.840'
;hupid = '200000'
;idlspmid = '200000'
;------------------------------------
;registered under IDLSPM with IANA
;www.isi.edu
;-------------------------------------
dicomroot = '1.3.6.1.4.1'
idlspmid = '12402'


timestamp = 		strtrim(rh.exam.ex_datetime,2)
patient_id= 		strtrim(rh.exam.ex_patid,2)
accession_number = 	strtrim(rh.exam.ex_reqnum,2)
series_number = 	strtrim(rh.series.se_no,2)
image_number=		strtrim(rh.image.im_no,2)
landmark = strtrim(rh.series.SE_LMHOR,2)
;Study_Instance_UID		'1.2.840.113619.2.1.1.2702429029.814.936099513.377'
;Series_Instance_UID		'1.2.840.113619.2.1.1.2702429029.814.936099513.634'
;dh.SOP_Instance_UID.value = 	'1.2.840.113619.2.1.1.2702429029.814.936099513.654'
;Frame_of_Reference_UID	=	'1.2.840.113619.2.1.1.2702429029.809.936099446.28'
dh.Study_Instance_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number 
dh.Series_Instance_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number + '.' + series_number
dh.SOP_Instance_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number + '.' + series_number + '.' + image_number
dh.Frame_of_Reference_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number + '.'  + landmark

sdate = invert_systime(rh.exam.ex_datetime)
month = sdate.month
day = sdate.day
if strlen(month) eq 1 then month = '0' + month
if strlen(day) eq 1 then day = '0' + day
dh.study_date.value = 		sdate.year + month + day
dh.series_date.value = 		sdate.year + month + day
dh.image_date.value = 		sdate.year + month + day
;dh.accession_number.value = 	strtrim(rh.exam.ex_no,2)
dh.accession_number.value = 	strtrim(rh.exam.ex_reqnum,2)
dh.modality.value = 		strtrim(rh.exam.ex_typ,2)
;dh.manufacturer.value =	'GE MEDICAL SYSTEMS'
dh.Institution_Name.value =	strtrim(rh.exam.ex_hospname,2)
dh.Referring_Physicians_Name.value =	strtrim(rh.exam.ex_refphy,2)
dh.Station_Name.value =		string(rh.series.se_pansysid)
	
dh.study_description.value = strtrim(rh.series.se_prtcl,2)
;dh.study_description.value = strtrim(rh.exam.ex_desc,2)
dh.series_description.value = strtrim(rh.series.se_desc,2)
;dh.Attending_Physicians_Name.value=
dh.Name_of_Physicians_Reading_Study.value= strtrim(rh.exam.ex_diagrad,2)

dh.patient_name.value = strtrim(rh.exam.ex_patname,2)
dh.patient_id.value = strtrim(rh.exam.ex_patid,2)
dh.patient_weight.value = strtrim(rh.exam.ex_patweight,2)

dh.patient_name.value = strtrim(rh.exam.ex_patname,2)
dh.patient_id.value = strtrim(rh.exam.ex_patid,2)
;Patient_Birthdate		={group:'0010',element:'0030',value:'19570303'}
sex='F'
if rh.exam.ex_patsex eq 1 then sex = 'M'
dh.Patient_Sex.value = sex
Other_Patient_Names = 		strtrim(rh.exam.ex_patname,2)
dh.Patient_Age.value =		strtrim(rh.exam.ex_patage,2) + 'Y'
dh.patient_weight.value = 	strtrim(rh.exam.ex_patweight,2)
dh.Additional_Patient_History.value = strtrim(rh.exam.ex_hist,2)


dh.Contrast_Bolus_Agent.value 	=	strtrim(rh.image.im_contrastiv,2)
;----------------------------------------------------------------------------
;strtrim(rh.image.SE_DESC,2) : Head,Sag,2D,Spin Echo,None
;Scanning_Sequence		={group:'0018',element:'0020',value:'SE'}
;Sequence_Variant		={group:'0018',element:'0021',value:'NONE'}
;Scan_Options		={group:'0018',element:'0022',value:'FC\SP'}
;MR_Acquisition_Type		={group:'0018',element:'0023',value:'2D'}
;-----------------------------------------------------------------------------
dh.sequence_name.value = strtrim(rh.image.im_psd_iname,2)
;Angio_Flag		={group:'0018',element:'0025',value:'Y'}
dh.slice_thickness.value =  strtrim(rh.image.im_slthick,2)
dh.repetition_time.value =  strtrim(rh.image.im_tr/1000.0,2)
dh.echo_time.value =  strtrim(rh.image.im_te/1000.0,2)
;dh.inversion_time.value =  strtrim(rh.image.im_ti/1000.0,2)
dh.Number_of_Averages.value =		strtrim(rh.image.IM_NEX,2)
dh.Imaging_Frequency.value =	strtrim(rh.image.IM_XMTFREQ/1e7,2)
;Imaged_Nucleus		={group:'0018',element:'0085',value:'H1'}
dh.echo_number.value = strtrim(rh.image.im_echonum,2)
dh.magnetic_field_strength.value = strtrim(rh.exam.ex_magstrength,2)
dh.spacing_between_slices.value = strtrim(rh.image.im_scanspacing + rh.image.im_slthick,2)

;-------------------------------------------------------------------------------------------
;Echo_Train_Length		={group:'0018',element:'0091',value:'0'}
;Percent_Sampling		={group:'0018',element:'0093',value:'100.000000'}
;Percent_Sampling_Field_of_View		={group:'0018',element:'0094',value:'75.000000'}
;Pixel_Bandwidth		={group:'0018',element:'0095',value:'125.000000'}
;Software_Version		={group:'0018',element:'1020',value:'07'}
;Contrast_Bolus_Route		={group:'0018',element:'1040',value:'IV'}
;Spatial_Resolution		={group:'0018',element:'1050',value:'1.145833'}
;Nominal_Interval		={group:'0018',element:'1062',value:'566'}
;Heart_Rate		={group:'0018',element:'1088',value:'0'}
;Cardiac_Number_of_Frames		={group:'0018',element:'1090',value:'0'}
;Trigger_Window		={group:'0018',element:'1094',value:'0'}
;-------------------------------------------------------------------------------------------

dh.Reconstruction_Diameter.value = strtrim(rh.image.im_dfov,2)
;dh.Reconstruction_Diameter.value = strtrim(rh.image.im_dfov_rect,2)
dh.Receiving_Coil.value	=strtrim(rh.image.IM_CNAME,2)
dh.Transmitting_Coil.value =strtrim(rh.image.IM_CNAME,2)
dh.Acquisition_Matrix.value = strtrim(rh.image.im_imatrix_x,2) + ' ' + strtrim(rh.image.im_imatrix_y,2)
dh.flip_angle.value = strtrim(rh.image.im_mr_flip,2)
;Variable_Flip_Angle		={group:'0018',element:'1315',value:'N'}
dh.SAR.value	=		strtrim(rh.image.im_saravg,2)
dh.Patient_Position.value='HFS'
dh.Study_ID.value	=		strtrim(rh.exam.ex_no,2)
dh.Series_Number.value	 = 		strtrim(rh.series.se_no,2)
;Acquisition_Number		={group:'0020',element:'0012',value:'1'}
dh.Image_Number.value		=	strtrim(rh.image.im_no,2)

plane = fix(rh.image.im_plane)
orientation = '1.000000\0.000000\0.000000\0.000000\1.000000\0.000000'
Case plane of
	2: begin
		pid = 'L\P'
		orientation = '1.000000\0.000000\0.000000\0.000000\1.000000\0.000000'
		end
	4: begin
		pid = 'P\F'
		orientation = '0.000000\1.000000\0.000000\0.000000\0.000000\ -1.000000'
		end
	6: begin
		pid = 'L\F'
		orientation = '1.000000\0.000000\0.000000\0.000000\1.000000\0.000000'	;? I don't know what coronal is now
		end
	else:  pid = 'L\P' 
endcase
;---------------------------
;RAS coordinates
;---------------------------
tlhc=	rh.image.im_tlhc
trhc=	rh.image.im_trhc
brhc=	rh.image.im_brhc 
R = -tlhc(0)
A = -tlhc(1)
S = tlhc(2)

dh.Patient_Orientation.value	=	pid
dh.Image_Position_Patient.value	=	strtrim(R,2) + '\' + strtrim(A,2) + '\' + strtrim(S,2)
dh.Image_Orientation_Patient.value =	orientation
dh.Images_in_Acquisition.value		=strtrim(rh.image.im_slquant,2)
;Position_Reference_Indicator		={group:'0020',element:'1040',value:'NA'}
Case plane of
	2: dh.Slice_Location.value = strtrim(S,2)
	4: dh.Slice_Location.value = strtrim(R,2)
	6: dh.Slice_Location.value = strtrim(A,2)
	else:  dh.Slice_Location.value		=	strtrim(rh.image.im_loc,2) 
endcase





;Samples_Per_Pixel		={group:'0028',element:'0002',value:'1 1'}
;Photometric_Interpretation		={group:'0028',element:'0004',value:'MONOCHROME2'}
;dh.Rows.value		=	strtrim(rh.image.im_imatrix_y,2)
;dh.Columns.value	=	strtrim(rh.image.im_imatrix_x,2)
dh.Pixel_Spacing.value 	=	strtrim(rh.image.im_pixsize_x,2) + '\' + strtrim(rh.image.im_pixsize_y,2)
;Bits_Allocated		={group:'0028',element:'0100',value:'10 16'}
;Bits_Stored		={group:'0028',element:'0101',value:'10 16'}
;High_Bit		={group:'0028',element:'0102',value:'f 15'}
;Pixel_Representation		={group:'0028',element:'0103',value:'1 1'}
;group_length_0029		={group:'0029',element:'0000',value:'3e 62'}
;Group_Length_0038		={group:'0038',element:'0000',value:'a 10'}
;Current_Patient_Location		={group:'0038',element:'0300',value:'S9'}
;group_length_7fd1		={group:'7fd1',element:'0000',value:'1a 26'}
;Group_Length_7fe0		={group:'7fe0',element:'0000',value:'20008 131080'}
;Pixel_Data		={group:'7fe0',element:'0010',value:'Data on disk'}



;ScannerName=string(rh.series.se_pansysid)
;PulseSequence=string(rh.image.IM_PSDNAME)
;CoilName=string(rh.image.IM_CNAME)

return,dh

end
