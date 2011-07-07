#ifdef PAR
#include <mpi.h>
#endif
#include "visit_writer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

char* build_string (char *first, char *second) {
	char* both = (char*) malloc((strlen(first) + strlen(second) + 2) * sizeof(char));
	sprintf(both, "%s/%s", first, second);
	//printf("And the string is: %s\n", both);
	return both;
}

int main(int argc, char *argv[]){
	int rank = 0;
#ifdef PAR
	int size;
	double start_time, end_time;
	/* record start time */
	start_time = MPI_Wtime();
	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
#endif
	struct rlimit rlp;
	float max_magnitude = 0;
	float max_u = 0;
	float max_v = 0;
	float max_w = 0;
	float min_u = 0;
	float min_v = 0;
	float min_w = 0;
	int NZ; //NZ is the total number of layers, read from the pickpoints file.
	int nx, ny, nz=0; //nz is the number of layers assigned to a particular processor.
	int lowerbound=0;
	char *root_folder = "";
	char *output_folder = "../vtks/"; 
	if (argc == 2) { root_folder = argv[1]; }

	//Open the pickpoints file
	int n_pickpoints, total_pickpoints;
	FILE *fp_pickpoints;
	fp_pickpoints=fopen( build_string(root_folder, "/pickpoints.dat"), "r");
	if( fp_pickpoints == NULL ){
		printf("Cannot open pickpoints file\n") ;
		return(1) ;
	} else {
		printf("Opened pickpoints file\n");
	}
	char buff[200];
	//Skip the line with the extents of the domain as we are not interested.
	fgets( buff, sizeof buff, fp_pickpoints );
	//printf("%s", buff);
	fgets( buff, sizeof buff, fp_pickpoints );
	if (sscanf( buff, "%d %d %d", &nx, &ny, &NZ) != 3) {
                printf( "Couldn't read the number of pickpoints.\n"
                        "The format expected for the pickpoints file is:\n"
                        "min_x max_x min_y max_y min_z max_z\n"
                        "n_pickpoints_x n_pickpoints_y n_pickpoints_z\n"
                        "<list of pickpoints coordinates>\n"
                        "<list of pickpoint file names>");
			return 1;
	}
	total_pickpoints=nx*ny*NZ;

	//create the output folder
	mkdir(build_string(root_folder,output_folder), S_IRWXU | S_IRWXG | S_IRWXO);

	rlp.rlim_cur = 16220;
	rlp.rlim_max = 16220;

	//set the number of open file desriptors to MAX_CONNECTIONS
	if (setrlimit (RLIMIT_NOFILE,&rlp) == -1) {
		perror("setrlimit");
#ifdef PAR
		MPI_Finalize();
#endif
		return(1);
	}
#ifdef PAR
	if(size > NZ) {
		if(rank == 0)
			printf("You have more processors than layers (nz = %i)!\nSince multiprocessor partitioning works on dividing layers amongs processors... well...\nI'm quitting!\nMake sure np < nz (np < %i, yes.. that's a lower than, not lower or equal than)\n", NZ, NZ);
		MPI_Finalize();
		return(1);
	}
	//VisIt won't interpolate between 2 time zones, therefoere we need to fill in the gap
	nz = NZ / size;
	lowerbound = nz * nx * ny * rank;
	if(rank == size - 1){
		nz = NZ - (nz * (size - 1));
		n_pickpoints = nx * ny * nz;
	} else{
		nz = (NZ / size) + 1;
		n_pickpoints = nz * nx * ny;
	}
	printf("[%i] lowerbound: %i\n", rank, lowerbound);
#else
	nz = NZ;
	n_pickpoints = nz*nx*ny;
#endif
	printf("[%i] Will be reading %i pickpoints out of %i.\n", rank, n_pickpoints, total_pickpoints);
	float pts[n_pickpoints * 3];
	int i, counter;
	i = 0;
	counter = 0;
	for(counter = 0; counter < total_pickpoints && (fgets( buff, sizeof buff, fp_pickpoints ) != NULL); counter ++){
//		if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3 ) {
		if(counter >= lowerbound && counter < lowerbound + n_pickpoints){
			//printf("Through the if\n");
			if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i+1], &pts[i+2] ) == 3 ) {
				printf("Couldn't read the %d pickpoint. Aborting.\n", i);
#ifdef PAR
				MPI_Finalize();
#endif
				return 1;
			} else {
				//printf("[%i]\t%d\t%f\t%f\t%f\n", rank, i, pts[i], pts[i+1], pts[i+2]);
				i += 3;
			}
		}/* else {
			printf("Failed the if\n");
		}*/
	}
#ifdef PAR
        printf("[%i] read in %i coordinates (shoulde be n_points * 3)\n", rank, i);
#else
        printf("Read in %i coordinates (shoulde be n_points * 3)\n", i);
#endif

