clear; close all;

scratchdir = fileparts(mfilename('fullpath'));
frame = im2double(imread(fullfile(scratchdir, 'frame1.png')));

% read the cpp output
xcpp = csvread(fullfile(scratchdir, 'xlocs_sampled.csv'));
ycpp = csvread(fullfile(scratchdir, 'ylocs_sampled.csv'));

cpp_r = csvread(fullfile(scratchdir, 'pts_sampled_red.csv'));
cpp_g = csvread(fullfile(scratchdir, 'pts_sampled_green.csv'));
cpp_b = csvread(fullfile(scratchdir, 'pts_sampled_blue.csv'));

mat_r = interp2(frame(:,:,1), xcpp+1, ycpp+1);
mat_g = interp2(frame(:,:,2), xcpp+1, ycpp+1);
mat_b = interp2(frame(:,:,3), xcpp+1, ycpp+1);

err_r = abs(mat_r - cpp_r)./mat_r;
err_g = abs(mat_g - cpp_g)./mat_g;
err_b = abs(mat_b - cpp_b)./mat_b;

figure;
subplot(311); plot(err_r(:), '*');
subplot(312); plot(err_g(:), '*');
subplot(313); plot(err_b(:), '*');

% corner = [1164.7, 109.2];
% theta_lim = [0.7401, pi];
% 
% cal_datafile = fullfile(scratchdir, 'test_samples_cal.mat');
% save(cal_datafile, 'corner', 'theta_lim');
% 
% params.cal_datafile = cal_datafile;
% params.nsamples = 20;
% params.rs = 30:30:90;
% 
% params = setNoRectObsXYLocs(params);
% 
% mat_r = interp2(frame(:,:,1), params.obs_xlocs, params.obs_ylocs);
% mat_g = interp2(frame(:,:,2), params.obs_xlocs, params.obs_ylocs);
% mat_b = interp2(frame(:,:,3), params.obs_xlocs, params.obs_ylocs);
% 
% figure; title('difference in sample xy locs');
% plot(abs(params.obs_xlocs - xcpp(:)), abs(params.obs_ylocs - ycpp(:)), 'ro');
