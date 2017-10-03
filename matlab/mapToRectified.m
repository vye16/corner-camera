function [xout, yout] = mapToRectified(homography_data, xin, yin)
% xin, yin are coordinates on the original image
% transform them to xout, yout on the rectified image

load(homography_data, 'v_inv', 'maxx', 'minx',...
    'maxy', 'miny', 'h_new', 'w_new');

coords = v_inv * [xin(:)'; yin(:)'; ones(1, numel(xin))];
xout = reshape(coords(1,:) ./ coords(3,:), size(xin));
yout = reshape(coords(2,:) ./ coords(3,:), size(yin));

xout = (xout - minx)/(maxx - minx) * w_new;
yout = (yout - miny)/(maxy - miny) * h_new;
end