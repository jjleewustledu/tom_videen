/*$Id: matrix.c,v 2.0 2002/12/26 20:35:13 tom Exp tom $
 *$Log: matrix.c,v $
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
/*	ECAT (Matrix 6) files were originally implemented on a VAX			*/
/*	and are LITTLE ENDIAN, often without alignment to word boundaries	*/

#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <string.h>
#include <strings.h>
#include <cti6.h>
#include <endian.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/matrix.c,v 2.0 2002/12/26 20:35:13 tom Exp tom $";
#endif

void            swap_word (buf, length)
	short          *buf;
	int             length;
{
	short           temp;
	int             i;

	for (i = 0; i < length; i += 2) {
		temp = buf[i + 1];
		buf[i + 1] = buf[i];
		buf[i] = temp;
	}
	return;
}

void            swap_byte (buf, length)
	char           *buf;
	int             length;
{
	char            temp;
	int             i;

	for (i = 0; i < length; i += 2) {
		temp = buf[i + 1];
		buf[i + 1] = buf[i];
		buf[i] = temp;
	}
	return;
}

int             get_vax_long (sbuf, i)
	short          *sbuf;
	int             i;
{
	union {
		unsigned short  wrd[2];
		long            lng;
	}               conv;
	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		conv.wrd[0] = sbuf[i + 1];
		conv.wrd[1] = sbuf[i];
	} else {
		conv.wrd[0] = sbuf[i];
		conv.wrd[1] = sbuf[i+1];
	}
	return (conv.lng);
}

float           get_vax_float (sbuf, i)
	short          *sbuf;
	int             i;
{
	union {
		unsigned short  wrd[2];
		float           flt;
	}               conv;
	if (sbuf[i] == 0 && sbuf[i + 1] == 0)
		return (0.0);
	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		conv.wrd[0] = sbuf[i] - 256;
		conv.wrd[1] = sbuf[i + 1];
	} else {
		conv.wrd[0] = sbuf[i + 1];
		conv.wrd[1] = sbuf[i] - 256;
	}
	return (conv.flt);
}

void            make_vax_long (sbuf, i, val)
	short          *sbuf;
	int             i;
	int             val;
{
	union {
		unsigned short  wrd[2];
		long            lng;
	}               conv;
	conv.lng = val;
	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		sbuf[i] = conv.wrd[1];
		sbuf[i + 1] = conv.wrd[0];
	} else {
		sbuf[i] = conv.wrd[0];
		sbuf[i + 1] = conv.wrd[1];
	}
	return;
}

void            make_vax_float (sbuf, i, val)
	short          *sbuf;
	int             i;
	float           val;
{
	unsigned short	exp;
	union {
		float           flt;
		unsigned short  wrd[2];
	}               conv;
	if (val == 0.0) {
		sbuf[i] = 0;
		sbuf[i + 1] = 0;
	} else {
		conv.flt = val;
		/* CONSTCOND */
		if (__BYTE_ORDER == __BIG_ENDIAN) {
			sbuf[i] = conv.wrd[0] + 256;
			sbuf[i + 1] = conv.wrd[1];
		} else {
			sbuf[i] = conv.wrd[0];
			sbuf[i + 1] = conv.wrd[1];
		}
	}
}

FILE           *mat_create (fname, mhead)
	char           *fname;
	Main_header    *mhead;
{
	FILE           *fptr;
	int            *lbuf;
	char           *buf;
	int             stat;

	fptr = fopen (fname, "w+");
	if (fptr == NULL)
		return (fptr);
	stat = mat_write_main_header (fptr, mhead);
	if (stat != 0) {
		(void) fclose (fptr);
		return (fptr);
	}
	lbuf = (int *) calloc (MatBLKSIZE, sizeof (int));
	buf = (char *) lbuf;
	lbuf[0] = 31;
	lbuf[1] = 2;

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		swap_word ((short *) lbuf, 256);
		swap_byte (buf, 512);
	}
	stat = mat_wblk (fptr, MatFirstDirBlk, buf, 1);
	if (stat != 0)
		(void) fclose (fptr);
	free (lbuf);
	return (fptr);
}

