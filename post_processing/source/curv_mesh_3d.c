#include "visit_writer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#define NX 126
#define NY 126 
#define NZ 1 

int main(int argc, char *argv[]){
	struct rlimit rlp;
	uid_t uid;
	float max_magnitude = 0;
	float max_u = 0;
	float max_v = 0;
	float max_w = 0;
	float min_u = 0;
	float min_v = 0;
	float min_w = 0;

	rlp.rlim_cur = 16000;
	rlp.rlim_max = 16000;
	/* set the number of open file desriptors to
	   MAX_CONNECTIONS */
	if (setrlimit (RLIMIT_NOFILE,&rlp) == -1) {
		perror("setrlimit");
		return(1);
	}
	
	//Open the pickpoints file
	int n_pickpoints;
	FILE *fp_pickpoints;
	fp_pickpoints=fopen("pickpoints.dat", "r");
	if( fp_pickpoints == NULL ){
		printf("Cannot open file\n") ;
		return(1) ;
	} else {
		printf("Opened pickpoints file\n");
	}
	char buff[200];
	fgets( buff, sizeof buff, fp_pickpoints );
	printf("%s", buff);
	if (sscanf( buff, "%d", &n_pickpoints) != 1) {
		printf("Couldn't find the number of pickpoints.\n");
	}
	printf("Will be reading %d pickpoints.\n", n_pickpoints);


	float pts[n_pickpoints * 3];
	int i;
	i = 0;
	int nx=0, ny=0, nz=0;
	while ( fgets( buff, sizeof buff, fp_pickpoints ) != NULL && i < n_pickpoints * 3) {
//		if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3 ) {
		if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i+1], &pts[i+2] ) == 3 ) {
			printf("Couldn't read the %d pickpoint. Aborting.\n", i);
			return 1;
		} else {
			printf("%d\t%f\t%f\t%f\n", i, pts[i], pts[i+1], pts[i+2]);
			i+=3;
		}
	}
	i=0;
	int len;
	FILE *fp_inputs[n_pickpoints];
	while ( fgets( buff, sizeof buff, fp_pickpoints ) != NULL && i < n_pickpoints) {
		//remove the \n at the end
		len = strlen(buff);
		if( buff[len-1] == '\n' )
		    buff[len-1] = 0;
		fp_inputs[i] = fopen(buff, "r");
		if (fp_inputs[i] == NULL) {
			printf("Unable to open file %s in slot %i.\nExiting.\n", buff, i);
			return(1);
		} else {
			printf("Opened %s in slot %i.\n", buff, i);
		}
		i++;
	}

	fclose (fp_pickpoints);

