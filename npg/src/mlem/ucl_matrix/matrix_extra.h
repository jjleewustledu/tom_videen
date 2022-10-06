#define ERROR   -1
#define OK 0
#define TRUE 1
#define FALSE 0
#define MAX_FRAMES 64
#define MAX_PLANES 1024
#define MAX_GATES   16
#define MAX_BED_POS 16

typedef enum {
		MAT_OK,
		MAT_MALLOC_ERR,
		MAT_MHDREAD_ERR,
		MAT_ACSDIR_ERR,
		MAT_ACS_FILE_NOT_FOUND,
		MAT_NFS_FILE_OPEN_ERR,
		MAT_INTERFILE_OPEN_ERR,
		MAT_NFS_CREATE_ERR,
		MAT_FILE_TYPE_NOT_MATCH,
		MAT_READ_FROM_NILFPTR,
		MAT_NOMHD_FILE_OBJECT,
		MAT_NIL_SHPTR,
		MAT_NIL_DATA_PTR,
		MAT_MATRIX_NOT_FOUND,
		MAT_UNKNOWN_FILE_TYPE,
		MAT_READ_ACS_SUBHD_ERR,
		MAT_READ_ACS_DATA_ERR,
		MAT_ACS_DATA_SIZE_MISMATCH,
		MAT_NFS_FILE_NOT_FOUND,
		MAT_ACS_READ_SUBHD_ERR,
		MAT_NODIRPTR_FILE_OBJECT,
		MAT_READ_ACS_DIRBLK_ERR,
		MAT_ACS_CREATE_ERR,
		MAT_BAD_ATTRIBUTE,
		MAT_BAD_FILE_ACCESS_MODE,
		MAT_END_ERRORS
	}
MatrixErrorCode;


int        mh_update(MatrixFile *file);

MatDirBlk  *mat_rdirblk(MatrixFile *file, int blknum);

MatDirList *mat_read_directory(MatrixFile *mptr);

MatrixFile *matrix_open(const char* fname, int fmode, int mtype);

MatrixFile *matrix_create( const char *fname,int fmode,Main_header *proto_mhptr);

int        matrix_close(MatrixFile *mptr);

MatrixData *matrix_read(MatrixFile *mptr, int matnum, int dtype);

int        matrix_write(MatrixFile *mptr, int matnum, MatrixData *data);

int        matrix_write_slice(MatrixFile *mptr, int matnum, MatrixData *data,
                              int slice);

int        find_bmax(const u_char *bdata, int nvals);

int        find_bmin(const u_char *bdata, int nvals);

int        find_smax( const short *sdata, int nvals);

int        find_smin( const short *sdata, int nvals);

int        find_imax( const int *idata, int nvals);

int        find_imin( const int *idata, int nvals);

float      find_fmin( const float *fdata, int nvals);

float      find_fmax( const float *fdata, int nvals);

int        read_host_data(MatrixFile *mptr, int matnum, MatrixData *data, int dtype);

int        write_host_data(MatrixFile mptr, int matnum, MatrixData *data);

int        matrix_find( MatrixFile *matfile, int matnum, struct MatDir *matdir);

int        matrix_freelist(MatDirList *matdirlist);

int        insert_mdir(struct MatDir matdir, MatDirList *dirlist);

void       free_matrix_data(MatrixData *data);

int        matrix_convert_data();

int        file_exists(char *filename);

int        is_acs(char *fname);

int        strindex(char s[], char t[]);

void       matrix_perror( const char* s);

int        copy_proto_object(MatrixData *new, MatrixData *old);

Main_header *matrix_init_main_header( char *fname, DataSetType ftype, Main_header *mh_proto);

int        matrix_free(MatrixData *matrix);

int        convert_float_scan( MatrixData *scan, float *fdata);

int        convert_long_scan( MatrixData *scan, int *ldata);

MatrixData *matrix_read_scan(MatrixFile *mptr, int matnum, int dtype,int  segment);

int file_data_to_host(char *dptr, int nblks, int dtype);

int write_z_data(MatrixFile *mptr, int matnum, MatrixData *data);
int read_z_data(MatrixFile *mptr, int matnum, MatrixData *data, int dtype);

