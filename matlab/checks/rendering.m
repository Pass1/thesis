root = '/home/passone/Documents/directed/raw/christian/';
pickpoints = dir(strcat(root, '*.dat'));

i=1;
%We are interested on all the points that lie on the plane y = 0.
for fp = 1:size(pickpoints,1)
    data(i) = importdata(strcat(root, pickpoints(fp).name), ' ', 5);
    tmp = sscanf(char(data(i).textdata(4)), '%s %s %e');
%    if (tmp(4) == 0)
        i = i+1;
%    end
end

% for fp = 1:size(pickpoints,1)
%      data(i) = importdata(strcat(root, pickpoints(fp).name), ' ', 5);
%      tmp = sscanf(char(data(i).textdata(4)), '%s %s %e');
%      if (tmp(4) == 0)
%          bps_data(i,:,:) = importdata(strcat(root, int2str(fp), '.bps'), ' ');
%          i = i+1;
%      end
% end


scale = 10000;
%chose the component u = 2; v = 3 and w = 4
comp = 2;

position = zeros(size(data,2), 3);
cdata = zeros(size(data,2),3);
%get x,y,z coordinates and get colour data
for i = 1:size(data,2)
    for j = 1:3
        tmp = sscanf(char(data(i).textdata(2+j)), '%s %s %e');
        position(i,j) = tmp(4);
    end
    cdata(i,1:3) = [1 1 1];
    %bps_cdata(i, 1:3) = [1 1 1];
end

%ht = scatter3(position(:,1), position(:, 2), position(:, 3), 80, cdata, 'filled' );
ht = scatter(position(:,1), position(:, 3), 80, cdata, 'filled' );
set(ht, 'CDataSource', 'cdata');
%polot the divders
%line([0, 0],[-0.1, -20000], 'Color', [0 0 0]);
%line([-5000, 5000], [-10000, -10000], 'Color', [0 0 0]);

for c = 1:500
    for i = 1:size(data,2)
        if data(i).data(c,comp) > 0
            if (data(i).data(c,comp) * scale < 1)
                cdata(i,1:3) = [1 1-(data(i).data(c,comp) * scale) 1-(data(i).data(c,comp) * scale)];
            else
                cdata(i,1:3) = [1 0 0];
            end
        end
        if data(i).data(c,comp) < 0
            if (data(i).data(c,comp) * scale > -1)
                cdata(i,1:3) = [1+(data(i).data(c,comp) * scale) 1 1+(data(i).data(c,comp) * scale)];
            else
                cdata(i,1:3) = [0 1 0];
            end
        end
        if data(i).data(c,comp) == 0
            cdata(i,1:3) = [1 1 1];
        end
        %cdata(i,1:3) = abs(data(i).data(c,2:4).*10);
    end
    refreshdata
    drawnow
end

% for c = 1:500
%     for i = 1:size(bps_data,1)
%         if bps_data(i,c,comp) > 0
%             if (bps_data(i,c,comp) * scale < 1)
%                 cdata(i,1:3) = [1 1-(bps_data(i,c,comp) * scale) 1-(bps_data(i,c,comp) * scale)];
%             else
%                 cdata(i,1:3) = [1 0 0];
%             end
%         end
%         if bps_data(i,c,comp) < 0
%             if (bps_data(i,c,comp) * scale > -1)
%                 cdata(i,1:3) = [1+(bps_data(i,c,comp) * scale) 1 1+(bps_data(i,c,comp) * scale)];
%             else
%                 cdata(i,1:3) = [0 1 0];
%             end
%         end
%         if bps_data(i,c,comp) == 0
%             cdata(i,1:3) = [1 1 1];
%         end
%     end
%     refreshdata
%     drawnow
%     M(c) = getframe;
% end






% figure(2)
% set(gca, 'XDir', 'reverse');
% set(gca, 'YDir', 'reverse');
% x = [-1250 1250 -1250 1250 -1250 1250 -1250 1250 -2500 2500 -2500 2500 -2500 2500 -2500 2500];
% y = [-1250 -1250 1250 1250 -1250 -1250 1250 1250 -2500 -2500 2500 2500 -2500 -2500 2500 2500];
% z = [-7500 -7500 -7500 -7500 -12500 -12500 -12500 -12500 -5000 -5000 -5000 -5000 -15000 -15000 -15000 -15000];
% C = [0 0 0; 0 0 1; 0 1 0; 0 1 1; 1 0 0; 1 0 1; 1 1 0; 0.5 0.5 0.5; 0 0 0; 0 0 1; 0 1 0; 0 1 1; 1 0 0; 1 0 1; 1 1 0; 0.5 0.5 0.5];
% 
% scatter3(x,y ,z,80, C, 'filled');
% hold on
% text(0, 0, -10000, 'X', 'FontSize', 18, 'HorizontalAlignment','center');
% line([-2500, 0],[-2500, 0], [-5000, -10000], 'Color', [0 0 0]);
% line([2500, 0],[-2500, 0], [-5000, -10000], 'Color', [0 0 1]);
% line([-2500, 0],[2500, 0], [-5000, -10000], 'Color', [0 1 0]);
% line([2500, 0],[2500, 0], [-5000, -10000], 'Color', [0 1 1]);
% line([-2500, 0],[-2500, 0], [-15000, -10000], 'Color', [1 0 0]);
% line([2500, 0],[-2500, 0], [-15000, -10000], 'Color', [1 0 1]);
% line([-2500, 0],[2500, 0], [-15000, -10000], 'Color', [1 1 0]);
% line([2500, 0],[2500, 0], [-15000, -10000], 'Color', [0.5 0.5 0.5]);
% 
% %draw the cube
% %front and top pannel connected by 1 line
% x = [-2500  2500  2500 -2500 -2500 -2500   2500   2500  -2500  -2500  ];
% y = [-2500 -2500  2500  2500 -2500 -2500  -2500   2500   2500  -2500  ];
% z = [-5000 -5000 -5000 -5000 -5000 -15000 -15000 -15000 -15000 -15000 ];
% line(x,y,z, 'Color', [0 0.5 0.5]);
% line([-2500 -2500], [2500 2500], [-5000 -15000], 'Color', [0 0.5 0.5]);
% line([2500 2500], [2500 2500], [-5000 -15000], 'Color', [0 0.5 0.5]);
% line([2500 2500], [-2500 -2500], [-5000 -15000], 'Color', [0 0.5 0.5]);
% 
% xlabel('x');
% ylabel('y');
% zlabel('z');
% title('Reference grid for point orientation');
% 
% 
% % title('U for pickpoint 03898')
% % 
% % title('U for pickpoint 04240')
% % 
% % title('V for pickpoint 03898')
% % 
% % title('V for pickpoint 04240')
% % 
% % title('W for pickpoint 03898')
% % 
% % title('W for pickpoint 04240')