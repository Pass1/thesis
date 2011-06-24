home;
disp(' ')
disp('     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
disp('     %%                                                             %%')
disp('     %%                        Gambit2Metis                         %%')
disp('     %%                                                             %%')
disp('     %%          (c) Martin Kaeser, Uni Trento, 22.01.2008          %%')
disp('     %%                                                             %%')
disp('     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
disp('     %%                                                             %%')
disp('     %% Gambit2Metis converts a GAMBIT-meshfile of                  %%')
disp('     %% triangles, quadrilaterals, tetrahedrons or hexahedrons      %%')
disp('     %% stored as "filename.neu"                                    %%')
disp('     %% into a METIS-file and calls METIS for mesh partitioning.    %%')
disp('     %% METIS mesh partitioner:                                     %%')
disp('     %% http://www-users.cs.umn.edu/~karypis/metis/metis/index.html %%')
disp('     %%                                                             %%')
disp('     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
disp(' '),disp(' ')
clear, close all;
filename = input('     Specify GAMBIT-filename:   ','s');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%        Read Gambit Data 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
t = cputime;

fid   = fopen([filename,'.neu']);
    disp(' ')
    disp('-----------------------------------------------------------------------------------')
    disp(sprintf('\t Reading data from: \t\t%s',[filename,'.neu']));
    for i= 1:6
        junk  = fgetl(fid);
    end
    tmp    = fscanf(fid,'%i',[5,1]); 
    NUMNP  = tmp(1);                     %Number of Vertices
    NELEM  = tmp(2);                     %Number of Elements 
    %NGRPS  = tmp(3);                    %Number of Element Groups
    %NBSETS = tmp(4);                    %Number of Boundary Condition Sets
    NDFCD  = tmp(5);                     %Number of Dimensions (2 or 3)

    switch NDFCD                         %Choose between 2-D or 3-D mesh input
        case 2     %2D-meshes
            disp(sprintf('\t Reading \t\t\t\t\t%i vertices',NUMNP));
            disp(sprintf('\t Reading \t\t\t\t\t%i elements',NELEM));
            junk  = fgetl(fid); junk  = fgetl(fid); junk  = fgetl(fid);
            X     = fscanf(fid,'%g',[3,NUMNP]);                            %Read Vertices
            junk  = fgetl(fid); junk  = fgetl(fid); junk  = fgetl(fid);
            %
            first        = fscanf(fid,'%i',[3,1]);
            element_type = first(2);
            
            switch element_type                                            %Read Elements
                case 3  %triangular mesh
                    elem1 = fscanf(fid,'%i',[3,1]);
                    tri   = fscanf(fid,'%i',[6,NELEM-1]);
                    tri   = [elem1, tri(4:6,:)];
                    disp(sprintf('\t Read all triangles successfully!'));

                case 2  %quadrilateral mesh
                    elem1 = fscanf(fid,'%i',[4,1]);
                    quad  = fscanf(fid,'%i',[7,NELEM-1]);
                    quad  = [elem1, quad(4:7,:)];
                    disp(sprintf('\t Read all quadrilaterals successfully!'));
            end
                        
        case 3     %3D-meshes
            disp(sprintf('\t Reading \t\t\t\t\t%i vertices',NUMNP));
            disp(sprintf('\t Reading \t\t\t\t\t%i tetrahedrons',NELEM));
            junk  = fgetl(fid); junk  = fgetl(fid); junk  = fgetl(fid);
            X     = fscanf(fid,'%g',[4,NUMNP]);                            %Read Vertices
            junk  = fgetl(fid); junk  = fgetl(fid); junk  = fgetl(fid);
            %
            first        = fscanf(fid,'%i',[3,1]);
            element_type = first(2);
            
            switch element_type                                            %Read Elements
                case 6  %tetrahedral mesh
                    elem1 = fscanf(fid,'%i',[4,1]);
                    tetra = fscanf(fid,'%i',[7,NELEM-1]);
                    tetra = [elem1, tetra(4:7,:)];
                    disp(sprintf('\t Read all tetrahedrons successfully!'));

                case 4  %hexahedral mesh
                    elem1 = fscanf(fid,'%i',[8,1]);
                    hexa  = fscanf(fid,'%i',[11,NELEM-1]);
                    hexa  = [elem1, hexa(4:11,:)];
                    disp(sprintf('\t Read all hexahedrons successfully!'));
            end
    
        otherwise
            disp(sprintf('\n\t ##ERROR: The GAMBIT input file shows a problem\n\n ##'))
            return;
            
    end
fclose(fid);
disp('-----------------------------------------------------------------------------------')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%        Writing Metis Data 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp(sprintf('\t Write METIS-file: \t\t\t%s',[filename,'.met']));

fid = fopen([filename,'.met'],'w');
    switch NDFCD
        case 2
           switch element_type
               case 3
                  fprintf(fid,'%12i %5i \n',NELEM,1);
                  fprintf(fid,'%12i %12i %12i \n',tri);
               case 2
                  fprintf(fid,'%12i %5i \n',NELEM,4);
                  fprintf(fid,'%12i %12i %12i %12i\n',quad);
           end
            
        case 3
            switch element_type
               case 6
                  fprintf(fid,'%12i %5i \n',NELEM,2);
                  fprintf(fid,'%12i %12i %12i %12i \n',tetra);
               case 4
                  fprintf(fid,'%12i %5i \n',NELEM,3);
                  fprintf(fid,'%12i %12i %12i %12i %12i %12i %12i %12i\n',hexa);
            end
    end
fclose(fid);
disp('-----------------------------------------------------------------------------------')
disp(sprintf('\n\t Conversion finished successfully!  (%g CPU sec)\n',cputime-t));
disp('-----------------------------------------------------------------------------------')
disp(' ')
pause(0.2);
proc = input('     Specify number of processor of the partition:   ','s');
disp('-----------------------------------------------------------------------------------')
disp(sprintf('\n\t METIS partition starts!\n'));
disp('-----------------------------------------------------------------------------------')
disp(' ')
%eval(['!partnmesh ',filename,num2str(i),'.met ',proc])
eval(['!./mesh2dual ',filename,'.met'])
eval(['!./pmetis ',filename,'.met.dgraph ',proc])
pause(0.5);
%clean up of files
eval(['!rm ',filename,'.met'])
eval(['!rm ',filename,'.met.dgraph'])    
eval(['!mv ',filename,'.met.dgraph.part.',proc,'  ',filename,'.met.epart.',proc])

disp('-----------------------------------------------------------------------------------')
disp(sprintf('\t METIS partition done!'));
disp('-----------------------------------------------------------------------------------')
disp(' ')
disp('-----------------------------------------------------------------------------------')
disp(sprintf('\t Wrote final METIS-file: \t\t%s',[filename,'.met.epart.',num2str(proc)]));
disp('-----------------------------------------------------------------------------------')
disp(' ')

