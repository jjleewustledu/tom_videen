#include "matrix.h"
main(argc, argv)
int argc;
char **argv;
{
	char geo_file[128], intf_file[128], eff_file[128], cmd_line[256];
	char *norm_file, *out_base;
	MatrixFile *norm_mf;
	Main_header *norm_mh;
	Norm3D_subheader *norm_sh;
	MatrixData *norm_matrix;
	int i, j,nelems, ncrystals;
	float *geometric;			/* nelems factor */
	float *crystal_interference;	/* 7 x nelems factor */
	float *crys_efficiency;		/* crystal_rings x crystals_per_ring factors */
	FILE* fp;

	if (argc < 2) crash("usage: show_norm  norm_file [out_file]\n");
	norm_file = argv[1];
	out_base = argc>2? argv[2] : "norm";
	sprintf(geo_file,"%s_geocor",out_base);
	sprintf(intf_file,"%s_crystal_intf",out_base);
	sprintf(eff_file,"%s_crystal_eff",out_base);
	norm_mf = matrix_open(argv[1],MAT_READ_ONLY,Norm3d);
	if (!norm_mf)
		crash("can't open 3D normalization file %s\n", norm_file);
	norm_matrix =  matrix_read(norm_mf,mat_numcod(1,1,1,0,0),GENERIC);
	if (!norm_matrix) 
		crash("norm3D matrix %s,1,1,1,0,0 not found\n",norm_file);
	norm_sh = (Norm3D_subheader *)norm_matrix->shptr;

	nelems = norm_sh->num_r_elements;
	ncrystals = norm_sh->num_crystal_rings*norm_sh->crystals_per_ring;
	geometric = (float*)norm_matrix->data_ptr;
    crystal_interference = geometric +  norm_sh->num_geo_corr_planes*nelems;
    crys_efficiency = crystal_interference + 7*nelems;
	ncrystals = norm_sh->num_crystal_rings*norm_sh->crystals_per_ring;
	fp = fopen(geo_file,"w");
	fprintf(fp,"#xy%d\n",norm_sh->num_geo_corr_planes);
	for (i=0; i<nelems; i++) {
		fprintf(fp,"%d ",i+1);
		for (j=0; j<norm_sh->num_geo_corr_planes; j++)
			fprintf(fp,"%g\n",geometric[j*nelems+i]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	fp = fopen(intf_file,"w");
	fprintf(fp,"#xy7\n");
	for (i=0; i<nelems; i++) fprintf(fp,"%d %g %g %g %g %g %g %g\n", i+1,
		crystal_interference[i], crystal_interference[i+nelems]+1,
		crystal_interference[i+2*nelems]+2, crystal_interference[i+3*nelems]+3,
		crystal_interference[i+4*nelems]+4, crystal_interference[i+4*nelems]+5,
		crystal_interference[i+6*nelems]+6);
	fclose(fp);
	fp = fopen(eff_file,"w");
	fprintf(fp,"#xy\n");
	for (i=0; i<ncrystals; i += 2) fprintf(fp,"%d %g\n",i+1,
		(crys_efficiency[i]+crys_efficiency[i+1])/2);
	fclose(fp);
	matrix_close(norm_mf);
}
