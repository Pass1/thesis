#include <math.h>
#include <stdio.h>
#define MAX_PICKPOINTS 20000

void printusage(int argc, char *argv[], double min_x, double max_x, double min_y, double max_y, double min_z, double max_z, double n_pickpoints_x, double n_pickpoints_y, double n_pickpoints_z){
	printf ("You specified %i arguments, when we are expecting 0 (for the defauls) or 9.\n"
		"Usage: %s [min_x max_x min_y max_y min_z max_z n_pickpoints_x n_pickpoints_y n_pickpoints_z]\n\n"
		"min_[x,y,z] is the minimum value for the particular dimension. For the z value this is the deepest point.\n"
		"max_[x,y,z] is the maximum value for the particular dimension. For the z value this is the point closest to the surface.\n"
		"n_pickpoints_[x,y,z] is the number of pickpoints for the particular dimension.\n\n"
		"Using defaults is equal to:\n"
		"%s %g %g %g %g %g %g %g %g %g\n",
		argc-1, argv[0], argv[0], min_x, max_x, min_y, max_y, min_z, max_z, n_pickpoints_x, n_pickpoints_y, n_pickpoints_z);
}

int main(int argc, char *argv[]){
	double min_x = -10000.;
	double max_x = 10000.;
	double min_y = -5000.;
	double max_y = 5000.;
	double min_z = -10000.;
	double max_z = 0.;
	
	int n_points_x = 25;
	int n_points_y = 25;
	int n_points_z = 25;

	//printf("Arguments specified:: %i\n", argc-1);
	if (argc > 1) {
		if (argc-1 != 9) {
			printusage(argc, argv, min_x, max_x, min_y, max_y, min_z, max_z, n_points_x, n_points_y, n_points_z);
			return(1);
		}
		if (argc-1 == 9) {
			//printf("Reading arguments from command line.\n");
			min_x = atoi(argv[1]);
			max_x = atoi(argv[2]);
			min_y = atoi(argv[3]);
			max_y = atoi(argv[4]);
			min_z = atoi(argv[5]);
			max_z = atoi(argv[6]);
			n_points_x = atoi(argv[7]);
			n_points_y = atoi(argv[8]);
			n_points_z = atoi(argv[9]);
		}
	}

	//Handle error cases
	if (min_z > max_z) {
		printf ("max_z (%g) must be >= min_z (%g).\nExiting.\n", max_z, min_z);
		return 1;
	}
	if (min_x > max_x) {
		printf ("max_x (%g) must be >= min_x (%g).\nExiting.\n", max_x, min_x);
		return 1;
	}
	if (min_y > max_y) {
		printf ("max_y (%g) must be >= min_y (%g).\nExiting.\n", max_y, min_y);
		return 1;
	}
	
	if (n_points_x * n_points_y * n_points_z > MAX_PICKPOINTS) {
		printf("SeisSol supports maximum %i. You specified %g.\nExiting.\n", MAX_PICKPOINTS, n_points_x * n_points_y * n_points_z);
		return 1;
	}
	
	if (n_points_x < 1 | n_points_y < 1 | n_points_z < 1){
		printf("n_pickpoints_[x,y,z] must be > 0.\nExiting.\n");
		return 1;
	}
	//end of handling error cases.

	double delta_x = max_x - min_x;
	double delta_y = max_y - min_y;
	double delta_z = max_z - min_z;
	
	double distance_x = delta_x / (n_points_x - 1);
	double distance_y = delta_y / (n_points_y - 1);
	double distance_z = 0;
	if (n_points_z != 0){
		distance_z = delta_z / (n_points_z - 1);
	} 
	
	double x, y, z;
	int nx, ny, nz;
	int i = 1;
	nz = 0;
	printf("%g %g %g %g %g %g\n", min_x, max_x, min_y, max_y, min_z, max_z);
	printf("%d %d %d\n", n_points_x, n_points_y, n_points_z);

	for(z = min_z; z <= max_z && nz<n_points_z ; z += distance_z){
		ny = 0;
		for (y = min_y; y <= max_y; y+= distance_y){
			nx = 0;
			for (x = min_x; x <= max_x; x+= distance_x){
				if (z!=0.0) {
					printf("%f %f %f\n", x, y, z);
				} else {
					printf("%f %f -0.1\n", x, y);
				}
				i++;
				nx++;
			}
			ny++;
			if (nx != n_points_x) {
				printf("NX = %i", nx);
			}
		}
		nz++;
		if (ny != n_points_y) {
			printf("NY = %i", ny);
		}
	}
	if (nz != n_points_z){
		printf("NZ = %i", nz);
	}
	
	return 0;
}
