function make_dicom_header
;-----------------------------------------------------
;This program will construct a structure of the dicom header 
;substructures include group, element, and value 
;The code in this file was automatically 
;generated using the parse_dicom routine 
;The patient specific entries were manually stripped
;-----------------------------------------------------
 
;-----------------------------------------------------
;Now define all the substructures
;-----------------------------------------------------
 
Specific_Character_Set		={group:'0008',element:'0005',value:'ISO_IR 100'}
Image_Type		={group:'0008',element:'0008',value:'DERIVED\PRIMARY'}
SOP_Class_UID		={group:'0008',element:'0016',value:'1.2.840.10008.5.1.4.1.1.4'}
SOP_Instance_UID		={group:'0008',element:'0018',value:'1.2.840.200000.2.1.1.2702429029.814.936099513.654'}
Study_Date		={group:'0008',element:'0020',value:'19990831'}
Series_Date		={group:'0008',element:'0021',value:'19990831'}
Image_Date		={group:'0008',element:'0023',value:'19990831'}
Study_Time		={group:'0008',element:'0030',value:'142522.000'}
Series_Time		={group:'0008',element:'0031',value:'144813'}
Image_Time		={group:'0008',element:'0033',value:'144813'}
Accession_Number		={group:'0008',element:'0050',value:'00000000'}
Modality		={group:'0008',element:'0060',value:'MR'}
Manufacturer		={group:'0008',element:'0070',value:'GE MEDICAL SYSTEMS'}
Institution_Name		={group:'0008',element:'0080',value:'HOSP OF THE UNIV OF PA UNIT 5'}
Referring_Physicians_Name		={group:'0008',element:'0090',value:'RESEARCH'}
Station_Name		={group:'0008',element:'1010',value:'MR05OC0'}
Study_Description		={group:'0008',element:'1030',value:'RESEARCH'}
Series_Description		={group:'0008',element:'103e',value:'RESEARCH'}
Attending_Physicians_Name		={group:'0008',element:'1050',value:'RESEARCH'}
Name_of_Physicians_Reading_Study		={group:'0008',element:'1060',value:'NEURO'}
Operators_Name		={group:'0008',element:'1070',value:'AF'}
Manufacturer_Model_Name		={group:'0008',element:'1090',value:'GENESIS_SIGNA'}
Derivation_Description		={group:'0008',element:'2111',value:'Lossy Compressed'}
Patient_Name		={group:'0010',element:'0010',value:'RESEARCH PATIENT'}
Patient_ID		={group:'0010',element:'0020',value:'00000000'}
Patient_Birthdate		={group:'0010',element:'0030',value:'19570303'}
Patient_Sex		={group:'0010',element:'0040',value:'M'}
Other_Patient_Names		={group:'0010',element:'1001',value:'RESEARCH PATIENT'}
Patient_Age		={group:'0010',element:'1010',value:'042Y'}
Patient_Weight		={group:'0010',element:'1030',value:'86.183000'}
Additional_Patient_History		={group:'0010',element:'21b0',value:'RESEARCH FUSION'}
Contrast_Bolus_Agent		={group:'0018',element:'0010',value:'NONE'}
Scanning_Sequence		={group:'0018',element:'0020',value:'SE'}
Sequence_Variant		={group:'0018',element:'0021',value:'NONE'}
Scan_Options		={group:'0018',element:'0022',value:'FC\SP'}
MR_Acquisition_Type		={group:'0018',element:'0023',value:'2D'}
Sequence_Name		={group:'0018',element:'0024',value:'fcmemp'}
Angio_Flag		={group:'0018',element:'0025',value:'Y'}
Slice_Thickness		={group:'0018',element:'0050',value:'5.000000'}
Repetition_Time		={group:'0018',element:'0080',value:'566.664001'}
Echo_Time		={group:'0018',element:'0081',value:'20.000000'}
Number_of_Averages		={group:'0018',element:'0083',value:'1.000000'}
Imaging_Frequency		={group:'0018',element:'0084',value:'63.8748560000'}
Imaged_Nucleus		={group:'0018',element:'0085',value:'H1'}
Echo_Number		={group:'0018',element:'0086',value:'1'}
Magnetic_Field_Strength		={group:'0018',element:'0087',value:'15000'}
Spacing_Between_Slices		={group:'0018',element:'0088',value:'5.000000'}
Echo_Train_Length		={group:'0018',element:'0091',value:'0'}
Percent_Sampling		={group:'0018',element:'0093',value:'100.000000'}
Percent_Sampling_Field_of_View		={group:'0018',element:'0094',value:'75.000000'}
Pixel_Bandwidth		={group:'0018',element:'0095',value:'125.000000'}
Software_Version		={group:'0018',element:'1020',value:'07'}
Contrast_Bolus_Route		={group:'0018',element:'1040',value:'IV'}
Spatial_Resolution		={group:'0018',element:'1050',value:'1.145833'}
Nominal_Interval		={group:'0018',element:'1062',value:'566'}
Heart_Rate		={group:'0018',element:'1088',value:'0'}
Cardiac_Number_of_Frames		={group:'0018',element:'1090',value:'0'}
Trigger_Window		={group:'0018',element:'1094',value:'0'}
Reconstruction_Diameter		={group:'0018',element:'1100',value:'220.0000000000'}
Receiving_Coil		={group:'0018',element:'1250',value:'HEAD'}
Transmitting_Coil		={group:'0018',element:'1251',value:'HEAD'}
Acquisition_Matrix		={group:'0018',element:'1310',value:'0 0'}
Flip_Angle		={group:'0018',element:'1314',value:'90'}
Variable_Flip_Angle		={group:'0018',element:'1315',value:'N'}
SAR		={group:'0018',element:'1316',value:'0.038486'}
Patient_Position		={group:'0018',element:'5100',value:'HFS'}
Study_Instance_UID		={group:'0020',element:'000d',value:'1.2.840.200000.2.1.1.2702429029.814.936099513.377'}
Series_Instance_UID		={group:'0020',element:'000e',value:'1.2.840.200000.2.1.1.2702429029.814.936099513.634'}
Study_ID		={group:'0020',element:'0010',value:'2956707'}
Series_Number		={group:'0020',element:'0011',value:'6'}
Acquisition_Number		={group:'0020',element:'0012',value:'1'}
Image_Number		={group:'0020',element:'0013',value:'10'}
Patient_Orientation		={group:'0020',element:'0020',value:'L\P'}
Image_Position_Patient		={group:'0020',element:'0032',value:'-115.000000\ -110.000000\-40.000000'}
Image_Orientation_Patient		={group:'0020',element:'0037',value:'1.000000\0.000000\0.000000\0.000000\1.000000\0.000000'}
Frame_of_Reference_UID		={group:'0020',element:'0052',value:'1.2.840.113619.2.1.1.2702429029.809.936099446.28'}
Images_in_Acquisition		={group:'0020',element:'1002',value:'29'}
Position_Reference_Indicator		={group:'0020',element:'1040',value:'NA'}
Slice_Location		={group:'0020',element:'1041',value:'-40.0000000000'}
Samples_Per_Pixel		={group:'0028',element:'0002',value:'1 1'}
Photometric_Interpretation		={group:'0028',element:'0004',value:'MONOCHROME2'}
Rows		={group:'0028',element:'0010',value:'256'}
Columns		={group:'0028',element:'0011',value:'256'}
Pixel_Spacing		={group:'0028',element:'0030',value:'0.8593750000\0.8593750000'}
Bits_Allocated		={group:'0028',element:'0100',value:'16'}
Bits_Stored		={group:'0028',element:'0101',value:'16'}
Pixel_Representation		={group:'0028',element:'0103',value:'1 1'}
 
