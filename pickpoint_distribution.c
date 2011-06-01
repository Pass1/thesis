#include <math.h>
#include <stdio.h>


int main(int argc, char *argv[]){
	double min_x = -16000.;
	double max_x = 2000.;
	double min_y = -16000.;
	double max_y = 2000.;
	double min_z = -0.;
	double max_z = -0.;
	
	int n_points_x = 126;
	int n_points_y = 126;
	int n_points_z = 1;

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
	/*
	int x, y, z;
	double current_x = min_x, current_y = min_y, current_z = min_z;
	for (x = 0; x < n_points_x; x++){
		current_y = min_y;
		for (y = 0; y < n_points_y; y++){
			current_z = min_z;
			for (z = 0; z < n_points_z; z++){
				printf("%f %f %f\n", current_x, current_y, current_z);
				current_z += distance_z;
			}
			current_y += distance_y;
		}
		current_x += distance_x;
	}*/
		int i = 1;
		nz = 0;
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
