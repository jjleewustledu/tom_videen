/*  $Id: get_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp tom $
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
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Read main header of version 7 matrix file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/get_mh7.c,v 1.3 2000/09/27 19:03:28 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

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
	}               conv;

	Main_header7   *mh7;
	int             i, j, k;
	int             recnum = 1;
	int             nrec = 1;

	if (file->m7->mh == NULL) {
		if ((mh7 = (Main_header7 *) calloc (1, sizeof (Main_header7))) == NULL) {
			(void) fprintf (stderr, "ERROR [get_mh7]: Cannot calloc mh7\n");
			return (ERROR);
		}
	} else
		mh7 = file->m7->mh;
	if (rdrec (file->fp, tmp.c, recnum, nrec) != 0) {
		(void) fprintf (stderr, "ERROR [get_mh7]: Cannot read MainHeader\n");
		file->m7->mh = mh7;
		return (ERROR);
	}
	/* Strings	 */
	for (i = 0; i < 14; i++)
		mh7->magic_number[i] = tmp.c[i];
	for (i = 0; i < 32; i++)
		mh7->original_file_name[i] = tmp.c[14 + i];
	for (i = 0; i < 10; i++)
		mh7->serial_number[i] = tmp.c[52 + i];
	for (i = 0; i < 8; i++)
		mh7->isotope_code[i] = tmp.c[66 + i];
	for (i = 0; i < 32; i++)
		mh7->radiopharmaceutical[i] = tmp.c[78 + i];
	for (i = 0; i < 12; i++)
		mh7->study_type[i] = tmp.c[154 + i];
	for (i = 0; i < 15; i++)
		mh7->patient_id[i] = tmp.c[166 + i];
	for (i = 0; i < 32; i++)
		mh7->patient_name[i] = tmp.c[182 + i];
	mh7->patient_sex[0] = tmp.c[214];
	mh7->patient_dexterity[0] = tmp.c[215];
	for (i = 0; i < 32; i++)
		mh7->physician_name[i] = tmp.c[232 + i];
	for (i = 0; i < 32; i++)
		mh7->operator_name[i] = tmp.c[264 + i];
	for (i = 0; i < 32; i++)
		mh7->study_description[i] = tmp.c[296 + i];
	for (i = 0; i < 20; i++)
		mh7->facility_name[i] = tmp.c[332 + i];
	for (i = 0; i < 10; i++)
		mh7->user_process_code[i] = tmp.c[434 + i];
	for (i = 0; i < 10; i++)
		mh7->data_units[i] = tmp.c[466 + i];

	/* Shorts	 */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte (tmp.c, LENREC);

	mh7->sw_version = tmp.s[23];
	mh7->system_type = tmp.s[24];
	mh7->file_type = tmp.s[25];
	if (mh7->file_type == 2)		 /* Correct for changes made by nonstandard code */
		mh7->file_type = (int) PetVolume;
	mh7->wobble_speed = tmp.s[63];
	mh7->transm_source_type = tmp.s[64];
	mh7->angular_compression = tmp.s[69];
	mh7->coin_samp_mode = tmp.s[70];
	mh7->axial_samp_mode = tmp.s[71];
	mh7->calibration_units = tmp.s[74];
	mh7->calibration_units_label = tmp.s[75];
	mh7->compression_code = tmp.s[76];
	mh7->acquisition_type = tmp.s[164];
	mh7->patient_orientation = tmp.s[165];
	mh7->num_planes = tmp.s[176];
	mh7->num_frames = tmp.s[177];
	mh7->num_gates = tmp.s[178];
	mh7->num_bed_pos = tmp.s[179];
	mh7->lwr_sctr_thres = tmp.s[214];
	mh7->lwr_true_thres = tmp.s[215];
	mh7->upr_true_thres = tmp.s[216];
	mh7->acquisition_mode = tmp.s[222];
	mh7->septa_state = tmp.s[249];
	for (i = 0; i < 6; i++)
		mh7->fill[i] = tmp.s[250 + i];

	/* Longs and Floats	 */
	/* CONSTCOND */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		j = 1;
	else
		j = 0;

	conv.s[0 + j] = tmp.s[31];
	conv.s[1 - j] = tmp.s[32];
	mh7->scan_start_time = conv.l;

	conv.s[0 + j] = tmp.s[37];
	conv.s[1 - j] = tmp.s[38];
	mh7->isotope_halflife = conv.f;

	conv.s[0 + j] = tmp.s[55];
	conv.s[1 - j] = tmp.s[56];
	mh7->gantry_tilt = conv.f;

	conv.s[0 + j] = tmp.s[57];
	conv.s[1 - j] = tmp.s[58];
	mh7->gantry_rotation = conv.f;

	conv.s[0 + j] = tmp.s[59];
	conv.s[1 - j] = tmp.s[60];
	mh7->bed_elevation = conv.f;

	conv.s[0 + j] = tmp.s[61];
	conv.s[1 - j] = tmp.s[62];
	mh7->intrinsic_tilt = conv.f;

	conv.s[0 + j] = tmp.s[65];
	conv.s[1 - j] = tmp.s[66];
	mh7->distance_scanned = conv.f;

	conv.s[0 + j] = tmp.s[67];
	conv.s[1 - j] = tmp.s[68];
	mh7->transaxial_fov = conv.f;

	conv.s[0 + j] = tmp.s[72];
	conv.s[1 - j] = tmp.s[73];
	mh7->calibration_factor = conv.f;

	conv.s[0 + j] = tmp.s[108];
	conv.s[1 - j] = tmp.s[109];
	mh7->patient_age = conv.f;

	conv.s[0 + j] = tmp.s[110];
	conv.s[1 - j] = tmp.s[111];
	mh7->patient_height = conv.f;

	conv.s[0 + j] = tmp.s[112];
	conv.s[1 - j] = tmp.s[113];
	mh7->patient_weight = conv.f;

	conv.s[0 + j] = tmp.s[114];
	conv.s[1 - j] = tmp.s[115];
	mh7->patient_birth_date = conv.l;

	conv.s[0 + j] = tmp.s[180];
	conv.s[1 - j] = tmp.s[181];
	mh7->init_bed_position = conv.f;

	for (i = 0, k = 0; i < 16; i++, k += 2) {
		conv.s[0 + j] = tmp.s[182 + k];
		conv.s[1 - j] = tmp.s[183 + k];
		mh7->bed_offset[i] = conv.f;
	}

	conv.s[0 + j] = tmp.s[212];
	conv.s[1 - j] = tmp.s[213];
	mh7->plane_separation = conv.f;

	conv.s[0 + j] = tmp.s[223];
	conv.s[1 - j] = tmp.s[224];
	mh7->bin_size = conv.f;

	conv.s[0 + j] = tmp.s[225];
	conv.s[1 - j] = tmp.s[226];
	mh7->branching_fraction = conv.f;

	conv.s[0 + j] = tmp.s[227];
	conv.s[1 - j] = tmp.s[228];
	mh7->dose_start_time = conv.l;

	conv.s[0 + j] = tmp.s[229];
	conv.s[1 - j] = tmp.s[230];
	mh7->dosage = conv.f;

	conv.s[0 + j] = tmp.s[231];
	conv.s[1 - j] = tmp.s[232];
	mh7->well_counter_factor = conv.f;

	file->m7->mh = mh7;
	return (OK);
}
