function [xout, yout] = mapFromRectified(homography_data, xin, yin)
% xin, yin are coordinates on the rectified image
% transform to coordinates xout, yout on the original image
load(homography_data, 'v', 'maxx', 'minx',...
    'maxy', 'miny', 'h_new', 'w_new');

xin = xin / w_new * (maxx - minx) + minx;
yin = yin / h_new * (maxy - miny) + miny;

coord_back = v * [xin(:)'; yin(:)'; ones(1, numel(xin))];
xout = reshape(coord_back(1,:) ./ coord_back(3,:), size(xin));
yout = reshape(coord_back(2,:) ./ coord_back(3,:), size(yin));
end