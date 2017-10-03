function angle = tangentAngle(xq, yq, cx, cy, cr)
% finds the tangent angle from the point (xq, yq) to the circle
% with origin (cx, cy) and radius cr
if cr <= 0
    angle = atan2(yq, xq);
else
    yq = yq - cy;
    xq = xq - cx;
    alpha = atan2(yq, xq);
    d = sqrt(yq.^2 + xq.^2);
    theta = asin(cr ./ d);
    beta = alpha + theta - pi/2;
    angle = atan2(yq - sin(beta), xq - cos(beta));
end
end