;-----------------------------------------------------
;Now define the parent dicom header structure
;-----------------------------------------------------
 
dheader = { $
Specific_Character_Set:Specific_Character_Set,$
Image_Type:Image_Type,$
SOP_Class_UID:SOP_Class_UID,$
SOP_Instance_UID:SOP_Instance_UID,$
Study_Date:Study_Date,$
Series_Date:Series_Date,$
Image_Date:Image_Date,$
Study_Time:Study_Time,$
Series_Time:Series_Time,$
Image_Time:Image_Time,$
Accession_Number:Accession_Number,$
Modality:Modality,$
Manufacturer:Manufacturer,$
Institution_Name:Institution_Name,$
Referring_Physicians_Name:Referring_Physicians_Name,$
Station_Name:Station_Name,$
Study_Description:Study_Description,$
Series_Description:Series_Description,$
Attending_Physicians_Name:Attending_Physicians_Name,$
Name_of_Physicians_Reading_Study:Name_of_Physicians_Reading_Study,$
Operators_Name:Operators_Name,$
Manufacturer_Model_Name:Manufacturer_Model_Name,$
Derivation_Description:Derivation_Description,$
Patient_Name:Patient_Name,$
Patient_ID:Patient_ID,$
Patient_Birthdate:Patient_Birthdate,$
Patient_Sex:Patient_Sex,$
Other_Patient_Names:Other_Patient_Names,$
Patient_Age:Patient_Age,$
Patient_Weight:Patient_Weight,$
Additional_Patient_History:Additional_Patient_History,$
Contrast_Bolus_Agent:Contrast_Bolus_Agent,$
Scanning_Sequence:Scanning_Sequence,$
Sequence_Variant:Sequence_Variant,$
Scan_Options:Scan_Options,$
MR_Acquisition_Type:MR_Acquisition_Type,$
Sequence_Name:Sequence_Name,$
Angio_Flag:Angio_Flag,$
Slice_Thickness:Slice_Thickness,$
Repetition_Time:Repetition_Time,$
Echo_Time:Echo_Time,$
Number_of_Averages:Number_of_Averages,$
Imaging_Frequency:Imaging_Frequency,$
Imaged_Nucleus:Imaged_Nucleus,$
Echo_Number:Echo_Number,$
Magnetic_Field_Strength:Magnetic_Field_Strength,$
Spacing_Between_Slices:Spacing_Between_Slices,$
Echo_Train_Length:Echo_Train_Length,$
Percent_Sampling:Percent_Sampling,$
Percent_Sampling_Field_of_View:Percent_Sampling_Field_of_View,$
Pixel_Bandwidth:Pixel_Bandwidth,$
Software_Version:Software_Version,$
Contrast_Bolus_Route:Contrast_Bolus_Route,$
Spatial_Resolution:Spatial_Resolution,$
Nominal_Interval:Nominal_Interval,$
Heart_Rate:Heart_Rate,$
Cardiac_Number_of_Frames:Cardiac_Number_of_Frames,$
Trigger_Window:Trigger_Window,$
Reconstruction_Diameter:Reconstruction_Diameter,$
Receiving_Coil:Receiving_Coil,$
Transmitting_Coil:Transmitting_Coil,$
Acquisition_Matrix:Acquisition_Matrix,$
Flip_Angle:Flip_Angle,$
Variable_Flip_Angle:Variable_Flip_Angle,$
SAR:SAR,$
Patient_Position:Patient_Position,$
Study_Instance_UID:Study_Instance_UID,$
Series_Instance_UID:Series_Instance_UID,$
Study_ID:Study_ID,$
Series_Number:Series_Number,$
Acquisition_Number:Acquisition_Number,$
Image_Number:Image_Number,$
Patient_Orientation:Patient_Orientation,$
Image_Position_Patient:Image_Position_Patient,$
Image_Orientation_Patient:Image_Orientation_Patient,$
Frame_of_Reference_UID:Frame_of_Reference_UID,$
Images_in_Acquisition:Images_in_Acquisition,$
Position_Reference_Indicator:Position_Reference_Indicator,$
Slice_Location:Slice_Location,$
Samples_Per_Pixel:Samples_Per_Pixel,$
Photometric_Interpretation:Photometric_Interpretation,$
Rows:Rows,$
Columns:Columns,$
Pixel_Spacing:Pixel_Spacing,$
Bits_Allocated:Bits_Allocated,$
Bits_Stored:Bits_Stored,$
Pixel_Representation:Pixel_Representation}
 
return,dheader
end