//Skip the information at the beginning
	int j;
	for(i=0; i<n_pickpoints; i++) {
		for(j=0; j<5; j++){
			fgets(buff,sizeof buff, fp_inputs[i]);
//			printf("%i - %s\n", i, buff);
		}
	}
	
	j=0;
	char outputFileName[100];
	float data[n_pickpoints][3], nodal_scalar_data[NZ][NY][NX], u[NZ][NY][NX], v[NZ][NY][NX], w[NZ][NY][NX], uvw[NZ][NY][NX][3], times[n_pickpoints];
	int dims[] = {NX, NY, NZ};
	int nvars = 5, x=0, y=0, z=0;
	int vardims[] = {1, 1, 1, 1, 3};
	int centering[] = {1, 1, 1, 1};
	const char *varnames[] = {"nodal", "u", "v", "w", "uvw"};
	
	//float *vars[] = {(float *)pts, data, data2};
	float *vars[] = {(float *)nodal_scalar_data, (float *)u, (float *)v, (float *)w, (float *)uvw};
	
	while ( fgets( buff, sizeof buff, fp_inputs[0] ) != NULL) {
		/*if (j==39){
			for (x = 0; x<25; x++){
			for (y = 0; y<25; y++){
			for (z = 0; z<25; z++){
				printf("%i: %20.12e\n",x , u[x][y][z]);
			}}}
			return(1);
		}*/
		x = 0;
		z = 0;
		y = 0;
		sprintf(outputFileName, "../vis/timestep.%08d.vtk",j);
		printf("Will be saving to %s\n", outputFileName );
		//printf("read %s\n", buff);
		if (!(sscanf(buff, "%f %f %f %f", &times[0], &data[0][0], &data[0][1], &data[0][2]) == 4)){
			printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", 0);
			return(1);
		}
		//Find out the maximum magnitude
		nodal_scalar_data[z][y][x] = 0; // sqrt(data[0][0]*data[0][0] + data[0][1] * data[0][1] + data[0][2] * data[0][2]);
		u[z][y][x] = data[0][0];
		v[z][y][x] = data[0][1];
		w[z][y][x] = data[0][2];
		uvw[z][y][x][0] = data[0][0];
		uvw[z][y][x][1] = data[0][1];
		uvw[z][y][x][2] = data[0][2];
		if ( nodal_scalar_data[z][y][x] > max_magnitude) {
			max_magnitude = nodal_scalar_data[0][0][0];
		}
		if (u[z][y][x] > max_u) max_u = u[z][y][x];
		if (u[z][y][x] < min_u) min_u = u[z][y][x];
		if (v[z][y][x] > max_v) max_v = v[z][y][x];
		if (v[z][y][x] < min_v) min_v = v[z][y][x];
		if (w[z][y][x] > max_w) max_w = w[z][y][x];
		if (w[z][y][x] < min_w) min_w = w[z][y][x];
		//printf("nodal_scalar_data[%d][%d][%d] = %.10lf\n", z, y, x, nodal_scalar_data[z][y][x] );
		y++;
		for (i=1; i<n_pickpoints; i++){
			if (fgets(buff, sizeof buff, fp_inputs[i]) == NULL){
				printf("The files don't seem to be of the same length (pickpoint %i)\nExiting loop\n", i);
				break;
			}
			if (!(sscanf(buff, "%f %f %f %f", &times[i], &data[i][0], &data[i][1], &data[i][2]) == 4)){
				printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", 0);
				return(1);
			}
			/*if (!(sscanf(buff, "%f %f %f", &data[i][0], &data[i][1], &data[i][2]) == 3)){
				printf("Scanf for pickpoint %i didn't return 3.\nExiting.\n", 0);
				return(1);
			}*/
			//printf("%d - %d - %d\n", x, y, z);
			nodal_scalar_data[z][y][x] = 0; //sqrt(data[i][0]*data[i][0] + data[i][1] * data[i][1] + data[i][2] * data[i][2]);
			u[z][y][x] = data[i][0];
			v[z][y][x] = data[i][1];
			w[z][y][x] = data[i][2];
			uvw[z][y][x][0] = data[i][0];
			uvw[z][y][x][1] = data[i][1];
			uvw[z][y][x][2] = data[i][2];
			//printf("nodal_scalar_data[%d][%d][%d] = %.10lf\n", z, y, x, nodal_scalar_data[z][y][x] );
			if (nodal_scalar_data[z][y][x] > max_magnitude) {
				max_magnitude = nodal_scalar_data[z][y][x];
			}
			if (u[z][y][x] > max_u) max_u = u[z][y][x];
			if (u[z][y][x] < min_u) min_u = u[z][y][x];
			if (v[z][y][x] > max_v) max_v = v[z][y][x];
			if (v[z][y][x] < min_v) min_v = v[z][y][x];
			if (w[z][y][x] > max_w) max_w = w[z][y][x];
			if (w[z][y][x] < min_w) min_w = w[z][y][x];
			
			if((++y % NY) == 0){
				x++;
				y=0;
			}
			if (x == NX) {
				z++;
				x = 0;
			}
		}
		//At this point I should have all the data for this time step.
		/* Pass the mesh and data to visit_writer. */
		write_curvilinear_mesh(outputFileName, 0, dims, (float*)pts, nvars, vardims, centering, varnames, vars);
		j++;
	}

	for (i=0; i<n_pickpoints; i++){
		fclose(fp_inputs[i]);
	}
	printf("Maxi magnitude was %lf\n", max_magnitude);
	printf("Max || Min u: %lf || %lf \n", max_u, min_u);
	printf("Max || Min v: %lf || %lf \n", max_v, min_v);
	printf("Max || Min w: %lf || %lf \n", max_w, min_w);
	
	return(0);	
}
