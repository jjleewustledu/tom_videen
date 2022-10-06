/*  $Id: put_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $
    $Log: put_mh7.c,v $
 * Revision 1.3  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/06  13:29:08  tom
 * assign version if it is 0 (CAPP requires this)
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write main header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/put_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             put_mh7 (file)
	ImageFile      *file;
{
	union temp_short {
		short           s[LENREC_S];
		char            c[LENREC];
	}               tmp;

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               tmpl;

	Main_header7   *mh7;
	int             recnum;
	int             i, j, len;

	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [put_mh7]: No version 7 header\n");
		return (ERROR);
	}
	if (file->m7->mh == NULL) {
		fprintf (stderr, "ERROR [put_mh7]: No version 7 MainHeader\n");
		return (ERROR);
	}
	mh7 = file->m7->mh;
	for (i = 0; i < 14; i++)
		tmp.c[i] = mh7->magic_number[i];
	for (i = 0; i < 32; i++)
		tmp.c[14 + i] = mh7->original_file_name[i];
	if (mh7->sw_version > 0)
		tmp.s[23] = mh7->sw_version;
	else
		tmp.s[23] = 71;
	tmp.s[24] = mh7->system_type;
	tmp.s[25] = mh7->file_type;
	for (i = 0; i < 10; i++)
		tmp.c[52 + i] = mh7->serial_number[i];

	tmpl.l = mh7->scan_start_time;
	tmp.s[31] = tmpl.s[0];
	tmp.s[32] = tmpl.s[1];

	for (i = 0; i < 8; i++)
		tmp.c[66 + i] = mh7->isotope_code[i];

	tmpl.f = mh7->isotope_halflife;
	tmp.s[37] = tmpl.s[0];
	tmp.s[38] = tmpl.s[1];

	for (i = 0; i < 32; i++)
		tmp.c[78 + i] = mh7->radiopharmaceutical[i];

	tmpl.f = mh7->gantry_tilt;
	tmp.s[55] = tmpl.s[0];
	tmp.s[56] = tmpl.s[1];

	tmpl.f = mh7->gantry_rotation;
	tmp.s[57] = tmpl.s[0];
	tmp.s[58] = tmpl.s[1];

	tmpl.f = mh7->bed_elevation;
	tmp.s[59] = tmpl.s[0];
	tmp.s[60] = tmpl.s[1];

	tmpl.f = mh7->intrinsic_tilt;
	tmp.s[61] = tmpl.s[0];
	tmp.s[62] = tmpl.s[1];

	tmp.s[63] = mh7->wobble_speed;
	tmp.s[64] = mh7->transm_source_type;

	tmpl.f = mh7->distance_scanned;
	tmp.s[65] = tmpl.s[0];
	tmp.s[66] = tmpl.s[1];

	tmpl.f = mh7->transaxial_fov;
	tmp.s[67] = tmpl.s[0];
	tmp.s[68] = tmpl.s[1];

	tmp.s[69] = mh7->angular_compression;
	tmp.s[70] = mh7->coin_samp_mode;
	tmp.s[71] = mh7->axial_samp_mode;

	tmpl.f = mh7->calibration_factor;
	tmp.s[72] = tmpl.s[0];
	tmp.s[73] = tmpl.s[1];

	tmp.s[74] = mh7->calibration_units;
	tmp.s[75] = mh7->calibration_units_label;
	tmp.s[76] = mh7->compression_code;

	for (i = 0; i < 12; i++)
		tmp.c[154 + i] = mh7->study_type[i];
	for (i = 0; i < 16; i++)
		tmp.c[166 + i] = mh7->patient_id[i];
	for (i = 0; i < 32; i++)
		tmp.c[182 + i] = mh7->patient_name[i];
	tmp.c[214] = mh7->patient_sex[0];
	tmp.c[215] = mh7->patient_dexterity[0];

	tmpl.f = mh7->patient_age;
	tmp.s[108] = tmpl.s[0];
	tmp.s[109] = tmpl.s[1];

	tmpl.f = mh7->patient_height;
	tmp.s[110] = tmpl.s[0];
	tmp.s[111] = tmpl.s[1];

	tmpl.f = mh7->patient_weight;
	tmp.s[112] = tmpl.s[0];
	tmp.s[113] = tmpl.s[1];

	tmpl.l = mh7->patient_birth_date;
	tmp.s[114] = tmpl.s[0];
	tmp.s[115] = tmpl.s[1];

	for (i = 0; i < 32; i++)
		tmp.c[232 + i] = mh7->physician_name[i];
	for (i = 0; i < 32; i++)
		tmp.c[264 + i] = mh7->operator_name[i];
	for (i = 0; i < 32; i++)
		tmp.c[296 + i] = mh7->study_description[i];

	tmp.s[164] = mh7->acquisition_type;
	tmp.s[165] = mh7->patient_orientation;

	for (i = 0; i < 20; i++)
		tmp.c[332 + i] = mh7->facility_name[i];

	tmp.s[176] = mh7->num_planes;
	tmp.s[177] = mh7->num_frames;
	tmp.s[178] = mh7->num_gates;
	tmp.s[179] = mh7->num_bed_pos;

	tmpl.f = mh7->init_bed_position;
	tmp.s[180] = tmpl.s[0];
	tmp.s[181] = tmpl.s[1];

	for (i = 0, j = 0; i < 16; i++, j += 2) {
		tmpl.f = mh7->bed_offset[i];
		tmp.s[182 + j] = tmpl.s[0];
		tmp.s[183 + j] = tmpl.s[1];
	}

	tmpl.f = mh7->plane_separation;
	tmp.s[212] = tmpl.s[0];
	tmp.s[213] = tmpl.s[1];

	tmp.s[214] = mh7->lwr_sctr_thres;
	tmp.s[215] = mh7->lwr_true_thres;
	tmp.s[216] = mh7->upr_true_thres;

	for (i = 0; i < 10; i++)
		tmp.c[434 + i] = mh7->user_process_code[i];
	tmp.s[222] = mh7->acquisition_mode;

	tmpl.f = mh7->bin_size;
	tmp.s[223] = tmpl.s[0];
	tmp.s[224] = tmpl.s[1];

	tmpl.f = mh7->branching_fraction;
	tmp.s[225] = tmpl.s[0];
	tmp.s[226] = tmpl.s[1];

	tmpl.l = mh7->dose_start_time;
	tmp.s[227] = tmpl.s[0];
	tmp.s[228] = tmpl.s[1];

	tmpl.f = mh7->dosage;
	tmp.s[229] = tmpl.s[0];
	tmp.s[230] = tmpl.s[1];

	tmpl.f = mh7->well_counter_factor;
	tmp.s[231] = tmpl.s[0];
	tmp.s[232] = tmpl.s[1];

	for (i = 0; i < 32; i++)
		tmp.c[466 + i] = mh7->data_units[i];
	tmp.s[249] = mh7->septa_state;

	for (i = 0; i < 6; i++)
		tmp.s[250 + i] = mh7->fill[i];

	len = 1;
	recnum = 1;
	if (wrtrec (file->fp, (char *) tmp.s, recnum, len) != 0) {
		fprintf (stderr, "ERROR [put_mh7]: Cannot write main header\n");
		return (ERROR);
	} else
		return (OK);
}
