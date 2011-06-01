#include "visit_writer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#define NX 10
#define NY 5
#define NZ 1

int main(int argc, char *argv[]){
	struct rlimit rlp;
	uid_t uid;
	float max_magnitude = 0;

	rlp.rlim_cur = 600;
	rlp.rlim_max = 600;
	/* set the number of open file desriptors to
	   MAX_CONNECTIONS */
	if (setrlimit (RLIMIT_NOFILE,&rlp) == -1) {
		perror("setrlimit");
		return(1);
	}
	
	//Open the pickpoints file
	int n_pickpoints;
	FILE *fp_pickpoints;
	fp_pickpoints=fopen("mai/SIVdsp500/pickpoints.dat", "r");
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
		//if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3 ) {
		//NASTY hack for proof of concept. Use the above line instead
		if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3) {
			printf("Couldn't read the %d pickpoint. Aborting.\n", i);
			return 1;
		} else {
			//This is to make it do 2D
			pts[i] = 0.;
			printf("%d\t%f\t%f\t%f\n", i, pts[i-2], pts[i-1], pts[i]);
		}
		i++;
		/*delete this*/

		if (nx == NX) ny = (ny + 1);
		if (ny == NY) nz = (nz + 1);
		ny = ny % NY;
		nx = nx % NX;
		pts[i-2] = nx;
		nx++;
		pts[i-3] = ny;
		pts[i-1] = nz;
				printf("AFTER: %f\t%f\t%f\n", pts[i-3], pts[i-2], pts[i-1]);
		/*end of delete this*/
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
	float data[n_pickpoints][3], nodal_scalar_data[NX][NY], times[n_pickpoints];
	int dims[] = {NX, NY, NZ};
	int nvars = 1, x=0, y=0;
	int vardims[] = {1};
	int centering[] = {1};
	const char *varnames[] = {"nodal"};
	
	//float *vars[] = {(float *)pts, data, data2};
	float *vars[] = {(float *)nodal_scalar_data};
	
	while ( fgets( buff, sizeof buff, fp_inputs[0] ) != NULL) {
		x = 0;
		sprintf(outputFileName, "tmp/luca_test_500_curv.%08d.vtk",j);
		printf("Will be saving to %s\n", outputFileName );
		//printf("read %s\n", buff);
		if (!(sscanf(buff, "%f %f %f %f", &times[0], &data[0][0], &data[0][1], &data[0][2]) == 4)){
			printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", 0);
			return(1);
		}
		//Find out the maximum magnitude
		nodal_scalar_data[x][y] = sqrt(data[0][0]*data[0][0] + data[0][1] * data[0][1] + data[0][2] * data[0][2]);
		if ( nodal_scalar_data[x][y] > max_magnitude) {
			max_magnitude = nodal_scalar_data[0][0];
		}
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
			printf("%d - %d\n", x, y);
			nodal_scalar_data[x][y] = sqrt(data[i][0]*data[i][0] + data[i][1] * data[i][1] + data[i][2] * data[i][2]);

			if (nodal_scalar_data[x][y] > max_magnitude) {
				max_magnitude = nodal_scalar_data[x][y];
			}
			if((++y % NY) == 0){
				x++;
				y=0;
			}
		}
		//At this point I should have all the data for this time step.
		/* Pass the mesh and data to visit_writer. */
		write_curvilinear_mesh(outputFileName, 1, dims, (float*)pts, nvars, vardims, centering, varnames, vars);
		j++;
	}

	for (i=0; i<n_pickpoints; i++){
		fclose(fp_inputs[i]);
	}
	printf("Maximum magnitude was %lf\n", max_magnitude);
	return(0);	
}
