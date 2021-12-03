#include <iostream>
#include <omp.h>
#include <error.h>
#include <gd.h>
#include <string.h>
using namespace std;

int main(int argc, char **argv) {
	FILE *fp, *fp1 = {0};
	gdImagePtr img;
	char *iname = NULL;
	char *oname = NULL;
	int color, x, y, w, h, i = 0;
	int red, green, blue, tmp, tid;
	color = x = y = w = h = 0;
	red = green = blue = 0;
	if(argc != 3)
		error(1, 0, "Usage: gdnegate input.png output.png\n");
	else {
		iname = argv[1];
		oname = argv[2];
	}

	for(int threads = 1; threads <= 8; threads *= 2) {
		if((fp = fopen(iname, "r")) == NULL)
			error(1, 0, "Error in fopen: %s", iname);
		else {
			img = gdImageCreateFromPng(fp);
		}	
		w = gdImageSX(img);
		h = gdImageSY(img);
		omp_set_num_threads(threads);
		double t = omp_get_wtime();
		cout << "w: " << w << " " << "h: " << h << "\n";
		#pragma omp for private(y, color, red, green, blue)
		for(x = 0; x < w; x++)
			for(y = 0; y < h; y++)
			{
				tid = omp_get_thread_num();
				color = gdImageGetPixel(img, x, y);
				red = gdImageRed(img, color);
				green = gdImageGreen(img, color);
				blue = gdImageBlue(img, color);
				double avg = (double)(red + green + blue) / 3.0;
				color = gdImageColorAllocate(img, avg, avg, avg);
				gdImageSetPixel(img, x, y, color);
			}
		if((fp1 = fopen(oname, "w")) == NULL) 
			error(1, 0, "Error opening %s", oname);
		gdImagePng(img, fp1);
		t = omp_get_wtime() - t;
		cout << "Threads: " << threads << "\n";
		cout << "Time taken: " << t << "\n\n";
		gdImageDestroy(img);
		fclose(fp1);
		fclose(fp);
	}
	return 0;
		
}
