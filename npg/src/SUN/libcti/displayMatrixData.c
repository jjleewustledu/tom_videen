/*$Id: displayMatrixData.c,v 1.7 1999/12/13 22:05:40 tom Exp $*/
/*$Log: displayMatrixData.c,v $
 *Revision 1.7  1999/12/13 22:05:40  tom
 *remove vax comment
 *
 * Revision 1.6  1999/12/13  22:02:19  tom
 * partially cleaned up with lint
 *
 * Revision 1.5  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.4  1995/10/30  16:54:44  tom
 * changed petutil/matrix.h to matrix.h
 *
 * Revision 1.3  1994/03/03  17:20:08  ty7777
 * Change from id to header.
 **/

#include <string.h>
#include <matrix.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/displayMatrixData.c,v 1.7 1999/12/13 22:05:40 tom Exp $";
#endif

displayMainHeader (mhead)
	Main_header    *mhead;
{
	printf ("Original file name : %s\n", mhead->original_file_name);
	printf ("Software version : %d\n", mhead->sw_version);
	printf ("Data type : %d\n", mhead->data_type);
	printf ("System type : %d\n", mhead->system_type);
	printf ("File type : %d\n", mhead->file_type);
	printf ("Node ID : %s\n", mhead->node_id);
	printf ("Scan start day : %d\n", mhead->scan_start_day);
	printf ("Scan start month : %d\n", mhead->scan_start_month);
	printf ("Scan start year : %d\n", mhead->scan_start_year);
	printf ("Scan start hour : %d\n", mhead->scan_start_hour);
	printf ("Scan start minute : %d\n", mhead->scan_start_minute);
	printf ("Scan start second : %d\n", mhead->scan_start_second);
	printf ("Isotope code : %s\n", mhead->isotope_code);
	printf ("Isotope half life %f\n", mhead->isotope_halflife);
	printf ("Radio-pharmaceutical %s\n", mhead->radiopharmaceutical);
	printf ("Gantry tilt %f\n", mhead->gantry_tilt);
	printf ("Gantry rotation %f\n", mhead->gantry_rotation);
	printf ("Bed elevation %f\n", mhead->bed_elevation);
	printf ("Rod source speed %d\n", mhead->rot_source_speed);
	printf ("Wobble speed %d\n", mhead->wobble_speed);
	printf ("Transmission source type : %d\n", mhead->transm_source_type);
	printf ("Axial FOV : %f\n", mhead->axial_fov);
	printf ("Transaxial FOV : %f\n", mhead->transaxial_fov);
	printf ("Transaxial sample mode : %d\n", mhead->transaxial_samp_mode);
	printf ("Coincidence sample mode : %d\n", mhead->coin_samp_mode);
	printf ("Axial sample mode : %d\n", mhead->axial_samp_mode);
	printf ("Calibration factor : %f\n", mhead->calibration_factor);
	printf ("Calibration Units : %d\n", mhead->calibration_units);
	printf ("Compression code : %d\n", mhead->compression_code);
	printf ("Study name : %s\n", mhead->study_name);
	printf ("Patient ID : %s\n", mhead->patient_id);
	printf ("Patient Name : %s\n", mhead->patient_name);
	printf ("Patient Sex : %c\n", mhead->patient_sex);
	printf ("Patient Age : %s\n", mhead->patient_age);
	printf ("Patient Height : %s\n", mhead->patient_height);
	printf ("Patient Weight : %s\n", mhead->patient_weight);
	printf ("Patient Dexterity : %c\n", mhead->patient_dexterity);
	printf ("Physician name : %s\n", mhead->physician_name);
	printf ("Operator name : %s\n", mhead->operator_name);
	printf ("Comment : %s\n", mhead->study_description);
	printf ("Acquisition type : %d\n", mhead->acquisition_type);
	printf ("Bed type : %d\n", mhead->bed_type);
	printf ("Septa type : %d\n", mhead->septa_type);
	printf ("Facility Name : %s\n", mhead->facility_name);
	printf ("Number of planes : %d\n", mhead->num_planes);
	printf ("Number of frames : %d\n", mhead->num_frames);
	printf ("Number of gates : %d\n", mhead->num_gates);
	printf ("Number of bed positions : %d\n", mhead->num_bed_pos);
	printf ("Initial bed position : %f\n", mhead->init_bed_position);
	printf ("Plane separation : %f\n", mhead->plane_separation);
	printf ("Lower scatter threshold : %d\n", mhead->lwr_sctr_thres);
	printf ("Lower trues threshold : %d\n", mhead->lwr_true_thres);
	printf ("Upper trues threshold : %d\n", mhead->upr_true_thres);
	printf ("Acquisition mode : %d\n", mhead->acquisition_mode);
	return 0;
}

