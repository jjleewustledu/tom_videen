pro mhdrdmp7,infile

mh = rd_mhdr(infile)

mhdrbase = widget_base(/COLUMN)
filname = widget_label(mhdrbase,value=string("File name: ", $
				infile),/ALIGN_LEFT)
system_type = widget_label(mhdrbase,value=string("System_type: ", $
				mh.system_type),/ALIGN_LEFT)
file_type = widget_label(mhdrbase,value=string("File type: ", $
				mh.file_type),/ALIGN_LEFT)
scan_start_time = widget_label(mhdrbase,value=string("Start time: ", $
				mh.scan_start_time),/ALIGN_LEFT)
num_planes = widget_label(mhdrbase,value=string("Number of planes: ", $
				mh.num_planes),/ALIGN_LEFT)
num_frames = widget_label(mhdrbase,value=string("Number of frames: ", $
				mh.num_frames),/ALIGN_LEFT)
num_gates = widget_label(mhdrbase,value=string("Number of gates: ", $
				mh.num_gates),/ALIGN_LEFT)
num_gates = widget_label(mhdrbase,value=string("Number of bed positions: ", $
				mh.num_bed_pos),/ALIGN_LEFT)
init_bed_position = widget_label(mhdrbase,value=string("Initial bed position: ", $
				mh.init_bed_position),/ALIGN_LEFT)
patient_id = widget_label(mhdrbase,value=string("Patient ID: ", $
			string(mh.patient_id)),/ALIGN_LEFT)
patient_name = widget_label(mhdrbase,value=string("Patient Name: ", $
			string(mh.patient_name)),/ALIGN_LEFT)
study_desc = widget_label(mhdrbase,value=string("Study description: ", $
			string(mh.study_desc)),/ALIGN_LEFT)
acq_type = widget_label(mhdrbase,value=string("Acquisition type: ", $
			mh.acquisition_type),/ALIGN_LEFT)

widget_control,mhdrbase,/REALIZE

return
end
