This directory contains a suite of IDL programs for dealing with DICOM files.
The programs can be used to:
	convert ANALYZE images to DICOM 
	convert DICOM images to ANALYZE format (GE, Siemens Mosaic, Philips, anatomic and fMRI)
	send DICOM images from any Unix workstation to any DICOM PACS
If you use these routines please reference:
	Maldjian, JA, Listerud J, Khalsa S.  Integrating Post-processed Functional MR Images with PACS.  
	AJNR 2002. 23:1393-1397.

;--------------------------------------------------------------------------------------------------------------------
;J. Maldjian 5-12-03
;Note:  the GEheader2DICOM conversion capability is not included in this suite as it contains proprietary information
;--------------------------------------------------------------------------------------------------------------------


The most useful routines are:

For converting from ANALYZE to DICOM:

convert2dicom,analyze = 'analyze .img filename'

For sending DICOM images into a PACS:
send_dicom
(You will need to configure the dicom_host_table.txt file)


For converting from DICOM to ANALYZE:

cycle_dicom
dicom2analyze
read_dicomheader2
get_dcm_value (in read_dicomheader)




To compile the code:

start idl from the program directory
type: @run

;--------------------------------------------------------------------------------------------------
;convert2dicom,analyze = analyze,images =images,outputdir=outputdir
;--------------------------------------------------------------------------------------------------
;This program will take any images and create dicom files
;syntax:  convert2dicom,analyze = analyze,images = images
;analyze = the analyze.img filename to convert to DICOM 
;a widget will pop open allowing you to make some limited header mods
;If you don't have an analyze file, you can just send an image stack using the images option:
;		convert2dicom, images = imagestack  (ie, a 64x64x30 image array)
;J. Maldjian 7-24-2000
;--------------------------------------------------------------------------------------------------



;--------------------------------------------------------------------------------------------------
;send_dicom,hostname = hostname,files = files,aetitle = aetitle,calltitle = calltitle,port = port
;--------------------------------------------------------------------------------------------------
;This program will send dicom images into a PACS or DICOM database
;syntax:  send_dicom,hostname = hostname,files = files,aetitle = aetitle,calltitle = calltitle,port = port
;You can call this program simply using:
;	send_dicom
;It will prompt you for the directory of file names, and it will read the entries in the 
;	dicom_host_table.txt file
;This is a tab-delimitted file, with the format described in the file.
;Command line options when not using the dicom_host_table.txt file:
;hostname = the hostname of the PACS or DICOM Recipient
;files = string array of dicom filenames to send
;aetitle = application entity title
;calltitle = application call title
;port = port on DICOM recipient to send images
;For the GE AW workstations all you need to send is the hostname (The defaults should work)

;You can also just call 

;J. Maldjian 7-24-2000
;--------------------------------------------------------------------------------------------------



************************************************************
;pro cycle_dicom,dir = dir,keep = keep,discard = discard
;-----------------------------------------------------------------
;Program will cycle through a directory with DICOM subdirectories
;generating analyze files from any DICOM file types encountered
;cycle_dicom,dir = dir,discard = discard
;;set discard to the number of volumes to discard for any fMRI acquistions
;J. Maldjian
;------------------------------------------------------------------



***************************************************************************
;pro dicom2analyze,dir = dir,keep = keep,outpath = outpath,discard = discard,dh = dh,last_dh = last_dh
;----------------------------------------------------------------------------------
;This program will convert dicom files to analyze
;It will work on anatomics, functionals, and mosaics
;GE, Philips, Siemens tested
;It reads the dicom headers directly, and correctly fills magnet transform information
;I haven't found the dicom field for the landmark (rh.series.se_lmhor) yet
;The landmark is generated from the dicom frame_of_reference field
;syntax:   dicom2analyze,dir = dir
;set discard to the number of volumes to discard for any fMRI acquistions
;J. Maldjian 5-10-2003
;----------------------------------------------------------------------------------


*************************************************************
;function read_dicomheader2,filename = filename,image = image,coords = coords,error = error,numimages = numimages
;--------------------------------------------------------------------------
;This program will read a dicom file and return some useful descriptors
;dh = read_dicomheader2(filename = dicomfile,image = image,coords = coords, numimages = numimages)
;image = image in dicom file.  For mosaics it gets converted to a 3D volume.
;dh.an = analyze header
;coords is the coords which should go into csmemp.loc files
;Only send numimages for Philips fMRI files
;These files don't have the number of slices in the dicom header, so we compute it from the number of temporal positions and numimages
;For Philips fMRI files, send numimages (# of files in the directory) so that the program doesn't have to recompute it with each read
;J. Maldjian 5-8-03
;--------------------------------------------------------------------------


*************************************************************
;function get_dcm_value,filename = filename,description = description,print = print,group = group,element = element
;--------------------------------------------------------------------------
;This program will return the values of individual elements in a dicom header
;description should = text description of the field you want from
;the dicom_dictionary
;example:
;sl_thick = get_dcm_value(filename = dicom_file,description = 'Slice Thickness')
;you can also provide string values for group and element
;J. Maldjian  7-24-2000
;---------------------------------------------------------------------------

*************************************************************
;function parse_dicom,filename = filename
;----------------------------------------------------------------------------------
;This program will create a callable dicom header function (make_dicom_pheader.pro)
;based on the input dicom file, compile the function, and return the structure
;syntax: dh = parse_dicom(filename = dicom_filename)
;ALL The DICOM header elements are returned as a structure in dh.  
;The structure elements can be viewed using:
;	helpstruc,dh
;J. Maldjian 7-24-2000
;----------------------------------------------------------------------------------
