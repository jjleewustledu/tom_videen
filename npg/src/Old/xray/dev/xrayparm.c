/*$Id: xrayparm.c,v 1.9 1994/08/31 20:30:14 ty7777 Exp tom $*/
/*$Log: xrayparm.c,v $
 * Revision 1.9  1994/08/31  20:30:14  ty7777
 * Before the algorithm change.
 **/

#ifndef lint
static char     sccsid[] = "@(#)xrayparm.c	10/27/92  Copyright Neural Pet Group, Washington University 1992";

#endif

static char     rcsid[] = "$Id: xrayparm.c,v 1.9 1994/08/31 20:30:14 ty7777 Exp tom $";

/*___________________________________________________________________________
  File:		xrayparm.c

  Function:	void GetXRayParameters ()

  Arguments:

  Description:	This function obtains the parameters that are pertinent to
		the skull X ray.

  History:	Created by Tom Yang on 10/27/92.
___________________________________________________________________________*/

#include <TY/xray.h>
#include <math.h>

PRIVATE void    ComputeXRayParameters ();
PRIVATE void    DefineACPC ();
PRIVATE int     FindZmax ();
PRIVATE void    TransACPC ();

PUBLIC void     GetXRayParameters (cols, rows, tracing, defining_points, drawing_points, sxr_output)
	int             cols;
	int             rows;
	short int      *tracing;
	DEFINING_POINTS defining_points;
	DRAWING_POINTS *drawing_points;
	SXR_OUTPUT     *sxr_output;
{
	POINT2D         anterior;
	POINT2D         apZmax;
	POINT2D         giZmax;
	POINT2D        *glabella;
	POINT2D        *inion;
	POINT2D         posterior;
	POINT2D         vertexZmax;
	float           slope;

	glabella = &defining_points.glabella;
	inion = &defining_points.inion;

	if (glabella->x == inion->x) {
		fprintf (stderr, "The glabella and inion lies on a line parallel to the Z axis\n");
		return;
	}
	slope = ((float) inion->y - glabella->y) / (inion->x - glabella->x);

	FindZmax (cols, rows, tracing, glabella, inion, slope, &giZmax, &apZmax, &vertexZmax);

	DefineACPC (cols, rows, tracing, slope, apZmax, &anterior, &posterior);

	ComputeXRayParameters (cols, rows, tracing, slope, defining_points,
			       drawing_points, anterior, posterior, sxr_output);

	sxr_output->apxray *= RASPIXEL_SIZE;
	sxr_output->zxray *= RASPIXEL_SIZE;
	sxr_output->dz *= RASPIXEL_SIZE;
	sxr_output->dap *= RASPIXEL_SIZE;
	sxr_output->eardap *= RASPIXEL_SIZE;
	sxr_output->eardz *= RASPIXEL_SIZE;

	drawing_points->glabella.x = glabella->x;
	drawing_points->glabella.y = glabella->y;
	drawing_points->inion.x = inion->x;
	drawing_points->inion.y = inion->y;
	drawing_points->acpc_anterior.x = anterior.x;
	drawing_points->acpc_anterior.y = anterior.y;
	drawing_points->acpc_posterior.x = posterior.x;
	drawing_points->acpc_posterior.y = posterior.y;
	drawing_points->acpc_center.x = (anterior.x + posterior.x) / 2;
	drawing_points->acpc_center.y = (anterior.y + posterior.y) / 2;
	drawing_points->vertex_zmax.x = vertexZmax.x;
	drawing_points->vertex_zmax.y = vertexZmax.y;
}

/*___________________________________________________________________________
  Function:	int FindZmax ()

  Arguments:

  Return Value:

  Description:	This function finds the point on the Zmax line that defines
		the AC-PC line.

  History:	Created by Tom Yang on 10/27/92.
___________________________________________________________________________*/

