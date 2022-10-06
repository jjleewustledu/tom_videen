#include <flirt++.h>
#include <stdio.h>

using namespace flirt;

int             main (int argc, char *argv[])
{
	int             m1, m2, m3;
	double          h1, h2, h3;
    char           *fileNameT = (char *) malloc (255 * sizeof (char));
    char           *fileNameOut = (char *) malloc (255 * sizeof (char));
    char           *fileNameU = (char *) malloc (255 * sizeof (char));
	FILE           *ptr;

	if (argc > 1)
		getWarp3DParm (argv[1], &m1, &m2, &m3, &h1, &h2, &h3, fileNameT, fileNameOut, fileNameU);
    else {
        fprintf (stderr,"ERROR [applywarp]: no parameter file name given\n\n");
        return 1;
    }

	TransformableImage3D    T (m1, m2, m3, h1, h2, h3);
	T.readFromFile (fileNameT);
	Displacement3D          u (T.getDimensions (), h1, h2, h3);

    if ((ptr = fopen (fileNameU, "r")) != NULL) {
        fprintf (stdout, "Reading 3D transform from %s\n", fileNameU);
        u.readTransform (ptr);
    } else {
        fprintf (stderr, "ERROR [applywarp]: opening %s\n\n", fileNameU);
        return 1;
    };

    fprintf (stdout, "Apply 3D transform\n");
    T.transformImage (u);

	T.writeTransformedImageToFile (fileNameOut);

	fprintf (stdout, "Saved:  %s\n", fileNameOut);
	return 0;
}
