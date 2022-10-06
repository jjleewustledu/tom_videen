/* $Id$
 * $Log$
 */

#include <string.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/displayMatrixData.c,v 1.7 1999/12/13 22:05:40 tom Exp tom $";
#endif

void            convertScanHeaderFromVax (b, h)
	short int      *b;
	Scan_subheader *h;
{
	int             i;
	float           get_vax_float ();

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
		h->phy_planes[i] = get_vax_long (b, 234 + 2 * i);
	return;
}

void            convertImageHeaderFromVax (b, h)
	short int      *b;
	Image_subheader *h;
{
	int             i;
	float           get_vax_float ();

	/* strncpy (h->annotation, b + 420, 40); */
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
	h->processing_code = b[188];
	h->quant_units = b[190];
	h->recon_start_day = b[191];
	h->recon_start_month = b[192];
	h->recon_start_year = b[193];
	h->ecat_calibration_fctr = get_vax_float (b, 194);
	h->well_counter_cal_fctr = get_vax_float (b, 196);
	for (i = 0; i < 6; i++)
		h->filter_params[i] = get_vax_float (b, 198 + 2 * i);
	return;
}

void            convertAttnHeaderFromVax (bufr, header)
	short int      *bufr;
	Attn_subheader *header;
{
	float           get_vax_float ();

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
	return;
}

void            convertNormHeaderFromVax (bufr, h)
	short int      *bufr;

	Norm_subheader *h;
{
	float           get_vax_float ();

	swap_byte (bufr, bufr, MatBLKSIZE);
	h->data_type = bufr[63];
	h->dimension_1 = bufr[66];
	h->dimension_2 = bufr[67];
	h->scale_factor = get_vax_float (bufr, 91);
	h->norm_hour = bufr[93];
	h->norm_minute = bufr[94];
	h->norm_second = bufr[95];
	h->norm_day = bufr[96];
	h->norm_month = bufr[97];
	h->norm_year = bufr[98];
	h->fov_source_width = get_vax_float (bufr, 99);
	h->ecat_calib_factor = get_vax_float (bufr, 101);
	return;
}

void            convertScanHeaderToVax (bufr, header)
	short int      *bufr;

	Scan_subheader *header;
{
	int             i;

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
	};
	sunftovaxf (header->tot_avg_cor, &bufr[222]);
	sunftovaxf (header->tot_avg_uncor, &bufr[224]);
	sunltovaxl (header->total_coin_rate, &bufr[226]);	/* total coin rate */
	sunltovaxl (header->frame_start_time, &bufr[228]);
	sunltovaxl (header->frame_duration, &bufr[230]);
	sunftovaxf (header->loss_correction_fctr, &bufr[232]);
	for (i = 0; i < 8; i++)
		sunltovaxl (header->phy_planes[i], &bufr[234 + 2 * i]);
	swap_byte (bufr, bufr, MatBLKSIZE);
	return;
}

void            convertImageHeaderToVax (bufr, header)
	short int      *bufr;

	Image_subheader *header;
{
	char           *bbufr;

	int             i;

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
	return;
}

void            convertAttnHeaderToVax (bufr, header)
	short int      *bufr;

	Attn_subheader *header;
{
	int             i;

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
	return;
}

void            convertNormHeaderToVax (bufr, header)
	short int      *bufr;

	Norm_subheader *header;
{
	int             i;

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
	return;
}
