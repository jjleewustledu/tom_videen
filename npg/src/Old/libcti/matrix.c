/*$Id: matrix.c,v 2.1 2003/03/10 22:35:51 tom Exp $
 *$Log: matrix.c,v $
 * Revision 2.1  2003/03/10  22:35:51  tom
 * *** empty log message ***
 *
 * Revision 2.0  2002/12/26  20:35:13  tom
 * remove unused functions
 * lint and splint cleaning
 *
 *Revision 1.8  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.7  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.6  1995/11/30  19:42:02  tom
 * TOV mod
 *
 * Revision 1.5  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.4  1994/03/03  17:29:43  ty7777
 * Change from id to header.
 *
 * Revision 1.3  1993/09/13  18:55:37  ty7777
 * Introduce acsid and change matrix.h to petutil/matrix.h.
 *
 * Revision 1.2  1993/06/17  17:29:55  ty7777
 * Checking in.
 *
 * Revision 1.1  1993/03/04  19:46:55  ty7777
 * Initial revision
 **/

/* @(#)matrix.c	2.4 4/24/92  */

#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <string.h>
#include <strings.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/Old/libcti/RCS/matrix.c,v 2.1 2003/03/10 22:35:51 tom Exp $";
#endif

void            swaw (from, to, length)
	short int      *from, *to;
	int             length;
{
	short int       temp;
	int             i;

	for (i = 0; i < length; i += 2) {
		temp = from[i + 1];
		to[i + 1] = from[i];
		to[i] = temp;
	}
	return;
}

void            swap_byte (from, to, length)
	char           *from, *to;
	int             length;
{
	char            temp;
	int             i;

	for (i = 0; i < length; i += 2) {
		temp = from[i + 1];
		to[i + 1] = from[i];
		to[i] = temp;
	}
	return;
}

int             get_vax_long (bufr, off)
	unsigned short *bufr;
	int             off;
{
	return ((bufr[off + 1] << 16) + bufr[off]);
}

float           get_vax_float (bufr, off)
	unsigned short  bufr[];
	int             off;
{
	unsigned short  t1, t2;
	union {
		unsigned long   t3;
		float           t4;
	}               test;

	if (bufr[off] == 0 && bufr[off + 1] == 0)
		return (0.0);
	t1 = bufr[off] & 0x80ff;
	t2 = (((bufr[off]) & 0x7f00) + 0xff00) & 0x7f00;
	test.t3 = (t1 + t2) << 16;
	test.t3 = test.t3 + bufr[off + 1];
	return (test.t4);
}

void            sunftovaxf (orig, number)
	float           orig;
	unsigned short  number[2];
{
	union {
		unsigned short  t[2];
		float           t4;
	}               test;
	unsigned short int exp;

	number[0] = 0;
	number[1] = 0;
	test.t4 = orig;
	if (test.t4 == 0.0)
		return;
	number[1] = test.t[1];
	exp = ((test.t[0] & 0x7f00) + 0x0100) & 0x7f00;
	test.t[0] = (test.t[0] & 0x80ff) + exp;
	number[0] = test.t[0];
	return;
}

void            sunltovaxl (in, out)
	int             in;
	unsigned short int out[2];
{
	out[0] = (in & 0x0000FFFF);
	out[1] = (in & 0xFFFF0000) >> 16;
	return;
}

int             str_find (s1, s2)
	char           *s1, *s2;
{
	int             i, j, k;

	for (i = 0; s1[i] != 0; i++) {
		for (j = i, k = 0; s2[k] != '\0' && s1[j] == s2[k]; j++, k++)
			if (s2[k] == '\0')
				return (i);
	} return (-1);
}

int             str_replace (s1, s2, s3, s4)
	char           *s1, *s2, *s3, *s4;
{
	int             nf = 0, n;
	int             busy = 1;

	*s1 = '\0';
	while (busy == 1) {
		if ((n = str_find (s2, s3)) == -1) {
			strcat (s1, s2);
			return (nf);
		} else {
			strncat (s1, s2, n);
			strcat (s1, s4);
			s2 += n + strlen (s3);
			nf++;
		}
	}
	return (0);
}