int             mat_numcod (frame, plane, gate, data, bed)
	int             frame, plane, gate, data, bed;
{
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
	int             busy = 1;

	blk = MatFirstDirBlk;
	while (busy == 1) {
		stat = mat_rblk (fptr, blk, (char *) dirbufr, 1);
		if (stat != 0)
			return (stat);
		/* CONSTCOND */
		if (__BYTE_ORDER == __BIG_ENDIAN) {
			swap_byte ((char *) dirbufr, MatBLKSIZE);
			swap_word ((short *) dirbufr, MatBLKSIZE / 2);
		}
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

int             mat_enter (fptr, matnum, nblks)
	FILE           *fptr;
	int             matnum, nblks;
{
	int             stat, dirblk, dirbufr[128], oldsize;
	int             nxtblk = 0;
	int             i = 0;
	int             busy = 1;

	dirblk = MatFirstDirBlk;
	stat = mat_rblk (fptr, dirblk, (char *) dirbufr, 1);
	if (stat != 0)
		return (stat);
	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		swap_byte ((char *) dirbufr, 512);
		swap_word ((short *) dirbufr, 256);
	}
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
					/* CONSTCOND */
					if (__BYTE_ORDER == __BIG_ENDIAN) {
						swap_word ((short *) dirbufr, 256);
						swap_byte ((char *) dirbufr, 512);
					}
					stat = mat_wblk (fptr, dirblk, (char *) dirbufr, 1);
					if (stat != 0)
						return (stat);
					/* CONSTCOND */
					if (__BYTE_ORDER == __BIG_ENDIAN) {
						swap_byte ((char *) dirbufr, 512);
						swap_word ((short *) dirbufr, 256);
					}
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
			stat = mat_rblk (fptr, dirblk, (char *) dirbufr, 1);
			if (stat != 0)
				return (stat);
			/* CONSTCOND */
			if (__BYTE_ORDER == __BIG_ENDIAN) {
				swap_byte ((char *) dirbufr, 512);
				swap_word ((short *) dirbufr, 256);
			}
		} else {
			dirbufr[1] = nxtblk;
			/* CONSTCOND */
			if (__BYTE_ORDER == __BIG_ENDIAN) {
				swap_word ((short *) dirbufr, 256);
				swap_byte ((char *) dirbufr, 512);
			}
			stat = mat_wblk (fptr, dirblk, (char *) dirbufr, 1);
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
	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) {
		swap_word ((short *) dirbufr, 256);
		swap_byte ((char *) dirbufr, 512);
	}
	stat = mat_wblk (fptr, dirblk, (char *) dirbufr, 1);
	if (stat != 0)
		return (stat);
	return (nxtblk);
}

int             mat_rblk (fptr, blkno, buf, nblks)
	FILE           *fptr;
	int             blkno, nblks;
	char           *buf;
{
	int             stat;

	stat = fseek (fptr, (blkno - 1) * MatBLKSIZE, 0);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_rblk]: fseek\n");
		return (stat);
	}
	stat = fread (buf, 1, nblks * MatBLKSIZE, fptr);
	if (stat != nblks * MatBLKSIZE) {
		fprintf (stderr, "ERROR [mat_rblk]: fread\n");
		return (stat);
	}
	return (0);
}

int             mat_wblk (fptr, blkno, buf, nblks)
	FILE           *fptr;
	int             blkno, nblks;
	char           *buf;
{
	int             stat;

	stat = fseek (fptr, (blkno - 1) * MatBLKSIZE, 0);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_wblk]: fseek\n");
		return (stat);
	}
	stat = fwrite (buf, 1, nblks * MatBLKSIZE, fptr);
	if (stat != nblks * MatBLKSIZE) {
		fprintf (stderr, "ERROR [mat_wblk]: fwrite\n");
		return (-1);
	}
	return (0);
}

