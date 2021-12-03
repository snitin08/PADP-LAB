#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <gd.h>
#include <error.h>
using namespace std;

int main(int argc, char **argv)  {
	gdImagePtr img;
	char *iname, *oname = {0};
	FILE *fp, *fp1 = {0};
	if(argc != 3)
		error(1, 0, "Usage: 4b input.png output.ong\n");
	iname = argv[1];
	oname = argv[2];

	cout << "Enter number of threads\n";
	int nt;
	cin >> nt;
	
	if((fp = fopen(iname, "r")) == NULL)
		error(1, 0, "Error opening %s\n", iname);
	img = gdImageCreateFromPng(fp);
	int w = gdImageSX(img);
	int h = gdImageSY(img);
	
	double t = omp_get_wtime();
	omp_set_num_threads(nt);
	int x, y, red, green, blue, color, tid;
	#pragma omp parallel for private (y, red, green, blue, color) schedule (static, 50)
	for(x = 0; x < w; x++)
		for(y = 0; y < h; y++)
		{
			tid = omp_get_thread_num();
			color = gdImageGetPixel(img, x, y);
			red = gdImageRed(img, color);
			green = gdImageGreen(img, color);
			blue = gdImageBlue(img, color);
			double avg = (red + green + blue) / 3.0;
			if (tid == 0) {
				color = gdImageColorAllocate(img, avg, 0, 0);
				gdImageSetPixel(img, x, y, color);
			}
			if (tid == 1) {
				color = gdImageColorAllocate(img, 0, avg, 0);
				gdImageSetPixel(img, x, y, color);
			}
			if (tid == 2) {
				color = gdImageColorAllocate(img, 0, 0, avg);
				gdImageSetPixel(img, x, y, color);
			}
			if (tid == 3) {
				color = gdImageColorAllocate(img, 0, 0, 0);
				gdImageSetPixel(img, x, y, color);
			}
		}
	t = omp_get_wtime() - t;
	cout << "Time taken: " << t << endl;
	cout << "Threads: " << nt << endl;
	if ((fp1 = fopen(oname, "w")) == NULL)
		error(1, 0, "Error opening %s", oname);
	gdImagePng(img, fp1);
	gdImageDestroy(img);
	fclose(fp1);
	fclose(fp);
	return 0;
}
