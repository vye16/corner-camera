function amat = getAmat(params)
% amat takes hidden 1d scene x to the observed interpolated samples on
% circles around the corner, going from the wall (where none of the scene
% is visible) outward until the entire scene is visible

angles = params.obs_angles;
thetas = linspace(params.theta_lim(1),...
    params.theta_lim(2), params.nsamples);
tdelta = thetas(2) - thetas(1);
tdir = sign(tdelta);

amat = zeros(length(angles), length(thetas)+1);
for i = 1:numel(angles)
    idx = sum((angles(i) - thetas) * tdir >= 0); % idx of largest theta <= angle(i)
    if idx <= 0
        amat(i,1) = 1; % still sees constant light at idx 1
    else % angle indexing starts at 2
        d = (angles(i) - thetas(idx)) / tdelta;
        amat(i,1:idx) = 1;
        amat(i,idx+1) = 0.5*(2-d)*d + 0.5;
        if idx < size(amat, 2) - 1
            amat(i,idx+2) = 0.5*d^2;
        end
    end
end
end
