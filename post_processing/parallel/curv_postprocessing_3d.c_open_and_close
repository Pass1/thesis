#include <mpi.h>
#include "visit_writer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#define NX 25
#define NY 25
#define NZ 25

int main(int argc, char *argv[]){
	int rank,size;
	double start_time, end_time;
	/* record start time */
	start_time = MPI_Wtime();
	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */

	struct rlimit rlp;
	float max_magnitude = 0;
	float max_u = 0;
	float max_v = 0;
	float max_w = 0;
	float min_u = 0;
	float min_v = 0;
	float min_w = 0;
	int nz=0, lowerbound;
	/*rlp.rlim_cur = 10220;
	rlp.rlim_max = 10220;
	//set the number of open file desriptors to MAX_CONNECTIONS
	if (setrlimit (RLIMIT_NOFILE,&rlp) == -1) {
		perror("setrlimit");
		MPI_Finalize();
		return(1);
	}*/
	if(size > NZ) {
		if(rank == 0)
			printf("You have more processors than layers (nz = %i)!\nSince multiprocessor partitioning works on dividing layers amongs processors... well...\nI'm quitting!\nMake sure np < nz (np < %i, yes.. that's a lower than, not lower or equal than)\n", NZ, NZ);
		MPI_Finalize();
		return(1);
	}
	//Open the pickpoints file
	int n_pickpoints, total_pickpoints;
	FILE *fp_pickpoints;
	fp_pickpoints=fopen("pickpoints.dat", "r");
	if( fp_pickpoints == NULL ){
		printf("Cannot open pickpoints file\n") ;
		return(1) ;
	} else {
		printf("Opened pickpoints file\n");
	}
	char buff[200];
	fgets( buff, sizeof buff, fp_pickpoints );
	//printf("%s", buff);
	if (sscanf( buff, "%d", &total_pickpoints) != 1) {
		printf("Couldn't find the number of pickpoints.\n");
	}
	//VisIt won't interpolate between 2 time zones, therefoere we need to fill in the gap
	nz = NZ / size;
	lowerbound = nz * NX * NY * rank;
	if(rank == size - 1){
		nz = NZ - (nz * (size - 1));
		n_pickpoints = NX * NY * nz;
	} else{
		nz = (NZ / size) + 1;
		n_pickpoints = nz * NX * NY;
	}

	printf("[%i] Will be reading %i pickpoints out of %i.\n", rank, n_pickpoints, total_pickpoints);
	/*MPI_Finalize();
	return(1);*/

	float pts[n_pickpoints * 3];
	int i, counter;
	i = 0;
	counter = 0;
	printf("[%i] lowerbound: %i\n", rank, lowerbound);
	//while ( fgets( buff, sizeof buff, fp_pickpoints ) != NULL && counter < total_pickpoints) {
	for(counter = 0; counter < total_pickpoints && (fgets( buff, sizeof buff, fp_pickpoints ) != NULL); counter ++){
//		if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3 ) {
		if(counter >= lowerbound && counter < lowerbound + n_pickpoints){
			//printf("Through the if\n");
			if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i+1], &pts[i+2] ) == 3 ) {
				printf("Couldn't read the %d pickpoint. Aborting.\n", i);
				MPI_Finalize();
				return 1;
			} else {
				//printf("[%i]\t%d\t%f\t%f\t%f\n", rank, i, pts[i], pts[i+1], pts[i+2]);
				i += 3;
			}
		}/* else {
			printf("Failed the if\n");
		}*/
	}
	printf("[%i] read in %i coordinates (shoulde be n_points * 3)\n", rank, i);	

//Get the name of the files
	i=0;
	counter = 0;
	int len;
	//FILE *fp_inputs[n_pickpoints];
	char inputs_files[n_pickpoints+1][100];
	off_t inputs_offsets[n_pickpoints+1];
	printf("Getting pickpoint file name information.\n");

	//while ( fgets( buff, sizeof buff, fp_pickpoints ) != NULL && counter++ < total_pickpoints) {
	for(counter = 0; counter < total_pickpoints && (fgets(inputs_files[i], sizeof buff, fp_pickpoints ) != NULL); counter ++){
		//printf("[%i] counter: %i lowerbound %i\n", rank, counter, lowerbound);
		if(counter >= lowerbound && counter < lowerbound + n_pickpoints){
			//printf("[%i] Got file name %s", rank, inputs_files[i]);
			//remove the \n at the end
			len = strlen(inputs_files[i]);
			if( inputs_files[i][len-1] == '\n' )
			    inputs_files[i][len-1] = 0;
			//inputs_files[i] = buff;
			/*fp_inputs[i] = fopen(inputs_files[i], "r");
			if (fp_inputs[i] == NULL) {
				printf("[%i] Unable to open file %s in slot %i.\nExiting.\n", rank, inputs_files[i], i);
				return(1);
			} else {
				printf("Opened %s in slot %i.\n", inputs_files[i], i);
			}*/
			i++;
		}
	}	

	fclose (fp_pickpoints);
	/*
	fgets(buff,sizeof buff, fp_inputs[167]);
	printf("[%i] %s", rank, buff);
	fgets(buff,sizeof buff, fp_inputs[199]);
	printf("[%i] %s", rank, buff);
	MPI_Finalize();
	return(0);//*/