FILE           *mat_create (fname, mhead)
	char           *fname;
	Main_header    *mhead;
{
	FILE           *fptr;
	int            *bufr;
	int             stat;

	fptr = mat_open (fname, "w+");
	if (fptr == NULL)
		return (fptr);
	stat = mat_write_main_header (fptr, mhead);
	if (stat != 0) {
		(void) fclose (fptr);
		return (fptr);
	}
	bufr = (int *) calloc (MatBLKSIZE, sizeof (int));
	bufr[0] = 31;
	bufr[1] = 2;
	swaw (bufr, bufr, 256);
	swap_byte (bufr, bufr, 512);
	stat = mat_wblk (fptr, MatFirstDirBlk, bufr, 1);
	if (stat != 0)
		(void) fclose (fptr);
	free (bufr);
	return (fptr);
}

FILE           *mat_open (fname, fmode)
	char           *fname, *fmode;
{
	FILE           *fptr;

	fptr = fopen (fname, fmode);
	return (fptr);
}

int             mat_close (fptr)
	FILE           *fptr;
{
	return (fclose (fptr));
}

int             mat_numcod (frame, plane, gate, data, bed)
	int             frame, plane, gate, data, bed;
{
/*	Version supporting 512 planes, 1024 planes, 8 data types	*/
	int             matnum8data16bed64gate1024plane512frame;
	int             loPlane, hiPlane, loData, hiData;

	hiPlane = (plane & 0x300);
	loPlane = (plane & 0xFF);
	loData = (data & 0x3);
	hiData = (data & 0x4);
	matnum8data16bed64gate1024plane512frame = ((frame & 0x1FF) | ((bed & 0xF) << 12) |
						   ((loPlane << 16) | (hiPlane << 1)) | ((gate & 0x3F) << 24) | ((loData << 30) | (hiData << 9)));
	return (matnum8data16bed64gate1024plane512frame);
}

int             mat_lookup (fptr, matnum, entry)
	FILE           *fptr;
	int             matnum;
	struct MatDir  *entry;
{
	int             blk, i, stat;
	int             nxtblk, matnbr, strtblk, endblk, matstat;
	int             dirbufr[MatBLKSIZE / 4];
	char            bytebufr[MatBLKSIZE];
	int             busy = 1;

	blk = MatFirstDirBlk;
	while (busy == 1) {
		stat = mat_rblk (fptr, blk, bytebufr, 1);
		if (stat != 0)
			return (stat);
		swap_byte (bytebufr, dirbufr, MatBLKSIZE);
		swaw (dirbufr, dirbufr, MatBLKSIZE / 2);
		nxtblk = dirbufr[1];
		for (i = 4; i < MatBLKSIZE / 4; i += 4) {
			matnbr = dirbufr[i];
			strtblk = dirbufr[i + 1];
			endblk = dirbufr[i + 2];
			matstat = dirbufr[i + 3];
			if (matnum == matnbr) {
				entry->matnum = matnbr;
				entry->strtblk = strtblk;
				entry->endblk = endblk;
				entry->matstat = matstat;
				return (-1);
			}
		}
		blk = nxtblk;
		if (blk == MatFirstDirBlk)
			break;
	}
	return (0);
}

int             mat_rblk (fptr, blkno, bufr, nblks)
	FILE           *fptr;
	int             blkno, nblks;
	char           *bufr;
{
	int             stat;

	stat = fseek (fptr, (blkno - 1) * MatBLKSIZE, 0);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_rblk]: fseek\n");
		return (stat);
	}
	stat = fread (bufr, 1, nblks * MatBLKSIZE, fptr);
	if (stat != nblks * MatBLKSIZE) {
		fprintf (stderr, "ERROR [mat_rblk]: fread\n");
		return (stat);
	}
	return (0);
}

int             mat_wblk (fptr, blkno, bufr, nblks)
	FILE           *fptr;
	int             blkno, nblks;
	char           *bufr;
{
	int             stat;

	stat = fseek (fptr, (blkno - 1) * MatBLKSIZE, 0);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_wblk]: fseek\n");
		return (stat);
	}
	stat = fwrite (bufr, 1, nblks * MatBLKSIZE, fptr);
	if (stat != nblks * MatBLKSIZE) {
		fprintf (stderr, "ERROR [mat_wblk]: fwrite\n");
		return (-1);
	}
	return (0);
}

