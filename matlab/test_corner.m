clear; close all;

% TODO: INPUT PATH TO TEST VIDEOS
datafolder = '/path/to/example_videos/';

% TODO: CHOOSE WHICH VIDEOS TO RUN
addpath('example_params')

% select the videos to process below
indoor_loc1
%indoor_loc2
%outdoor_concrete
%outdoor_bricks

debug = 1;

expfolder = fullfile(datafolder, expname);

% TODO: SET RESULT DIRECTORY
resfolder = fullfile(datafolder, expname, 'results');

gridfile = fullfile(expfolder, strcat(calname, vidtype));
cornerfile = fullfile(expfolder, strcat(names{1}, vidtype));

% number of corners we're looking at
ncorners = 1;
params.corner_idx = 1;

params.rectify = 1;
% method used to sample observations from input video
% choose from grid, rays, even_arc
% grid: sample in a square grid, around the corner
% rays: sample in rays emanating from the corner, at specified radii
% even_arc: sample in cirlces around the corner, at evenly spaced arc lengths
params.sampling = 'rays';
params.online = 1; % 1 if online stream
params.sub_mean = 1; % subtract the mean as an approximation of albedo

params.beta = 1/0.085^2;

params.filter_width = 5;
params.downlevs = 2;
params.corner_r = 0;

params.nsamples = 200; % size of our 1D reconstruction

% if we sample along rays emanating from corner
rstep = 8; 
ncircles = 50;
params.rs = rstep:rstep:rstep*ncircles; % the circle radii we sample at

% if we sample in evenly spaced arcs around the corner
params.arc_res = 1; % the spacing (in pixels) between samples

% if we sample in a grid, 
params.grid_r = 60; % the size of the grid we sample

start_time = 2; % in seconds
step = 6;
end_time = 22;


test_run