int             mat_write_idata (fptr, blk, data, size)
	FILE           *fptr;
	int             blk, size;
	char           *data;
{
	char            buf[512];
	int             i, nbytes, nblks;
	int             stat;

	nblks = (size + 511) / 512;
	for (i = 0; i < nblks; i++) {
		nbytes = (size < 512) ? size : 512;
		bcopy (data, buf, nbytes);
		/* CONSTCOND */
		if (__BYTE_ORDER == __BIG_ENDIAN)
			swap_byte (buf, 512);
		stat = mat_wblk (fptr, blk + i, buf, 1);
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
	float           fbuf[128];
	short          *sbuf;
	char           *buf;
	int             i, j, nvals, nblks;
	int             stat;

	sbuf = (short *) fbuf;
	buf = (char *) fbuf;

	nblks = (size + 511) / 512;
	for (i = 0; i < nblks; i++) {
		nvals = (size < 512) ? size / 4 : 512 / 4;
		for (j = 0; j < nvals; j++)
			fbuf[j] = data[j];
		/* CONSTCOND */
		if (__BYTE_ORDER == __BIG_ENDIAN) {
			for (j = 0; j < nvals; j++)
				make_vax_float (sbuf, 2 * j, fbuf[j]);
			swap_byte (buf, 512);
		}
		stat = mat_wblk (fptr, blk + i, buf, 1);
		if (stat != 0)
			return (stat);
		size -= 4 * nvals;
	}
	return (0);
}

int             mat_read_main_header (fptr, h)
	FILE           *fptr;
	Main_header    *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;

    union {
        unsigned short  a1[2];
        unsigned long   a2;
        float           a3;
    }   test;

	buf = (char *) sbuf;

	stat = mat_rblk (fptr, 1, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_main_header]\n");
		return (stat);
	}
	(void) strncpy (h->original_file_name, buf + 28, 20);
	(void) strncpy (h->node_id, buf + 56, 10);
	(void) strncpy (h->isotope_code, buf + 78, 8);
	(void) strncpy (h->radiopharmaceutical, buf + 90, 32);
	(void) strncpy (h->study_name, buf + 162, 12);
	(void) strncpy (h->patient_id, buf + 174, 16);
	(void) strncpy (h->patient_name, buf + 190, 32);
	h->patient_sex = buf[222];
	(void) strncpy (h->patient_age, buf + 223, 10);
	(void) strncpy (h->patient_height, buf + 233, 10);
	(void) strncpy (h->patient_weight, buf + 243, 10);
	h->patient_dexterity = buf[253];
	(void) strncpy (h->physician_name, buf + 254, 32);
	(void) strncpy (h->operator_name, buf + 286, 32);
	(void) strncpy (h->study_description, buf + 318, 32);
	(void) strncpy (h->facility_name, buf + 356, 20);
	(void) strncpy (h->user_process_code, buf + 462, 10);