int             mat_enter (fptr, matnum, nblks)
	FILE           *fptr;
	int             matnum, nblks;
{
	int             stat, dirblk, dirbufr[128], oldsize;
	int             nxtblk = 0;
	int             i = 0;
	int             busy = 1;

	dirblk = MatFirstDirBlk;
	stat = mat_rblk (fptr, dirblk, dirbufr, 1);
	if (stat != 0)
		return (stat);
	swap_byte (dirbufr, dirbufr, 512);
	swaw (dirbufr, dirbufr, 256);
	while (busy == 1) {
		nxtblk = dirblk + 1;
		for (i = 4; i < 128; i += 4) {
			if (dirbufr[i] == 0) {
				busy = 0;
				break;
			} else if (dirbufr[i] == matnum) {
				oldsize = dirbufr[i + 2] - dirbufr[i + 1] + 1;
				if (oldsize < nblks) {
					dirbufr[i] = 0xFFFFFFFF;
					swaw (dirbufr, dirbufr, 256);
					swap_byte (dirbufr, dirbufr, 512);
					stat = mat_wblk (fptr, dirblk, dirbufr, 1);
					if (stat != 0)
						return (stat);
					swap_byte (dirbufr, dirbufr, 512);
					swaw (dirbufr, dirbufr, 256);
					nxtblk = dirbufr[i + 2] + 1;
				} else {
					nxtblk = dirbufr[i + 1];
					dirbufr[0]++;
					dirbufr[3]--;
					busy = 0;
					break;
				}
			} else
				nxtblk = dirbufr[i + 2] + 1;
		}
		if (busy == 0)
			break;
		if (dirbufr[1] != MatFirstDirBlk) {
			dirblk = dirbufr[1];
			stat = mat_rblk (fptr, dirblk, dirbufr, 1);
			if (stat != 0)
				return (stat);
			swap_byte (dirbufr, dirbufr, 512);
			swaw (dirbufr, dirbufr, 256);
		} else {
			dirbufr[1] = nxtblk;
			swaw (dirbufr, dirbufr, 256);
			swap_byte (dirbufr, dirbufr, 512);
			stat = mat_wblk (fptr, dirblk, dirbufr, 1);
			if (stat != 0)
				return (stat);
			dirbufr[0] = 31;
			dirbufr[1] = MatFirstDirBlk;
			dirbufr[2] = dirblk;
			dirbufr[3] = 0;
			dirblk = nxtblk;
			for (i = 4; i < 128; i++)
				dirbufr[i] = 0;
		}
	}
	dirbufr[i] = matnum;
	dirbufr[i + 1] = nxtblk;
	dirbufr[i + 2] = nxtblk + nblks;
	dirbufr[i + 3] = 1;
	dirbufr[0]--;
	dirbufr[3]++;
	swaw (dirbufr, dirbufr, 256);
	swap_byte (dirbufr, dirbufr, 512);
	stat = mat_wblk (fptr, dirblk, dirbufr, 1);
	if (stat != 0)
		return (stat);
	return (nxtblk);
}