//Get the name of the files
	i=0;
	counter = 0;
	int len;
	FILE *fp_inputs[n_pickpoints];
	printf("Getting pickpoint file name information.\n");

	//while ( fgets( buff, sizeof buff, fp_pickpoints ) != NULL && counter++ < total_pickpoints) {
	for(counter = 0; counter < total_pickpoints && (fgets(buff, sizeof buff, fp_pickpoints ) != NULL); counter ++){
		//printf("[%i] counter: %i lowerbound %i\n", rank, counter, lowerbound);
		if(counter >= lowerbound && counter < lowerbound + n_pickpoints){
			//printf("[%i] Got file name %s", rank, inputs_files[i]);
			//remove the \n at the end
			len = strlen(buff);
			if( buff[len-1] == '\n' )
			    buff[len-1] = 0;
			fp_inputs[i] = fopen(build_string(root_folder, buff), "r");
			if (fp_inputs[i] == NULL) {
				printf("[%i] Unable to open file %s in slot %i.\nExiting.\n", rank, buff, i);
				return(1);
			}/* else {
				printf("Opened %s in slot %i.\n", buff, i);
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
	for(i=0; i < n_pickpoints; i++) {
		//printf("[%i] skipping for file %i - \n", rank, i);
		for(j=0; j<5; j++){
			fgets(buff,sizeof buff, fp_inputs[i]);
			//printf("[%i] %i - %s\n", rank, i, buff);
		}
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
	float data[n_pickpoints][3], u[nz][ny][nx], v[nz][ny][nx], w[nz][ny][nx], uvw[nz][ny][nx][3], times[n_pickpoints];
	int dims[] = {nx, ny, nz};
	int nvars = 4, x=0, y=0, z=0;
	int vardims[] = {1, 1, 1, 3};
	int centering[] = {1, 1, 1, 1};
	const char *varnames[] = {"u", "v", "w", "uvw"};

	//float *vars[] = {(float *)pts, data, data2};
	float *vars[] = {(float *)u, (float *)v, (float *)w, (float *)uvw};

	printf("[%i] Starting processing\n", rank);	
	j=0;
	int finished = 0;
	while (!finished){
		x = 0;
		z = 0;
		y = 0;
		
		for (i=0; i<n_pickpoints; i++){
			//printf("[%i] i = %i reading from file.\n", rank, i);
			if (fgets(buff, sizeof buff, fp_inputs[i]) == NULL){
				finished = 1;
				printf("[%i] Finished and exiting loop\n", i);
				break;
			}
			if (!(sscanf(buff, "%f %f %f %f", &times[i], &data[i][0], &data[i][1], &data[i][2]) == 4)){
				printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", i);
#ifdef PAR
				MPI_Finalize();
#endif
				return(1);
			}
			//printf("%f %f %f %f\n", &times[i], &data[i][0], &data[i][1], &data[i][2]);
			//printf("%d - %d - %d\n", x, y, z);
			u[z][y][x] = data[i][0];
			v[z][y][x] = data[i][1];
			w[z][y][x] = data[i][2];
			uvw[z][y][x][0] = data[i][0];
			uvw[z][y][x][1] = data[i][1];
			uvw[z][y][x][2] = data[i][2];

			if (u[z][y][x] > max_u) max_u = u[z][y][x];
			if (u[z][y][x] < min_u) min_u = u[z][y][x];
			if (v[z][y][x] > max_v) max_v = v[z][y][x];
			if (v[z][y][x] < min_v) min_v = v[z][y][x];
			if (w[z][y][x] > max_w) max_w = w[z][y][x];
			if (w[z][y][x] < min_w) min_w = w[z][y][x];
			
			if((++y % ny) == 0){
				x++;
				y=0;
			}	
			if (x == nx) {
				z++;
				x = 0;
			}
		}
		//At this point I should have all the data for this time step.
		/* Pass the mesh and data to visit_writer. */
		//cd printf("I'm out of the loop!\n");
		if (!finished){
			//printf("I'm in the if statement\n");
			sprintf(outputFileName, "%s/%s/proc-%03i.%08d.vtk", root_folder, output_folder, rank,j);
			//printf("Will be saving to %s\n", outputFileName );
			//printf("%i %i %i\n", nx, ny, nz);
			write_curvilinear_mesh(outputFileName, 0, dims, (float*)pts, nvars, vardims, centering, varnames, vars);
		}
		/*MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();
		return(0);*/
		j++;
	}

	/*for (i=0; i<n_pickpoints; i++){
		fclose(fp_inputs[i]);
	}*/
	
	//printf("Maxi magnitude was %lf\n", max_magnitude);
	printf("Max || Min u: %lf || %lf \n", max_u, min_u);
	printf("Max || Min v: %lf || %lf \n", max_v, min_v);
	printf("Max || Min w: %lf || %lf \n", max_w, min_w);
	
#ifdef PAR
	//write out the .visit file. Make sure you have altered tha variable that was counting the timesteps (j in this case)...
	if (rank==0){
		FILE *fp_visit_config;
		fp_visit_config = fopen(build_string(root_folder,build_string(output_folder, "config.visit")), "w");
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
#endif
	for (i=0; i<n_pickpoints; i++){
		fclose(fp_inputs[i]);
	}
	return(0);	
}
