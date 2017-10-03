datafolder = '/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/testvideos_Mar04';
% datafolder = '/data/vision/billf/shadowImaging/edgeImaging/data/testvideos_Mar04';

expfolder = fullfile(datafolder, 'experiments');
resfolder = fullfile(datafolder, 'results2');

name = 'blue_randomwalking1';
input_type = 'video';
vidtype = '.MP4';
srcfile = fullfile(expfolder, strcat(name, vidtype));

cal_imfile = fullfile(expfolder, strcat('calibrationgrid', vidtype));
vcal = VideoReader(cal_imfile);
vcal.CurrentTime = 1; % one second in
calimg = readFrame(vcal);

%% find the transformation
figure; imagesc(calimg(:,:,1));
title('Select 4 points on a square counter-clockwise from top left'); 
pts = ginput(4); % first column is x, second is y
hold on; plot(pts(:,1), pts(:,2), 'r');

% solve for the homography that takes you from the rectified square to
% points in the image, and the inverse
pts_ref = [0 0; 0 1; 1 1; 1 0]; 
v = solveHomography.m(pts_ref', pts');
v_inv = solveHomography.m(pts', pts_ref');


%% find the corresponding coordinates
[h, w, ~] = size(calimg);
sz = min(h,w)/3; 

tl = v_inv * [1; 1; 1]; tl = tl./tl(3);
bl = v_inv * [1; h; 1]; bl = bl./bl(3);
tr = v_inv * [w; 1; 1]; tr = tr./tr(3);
br = v_inv * [w; h; 1]; br = br./br(3);
minx = min([tl(1), bl(1), tr(1), br(1)]); 
maxx = max([tl(1), bl(1), tr(1), br(1)]); 
miny = min([tl(2), bl(2), tr(2), br(2)]); 
maxy = max([tl(2), bl(2), tr(2), br(2)]);

hvec = linspace(miny, maxy, round((maxy - miny)*sz)); 
wvec = linspace(minx, maxx, round((maxx - minx)*sz)); 
warning('this is rounded to be a square - alternatively we can set hvec and wvec to be the same'); 

[jjj, iii] = ndgrid(hvec, wvec);

cord = v * [iii(:)'; jjj(:)'; ones(1,numel(iii))];
iold = reshape( cord(1,:)./cord(3,:), size(iii)); 
jold = reshape( cord(2,:)./cord(3,:), size(jjj)); 

[jj, ii] = ndgrid(1:h, 1:w);
rectified = interp2(ii, jj, double(calimg(:,:,1)), iold, jold);


%% select the corner in the rectified image and transform it back
figure; imagesc(rectified);
title('Please select the corner');
corner = ginput(1); hold on; plot(corner(1), corner(2), 'ro');

corner_loc = corner ./ size(rectified);
corner_loc = corner_loc .* [maxx - minx, maxy - miny] + [minx, miny];

orig_corner = v * [corner_loc'; 1];
orig_corner = orig_corner ./ orig_corner(3);

figure; subplot(121); imagesc(calimg(:,:,1));
hold on; plot(orig_corner(1), orig_corner(2), 'bo');
subplot(122); imagesc(rectified);
hold on; plot(corner(1), corner(2), 'ro');


%% now select the corner in the original image and rectify
figure; imagesc(calimg(:,:,1));
corner1 = ginput(1); hold on; plot(corner1(1), corner1(2), 'bo');

corner_rect = v_inv * [corner1'; 1];
corner_rect = corner_rect ./ corner_rect(3);
corner_rect = corner_rect(1:2)';
corner_rect = (corner_rect - [minx, miny])./[maxx - minx, maxy - miny];
corner_rect = corner_rect .* [size(rectified, 1), size(rectified, 2)];

figure; subplot(121); imagesc(rectified);
hold on; plot(corner_rect(1), corner_rect(2), 'ro');
subplot(122); imagesc(calimg(:,:,1));
hold on; plot(corner1(1), corner1(2), 'bo');


%% circles around the rectified corner here
nsamples = 100;
rs = 20:20:200;
theta_lim = [pi/2, pi];
angles = linspace(theta_lim(1), theta_lim(2), nsamples)';
xq = corner_rect(1) + cos(angles) * rs;
yq = corner_rect(2) + sin(angles) * rs;

% transform it back
xu = xq / size(rectified,1) * (maxx - minx) + minx;
yu = yq / size(rectified,2) * (maxy - miny) + miny;

coord_back = v * [xu(:)'; yu(:)'; ones(1, numel(xu))];
x0 = reshape(coord_back(1,:)./coord_back(3,:), size(xu));
y0 = reshape(coord_back(2,:)./coord_back(3,:), size(yu));

figure; subplot(121); imagesc(rectified);
hold on; plot(corner_rect(1), corner_rect(2), 'ro'); plot(xq, yq);
subplot(122); imagesc(calimg(:,:,1));
hold on; plot(corner1(1), corner1(2), 'bo'); plot(x0, y0);
