#ifdef PAR
#include <mpi.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

static FILE *fp = NULL;
static int useBinary = 0;
static int numInColumn = 0;

char* build_string (char *first, char *second) {
	char* both = (char*) malloc((strlen(first) + strlen(second) + 2) * sizeof(char));
	sprintf(both, "%s/%s", first, second);
	//printf("And the string is: %s\n", both);
	return both;
}

static void end_line(void)
{
    if (!useBinary)
    {
        fprintf(fp, "\n");
        numInColumn = 0;
    }
}

static void write_string(const char *str)
{
    fprintf(fp, "%s", str);
}

static void open_file(const char *filename)
{
    char full_filename[1024];
    if (strstr(filename, ".am") != NULL)
    {
        strcpy(full_filename, filename);
    }
    else
    {
        sprintf(full_filename, "%s.am", filename);
    }

    fp = fopen(full_filename, "w+");
}

static void close_file(void)
{
    end_line();
    fclose(fp);
    fp = NULL;
}

static void force_big_endian(unsigned char *bytes)
{
    static int doneTest = 0;
    static int shouldSwap = 0;
    if (!doneTest)
    {
        int tmp1 = 1;
        unsigned char *tmp2 = (unsigned char *) &tmp1;
        if (*tmp2 != 0)
            shouldSwap = 1;
        doneTest = 1;
    }

    if (shouldSwap & useBinary)
    {
        unsigned char tmp = bytes[0];
        bytes[0] = bytes[3];
        bytes[3] = tmp;
        tmp = bytes[1];
        bytes[1] = bytes[2];
        bytes[2] = tmp;
    }
}

static void write_float(float val)
{
    if (useBinary)
    {
        force_big_endian((unsigned char *) &val);
        fwrite(&val, sizeof(float), 1, fp);
    }
    else
    {
        fprintf(fp, "%20.12e ", val);
    }
}