int             mat_read_main_header (fptr, h)
	FILE           *fptr;
	Main_header    *h;
{
	short int       b[256];
	char           *bb;
	int             stat, i;

	stat = mat_rblk (fptr, 1, b, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_main_header]\n");
		return (stat);
	}
	bb = (char *) b;
	strncpy (h->original_file_name, bb + 28, 20);
	strncpy (h->node_id, bb + 56, 10);
	strncpy (h->isotope_code, bb + 78, 8);
	strncpy (h->radiopharmaceutical, bb + 90, 32);
	strncpy (h->study_name, bb + 162, 12);
	strncpy (h->patient_id, bb + 174, 16);
	strncpy (h->patient_name, bb + 190, 32);
	h->patient_sex = bb[222];
	strncpy (h->patient_age, bb + 223, 10);
	strncpy (h->patient_height, bb + 233, 10);
	strncpy (h->patient_weight, bb + 243, 10);
	h->patient_dexterity = bb[253];
	strncpy (h->physician_name, bb + 254, 32);
	strncpy (h->operator_name, bb + 286, 32);
	strncpy (h->study_description, bb + 318, 32);
	strncpy (h->facility_name, bb + 356, 20);
	strncpy (h->user_process_code, bb + 462, 10);

	swap_byte (b, b, MatBLKSIZE);
	h->sw_version = b[24];
	h->data_type = b[25];
	h->system_type = b[26];
	h->file_type = b[27];
	h->scan_start_day = b[33];
	h->scan_start_month = b[34];
	h->scan_start_year = b[35];
	h->scan_start_hour = b[36];
	h->scan_start_minute = b[37];
	h->scan_start_second = b[38];
	h->isotope_halflife = get_vax_float (b, 43);
	h->gantry_tilt = get_vax_float (b, 61);
	h->gantry_rotation = get_vax_float (b, 63);
	h->bed_elevation = get_vax_float (b, 65);
	h->rot_source_speed = b[67];
	h->wobble_speed = b[68];
	h->transm_source_type = b[69];
	h->axial_fov = get_vax_float (b, 70);
	h->transaxial_fov = get_vax_float (b, 72);
	h->transaxial_samp_mode = b[74];
	h->coin_samp_mode = b[75];
	h->axial_samp_mode = b[76];
	h->calibration_factor = get_vax_float (b, 77);
	h->calibration_units = b[79];
	h->compression_code = b[80];
	h->acquisition_type = b[175];
	h->bed_type = b[176];
	h->septa_type = b[177];
	h->num_planes = b[188];
	h->num_frames = b[189];
	h->num_gates = b[190];
	h->num_bed_pos = b[191];
	h->init_bed_position = get_vax_float (b, 192);
	for (i = 0; i < 15; i++)
		h->bed_offset[i] = get_vax_float (b, 194 + 2 * i);
	h->plane_separation = get_vax_float (b, 224);
	h->lwr_sctr_thres = b[226];
	h->lwr_true_thres = b[227];
	h->upr_true_thres = b[228];
	h->collimator = get_vax_float (b, 229);
	h->acquisition_mode = b[236];
	return (0);
}

int             mat_read_image_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Image_subheader *h;
{
	short int       b[256];
	int             i, stat;
	char           *bb;

	stat = mat_rblk (fptr, blknum, b, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_image_subheader]\n");
		return (stat);
	}
	bb = (char *) b;

	strncpy (h->annotation, bb + 420, 40);
	swap_byte (b, b, MatBLKSIZE);
	h->data_type = b[63];
	h->num_dimensions = b[64];
	h->dimension_1 = b[66];
	h->dimension_2 = b[67];
	h->x_origin = get_vax_float (b, 80);
	h->y_origin = get_vax_float (b, 82);
	h->recon_scale = get_vax_float (b, 84);
	h->quant_scale = get_vax_float (b, 86);
	h->image_min = b[88];
	h->image_max = b[89];
	h->pixel_size = get_vax_float (b, 92);
	h->slice_width = get_vax_float (b, 94);
	h->frame_duration = get_vax_long (b, 96);
	h->frame_start_time = get_vax_long (b, 98);
	h->slice_location = b[100];
	h->recon_start_hour = b[101];
	h->recon_start_minute = b[102];
	h->recon_start_sec = b[103];
	h->recon_duration = get_vax_long (b, 104);
	h->filter_code = b[118];
	h->scan_matrix_num = get_vax_long (b, 119);
	h->norm_matrix_num = get_vax_long (b, 121);
	h->atten_cor_matrix_num = get_vax_long (b, 123);
	h->image_rotation = get_vax_float (b, 148);
	h->plane_eff_corr_fctr = get_vax_float (b, 150);
	h->decay_corr_fctr = get_vax_float (b, 152);
	h->loss_corr_fctr = get_vax_float (b, 154);
	h->intrinsic_tilt = get_vax_float (b, 156);
	h->processing_code = b[188];
	h->quant_units = b[190];
	h->recon_start_day = b[191];
	h->recon_start_month = b[192];
	h->recon_start_year = b[193];
	h->ecat_calibration_fctr = get_vax_float (b, 194);
	h->well_counter_cal_fctr = get_vax_float (b, 196);
	for (i = 0; i < 6; i++)
		h->filter_params[i] = get_vax_float (b, 198 + 2 * i);
	return (0);
}

