clear; close all;


datafolder = '/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/';
% datafolder = '/data/vision/billf/shadowImaging/edgeImaging/data/';

expname = 'testvideos_Jan22';

expfolder = fullfile(datafolder, expname, 'experiments');
resfolder = fullfile(datafolder, expname, 'results');

name = 'redblue_dark';
calname = 'grid_light';
input_type = 'video';
vidtype = '.MOV';
srcfile = fullfile(expfolder, strcat(name, vidtype));
cal_file = fullfile(expfolder, strcat(calname, vidtype));

overwrite = 0;
cal_datafile = fullfile(expfolder, 'calibration_data.mat');
saveCalData(cal_file, input_type, cal_datafile, overwrite);

overwrite = 0;
mean_datafile = fullfile(expfolder,...
    sprintf('%s_mean_vidframe.mat', name));
saveMeanImage(srcfile, input_type, mean_datafile, overwrite);

% set parameters
params = getInputProperties(srcfile, input_type);
params.cal_datafile = cal_datafile;
params.mean_datafile = mean_datafile;

params.startframe = ceil(params.frame_rate * 2);
params.step = round(params.frame_rate/12);
params.endframe = ceil(params.frame_rate * 22);

params.beta = 1/0.2^2;
params.lambda = 15;
params.nsamples = 150;

rmin = 10;
rstep = 10;
rmax = 120;
rs = rmin:rstep:rmax;

load(params.cal_datafile, 'rectified', 'calimg',...
    'corner_rect', 'theta_lim');

% angles of the reconstruction
thetas = linspace(theta_lim(1), theta_lim(2), params.nsamples);
tdelta = thetas(2) - thetas(1);
tdir = sign(tdelta);

res = 1;
angles = cell(size(rs));
xq = cell(size(rs));
yq = cell(size(rs));
for i = 1:length(rs)
    nangles = round((theta_lim(2) - theta_lim(1))*rs(i)*tdir / res);
    angles{i} = linspace(theta_lim(1), theta_lim(2), nangles);
    xq{i} = corner_rect(1) + rs(i) * cos(angles{i});
    yq{i} = corner_rect(2) + rs(i) * sin(angles{i});
end
angles = cell2mat(angles);
xq = cell2mat(xq);
yq = cell2mat(yq);
[xout, yout] = mapFromRectified(params.cal_datafile, xq, yq);

figure;
imagesc(rectified(:,:,1)); hold on;
plot(xq, yq, 'r.');

figure;
imagesc(calimg(:,:,1)); hold on;
plot(xout, yout, 'r.');

amat = zeros(length(xout), length(thetas));
for i = 1:numel(angles)
    idx = sum((angles(i) - thetas) * tdir >= 0); % idx of largest theta <= angle(i)
    if idx <= 0
        continue;
    else
        d = (angles(i) - thetas(idx)) / tdelta;
        amat(i,1:idx-1) = 1;
        amat(i,idx) = 0.5*(2-d)*d + 0.5;
        if idx < size(amat, 2)
            amat(i,idx+1) = 0.5*d^2;
        end
    end
end
