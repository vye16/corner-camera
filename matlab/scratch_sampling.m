% clear; close all; 


test = 0;
switch test
    case 0
        [xx, yy] = meshgrid(1:100,1:100); 
        angles = atan(yy./xx);
    case 1
        angles = linspace(0, pi/2, 200);
        rs = 10:10:100;
        xx = rs' * cos(angles);
        yy = rs' * sin(angles);
    case 2
        sector_res = 1;
        rs = 10:10:100;
        angles = cell(size(rs));
        xx = cell(size(rs));
        yy = cell(size(rs));
        for i = 1:length(rs)
            angles{i} = 0:sector_res/rs(i):pi/2;
            xx{i} = rs(i) * cos(angles{i});
            yy{i} = rs(i) * sin(angles{i});
        end
        angles = cell2mat(angles);
        xx = cell2mat(xx);
        yy = cell2mat(yy);
end

potangles = linspace(0,pi/2,200); 
valsinangle = zeros(size(potangles));
for i=1:length(potangles)
valsinangle(i) = sum(angles(:)<=potangles(i)); 
end
figure; subplot(211); plot(valsinangle)
ylabel('number of samples');
xlabel('angular position');
subplot(212); plot(diff(valsinangle))
ylabel('difference in number of samples');
xlabel('angular position');
% figure; plot(xx, yy, '.'); axis equal;