/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN)
		swap_byte (buf, MatBLKSIZE);

	h->sw_version = sbuf[24];
	h->data_type = sbuf[25];
	h->system_type = sbuf[26];
	h->file_type = sbuf[27];
	h->scan_start_day = sbuf[33];
	h->scan_start_month = sbuf[34];
	h->scan_start_year = sbuf[35];
	h->scan_start_hour = sbuf[36];
	h->scan_start_minute = sbuf[37];
	h->scan_start_second = sbuf[38];
	h->isotope_halflife = get_vax_float (sbuf, 43);
	h->gantry_tilt = get_vax_float (sbuf, 61);
	h->gantry_rotation = get_vax_float (sbuf, 63);
	h->bed_elevation = get_vax_float (sbuf, 65);
	h->rot_source_speed = sbuf[67];
	h->wobble_speed = sbuf[68];
	h->transm_source_type = sbuf[69];
	h->axial_fov = get_vax_float (sbuf, 70);
	h->transaxial_fov = get_vax_float (sbuf, 72);
	h->transaxial_samp_mode = sbuf[74];
	h->coin_samp_mode = sbuf[75];
	h->axial_samp_mode = sbuf[76];
	h->calibration_factor = get_vax_float (sbuf, 77);
	h->calibration_units = sbuf[79];
	h->compression_code = sbuf[80];
	h->acquisition_type = sbuf[175];
	h->bed_type = sbuf[176];
	h->septa_type = sbuf[177];
	h->num_planes = sbuf[188];
	h->num_frames = sbuf[189];
	h->num_gates = sbuf[190];
	h->num_bed_pos = sbuf[191];
	h->init_bed_position = get_vax_float (sbuf, 192);
	for (i = 0; i < 15; i++)
		h->bed_offset[i] = get_vax_float (sbuf, 194 + 2 * i);
	h->plane_separation = get_vax_float (sbuf, 224);
	h->lwr_sctr_thres = sbuf[226];
	h->lwr_true_thres = sbuf[227];
	h->upr_true_thres = sbuf[228];
	h->collimator = get_vax_float (sbuf, 229);
	h->acquisition_mode = sbuf[236];
	return (0);
}

int             mat_read_image_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Image_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;

	buf = (char *) sbuf;

	stat = mat_rblk (fptr, blknum, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_image_subheader]\n");
		return (stat);
	}
	(void) strncpy (h->annotation, buf + 420, 40);

/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN)
		swap_byte (buf, MatBLKSIZE);
	h->data_type = sbuf[63];
	h->num_dimensions = sbuf[64];
	h->dimension_1 = sbuf[66];
	h->dimension_2 = sbuf[67];
	h->x_origin = get_vax_float (sbuf, 80);
	h->y_origin = get_vax_float (sbuf, 82);
	h->recon_scale = get_vax_float (sbuf, 84);
	h->quant_scale = get_vax_float (sbuf, 86);
	h->image_min = sbuf[88];
	h->image_max = sbuf[89];
	h->pixel_size = get_vax_float (sbuf, 92);
	h->slice_width = get_vax_float (sbuf, 94);
	h->frame_duration = get_vax_long (sbuf, 96);
	h->frame_start_time = get_vax_long (sbuf, 98);
	h->slice_location = sbuf[100];
	h->recon_start_hour = sbuf[101];
	h->recon_start_minute = sbuf[102];
	h->recon_start_sec = sbuf[103];
	h->recon_duration = get_vax_long (sbuf, 104);
	h->filter_code = sbuf[118];
	h->scan_matrix_num = get_vax_long (sbuf, 119);
	h->norm_matrix_num = get_vax_long (sbuf, 121);
	h->atten_cor_matrix_num = get_vax_long (sbuf, 123);
	h->image_rotation = get_vax_float (sbuf, 148);
	h->plane_eff_corr_fctr = get_vax_float (sbuf, 150);
	h->decay_corr_fctr = get_vax_float (sbuf, 152);
	h->loss_corr_fctr = get_vax_float (sbuf, 154);
	h->intrinsic_tilt = get_vax_float (sbuf, 156);
	h->processing_code = sbuf[188];
	h->quant_units = sbuf[190];
	h->recon_start_day = sbuf[191];
	h->recon_start_month = sbuf[192];
	h->recon_start_year = sbuf[193];
	h->ecat_calibration_fctr = get_vax_float (sbuf, 194);
	h->well_counter_cal_fctr = get_vax_float (sbuf, 196);
	for (i = 0; i < 6; i++)
		h->filter_params[i] = get_vax_float (sbuf, 198 + 2 * i);
	return (0);
}

int             mat_read_scan_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Scan_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;

	buf = (char *) sbuf;

	stat = mat_rblk (fptr, blknum, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_scan_subheader]\n");
		return (stat);
	}
