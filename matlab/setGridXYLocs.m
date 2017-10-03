function params = setGridXYLocs(params)
% sets the sampling locations to be on a grid on the ground plane

cr = params.corner_r;
grid_step = 2^params.downlevs;

if params.rectify
    [center(1), center(2)] = mapToRectified(params.homography,...
        params.corner(1), params.corner(2));
    if mod(abs(params.theta_lim(1) - pi), pi) < 5 * (pi/180) % within 5 degrees of pi
        xstep = sign(cos(params.theta_lim(1))) * grid_step;
        ystep = sign(sin(params.theta_lim(2))) * grid_step;
    else
        xstep = sign(cos(params.theta_lim(2))) * grid_step;
        ystep = sign(sin(params.theta_lim(1))) * grid_step;
    end
    xmax = params.grid_r * xstep;
    ymax = params.grid_r * ystep;
    [yq, xq] = ndgrid(ystep:ystep:ymax, xstep:xstep:xmax);
    
    params.obs_angles = tangentAngle(xq(:), yq(:),...
        -sign(xstep) * cr, sign(ystep) * cr, cr);
    
    xq = round(xq + center(1));
    yq = round(yq + center(2));

    [xq, yq] = mapFromRectified(params.homography, xq, yq);
else
    center = corner;
    [ii, jj] = ndgrid(1:params.grid_r, 1:params.grid_r);
    step = grid_step * sqrt(2);
    xq = center(1) + step*(ii * cos(params.theta_lim(1))...
        + jj * cos(params.theta_lim(2)));
    yq = center(2) + step*(ii * sin(params.theta_lim(1))...
        + jj * sin(params.theta_lim(2)));
    
    cx = -cos(params.theta_lim(1)) - cos(params.theta_lim(2));
    cy = sin(params.theta_lim(1)) + sin(params.theta_lim(2));
    params.obs_angles = tangentAngle(xq(:) - center(1),...
        yq(:) - center(2), cx, cy, cr);
end

params.obs_angles(params.obs_angles <= 0) = params.theta_lim(2);
params.obs_xlocs = (xq(:));
params.obs_ylocs = (yq(:));
end