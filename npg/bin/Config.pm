# $Log: Config.pm,v $
# Revision 1.1  1999/09/10  15:15:03  tom
# Initial revision
#
# Revision 1.1  1998/12/30  19:22:50  tom
# Initial revision
#
# =====
# Filename: Config.pm
# Date: 8-27-98
# Purpose: Editable Dicom file characteristics.
#
#     Dicom fields:
#     Fields which are present in Dicom files appear to vary.
#     Therefore, it is not feasible to require that fields present
#     match a given template. Instead, we must define a subset of
#     fields which are necessary for our purposes. 
# =====

package Config;

# Significant fields:
# These fields are required by default.

@signif_fields = (
                'ID Image Type',
                'ID Image Date',
                'ID Image Time',
                'ID Study Description',
                'PAT Patient Name',
                'PAT Patient ID',
                'PAT Patient Birthdate',
                'ACQ Slice Thickness',
                'ACQ KVP',
                'ACQ Gantry/Detector Tilt',
                'ACQ Exposure Time',
                'ACQ Patient Position',
                'REL Acquisition Number',
                'REL Image Number',
                'REL Image Position Patient',
                'REL Slice Location',
                'IMG Rows',
                'IMG Columns',
                'IMG Pixel Spacing',
                'IMG Bits Allocated',
                'IMG Window Center',
                'IMG Window Width',
		 );

# Required fields:
# These are minimum fields required - used with '-f' ("force") option.

@required_fields = (
                'ID Image Type',
                'ID Image Date',
                'ID Image Time',
                'PAT Patient Name',
                'PAT Patient Birthdate',
                'ACQ Slice Thickness',
                'ACQ Gantry/Detector Tilt',
                'ACQ Patient Position',
                'REL Acquisition Number',
                'REL Image Number',
                'REL Image Position Patient',
                'IMG Rows',
                'IMG Columns',
                'IMG Pixel Spacing',
                'IMG Bits Allocated',
		 );