int             mat_read_scan_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Scan_subheader *h;
{
	short int       b[256];
	int             i, stat;

	stat = mat_rblk (fptr, blknum, b, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_scan_subheader]\n");
		return (stat);
	}
	swap_byte (b, b, MatBLKSIZE);
	h->data_type = b[63];
	h->dimension_1 = b[66];
	h->dimension_2 = b[67];
	h->smoothing = b[68];
	h->processing_code = b[69];
	h->sample_distance = get_vax_float (b, 73);
	h->isotope_halflife = get_vax_float (b, 83);
	h->frame_duration_sec = b[85];
	h->gate_duration = get_vax_long (b, 86);
	h->r_wave_offset = get_vax_long (b, 88);
	h->scale_factor = get_vax_float (b, 91);
	h->scan_min = b[96];
	h->scan_max = b[97];
	h->prompts = get_vax_long (b, 98);
	h->delayed = get_vax_long (b, 100);
	h->multiples = get_vax_long (b, 102);
	h->net_trues = get_vax_long (b, 104);
	for (i = 0; i < 16; i++) {
		h->cor_singles[i] = get_vax_float (b, 158 + 2 * i);
		h->uncor_singles[i] = get_vax_float (b, 190 + 2 * i);
	}
	h->tot_avg_cor = get_vax_float (b, 222);
	h->tot_avg_uncor = get_vax_float (b, 224);
	h->total_coin_rate = get_vax_long (b, 226);
	h->frame_start_time = get_vax_long (b, 228);
	h->frame_duration = get_vax_long (b, 230);
	h->loss_correction_fctr = get_vax_float (b, 232);
	for (i = 0; i < 8; i++)
		h->phy_planes[i] = get_vax_long (b, 234 + (2 * i));
	return (0);
}

int             mat_read_attn_subheader (fptr, blknum, header)
	FILE           *fptr;
	int             blknum;
	Attn_subheader *header;
{
	short int       bufr[256];
	int             stat;

	stat = mat_rblk (fptr, blknum, bufr, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_attn_subheader]\n");
		return (stat);
	}
	swap_byte (bufr, bufr, MatBLKSIZE);

	header->data_type = bufr[63];
	header->attenuation_type = bufr[64];
	header->dimension_1 = bufr[66];
	header->dimension_2 = bufr[67];
	header->scale_factor = get_vax_float (bufr, 91);
	header->x_origin = get_vax_float (bufr, 93);
	header->y_origin = get_vax_float (bufr, 95);
	header->x_radius = get_vax_float (bufr, 97);
	header->y_radius = get_vax_float (bufr, 99);
	header->tilt_angle = get_vax_float (bufr, 101);
	header->attenuation_coeff = get_vax_float (bufr, 103);
	header->sample_distance = get_vax_float (bufr, 105);
	return (0);
}

int             mat_read_norm_subheader (fptr, blknum, header)
	FILE           *fptr;
	int             blknum;
	Norm_subheader *header;
{
	short int       bufr[256];
	int             stat;

	stat = mat_rblk (fptr, blknum, bufr, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_norm_subheader]\n");
		return (stat);
	}
	swap_byte (bufr, bufr, MatBLKSIZE);

	header->data_type = bufr[63];
	header->dimension_1 = bufr[66];
	header->dimension_2 = bufr[67];
	header->scale_factor = get_vax_float (bufr, 91);
	header->norm_hour = bufr[93];
	header->norm_minute = bufr[94];
	header->norm_second = bufr[95];
	header->norm_day = bufr[96];
	header->norm_month = bufr[97];
	header->norm_year = bufr[98];
	header->fov_source_width = get_vax_float (bufr, 99);
	header->ecat_calib_factor = get_vax_float (bufr, 101);
	return (0);
}

