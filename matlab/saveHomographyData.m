function outfile = saveHomographyData(gridfile, input_type, overwrite)
[folder, name] = fileparts(gridfile);
outfile = fullfile(folder, sprintf('%s_homography.mat', name));

if ~overwrite && exist(outfile, 'file')
    fprintf('Using homography data in %s\n', outfile);
    return;
end
fprintf('Saving homography data in %s\n', outfile);

% solve for homography
switch input_type
case 'video'
    vgrid = VideoReader(gridfile);
    vgrid.CurrentTime = 1; % one second in
    gridimg = readFrame(vgrid);
case 'images'
    gridimg = imread(gridfile); % single grid image
end

% get the camera calibration
figure; imagesc(gridimg(:,:,1));
title('Select 4 points on a square counter-clockwise from top left'); 
pts = ginput(4); % first column is x, second is y
hold on; plot(pts(:,1), pts(:,2), 'r');

% solve for the homography that takes you from the rectified square to
% points in the image, and the inverse
pts_ref = [0 0; 0 1; 1 1; 1 0]; 
v = solveHomography(pts_ref', pts');
v_inv = solveHomography(pts', pts_ref');

[h, w, ~] = size(gridimg);
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
rectified = interp2(ii, jj, double(gridimg(:,:,1)), iold, jold);
[h_new, w_new, ~] = size(rectified); % save for later use

save(outfile); % save out
end