/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN)
		swap_byte (buf, MatBLKSIZE);
	h->data_type = sbuf[63];
	h->dimension_1 = sbuf[66];
	h->dimension_2 = sbuf[67];
	h->smoothing = sbuf[68];
	h->processing_code = sbuf[69];
	h->sample_distance = get_vax_float(sbuf, 73);
	h->isotope_halflife = get_vax_float(sbuf, 83);
	h->frame_duration_sec = sbuf[85];
	h->gate_duration = get_vax_long (sbuf, 86);
	h->r_wave_offset = get_vax_long (sbuf, 88);
	h->scale_factor = get_vax_float (sbuf, 91);
	h->scan_min = sbuf[96];
	h->scan_max = sbuf[97];
	h->prompts = get_vax_long (sbuf, 98);
	h->delayed = get_vax_long (sbuf, 100);
	h->multiples = get_vax_long (sbuf, 102);
	h->net_trues = get_vax_long (sbuf, 104);
	for (i = 0; i < 16; i++) {
		h->cor_singles[i] = get_vax_float (sbuf, 158 + i);
		h->uncor_singles[i] = get_vax_float (sbuf, 190 + i);
	}
	h->tot_avg_cor = get_vax_float (sbuf, 222);
	h->tot_avg_uncor = get_vax_float (sbuf, 224);
	h->total_coin_rate = get_vax_long (sbuf, 226);
	h->frame_start_time = get_vax_long (sbuf, 228);
	h->frame_duration = get_vax_long (sbuf, 230);
	h->loss_correction_fctr = get_vax_float (sbuf, 232);
	for (i = 0; i < 8; i++)
		h->phy_planes[i] = get_vax_long (sbuf, 234 + i);
	return (0);
}

int             mat_read_norm_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Norm_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat;

	buf = (char *) sbuf;

	stat = mat_rblk (fptr, blknum, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_norm_subheader]\n");
		return (stat);
	}
/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN)
		swap_byte (buf, MatBLKSIZE);
	h->data_type = sbuf[63];
	h->dimension_1 = sbuf[66];
	h->dimension_2 = sbuf[67];
	h->scale_factor = get_vax_float (sbuf, 91);
	h->norm_hour = sbuf[93];
	h->norm_minute = sbuf[94];
	h->norm_second = sbuf[95];
	h->norm_day = sbuf[96];
	h->norm_month = sbuf[97];
	h->norm_year = sbuf[98];
	h->fov_source_width = get_vax_float (sbuf, 99);
	h->ecat_calib_factor = get_vax_float (sbuf, 101);
	return (0);
}

int             mat_read_attn_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Attn_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat;

	buf = (char *) sbuf;

	stat = mat_rblk (fptr, blknum, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_read_attn_subheader]\n");
		return (stat);
	}

/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) 
		swap_byte (buf, MatBLKSIZE);
	h->data_type = sbuf[63];
	h->attenuation_type = sbuf[64];
	h->dimension_1 = sbuf[66];
	h->dimension_2 = sbuf[67];
	h->scale_factor = get_vax_float(sbuf, 91);
	h->x_origin = get_vax_float(sbuf, 93);
	h->y_origin = get_vax_float(sbuf, 95);
	h->x_radius = get_vax_float(sbuf, 97);
	h->y_radius = get_vax_float(sbuf, 99);
	h->tilt_angle = get_vax_float(sbuf, 101);
	h->attenuation_coeff = get_vax_float(sbuf, 103);
	h->sample_distance = get_vax_float(sbuf, 105);
	return (0);
}