displayScanHeader (scan)
	Scan_subheader *scan;
{
	int             i;

	printf ("Data type %d\n", scan->data_type);
	printf ("Number of elements : %d\n", scan->dimension_1);
	printf ("Number of views : %d\n", scan->dimension_2);
	printf ("Smoothing method : %d\n", scan->smoothing);
	printf ("Processing code : %d\n", scan->processing_code);
	printf ("Sampling distance : %f\n", scan->sample_distance);
	printf ("Isotope half life : %f\n", scan->isotope_halflife);
	printf ("Frame Duration : %d (seconds)\n", scan->frame_duration_sec);
	printf ("Gate Duration : %d\n", scan->gate_duration);
	printf ("R wave offset : %d\n", scan->r_wave_offset);
	printf ("Scale factor : %f\n", scan->scale_factor);
	printf ("Scan min : %d\n", scan->scan_min);
	printf ("Scan max : %d\n", scan->scan_max);
	printf ("Prompts : %d\n", scan->prompts);
	printf ("Delayed : %d\n", scan->delayed);
	printf ("Multiples : %d\n", scan->multiples);
	printf ("Net trues : %d\n", scan->net_trues);
	printf ("Corrected Singles     Uncorrected Singles\n");
	for (i = 0; i < 16; i++)
		printf ("      %f          %f\n", scan->cor_singles[i], scan->uncor_singles[i]);
	printf ("Total Average Corrected Singles : %f\n", scan->tot_avg_cor);
	printf ("Total Average Uncorrected Singles : %f\n", scan->tot_avg_uncor);
	printf ("Total Coincidence rate : %f\n", scan->total_coin_rate);
	printf ("Frame Start Time : %d\n", scan->frame_start_time);
	printf ("Frame Duration : %d (msecs)\n", scan->frame_duration);
	printf ("Loss Correction Factor : %f\n", scan->loss_correction_fctr);
	printf ("Physical Plane List:\n");
	for (i = 0; i < 8; i++)
		printf ("      %d ", scan->phy_planes[i]);
	printf ("\n");
	return 0;
}