int             mat_write_main_header (fptr, header)
	FILE           *fptr;
	Main_header    *header;
{
	char           *bbufr;
	short int       bufr[256];
	int             stat, i;

	for (i = 0; i < 256; i++)
		bufr[i] = 0;
	bbufr = (char *) bufr;

	bufr[24] = header->sw_version;
	bufr[25] = header->data_type;
	bufr[26] = header->system_type;
	bufr[27] = header->file_type;
	bufr[33] = header->scan_start_day;
	bufr[34] = header->scan_start_month;
	bufr[35] = header->scan_start_year;
	bufr[36] = header->scan_start_hour;
	bufr[37] = header->scan_start_minute;
	bufr[38] = header->scan_start_second;
	sunftovaxf (header->isotope_halflife, &bufr[43]);
	sunftovaxf (header->gantry_tilt, &bufr[61]);
	sunftovaxf (header->gantry_rotation, &bufr[63]);
	sunftovaxf (header->bed_elevation, &bufr[65]);
	bufr[67] = header->rot_source_speed;
	bufr[68] = header->wobble_speed;
	bufr[69] = header->transm_source_type;
	sunftovaxf (header->axial_fov, &bufr[70]);
	sunftovaxf (header->transaxial_fov, &bufr[72]);
	bufr[74] = header->transaxial_samp_mode;
	bufr[75] = header->coin_samp_mode;
	bufr[76] = header->axial_samp_mode;
	sunftovaxf (header->calibration_factor, &bufr[77]);
	bufr[79] = header->calibration_units;
	bufr[80] = header->compression_code;
	bufr[175] = header->acquisition_type;
	bufr[176] = header->bed_type;
	bufr[177] = header->septa_type;
	bufr[188] = header->num_planes;
	bufr[189] = header->num_frames;
	bufr[190] = header->num_gates;
	bufr[191] = header->num_bed_pos;
	sunftovaxf (header->init_bed_position, &bufr[192]);
	for (i = 0; i < 15; i++) {
		sunftovaxf (header->bed_offset[i], &bufr[194 + 2 * i]);
	}
	sunftovaxf (header->plane_separation, &bufr[224]);
	bufr[226] = header->lwr_sctr_thres;
	bufr[227] = header->lwr_true_thres;
	bufr[228] = header->upr_true_thres;
	sunftovaxf (header->collimator, &bufr[229]);
	bufr[236] = header->acquisition_mode;

	swap_byte (bufr, bufr, MatBLKSIZE);

	bcopy (header->original_file_name, bbufr + 28, 20);
	bcopy (header->node_id, bbufr + 56, 10);
	bcopy (header->isotope_code, bbufr + 78, 8);
	bcopy (header->radiopharmaceutical, bbufr + 90, 32);
	bcopy (header->study_name, bbufr + 162, 12);
	bcopy (header->patient_id, bbufr + 174, 16);
	bcopy (header->patient_name, bbufr + 190, 32);
	bbufr[222] = header->patient_sex;
	bcopy (header->patient_age, bbufr + 223, 10);
	bcopy (header->patient_height, bbufr + 233, 10);
	bcopy (header->patient_weight, bbufr + 243, 10);
	bbufr[253] = header->patient_dexterity;
	bcopy (header->physician_name, bbufr + 254, 32);
	bcopy (header->operator_name, bbufr + 286, 32);
	bcopy (header->study_description, bbufr + 318, 32);
	bcopy (header->facility_name, bbufr + 356, 20);
	bcopy (header->user_process_code, bbufr + 462, 10);

	stat = mat_wblk (fptr, 1, bufr, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_write_main_header]\n");
		return (stat);
	}
	return (0);
}

int             mat_write_idata (fptr, blk, data, size)
	FILE           *fptr;
	int             blk, size;
	char           *data;
{
	char            bufr[512];
	int             i, nbytes, nblks;
	int             stat;

	nblks = (size + 511) / 512;
	for (i = 0; i < nblks; i++) {
		nbytes = (size < 512) ? size : 512;
		bcopy (data, bufr, nbytes);
		swap_byte (bufr, bufr, 512);
		stat = mat_wblk (fptr, blk + i, bufr, 1);
		if (stat != 0)
			return (stat);
		data += nbytes;
		size -= nbytes;
	}
	return (0);
}

int             mat_write_fdata (fptr, blk, data, size)
	FILE           *fptr;
	int             blk, size;
	float          *data;
{
	float           bufr[128];
	int             i, j, nvals, nblks;
	int             stat;

	nblks = (size + 511) / 512;
	for (i = 0; i < nblks; i++) {
		nvals = (size < 512) ? size / 4 : 512 / 4;
		for (j = 0; j < nvals; j++)
			sunftovaxf (*data++, &bufr[j]);
		swap_byte (bufr, bufr, 512);
		stat = mat_wblk (fptr, blk + i, bufr, 1);
		if (stat != 0)
			return (stat);
		size -= 4 * nvals;
	}
	return (0);
}