int             mat_write_main_header (fptr, h)
	FILE           *fptr;
	Main_header    *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;
	
    union {
        unsigned short  a1[2];
        unsigned long   a2;
        float           a3;
    }   test;

    union {
        unsigned short  a1[2];
        unsigned long   a2;
        float           a3;
    }   ttst;

	test.a3 = 122.3;

	buf = (char *) sbuf;

	for (i = 0; i < 256; i++)
		sbuf[i] = 0;

	sbuf[24] = h->sw_version;
	sbuf[25] = h->data_type;
	sbuf[26] = h->system_type;
	sbuf[27] = h->file_type;
	sbuf[33] = h->scan_start_day;
	sbuf[34] = h->scan_start_month;
	sbuf[35] = h->scan_start_year;
	sbuf[36] = h->scan_start_hour;
	sbuf[37] = h->scan_start_minute;
	sbuf[38] = h->scan_start_second;
	make_vax_float(sbuf, 43, h->isotope_halflife);
	make_vax_float(sbuf, 61, h->gantry_tilt);
	make_vax_float(sbuf, 63, h->gantry_rotation);
	make_vax_float(sbuf, 65, h->bed_elevation);
	sbuf[67] = h->rot_source_speed;
	sbuf[68] = h->wobble_speed;
	sbuf[69] = h->transm_source_type;
	make_vax_float (sbuf, 70, h->axial_fov);
	make_vax_float (sbuf, 72, h->transaxial_fov);
	sbuf[74] = h->transaxial_samp_mode;
	sbuf[75] = h->coin_samp_mode;
	sbuf[76] = h->axial_samp_mode;
	make_vax_float(sbuf, 77, h->calibration_factor);
	sbuf[79] = h->calibration_units;
	sbuf[80] = h->compression_code;
	sbuf[175] = h->acquisition_type;
	sbuf[176] = h->bed_type;
	sbuf[177] = h->septa_type;
	sbuf[188] = h->num_planes;
	sbuf[189] = h->num_frames;
	sbuf[190] = h->num_gates;
	sbuf[191] = h->num_bed_pos;
	make_vax_float (sbuf, 192, h->init_bed_position);
	for (i = 0; i < 15; i++)
		make_vax_float (sbuf, 194 + 2 * i, h->bed_offset[i]);
	make_vax_float (sbuf, 224, h->plane_separation);
	sbuf[226] = h->lwr_sctr_thres;
	sbuf[227] = h->lwr_true_thres;
	sbuf[228] = h->upr_true_thres;
	make_vax_float(sbuf, 229, h->collimator);
	sbuf[236] = h->acquisition_mode;

/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) 
		swap_byte (buf, MatBLKSIZE);

	(void) strncpy (buf + 28, h->original_file_name, 20);
	(void) strncpy (buf + 56, h->node_id, 10);
	(void) strncpy (buf + 78, h->isotope_code, 8);
	(void) strncpy (buf + 90, h->radiopharmaceutical, 32);
	(void) strncpy (buf + 162, h->study_name, 12);
	(void) strncpy (buf + 174, h->patient_id, 16);
	(void) strncpy (buf + 190, h->patient_name, 32);
	buf[222] = h->patient_sex;
	(void) strncpy (buf + 223, h->patient_age, 10);
	(void) strncpy (buf + 233, h->patient_height, 10);
	(void) strncpy (buf + 243, h->patient_weight, 10);
	buf[253] = h->patient_dexterity;
	(void) strncpy (buf + 254, h->physician_name, 32);
	(void) strncpy (buf + 286, h->operator_name, 32);
	(void) strncpy (buf + 318, h->study_description, 32);
	(void) strncpy (buf + 356, h->facility_name, 20);
	(void) strncpy (buf + 462, h->user_process_code, 10);

	stat = mat_wblk (fptr, 1, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_write_main_header]\n");
		return (stat);
	}
	return (0);
}