void write_curvilinear_mesh(const char *filename, int ub, int *dims, double *domain_extents, float *pts,
                            int nvars, int *vardim, int *centering,
                            const char * const *varnames, float **vars)
{
    int   i, j, k;
    char  str[512];
    int npts = dims[0]*dims[1]*dims[2];
/*    int ncX = (dims[0] - 1 < 1 ? 1 : dims[0] - 1);
    int ncY = (dims[1] - 1 < 1 ? 1 : dims[1] - 1);
    int ncZ = (dims[2] - 1 < 1 ? 1 : dims[2] - 1);
*/
    int ncX = dims[0];
    int ncY = dims[1];
    int ncZ = dims[2];

    int ncells = ncX*ncY*ncZ;

    useBinary = ub;
    open_file(filename);

    //write_variables(nvars, vardim, centering, varnames, vars, npts, ncells);
    int num_to_write = 0;
    num_to_write = ncells;
    k = 0;
    //printf("ncells = %i; ncX = %i; ncY = %i; ncZ = %i\n", ncells, ncX, ncY, ncZ);
    //We read the first 2k bytes into memory to parse the header.
    //The fixed buffer size looks a bit like a hack, and it is one, but it gets the job done.
    char buffer[2048];
    fread(buffer, sizeof(char), 2047, fp);
    buffer[2047] = '\0'; //The following string routines prefer null-terminated strings

    write_string("# AmiraMesh BINARY-LITTLE-ENDIAN 2.1\n");
    write_string("# CreationDate: Sat Feb 26 00:08:50 2005\n\n");

    //Lattice definition, i.e., the dimensions of the uniform grid
    sprintf(str, "define Lattice %i %i %i\n\n", ncX, ncY, ncZ);
    write_string(str);
 
    write_string("Parameters {\n");
    //write_string("\tExpression \"[ Ax , Ay, Az ]\",\n");
    //write_string("\tBoundingBox -16000 2000 -16000 2000 0 0,\n");
    
    sprintf(str, "\tBoundingBox %f %f %f %f %f %f,\n", domain_extents[0], domain_extents[1], domain_extents[2], domain_extents[3], domain_extents[4], domain_extents[5]);
    write_string(str);
    write_string("\tCoordType \"uniform\"\n");
    write_string("}\n\n");

    write_string("Lattice { float[3] Data } @1\n\n");
    write_string("Lattice { float Data } @2\n\n");
    write_string("Lattice { float Data } @3\n\n");
    write_string("Lattice { float Data } @4\n\n");

    write_string("# Data section follows\n");
    write_string("@1\n");
    useBinary = ub;
    for (j = 0 ; j < num_to_write ; j++) {
    	for (i = 0 ; i < nvars ; i++)
    	{
	      //printf("[%i, %i] = %f", i, j, vars[i][j]);
             //write_float(vars[i][j]);
	     fwrite(&vars[i][j], sizeof(float), 1, fp);
        }
    }
    //Write u
    useBinary = 0;
    write_string("\n@2\n");
    useBinary = ub;
    for (j = 0 ; j < num_to_write ; j++) {
       //write_float(vars[0][j]);
       fwrite(&vars[0][j], sizeof(float), 1, fp);
    }
    //Write u
    useBinary = 0;
    write_string("\n@3\n");
    useBinary = ub;
    for (j = 0 ; j < num_to_write ; j++) {
       //write_float(vars[1][j]);
       fwrite(&vars[1][j], sizeof(float), 1, fp);
    }
    //Write u
    useBinary = 0;
    write_string("\n@4\n");
    useBinary = ub;
    for (j = 0 ; j < num_to_write ; j++) {
       //write_float(vars[2][j]);
       fwrite(&vars[2][j], sizeof(float), 1, fp);
    }
    close_file();
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

	if (size > 1){
		printf("Support for > 1 process is not active yet.\nSorry!\n");
	}
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
	double max_x, max_y, max_z, min_x, min_y, min_z;
	char *root_folder = ".";
	char *output_folder = "../amiramesh/";
	if (argc == 2) { root_folder = argv[1]; }
	
	/*rlp.rlim_cur = 16000;
	rlp.rlim_max = 16000;
	// set the number of open file desriptors to MAX_CONNECTIONS
	if (setrlimit (RLIMIT_NOFILE,&rlp) == -1) {
		perror("setrlimit");
		MPI_Finalize();
		return(1);
	}*/

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
	fgets( buff, sizeof buff, fp_pickpoints );
	double domain_extents[] = { 0, 0, 0, 0, 0, 0 };
	if (sscanf(buff, "%lg %lg %lg %lg %lg %lg", &domain_extents[0], &domain_extents[1], &domain_extents[2], &domain_extents[3], &domain_extents[4], &domain_extents[5]) != 6) {
		printf("Couldn't read the coordinates.\n");
	}	
	printf("%lg %lg %lg %lg %lg %lg\n", domain_extents[0], domain_extents[1], domain_extents[2], domain_extents[3], domain_extents[4], domain_extents[5]);
	fgets( buff, sizeof buff, fp_pickpoints );
	if (sscanf( buff, "%d %d %d", &nx, &ny, &NZ) != 3) {
		printf(	"Couldn't read the number of pickpoints.\n"
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
#ifdef PAR
	if(size > NZ) {
		if(rank == 0)
			printf("You have more processors than layers (nz = %i)!\nSince multiprocessor partitioning works on dividing layers amongs processors... well...\nI'm quitting!\nMake sure np < nz (np < %i, yes.. that's a lower than, not lower or equal than)\n", NZ, NZ);
		MPI_Finalize();
		return(1);
	}
#endif
	//VisIt won't interpolate between 2 zones, therefoere we need to fill in the gap
#ifdef PAR
        nz = NZ / size;
        lowerbound = nz * nx * ny * rank;
        if(rank == size - 1){
                nz = NZ - (nz * (size - 1));
                n_pickpoints = nx * ny * nz;
        } else{
                nz = (NZ / size) + 1;
                n_pickpoints = nz * nx * ny;
        }

        printf("[%i] Will be reading %i pickpoints out of %i.\n", rank, n_pickpoints, total_pickpoints);
        printf("[%i] lowerbound: %i\n", rank, lowerbound);
#else
        nz = NZ;
        n_pickpoints = nz*nx*ny;
#endif
        float pts[n_pickpoints * 3];
        int i, counter;
        i = 0;
        counter = 0;
	printf("%d %d %d %d\n", total_pickpoints, lowerbound, n_pickpoints,nz);
        for(counter = 0; counter < total_pickpoints && (fgets( buff, sizeof buff, fp_pickpoints ) != NULL); counter ++){
//              if ( !sscanf( buff, "%f %f %f", &pts[i], &pts[i++], &pts[i++] ) == 3 ) {
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

//Skip the information at the beginning
	printf("[%i]Skipping information at the beginning\n", rank);
	int j;
	FILE *fp_input;
	for(i=0; i < n_pickpoints; i++) {
		//printf("[%i] skipping for file %i - \n", rank, i);
		fp_input = fopen(build_string(root_folder, inputs_files[i]), "r");
		if (fp_input == NULL) {
			printf("[%i] File was null %s!", rank,  inputs_files[i]);
#ifdef PAR
			MPI_Finalize();
#endif
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
	float data[n_pickpoints][3], u[nz][ny][nx], v[nz][ny][nx], w[nz][ny][nx], uvw[nz][ny][nx][3], times[n_pickpoints];
	int dims[] = {nx, ny, nz};
	int nvars = 3, x=0, y=0, z=0;
	int vardims[] = {1, 1, 1 };
	int centering[] = {1, 1, 1};
	const char *varnames[] = {"u", "v", "w"};
	
	//float *vars[] = {(float *)pts, data, data2};
	float *vars[] = {(float *)u, (float *)v, (float *)w};
	printf("[%i] Starting processing\n", rank);	
	j=0;
	int finished = 0;
	while (!finished){
		x = 0;
		z = 0;
		y = 0;
		
		for (i=0; i<n_pickpoints; i++){
			//printf("[%i] i = %i reading from file: \n", rank, i, inputs_files[i]);
			fp_input = fopen(build_string(root_folder,inputs_files[i]), "r");
			if (fp_input == NULL){
				printf("[%i] Unable to open file %s\n", rank, inputs_files[i]);
			}
			fseek(fp_input, inputs_offsets[i], SEEK_SET);
			if (fgets(buff, sizeof buff, fp_input) == NULL){
				finished = 1;
				printf("[%i] Finished and exiting loop\n", i);
				fclose(fp_input);
				break;
			}
			//printf("[%i] Previous offset: %d\n",rank, inputs_offsets[i]);
			inputs_offsets[i] = ftello(fp_input);
			//printf("[%i] After offset: %d\n", i, inputs_offsets[i]);
			//printf("[%i] Read in %s\n", buff);
			if (!(sscanf(buff, "%f %f %f %f", &times[i], &data[i][0], &data[i][1], &data[i][2]) == 4)){
				printf("Scanf for pickpoint %i didn't return 4.\nExiting.\n", i);
#ifdef PAR
				MPI_Finalize();
#endif
				return(1);
			}
			//printf("%f %f %f %f\n", &times[i], &data[i][0], &data[i][1], &data[i][2]);
			//printf("%d - %d - %d\n", x, y, z);
			//nodal_scalar_data[z][y][x] = sqrt(data[i][0]*data[i][0] + data[i][1] * data[i][1] + data[i][2] * data[i][2]);
			u[z][y][x] = data[i][0];
			v[z][y][x] = data[i][1];
			w[z][y][x] = data[i][2];
			uvw[z][y][x][0] = data[i][0];
			uvw[z][y][x][1] = data[i][1];
			uvw[z][y][x][2] = data[i][2];
			//printf("nodal_scalar_data[%d][%d][%d] = %.10lf\n", z, y, x, nodal_scalar_data[z][y][x] );
			/*if (nodal_scalar_data[z][y][x] > max_magnitude) {
				max_magnitude = nodal_scalar_data[z][y][x];
			}*/
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
			fclose(fp_input);
		}
		//At this point I should have all the data for this time step.
		/* Pass the mesh and data to visit_writer. */
		if (!finished){
			//printf("I'm in the if statement\n");
			sprintf(outputFileName, "%s/%s/step.%08d.am", root_folder, output_folder, j);
			//printf("Will be saving to %s\n", outputFileName );
			//printf("%i %i %i\n", nx, ny, nz);
			write_curvilinear_mesh(outputFileName, 1, dims, domain_extents, (float*)pts, nvars, vardims, centering, varnames, vars);
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

#ifdef PAR
	MPI_Barrier(MPI_COMM_WORLD);
	/* record end time */
	if (rank == 0){
    	end_time = MPI_Wtime();
		printf("time to compute = %g seconds\n", end_time - start_time);
	}
	MPI_Finalize();	
#endif
	return(0);	
}