displayImageHeader (imsub)
	Image_subheader *imsub;
{
	int             i;

	printf ("Data Type : %d\n", imsub->data_type);
	printf ("Number of dimensions : %d\n", imsub->num_dimensions);
	printf ("Dimensions : %d x %d\n", imsub->dimension_1, imsub->dimension_2);
	printf ("X Origin : %f\n", imsub->x_origin);
	printf ("Y Origin : %f\n", imsub->y_origin);
	printf ("Recon Scale : %f\n", imsub->recon_scale);
	printf ("Quant Scale : %e\n", imsub->quant_scale);
	printf ("Image min %d\n", imsub->image_min);
	printf ("Image max %d\n", imsub->image_max);
	printf ("Image Pixel Size : %f\n", imsub->pixel_size);
	printf ("Slice width : %f\n", imsub->slice_width);
	printf ("Frame Duration %d\n", imsub->frame_duration);
	printf ("Frame start time : %d\n", imsub->frame_start_time);
	printf ("Slice Location : %d\n", imsub->slice_location);
	printf ("Recon Start Hour : %d\n", imsub->recon_start_hour);
	printf ("Recon Start Minute : %d\n", imsub->recon_start_minute);
	printf ("Recon Start Second : %d\n", imsub->recon_start_sec);
	printf ("Recon Duration : %d\n", imsub->recon_duration);
	printf ("Filter Code : %d\n", imsub->filter_code);
	printf ("Scan matrix number : %x\n", imsub->scan_matrix_num);
	printf ("Norm matrix number : %x\n", imsub->norm_matrix_num);
	printf ("Attenuation matrix number : %x\n", imsub->atten_cor_matrix_num);
	printf ("Image Rotation %f\n", imsub->image_rotation);
	printf ("Plane efficiency correction factor %f\n", imsub->plane_eff_corr_fctr);
	printf ("Decay Correction Factor : %f\n", imsub->decay_corr_fctr);
	printf ("Loss Correction Factor : %f\n", imsub->loss_corr_fctr);
	printf ("Processing Code : %d\n", imsub->processing_code);
	printf ("Quantification Units : %d\n", imsub->quant_units);
	printf ("Recon Start Day : %d\n", imsub->recon_start_day);
	printf ("Recon Start Month : %d\n", imsub->recon_start_month);
	printf ("Recon Start Year : %d\n", imsub->recon_start_year);
	printf ("ECAT calibration factor : %f\n", imsub->ecat_calibration_fctr);
	printf ("Well Counter calibration factor : %f\n", imsub->well_counter_cal_fctr);
	for (i = 0; i < 6; i++)
		printf ("Filter Parameter %d : %f\n", i + 1, imsub->filter_params[i]);
	printf ("Comment : %s\n", imsub->annotation);

	return 0;
}

displayAttnHeader (attnsub)
	Attn_subheader *attnsub;
{
	printf ("Data type : %d\n", attnsub->data_type);
	printf ("Attenuation type : %d\n", attnsub->attenuation_type);
	printf ("Number of elements : %d\n", attnsub->dimension_1);
	printf ("Number of views : %d\n", attnsub->dimension_2);
	printf ("Scale Factor : %f\n", attnsub->scale_factor);
	printf ("X origin : %f\n", attnsub->x_origin);
	printf ("Y origin : %f\n", attnsub->y_origin);
	printf ("X radius : %f\n", attnsub->x_radius);
	printf ("Y radius : %f\n", attnsub->y_radius);
	printf ("Tilt angle : %f\n", attnsub->tilt_angle);
	printf ("Attenuation coefficient %f\n", attnsub->attenuation_coeff);
	printf ("Sample distance %f\n", attnsub->sample_distance);

	return 0;
}

displayNormHeader (normsub)
	Norm_subheader *normsub;
{
	printf ("Data type : %d\n", normsub->data_type);
	printf ("Number of elements : %d\n", normsub->dimension_1);
	printf ("Number of views : %d\n", normsub->dimension_2);
	printf ("Scale Factor : %f\n", normsub->scale_factor);
	printf ("Norm hour : %d\n", normsub->norm_hour);
	printf ("Norm minute : %d\n", normsub->norm_minute);
	printf ("Norm second : %d\n", normsub->norm_second);
	printf ("Norm day : %d\n", normsub->norm_day);
	printf ("Norm month : %d\n", normsub->norm_month);
	printf ("Norm year : %d\n", normsub->norm_year);
	printf ("FOV source width : %f\n", normsub->fov_source_width);
	printf ("ECF : %f\n", normsub->ecat_calib_factor);

	return 0;
}