int             mat_write_image_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Image_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;

	buf = (char *) sbuf;

	for (i = 0; i < 256; i++)
		sbuf[i] = 0;

	sbuf[63] = h->data_type;
	sbuf[64] = h->num_dimensions;
	sbuf[66] = h->dimension_1;
	sbuf[67] = h->dimension_2;
	make_vax_float (sbuf, 80, h->x_origin);
	make_vax_float (sbuf, 82, h->y_origin);
	make_vax_float (sbuf, 84, h->recon_scale);
	make_vax_float (sbuf, 86, h->quant_scale);
	sbuf[88] = h->image_min;
	sbuf[89] = h->image_max;
	make_vax_float (sbuf, 92, h->pixel_size);
	make_vax_float (sbuf, 94, h->slice_width);
	make_vax_long (sbuf, 96, h->frame_duration);
	make_vax_long (sbuf, 98, h->frame_start_time);
	sbuf[100] = h->slice_location;
	sbuf[101] = h->recon_start_hour;
	sbuf[102] = h->recon_start_minute;
	sbuf[103] = h->recon_start_sec;
	make_vax_long (sbuf, 104, h->recon_duration);
	sbuf[118] = h->filter_code;
	make_vax_long (sbuf, 119, h->scan_matrix_num);
	make_vax_long (sbuf, 121, h->norm_matrix_num);
	make_vax_long (sbuf, 123, h->atten_cor_matrix_num);
	make_vax_float (sbuf, 148, h->image_rotation);
	make_vax_float (sbuf, 150, h->plane_eff_corr_fctr);
	make_vax_float (sbuf, 152, h->decay_corr_fctr);
	make_vax_float (sbuf, 154, h->loss_corr_fctr);
	make_vax_float (sbuf, 156, h->intrinsic_tilt);
	sbuf[188] = h->processing_code;
	sbuf[190] = h->quant_units;
	sbuf[191] = h->recon_start_day;
	sbuf[192] = h->recon_start_month;
	sbuf[193] = h->recon_start_year;
	make_vax_float (sbuf, 194, h->ecat_calibration_fctr);
	make_vax_float (sbuf, 196, h->well_counter_cal_fctr);
	for (i = 0; i < 6; i++)
		make_vax_float (sbuf, 198 + 2 * i, h->filter_params[i]);