int             mat_write_image_subheader (fptr, blknum, header)
	FILE           *fptr;
	int             blknum;
	Image_subheader *header;
{
	char           *bbufr;
	short int       bufr[256];
	int             i, stat;

	for (i = 0; i < 256; i++)
		bufr[i] = 0;
	bbufr = (char *) bufr;

	bufr[63] = header->data_type;
	bufr[64] = header->num_dimensions;
	bufr[66] = header->dimension_1;
	bufr[67] = header->dimension_2;
	sunftovaxf (header->x_origin, &bufr[80]);
	sunftovaxf (header->y_origin, &bufr[82]);
	sunftovaxf (header->recon_scale, &bufr[84]);
	sunftovaxf (header->quant_scale, &bufr[86]);
	bufr[88] = header->image_min;
	bufr[89] = header->image_max;
	sunftovaxf (header->pixel_size, &bufr[92]);
	sunftovaxf (header->slice_width, &bufr[94]);
	sunltovaxl (header->frame_duration, &bufr[96]);
	sunltovaxl (header->frame_start_time, &bufr[98]);
	bufr[100] = header->slice_location;
	bufr[101] = header->recon_start_hour;
	bufr[102] = header->recon_start_minute;
	bufr[103] = header->recon_start_sec;
	sunltovaxl (header->recon_duration, &bufr[104]);
	bufr[118] = header->filter_code;
	sunltovaxl (header->scan_matrix_num, &bufr[119]);
	sunltovaxl (header->norm_matrix_num, &bufr[121]);
	sunltovaxl (header->atten_cor_matrix_num, &bufr[123]);
	sunftovaxf (header->image_rotation, &bufr[148]);
	sunftovaxf (header->plane_eff_corr_fctr, &bufr[150]);
	sunftovaxf (header->decay_corr_fctr, &bufr[152]);
	sunftovaxf (header->loss_corr_fctr, &bufr[154]);
	sunftovaxf (header->intrinsic_tilt, &bufr[156]);
	bufr[188] = header->processing_code;
	bufr[190] = header->quant_units;
	bufr[191] = header->recon_start_day;
	bufr[192] = header->recon_start_month;
	bufr[193] = header->recon_start_year;
	sunftovaxf (header->ecat_calibration_fctr, &bufr[194]);
	sunftovaxf (header->well_counter_cal_fctr, &bufr[196]);

	for (i = 0; i < 6; i++)
		sunftovaxf (header->filter_params[i], &bufr[198 + 2 * i]);

	swap_byte (bufr, bufr, MatBLKSIZE);
	strcpy (bbufr + 420, header->annotation);

	stat = mat_wblk (fptr, blknum, bbufr, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_write_image_subheader]\n");
		return (stat);
	}
	return (0);
}

int             mat_write_scan_subheader (fptr, blknum, header)
	FILE           *fptr;
	int             blknum;
	Scan_subheader *header;
{
	short int       bufr[256];
	int             i, stat;

	for (i = 0; i < 256; i++)
		bufr[i] = 0;
	bufr[0] = 256;
	bufr[1] = 1;
	bufr[2] = 22;
	bufr[3] = -1;
	bufr[4] = 25;
	bufr[5] = 62;
	bufr[6] = 79;
	bufr[7] = 106;
	bufr[24] = 37;
	bufr[25] = -1;
	bufr[61] = 17;
	bufr[62] = -1;
	bufr[78] = 27;
	bufr[79] = -1;
	bufr[105] = 52;
	bufr[106] = -1;
	bufr[63] = header->data_type;
	bufr[66] = header->dimension_1;		 /* x dimension */
	bufr[67] = header->dimension_2;		 /* y_dimension */
	bufr[68] = header->smoothing;
	bufr[69] = header->processing_code;
	sunftovaxf (header->sample_distance, &bufr[73]);
	sunftovaxf (header->isotope_halflife, &bufr[83]);
	bufr[85] = header->frame_duration_sec;
	sunltovaxl (header->gate_duration, &bufr[86]);
	sunltovaxl (header->r_wave_offset, &bufr[88]);
	sunftovaxf (header->scale_factor, &bufr[91]);
	bufr[96] = header->scan_min;
	bufr[97] = header->scan_max;
	sunltovaxl (header->prompts, &bufr[98]);
	sunltovaxl (header->delayed, &bufr[100]);
	sunltovaxl (header->multiples, &bufr[102]);
	sunltovaxl (header->net_trues, &bufr[104]);
	for (i = 0; i < 16; i++) {
		sunftovaxf (header->cor_singles[i], &bufr[158 + 2 * i]);
		sunftovaxf (header->uncor_singles[i], &bufr[190 + 2 * i]);
	}
	sunftovaxf (header->tot_avg_cor, &bufr[222]);
	sunftovaxf (header->tot_avg_uncor, &bufr[224]);
	sunltovaxl (header->total_coin_rate, &bufr[226]);	/* total coin rate */
	sunltovaxl (header->frame_start_time, &bufr[228]);
	sunltovaxl (header->frame_duration, &bufr[230]);
	sunftovaxf (header->loss_correction_fctr, &bufr[232]);
	for (i = 0; i < 8; i++)
		sunltovaxl (header->phy_planes[i], &bufr[234 + 2 * i]);
	swap_byte (bufr, bufr, MatBLKSIZE);
	stat = mat_wblk (fptr, blknum, bufr, 1);
	return (stat);
}