PRIVATE int     FindZmax (cols, rows, tracing, glabella, inion, slope, giZmax, apZmax, vertexZmax)
	int             cols;
	int             rows;
	short int      *tracing;
	POINT2D         glabella;
	POINT2D         inion;
	float           slope;
	POINT2D        *giZmax;
	POINT2D        *apZmax;
	POINT2D        *vertexZmax;
{
	int             left;
	int             right;
	int             x;
	int             y;
	int             xn;
	int             yn;
	float           length;
	float           temp_length;

	/*
	 * Find the point where it is not part of the glabella point.
	 */
	left = glabella.x;
	for (x = glabella.x; x < inion.x; x++) {
		y = ROUND (glabella.y + slope * (x - glabella.x));
		if (tracing[y * cols + x] <= 0)
			break;
	}
	left = x;

	/*
	 * Find the point where it is the left most point on the skull xray on the glabella-inion
	 * line.
	 */
	for (x = left + 1; x < inion.x; x++) {
		y = ROUND (glabella.y + slope * (x - glabella.x));
		if (tracing[y * cols + x] > 0)
			break;
	}
	left = x;

	/*
	 * Find the point where it is not part of the inion point.
	 */
	right = inion.x;
	for (x = inion.x; x > glabella.x; x--) {
		y = ROUND (glabella.y + slope * (x - glabella.x));
		if (tracing[y * cols + x] <= 0)
			break;
	}
	right = x;

	/*
	 * Find the point where it is the right most point on the skull xray on the glabella-inion
	 * line.
	 */
	for (x = right - 1; x > glabella.x; x--) {
		y = ROUND (glabella.y + slope * (x - glabella.x));
		if (tracing[y * cols + x] > 0)
			break;
	}
	right = x;

	length = 0.0;
	giZmax->x = left;
	giZmax->y = ROUND (glabella.y + slope * (left - glabella.x));
	vertexZmax->x = ROUND (left - slope);
	vertexZmax->y = giZmax->y + 1;
	for (x = left; x <= right; x++) {
		y = ROUND (glabella.y + slope * (x - glabella.x));

		for (yn = y + 1; yn >= 0; yn--) {
			xn = ROUND (x - slope * (yn - y));
			if (xn >= 0 && xn < cols && tracing[yn * cols + xn] > 0) {
				temp_length = sqrt (((float) xn - x) * (xn - x)
						    + ((float) yn - y) * (yn - y));
				if (temp_length > length) {
					length = temp_length;
					giZmax->x = x;
					giZmax->y = y;
					vertexZmax->x = xn;
					vertexZmax->y = yn;
				}
				break;
			}
		}
	}

	apZmax->x = ROUND (giZmax->x + ACPC_PERCENTAGE * (vertexZmax->x - giZmax->x));
	apZmax->y = ROUND (giZmax->y + ACPC_PERCENTAGE * (vertexZmax->y - giZmax->y));

	return SUCCEED;
}

/*___________________________________________________________________________
  Function:	int DefineACPC ()

  Arguments:

  Return Value:

  Description:	This function finds the two boundary points that define the
		AC-PC line.

  History:	Created by Tom Yang on 10/28/92.
___________________________________________________________________________*/

PRIVATE void    DefineACPC (cols, rows, tracing, slope, apZmax, anterior, posterior)
	int             cols;
	int             rows;
	short int      *tracing;
	float           slope;
	POINT2D         apZmax;
	POINT2D        *anterior;
	POINT2D        *posterior;
{
	int             x;
	int             y;
	int             thickness;
	int             anterior_thickness;
	int             posterior_thickness;

	/*
	 * Search for anterior intersection of AC-PC line and X ray tracing.
	 */
	for (x = apZmax.x - 1; x >= 0; x--) {
		y = ROUND (apZmax.y + slope * (x - apZmax.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] > 0) {
			anterior->x = x;
			anterior->y = y;
			break;
		}
	}

	/*
	 * Find the thickness of anterior intersection of X ray tracing and and AC-PC.
	 */
	anterior_thickness = 0;
	for (x = anterior->x; x >= 0; x--) {
		y = ROUND (apZmax.y + slope * (x - apZmax.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] == 0) {
			break;
		}
		anterior_thickness++;
	}

	/*
	 * Search for posterior intersection of AC-PC line and X ray tracing.
	 */
	for (x = apZmax.x; x < cols; x++) {
		y = ROUND (apZmax.y + slope * (x - apZmax.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] > 0) {
			posterior->x = x;
			posterior->y = y;
			break;
		}
	}

	/*
	 * Find the thickness of posterior intersection of X ray tracing and and AC-PC.
	 */
	posterior_thickness = 0;
	for (x = posterior->x; x < cols; x++) {
		y = ROUND (apZmax.y + slope * (x - apZmax.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] == 0) {
			break;
		}
		posterior_thickness++;
	}

	/*
	 * Redefine the anterior and posterior point on AC-PC.
	 */
	thickness = (anterior_thickness + posterior_thickness) / 2;
	anterior->x -= thickness;
	anterior->y = ROUND (apZmax.y + slope * (anterior->x - apZmax.x));

	posterior->x += thickness;
	posterior->y = ROUND (apZmax.y + slope * (posterior->x - apZmax.x));
}

