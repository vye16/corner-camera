function params = setEvenArcXYLocs(params)
% samples divided by same arc length along circles,
% for every concentric circle

tdir = sign(params.theta_lim(2) - params.theta_lim(1));

if size(params.rs, 1) ~= 1 % not a row vector
    params.rs = params.rs';
end

if params.rectify
    [center(1), center(2)] = mapToRectified(params.homography,...
        params.corner(1), params.corner(2));
else
    center = corner;
end

res = params.arc_res;
angles = cell(size(params.rs));
xq = cell(size(angles));
yq = cell(size(angles));
for i = 1:length(params.rs)
    r = params.rs(i);
    nangles = round((params.theta_lim(2) - params.theta_lim(1))*r*tdir / res);
    angles{i} = linspace(params.theta_lim(1), params.theta_lim(2), nangles);
    xq{i} = center(1) + r * cos(angles{i});
    yq{i} = center(2) + r * sin(angles{i});
end
params.obs_angles = cell2mat(angles);

xq = cell2mat(xq);
yq = cell2mat(yq);
if params.rectify
    [xq, yq] = mapFromRectified(params.homography, xq, yq);
end

params.obs_angles(params.obs_angles <= 0) = theta_lim(2);
params.obs_xlocs = xq(:);
params.obs_ylocs = yq(:);
end