int             mat_write_attn_subheader (fptr, blknum, header)
	FILE           *fptr;

	int             blknum;

	Attn_subheader *header;
{
	short int       bufr[256];

	int             i, stat;

	for (i = 0; i < 256; i++)
		bufr[i] = 0;
	bufr[0] = 256;
	bufr[1] = 1;
	bufr[2] = 22;
	bufr[3] = -1;
	bufr[4] = 25;
	bufr[5] = 62;
	bufr[6] = 79;
	bufr[7] = 106;
	bufr[24] = 37;
	bufr[25] = -1;
	bufr[61] = 17;
	bufr[62] = -1;
	bufr[78] = 27;
	bufr[79] = -1;
	bufr[105] = 52;
	bufr[106] = -1;
	bufr[63] = header->data_type;
	bufr[64] = header->attenuation_type;
	bufr[66] = header->dimension_1;
	bufr[67] = header->dimension_2;
	sunftovaxf (header->scale_factor, &bufr[91]);
	sunftovaxf (header->x_origin, &bufr[93]);
	sunftovaxf (header->y_origin, &bufr[95]);
	sunftovaxf (header->x_radius, &bufr[97]);
	sunftovaxf (header->y_radius, &bufr[99]);
	sunftovaxf (header->tilt_angle, &bufr[101]);
	sunftovaxf (header->attenuation_coeff, &bufr[103]);
	sunftovaxf (header->sample_distance, &bufr[105]);
	swap_byte (bufr, bufr, 512);
	stat = mat_wblk (fptr, blknum, bufr, 1);
	return (stat);
}

int             mat_write_norm_subheader (fptr, blknum, header)
	FILE           *fptr;
	int             blknum;
	Norm_subheader *header;
{
	short int       bufr[256];
	int             i, stat;

	for (i = 0; i < 256; i++)
		bufr[i] = 0;
	bufr[0] = 256;
	bufr[1] = 1;
	bufr[2] = 22;
	bufr[3] = -1;
	bufr[4] = 25;
	bufr[5] = 62;
	bufr[6] = 79;
	bufr[7] = 106;
	bufr[24] = 37;
	bufr[25] = -1;
	bufr[61] = 17;
	bufr[62] = -1;
	bufr[78] = 27;
	bufr[79] = -1;
	bufr[105] = 52;
	bufr[106] = -1;
	bufr[63] = header->data_type;
	bufr[66] = header->dimension_1;
	bufr[67] = header->dimension_2;
	sunftovaxf (header->scale_factor, &bufr[91]);
	bufr[93] = header->norm_hour;
	bufr[94] = header->norm_minute;
	bufr[95] = header->norm_second;
	bufr[96] = header->norm_day;
	bufr[97] = header->norm_month;
	bufr[98] = header->norm_year;
	sunftovaxf (header->fov_source_width, &bufr[99]);
	sunftovaxf (header->ecat_calib_factor, &bufr[101]);
	swap_byte (bufr, bufr, 512);
	stat = mat_wblk (fptr, blknum, bufr, 1);
	return (stat);
}

