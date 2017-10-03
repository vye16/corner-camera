function outfile = saveCornerData(cornerfile, ncorners, input_type, overwrite)
% saves the corner(s) and theta lims for the given input
[folder, name] = fileparts(cornerfile);
outfile = fullfile(folder, sprintf('%s_cornerdata.mat', name));

if ~overwrite && exist(outfile, 'file')
    fprintf('Using corner data in %s\n', outfile);
    return;
end

fprintf('Saving corner data in %s\n', outfile);

switch input_type
case 'video'
    vcal = VideoReader(cornerfile);
    vcal.CurrentTime = 1; % one second in
    calimg = readFrame(vcal);
case 'images'
    calimg = imread(cornerfile); % single calibration image
end

framesize = size(calimg);

%  get the corner(s)
figure; imagesc(calimg(:,:,1));
title(sprintf('Please select %d corner(s)', ncorners));
corner = ginput(ncorners);
hold on; plot(corner(:,1), corner(:,2), 'ro');

% get a point along the edge of the wall for every corner
title('For every corner, please select a point on the base of the wall');
wall_point = ginput(ncorners);
plot(wall_point(:,1), wall_point(:,2), 'bo');

title('For every corner, please select an angular endpoint');
ground_point = ginput(ncorners);
plot(ground_point(:,1), ground_point(:,2), 'yo');

save(outfile);
end