/*	big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) 
		swap_byte (buf, MatBLKSIZE);

	(void) strcpy (buf + 420, h->annotation);

	stat = mat_wblk (fptr, blknum, buf, 1);
	if (stat != 0) {
		fprintf (stderr, "ERROR [mat_write_image_subheader]\n");
		return (stat);
	}
	return (0);
}

int             mat_write_scan_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Scan_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             stat, i;

	buf = (char *) sbuf;

	for (i = 0; i < 256; i++)
		sbuf[i] = 0;
	sbuf[0] = 256;
	sbuf[1] = 1;
	sbuf[2] = 22;
	sbuf[3] = -1;
	sbuf[4] = 25;
	sbuf[5] = 62;
	sbuf[6] = 79;
	sbuf[7] = 106;

	sbuf[25] = -1;
	sbuf[61] = 17;
	sbuf[62] = -1;
	sbuf[78] = 27;
	sbuf[79] = -1;
	sbuf[105] = 52;
	sbuf[106] = -1;
	sbuf[63] = h->data_type;
	sbuf[66] = h->dimension_1;
	sbuf[67] = h->dimension_2;
	sbuf[68] = h->smoothing;
	sbuf[69] = h->processing_code;
	make_vax_float(sbuf, 73, h->sample_distance);
	make_vax_float(sbuf, 83, h->isotope_halflife);
	sbuf[85] = h->frame_duration_sec;
	make_vax_long (sbuf, 86, h->gate_duration);
	make_vax_long (sbuf, 88, h->r_wave_offset);
	make_vax_float(sbuf, 91, h->scale_factor);
	sbuf[96] = h->scan_min;
	sbuf[97] = h->scan_max;
	make_vax_long (sbuf, 98, h->prompts);
	make_vax_long (sbuf, 100, h->delayed);
	make_vax_long (sbuf, 102, h->multiples);
	make_vax_long (sbuf, 104, h->net_trues);
	for (i = 0; i < 16; i++) {
		make_vax_float (sbuf, 158 + 2 * i, h->cor_singles[i]);
		make_vax_float (sbuf, 190 + 2 * i, h->uncor_singles[i]);
	}
	make_vax_float (sbuf, 222, h->tot_avg_cor);
	make_vax_float (sbuf, 224, h->tot_avg_uncor);
	make_vax_long (sbuf, 226, h->total_coin_rate);
	make_vax_long (sbuf, 228, h->frame_start_time);
	make_vax_long (sbuf, 230, h->frame_duration);
	make_vax_float (sbuf, 232, h->loss_correction_fctr);
	for (i = 0; i < 8; i++)
		make_vax_long (sbuf, 234 + 2 * i, h->phy_planes[i]);

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) 
		swap_byte (buf, MatBLKSIZE);
	
	stat = mat_wblk (fptr, blknum, buf, 1);
	return (stat);
}

int             mat_write_norm_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Norm_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             i, stat;

	buf = (char *) sbuf;

	for (i = 0; i < 256; i++)
		sbuf[i] = 0;
	sbuf[0] = 256;
	sbuf[1] = 1;
	sbuf[2] = 22;
	sbuf[3] = -1;
	sbuf[4] = 25;
	sbuf[5] = 62;
	sbuf[6] = 79;
	sbuf[7] = 106;
	sbuf[24] = 37;
	sbuf[25] = -1;
	sbuf[61] = 17;
	sbuf[62] = -1;
	sbuf[78] = 27;
	sbuf[79] = -1;
	sbuf[105] = 52;
	sbuf[106] = -1;
	sbuf[63] = h->data_type;
	sbuf[66] = h->dimension_1;
	sbuf[67] = h->dimension_2;
	make_vax_float(sbuf, 91, h->scale_factor);
	sbuf[93] = h->norm_hour;
	sbuf[94] = h->norm_minute;
	sbuf[95] = h->norm_second;
	sbuf[96] = h->norm_day;
	sbuf[97] = h->norm_month;
	sbuf[98] = h->norm_year;
	make_vax_float(sbuf, 90, h->fov_source_width);
	make_vax_float(sbuf, 101, h->ecat_calib_factor);

/*  big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN) 
		swap_byte (buf, MatBLKSIZE);
	stat = mat_wblk (fptr, blknum, buf, 1);
	return (stat);
}

int             mat_write_attn_subheader (fptr, blknum, h)
	FILE           *fptr;
	int             blknum;
	Attn_subheader *h;
{
	short           sbuf[256];
	char           *buf;
	int             i, stat;

	buf = (char *) sbuf;

	for (i = 0; i < 256; i++)
		sbuf[i] = 0;
	sbuf[0] = 256;
	sbuf[1] = 1;
	sbuf[2] = 22;
	sbuf[3] = -1;
	sbuf[4] = 25;
	sbuf[5] = 62;
	sbuf[6] = 79;
	sbuf[7] = 106;
	sbuf[24] = 37;
	sbuf[25] = -1;
	sbuf[61] = 17;
	sbuf[62] = -1;
	sbuf[78] = 27;
	sbuf[79] = -1;
	sbuf[105] = 52;
	sbuf[106] = -1;
	sbuf[63] = h->data_type;
	sbuf[64] = h->attenuation_type;
	sbuf[66] = h->dimension_1;
	sbuf[67] = h->dimension_2;
	make_vax_float(sbuf, 91, h->scale_factor);
	make_vax_float(sbuf, 93, h->x_origin);
	make_vax_float(sbuf, 95, h->y_origin);
	make_vax_float(sbuf, 97, h->x_radius);
	make_vax_float(sbuf, 99, h->y_radius);
	make_vax_float(sbuf, 101, h->tilt_angle);
	make_vax_float(sbuf, 103, h->attenuation_coeff);
	make_vax_float(sbuf, 105, h->sample_distance);

/*  big endian: swap all bytes except strings	*/

	/* CONSTCOND */
	if (__BYTE_ORDER == __BIG_ENDIAN)
		swap_byte (buf, MatBLKSIZE);
	stat = mat_wblk (fptr, blknum, buf, 1);
	return (stat);
}
