function params = setRaysXYLocs(params)
% get sample x and y locations from the image
% we sample from the unrectified image

if size(params.rs, 1) ~= 1 % not a row vector
    params.rs = params.rs';
end

if params.rectify
    [center(1), center(2)] = mapToRectified(params.homography,...
        params.corner(1), params.corner(2));
else
    center = corner;
end

% circles in the rectified image
angles = linspace(params.theta_lim(1),...
    params.theta_lim(2), params.nsamples);
xq = center(1) + cos(angles') * params.rs;
yq = center(2) + sin(angles') * params.rs;
params.obs_angles = atan2(yq(:) - center(2), xq(:) - center(1));

if params.rectify
    [xq, yq] = mapFromRectified(params.homography, xq, yq);
end
params.obs_angles(params.obs_angles <= 0) = params.theta_lim(2);
% params.obs_xlocs = xq(:);
% params.obs_ylocs = yq(:);
params.obs_xlocs = xq;
params.obs_ylocs = yq;
end
