/*  $Id: get_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $
 *	$Log: get_mh7.c,v $
 * Revision 1.3  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_mh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read main header of version 7 matrix file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/get_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_mh7 (file)
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
	int             recnum, lenrd;
	int             i, j;

	if (file->m7->mh == NULL) {
		if ((mh7 = (Main_header7 *) calloc (1, sizeof (Main_header7))) == NULL) {
			(void) fprintf (stderr, "ERROR [get_mh7]: Cannot calloc mh7\n");
			return (ERROR);
		}
	} else
		mh7 = file->m7->mh;
	lenrd = 1;
	recnum = 1;
	if (rdrec (file->fp, (char *) tmp.s, recnum, lenrd) != 0) {
		(void) fprintf (stderr, "ERROR [get_mh7]: Cannot read MainHeader\n");
		file->m7->mh = mh7;
		return (ERROR);
	}
	for (i = 0; i < 14; i++)
		mh7->magic_number[i] = tmp.c[i];
	for (i = 0; i < 32; i++)
		mh7->original_file_name[i] = tmp.c[14 + i];
	mh7->sw_version = tmp.s[23];
	mh7->system_type = tmp.s[24];
	mh7->file_type = tmp.s[25];
	if (mh7->file_type == 2)		 /* Correct for changes made by nonstandard code */
		mh7->file_type = (int) PetVolume;
	for (i = 0; i < 10; i++)
		mh7->serial_number[i] = tmp.c[52 + i];

	tmpl.s[0] = tmp.s[31];
	tmpl.s[1] = tmp.s[32];
	mh7->scan_start_time = tmpl.l;

	for (i = 0; i < 8; i++)
		mh7->isotope_code[i] = tmp.c[66 + i];

	tmpl.s[0] = tmp.s[37];
	tmpl.s[1] = tmp.s[38];
	mh7->isotope_halflife = tmpl.f;

	for (i = 0; i < 32; i++)
		mh7->radiopharmaceutical[i] = tmp.c[78 + i];

	tmpl.s[0] = tmp.s[55];
	tmpl.s[1] = tmp.s[56];
	mh7->gantry_tilt = tmpl.f;

	tmpl.s[0] = tmp.s[57];
	tmpl.s[1] = tmp.s[58];
	mh7->gantry_rotation = tmpl.f;

	tmpl.s[0] = tmp.s[59];
	tmpl.s[1] = tmp.s[60];
	mh7->bed_elevation = tmpl.f;

	tmpl.s[0] = tmp.s[61];
	tmpl.s[1] = tmp.s[62];
	mh7->intrinsic_tilt = tmpl.f;

	mh7->wobble_speed = tmp.s[63];
	mh7->transm_source_type = tmp.s[64];

	tmpl.s[0] = tmp.s[65];
	tmpl.s[1] = tmp.s[66];
	mh7->distance_scanned = tmpl.f;

	tmpl.s[0] = tmp.s[67];
	tmpl.s[1] = tmp.s[68];
	mh7->transaxial_fov = tmpl.f;

	mh7->angular_compression = tmp.s[69];
	mh7->coin_samp_mode = tmp.s[70];
	mh7->axial_samp_mode = tmp.s[71];

	tmpl.s[0] = tmp.s[72];
	tmpl.s[1] = tmp.s[73];
	mh7->calibration_factor = tmpl.f;

	mh7->calibration_units = tmp.s[74];
	mh7->calibration_units_label = tmp.s[75];
	mh7->compression_code = tmp.s[76];

	for (i = 0; i < 12; i++)
		mh7->study_type[i] = tmp.c[154 + i];
	for (i = 0; i < 15; i++)
		mh7->patient_id[i] = tmp.c[166 + i];
	for (i = 0; i < 32; i++)
		mh7->patient_name[i] = tmp.c[182 + i];
	mh7->patient_sex[0] = tmp.c[214];
	mh7->patient_dexterity[0] = tmp.c[215];

	tmpl.s[0] = tmp.s[108];
	tmpl.s[1] = tmp.s[109];
	mh7->patient_age = tmpl.f;

	tmpl.s[0] = tmp.s[110];
	tmpl.s[1] = tmp.s[111];
	mh7->patient_height = tmpl.f;

	tmpl.s[0] = tmp.s[112];
	tmpl.s[1] = tmp.s[113];
	mh7->patient_weight = tmpl.f;

	tmpl.s[0] = tmp.s[114];
	tmpl.s[1] = tmp.s[115];
	mh7->patient_birth_date = tmpl.l;

	for (i = 0; i < 32; i++)
		mh7->physician_name[i] = tmp.c[232 + i];
	for (i = 0; i < 32; i++)
		mh7->operator_name[i] = tmp.c[264 + i];
	for (i = 0; i < 32; i++)
		mh7->study_description[i] = tmp.c[296 + i];

	mh7->acquisition_type = tmp.s[164];
	mh7->patient_orientation = tmp.s[165];

	for (i = 0; i < 20; i++)
		mh7->facility_name[i] = tmp.c[332 + i];

	mh7->num_planes = tmp.s[176];
	mh7->num_frames = tmp.s[177];
	mh7->num_gates = tmp.s[178];
	mh7->num_bed_pos = tmp.s[179];

	tmpl.s[0] = tmp.s[180];
	tmpl.s[1] = tmp.s[181];
	mh7->init_bed_position = tmpl.f;

	for (i = 0, j = 0; i < 16; i++, j += 2) {
		tmpl.s[0] = tmp.s[182 + j];
		tmpl.s[1] = tmp.s[183 + j];
		mh7->bed_offset[i] = tmpl.f;
	}

	tmpl.s[0] = tmp.s[212];
	tmpl.s[1] = tmp.s[213];
	mh7->plane_separation = tmpl.f;

	mh7->lwr_sctr_thres = tmp.s[214];
	mh7->lwr_true_thres = tmp.s[215];
	mh7->upr_true_thres = tmp.s[216];

	for (i = 0; i < 10; i++)
		mh7->user_process_code[i] = tmp.c[434 + i];
	mh7->acquisition_mode = tmp.s[222];

	tmpl.s[0] = tmp.s[223];
	tmpl.s[1] = tmp.s[224];
	mh7->bin_size = tmpl.f;

	tmpl.s[0] = tmp.s[225];
	tmpl.s[1] = tmp.s[226];
	mh7->branching_fraction = tmpl.f;

	tmpl.s[0] = tmp.s[227];
	tmpl.s[1] = tmp.s[228];
	mh7->dose_start_time = tmpl.l;

	tmpl.s[0] = tmp.s[229];
	tmpl.s[1] = tmp.s[230];
	mh7->dosage = tmpl.f;

	tmpl.s[0] = tmp.s[231];
	tmpl.s[1] = tmp.s[232];
	mh7->well_counter_factor = tmpl.f;

	for (i = 0; i < 10; i++)
		mh7->data_units[i] = tmp.c[466 + i];
	mh7->septa_state = tmp.s[249];

	for (i = 0; i < 6; i++)
		mh7->fill[i] = tmp.s[250 + i];

	file->m7->mh = mh7;
	return (OK);
}