//Skip the information at the beginning
	printf("[%i]Skipping information at the beginning\n", rank);
	int j;
	FILE *fp_input;
	for(i=0; i < n_pickpoints; i++) {
		//printf("[%i] skipping for file %i - \n", rank, i);
		fp_input = fopen(inputs_files[i], "r");
		if (fp_input == NULL) {
			printf("[%i] File was null %s!", rank,  inputs_files[i]);
			MPI_Finalize();
			return(1);
		}
		for(j=0; j<5; j++){
			fgets(buff,sizeof buff, fp_input);
			//printf("[%i] %i - %s\n", rank, i, buff);
		}
		inputs_offsets[i] = ftello(fp_input);
		//printf("Current location in the file: %i __ %i",  ftell(fp_inputs[i]), inputs_offsets[i]);
		fclose(fp_input);
	}
	printf("[%i] Completed skipping information at the beginning\n", rank);
	
	/*for(i=0; i < 10; i++) {
		fgets(buff,sizeof buff, fp_inputs[i]);
		printf("Before %s\n", buff );
		fclose(fp_inputs[i]);
		fp_inputs[i] = fopen(inputs_files[i], "r");
		fseek(fp_inputs[i], inputs_offsets[i], SEEK_SET);
		fgets(buff,sizeof buff, fp_inputs[i]);
		printf("After %s\n\n", buff );
	}*/
	
	j=0;
	char outputFileName[100];
	float data[n_pickpoints][3], nodal_scalar_data[nz][NY][NX], u[nz][NY][NX], v[nz][NY][NX], w[nz][NY][NX], uvw[nz][NY][NX][3], times[n_pickpoints];
	int dims[] = {NX, NY, nz};
	int nvars = 5, x=0, y=0, z=0;
	int vardims[] = {1, 1, 1, 1, 3};
	int centering[] = {1, 1, 1, 1};
	const char *varnames[] = {"nodal", "u", "v", "w", "uvw"};

	//float *vars[] = {(float *)pts, data, data2};
	float *vars[] = {(float *)nodal_scalar_data, (float *)u, (float *)v, (float *)w, (float *)uvw};

	printf("[%i] Starting processing\n", rank);	
	j=0;
	int finished = 0;
	while (!finished){
		x = 0;
		z = 0;
		y = 0;
		
		for (i=0; i<n_pickpoints; i++){
			//printf("[%i] i = %i reading from file: \n", rank, i, inputs_files[i]);
			fp_input = fopen(inputs_files[i], "r");
			if (fp_input == NULL){
				printf("[%i] Unable to open file %s\n", rank, inputs_files[i]);
			}
			fseek(fp_input, inputs_offsets[i], SEEK_SET);
			if (fgets(buff, sizeof buff, fp_input) == NULL){
				finished = 1;
				printf("[%i] Finisde and exiting loop\n", i);
				fclose(fp_input);
				break;
			}
			//printf("[%i] Previous offset: %d\n",rank, inputs_offsets[i]);
			inputs_offsets[i] = ftello(fp_input);
			//printf("[%i] After offset: %d\n", i, inputs_offsets[i]);
			//printf("[%i] Read in %s\n", buff);
			if (!(sscanf(buff, "%f %f %f %f", &times[i], &data[i][0], &data[i][1], &data[i][2]) == 4)){
				printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", i);
				MPI_Finalize();
				return(1);
			}
			//printf("%f %f %f %f\n", &times[i], &data[i][0], &data[i][1], &data[i][2]);
			//printf("%d - %d - %d\n", x, y, z);
			nodal_scalar_data[z][y][x] = sqrt(data[i][0]*data[i][0] + data[i][1] * data[i][1] + data[i][2] * data[i][2]);
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
			fclose(fp_input);
		}
		//At this point I should have all the data for this time step.
		/* Pass the mesh and data to visit_writer. */
		//cd printf("I'm out of the loop!\n");
		if (!finished){
			//printf("I'm in the if statement\n");
			sprintf(outputFileName, "../vis/proc-%03i.%08d.vtk",rank,j);
			//printf("Will be saving to %s\n", outputFileName );
			//printf("%i %i %i\n", NX, NY, nz);
			write_curvilinear_mesh(outputFileName, 1, dims, (float*)pts, nvars, vardims, centering, varnames, vars);
		}
		/*MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();
		return(0);*/
		j++;
	}

	/*for (i=0; i<n_pickpoints; i++){
		fclose(fp_inputs[i]);
	}*/
	
	printf("Maxi magnitude was %lf\n", max_magnitude);
	printf("Max || Min u: %lf || %lf \n", max_u, min_u);
	printf("Max || Min v: %lf || %lf \n", max_v, min_v);
	printf("Max || Min w: %lf || %lf \n", max_w, min_w);
	
	//write out the .visit file. Make sure you have altered tha variable that was counting the timesteps (j in this case)...
	if (rank==0){
		FILE *fp_visit_config;
		fp_visit_config = fopen("../vis/config.visit", "w");
		//NBLOCKS tells visit that every block of 4 ﬁles is related in a single time step
		fprintf(fp_visit_config, "!NBLOCKS %i\n", size);
		for( counter=0; counter < j; counter++ ){
			for(i=0; i < size; i++){
				fprintf(fp_visit_config, "proc-%03i.%08d.vtk\n", i, counter);
			}
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	/* record end time */
	if (rank == 0){
    	end_time = MPI_Wtime();
		printf("time to compute = %g seconds\n", end_time - start_time);
	}
	MPI_Finalize();	
	return(0);	
}