/*___________________________________________________________________________
  Function:	void ComputeXRayParameters ()

  Arguments:

  Return Value:

  Description:	This function computes all the parameters that are pertinent
		to the X ray tracing.

  History:	Created by Tom Yang on 10/28/92.
___________________________________________________________________________*/

PRIVATE void    ComputeXRayParameters (cols, rows, tracing, slope, defining_points,
				                  drawing_points, anterior, posterior, sxr_output)
	int             cols;
	int             rows;
	short int      *tracing;
	float           slope;
	DEFINING_POINTS defining_points;
	DRAWING_POINTS *drawing_points;
	POINT2D         anterior;
	POINT2D         posterior;
	SXR_OUTPUT     *sxr_output;
{
	FLOAT_POINT2D   acpc_vector;
	FLOAT_POINT2D   acpc_unit_vector;
	FLOAT_POINT2D   deltaz_refslice;
	FLOAT_POINT2D   left_ear_acpc;
	FLOAT_POINT2D   left_ear_marker;
	FLOAT_POINT2D   right_ear_acpc;
	FLOAT_POINT2D   right_ear_marker;
	FLOAT_POINT2D   lslice_acpc;
	FLOAT_POINT2D   rslice_acpc;
	FLOAT_POINT2D   slice_center;
	FLOAT_POINT2D   slice_lpoint;
	FLOAT_POINT2D   slice_rpoint;
	FLOAT_POINT2D   slice_vector;
	POINT2D         acpc_center;
	POINT2D         slice_anterior;
	POINT2D         slice_left;
	POINT2D         slice_posterior;
	POINT2D         slice_right;
	POINT2D         zvertex;
	float           degxcos;
	float           determinant;
	float           length;
	float           offset;
	float           separation_pixels;
	float           slice_offset;
	float           slice_slope;
	float           theta;
	float           xdeterminant;
	float           xn;
	float           ydeterminant;
	float           yn;
	float           ystep;
	int             i;
	int             x;
	int             y;

	/*
	 * Compute the center of AC-PC.
	 */
	acpc_center.x = (anterior.x + posterior.x) / 2;
	acpc_center.y = (anterior.y + posterior.y) / 2;

	/*
	 * Compute the length of the AC-PC line.
	 */
	acpc_vector.x = (float) (posterior.x - anterior.x);
	acpc_vector.y = (float) (posterior.y - anterior.y);

	sxr_output->apxray = sqrt (acpc_vector.x * acpc_vector.x +
				   acpc_vector.y * acpc_vector.y);

	/*
	 * Compute the angle between the reference PET slice and the AC-PC line.
	 */
	slice_lpoint.x = (float) defining_points.slice_lpoint.x;
	slice_lpoint.y = (float) defining_points.slice_lpoint.y;
	slice_rpoint.x = (float) defining_points.slice_rpoint.x;
	slice_rpoint.y = (float) defining_points.slice_rpoint.y;
	slice_vector.x = slice_rpoint.x - slice_lpoint.x;
	slice_vector.y = slice_rpoint.y - slice_lpoint.y;
	theta = atan (slope);

	TransACPC (acpc_center, theta, slice_lpoint, &lslice_acpc);
	TransACPC (acpc_center, theta, slice_rpoint, &rslice_acpc);


	slice_slope = (rslice_acpc.y - lslice_acpc.y) / (rslice_acpc.x - lslice_acpc.x);
	sxr_output->degx = -radian2deg (atan (slice_slope));

	/*
	 * Compute the distance from the center of AC-PC to the vertex of the skull.
	 */
	for (y = 0; y <= acpc_center.y; y++) {
		x = ROUND (acpc_center.x - slope * (y - acpc_center.y));
		if (x >= 0 && x < cols && tracing[y * cols + x] > 0) {
			zvertex.x = x;
			zvertex.y = y;
			break;
		}
	}

	sxr_output->zxray = sqrt (((float) zvertex.x - acpc_center.x) * (zvertex.x - acpc_center.x)
			     + ((float) zvertex.y - acpc_center.y) * (zvertex.y - acpc_center.y));

	/*
	 * Compute the distance from the center of AC-PC to the reference slice, using a line
	 * perpendicular to the reference slice.
	 */
	determinant = -(slice_vector.x * slice_vector.x + slice_vector.y * slice_vector.y);
	xdeterminant = -(slice_vector.x * slice_vector.x * acpc_center.x)
		+ slice_vector.x * slice_vector.y * (slice_lpoint.y - acpc_center.y)
		- slice_vector.y * slice_vector.y * slice_lpoint.x;
	ydeterminant = -(slice_vector.y * slice_vector.y * acpc_center.y)
		+ slice_vector.x * slice_vector.y * (slice_lpoint.x - acpc_center.x)
		- slice_vector.x * slice_vector.x * slice_lpoint.y;

	xn = xdeterminant / determinant;
	yn = ydeterminant / determinant;
	length = sqrt ((xn - acpc_center.x) * (xn - acpc_center.x)
		       + (yn - acpc_center.y) * (yn - acpc_center.y));
	slice_offset = ((sxr_output->reference_slice - sxr_output->init_slice)
			* sxr_output->slice_separation) / RASPIXEL_SIZE;
	offset = sxr_output->reference_offset / (MMS_PER_CM * RASPIXEL_SIZE)
		+ slice_offset;
	offset /= sxr_output->magnification_factor;

	if (acpc_center.y - yn < 0.0) {
		offset *= -1;
	}
	deltaz_refslice.x = xn + (offset / length) * (acpc_center.x - xn);
	deltaz_refslice.y = yn + (offset / length) * (acpc_center.y - yn);
	sxr_output->dz = sqrt ((deltaz_refslice.x - acpc_center.x)
			       * (deltaz_refslice.x - acpc_center.x)
			       + (deltaz_refslice.y - acpc_center.y)
			       * (deltaz_refslice.y - acpc_center.y));

	if (deltaz_refslice.y < acpc_center.y)
		sxr_output->dz *= -1;

	slice_slope = (slice_rpoint.y - slice_lpoint.y) / (slice_rpoint.x - slice_lpoint.x);
	for (x = deltaz_refslice.x; x >= 0; x--) {
		y = ROUND (deltaz_refslice.y + slice_slope * (x - deltaz_refslice.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
			break;
	}
	drawing_points->slice_anterior.x = x;
	drawing_points->slice_anterior.y = y;

	for (x = deltaz_refslice.x; x < cols; x++) {
		y = ROUND (deltaz_refslice.y + slice_slope * (x - deltaz_refslice.x));
		if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
			break;
	}

	drawing_points->slice_posterior.x = x;
	drawing_points->slice_posterior.y = y;

	/*
	 * Modified for Fran on March 2, 1993 by Tom Yang.
	 */
	separation_pixels = sxr_output->slice_separation / (RASPIXEL_SIZE
							    * sxr_output->magnification_factor);
	ystep = separation_pixels / sqrt (1 + slice_slope * slice_slope);
	for (i = 1; i <= sxr_output->number_slices; i++) {
		xn = deltaz_refslice.x - slice_slope * (i - sxr_output->reference_slice) * ystep;
		yn = deltaz_refslice.y + (i - sxr_output->reference_slice) * ystep;

		for (x = xn; x >= 0; x--) {
			y = ROUND (yn + slice_slope * (x - xn));
			if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
				break;
		}

		slice_anterior.x = x;
		slice_anterior.y = y;
		drawing_points->slices_ap[2 * (i - 1)].x = x;
		drawing_points->slices_ap[2 * (i - 1)].y = y;

		for (x = 0; x <= xn; x++) {
			y = ROUND (yn + slice_slope * (x - xn));
			if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
				break;
		}
		slice_left.x = x;
		slice_left.y = y;

		for (x = xn; x < cols; x++) {
			y = ROUND (yn + slice_slope * (x - xn));
			if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
				break;
		}

		slice_posterior.x = x;
		slice_posterior.y = y;
		drawing_points->slices_ap[2 * (i - 1) + 1].x = x;
		drawing_points->slices_ap[2 * (i - 1) + 1].y = y;

		for (x = cols - 1; x >= xn; x--) {
			y = ROUND (yn + slice_slope * (x - xn));
			if (y >= 0 && y < rows && tracing[y * cols + x] > 0)
				break;
		}
		slice_right.x = x;
		slice_right.y = y;

		length = sqrt (((float) slice_right.x - slice_left.x)
			       * (slice_right.x - slice_left.x)
			       + ((float) slice_right.y - slice_left.y)
			       * (slice_right.y - slice_left.y)) * RASPIXEL_SIZE;

		sxr_output->rlslicedim[i - 1]
			= sqrt (((float) slice_posterior.x - slice_anterior.x)
				* (slice_posterior.x - slice_anterior.x)
				+ ((float) slice_posterior.y - slice_anterior.y)
				* (slice_posterior.y - slice_anterior.y)) * RASPIXEL_SIZE;

		if (sxr_output->rlslicedim[i - 1] > length) {
			sxr_output->rlslicedim[i - 1] = 0.0;
		}
	}

	/*
	 * Compute the distance from the center of the reference slice to the point of intersection
	 * of the the reference slice and the normal line of the reference slice passing through
	 * the AC-PC center.
	 */
	slice_center.x = (drawing_points->slice_anterior.x + drawing_points->slice_posterior.x) / 2;
	slice_center.y = (drawing_points->slice_anterior.y + drawing_points->slice_posterior.y) / 2;

	sxr_output->dap = sqrt ((deltaz_refslice.x - slice_center.x)
				* (deltaz_refslice.x - slice_center.x)
				+ (deltaz_refslice.y - slice_center.y)
				* (deltaz_refslice.y - slice_center.y));
	if (slice_center.x > deltaz_refslice.x)
		sxr_output->dap *= -1;

	/*
	 * Compute the left and right separation of the ear markers.
	 */
	left_ear_marker.x = (float) defining_points.left_ear_marker.x;
	left_ear_marker.y = (float) defining_points.left_ear_marker.y;
	right_ear_marker.x = (float) defining_points.right_ear_marker.x;
	right_ear_marker.y = (float) defining_points.right_ear_marker.y;
	TransACPC (acpc_center, theta, left_ear_marker, &left_ear_acpc);
	TransACPC (acpc_center, theta, right_ear_marker, &right_ear_acpc);

	sxr_output->eardap = left_ear_acpc.x - right_ear_acpc.x;
	sxr_output->eardz = left_ear_acpc.y - right_ear_acpc.y;

	drawing_points->zvertex.x = zvertex.x;
	drawing_points->zvertex.y = zvertex.y;
	drawing_points->ref_normalref.x = deltaz_refslice.x;
	drawing_points->ref_normalref.y = deltaz_refslice.y;
}

/*___________________________________________________________________________
  Function:	void TransACPC ()

  Arguments:	center,	coordinates of Talairach space origin in the image space.
		theta,	angle (degrees) of glabella-inion line with respect to the
			horizontal line.

  Return Value:

  Description:	Transformation from image sapce to Talairach space.

  History:	Created by Tom Yang on 11/06/92.
___________________________________________________________________________*/

PRIVATE void    TransACPC (center, theta, old, new)
	FLOAT_POINT2D   center;
	float           theta;
	FLOAT_POINT2D   old;
	FLOAT_POINT2D  *new;
{
	new->x = (old.x - center.x) * cos (theta) + (old.y - center.y) * sin (theta);
	new->y = -(old.x - center.x) * sin (theta) + (old.y - center.y) * cos (theta);
}
