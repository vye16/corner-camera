function params = setObsXYLocs(params)
% set sampling locations, depending on user-provided sampling method

% set corner and theta limits to calculate sampling locations
load(params.corner_data, 'corner', 'wall_point',...
    'ground_point', 'framesize');
corner = corner(params.corner_idx,:); %#ok<*NODEF>
wall_pt = wall_point(params.corner_idx,:);
wall_vec = wall_pt - corner;
ground_pt = ground_point(params.corner_idx,:);
ground_vec = ground_pt - corner;

if params.rectify
    [cx, cy] = mapToRectified(params.homography, corner(1), corner(2));
    [wx, wy] = mapToRectified(params.homography, wall_pt(1), wall_pt(2));
    [gx, gy] = mapToRectified(params.homography, ground_pt(1), ground_pt(2));
    wall_angle = mod(atan2(wy - cy, wx - cx), 2*pi);
    wall_angle = mod(round(wall_angle * 2/pi) * pi/2, 2*pi);
    ground_angle = mod(atan2(gy - cy, gx - cx), 2*pi);
    ground_angle = mod(round(ground_angle * 2/pi)*pi/2, 2*pi);
else
    wall_angle = mod(atan2(wall_vec(:,2), wall_vec(:,1)), 2*pi);
    ground_angle = mod(atan2(ground_vec(:,2), ground_vec(:,1)), 2*pi);
end

diff_angle = ground_angle - wall_angle;
if abs(diff_angle) < pi
    end_angle = wall_angle + diff_angle;
else
    diff_angle = 2*pi - abs(diff_angle);
    end_angle = wall_angle - sign(diff_angle) * diff_angle;
end

theta_lim = [wall_angle, end_angle];

fprintf('corner located at %.2f, %.2f\n', corner(1), corner(2));
fprintf('theta limits set to %.2f, %.2f\n', theta_lim(1), theta_lim(2));

params.corner = corner;
params.theta_lim = theta_lim;
params.framesize = framesize;

switch params.sampling
    case 'rays'
        params = setRaysXYLocs(params);
    case 'even_arc'
        params = setEvenArcXYLocs(params);
    case 'grid'
        params = setGridXYLocs(params);
    otherwise
        error('unsupported sampling scheme');
end

% adjusting for preprocessing (crop, blur and downsample)
pad = params.filter_width * 2^(params.downlevs+1);
xmin = max(1, floor(min(params.obs_xlocs(:))) - pad);
xmax = min(framesize(2), ceil(max(params.obs_xlocs(:))) + pad);
ymin = max(1, floor(min(params.obs_ylocs(:))) - pad);
ymax = min(framesize(1), ceil(max(params.obs_ylocs(:))) + pad);
params.xrange = [xmin, xmax];
params.yrange = [ymin, ymax];

params.obs_xlocs = (params.obs_xlocs - xmin)/(2^params.downlevs)+1;
params.obs_ylocs = (params.obs_ylocs - ymin)/(2^params.downlevs)+1;
end