displayData (data, nViews, nElems, limitFlag, lowLimit, highLimit)
	short int      *data;
	int             nViews, nElems, limitFlag, lowLimit, highLimit;
{
	int             i, j, dataMax, dataMin, maxV, maxE, minV, minE;

	for (maxV = maxE = minV = minE = 0, dataMax = -32767, dataMin = 32768, i = 0; i < nViews; i++) {
		if (!limitFlag)
			printf ("\n\nData dump for view %3d\n", i);
		for (j = 0; j < nElems; j++, data++) {
			if (limitFlag) {
				if (*data >= lowLimit && *data <= highLimit)
					printf ("View: %5d Elem: %3d Value: %5d \n", i - (nViews / 2 + 1), j, *data);
			} else {
				if (j % 16 == 0 && j > 0)
					printf ("\n");
				printf ("%5d ", *data);
			}
			if (dataMax < *data) {
				dataMax = *data;
				maxV = i;
				maxE = j;
			}
			if (dataMin > *data) {
				dataMin = *data;
				minV = i;
				minE = j;
			}
		}
	}
	printf ("\n\nMaximum value of %5d occurred in view %3d, element %3d\n", dataMax, maxV - (nViews / 2 + 1), maxE);
	printf ("Minimum value of %5d occurred in view %3d, element %3d\n", dataMin, minV - (nViews / 2 + 1), minE);
	return 0;
}

displayFloatData (data, nViews, nElems, limitFlag, lowLimit, highLimit)
	float          *data, lowLimit, highLimit;
	int             nViews, nElems, limitFlag;
{
	int             i, j, maxV, maxE, minV, minE;
	float           dataMax, dataMin;

	for (maxV = maxE = minV = minE = 0, dataMax = -32767., dataMin = 32768., i = 0; i < nViews; i++) {
		if (!limitFlag)
			printf ("\n\nData dump for view %3d\n", i);
		for (j = 0; j < nElems; j++, data++) {
			if (limitFlag) {
				if (*data >= lowLimit && *data <= highLimit)
					printf ("View: %5d Elem: %3d Value: %4.2f \n", i - (nViews / 2 + 1), j, *data);
			} else {
				if (j % 16 == 0 && j > 0)
					printf ("\n");
				printf ("%4.2f ", *data);
			}
			if (dataMax < *data) {
				dataMax = *data;
				maxV = i;
				maxE = j;
			}
			if (dataMin > *data) {
				dataMin = *data;
				minV = i;
				minE = j;
			}
		}
	}
	printf ("\n\nMaximum value of %4.2f occurred in view %3d, element %3d\n", dataMax, maxV - (nViews / 2 + 1), maxE);
	printf ("Minimum value of %4.2f occurred in view %3d, element %3d\n", dataMin, minV - (nViews / 2 + 1), minE);
	return 0;
}

dumpHexData (buff, size)
	unsigned short int *buff;
	int             size;
{
	int             i;

	for (i = 0; i < size; i++) {
		if (i % 16 == 0 && i > 0)
			printf ("\n");
		printf ("%04x ", *buff++);
	}
	printf ("\n");
	return 0;
}

convertScanHeaderFromVax (b, h)
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
	return 0;
}

convertImageHeaderFromVax (b, h)
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

	return 0;
}

convertAttnHeaderFromVax (bufr, header)
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

	return 0;
}

convertNormHeaderFromVax (bufr, h)
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

	return 0;
}

convertScanHeaderToVax (bufr, header)
	short int      *bufr;

	Scan_subheader *header;
{
	int             i;

	for (i = 0; i < 256; bufr[i++] = 0);
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
	return 0;
}

convertImageHeaderToVax (bufr, header)
	short int      *bufr;

	Image_subheader *header;
{
	char           *bbufr;

	int             i;

	for (i = 0; i < 256; bufr[i++] = 0);
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

	return 0;
}

convertAttnHeaderToVax (bufr, header)
	short int      *bufr;

	Attn_subheader *header;
{
	int             i;

	for (i = 0; i < 256; bufr[i++] = 0);
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

	return 0;
}

convertNormHeaderToVax (bufr, header)
	short int      *bufr;

	Norm_subheader *header;
{
	int             i;

	for (i = 0; i < 256; bufr[i++] = 0);
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

	return 0;
}
