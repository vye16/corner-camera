clear; close all;

nsamples = 200;
thetas = linspace(0, pi/2, nsamples);
tdelta = thetas(2) - thetas(1);
tdir = sign(tdelta);

angles = linspace(0, pi/2, nsamples);
rs = 10:10:100;
xx = cos(angles') * rs;
yy = sin(angles') * rs;

xout = xx(:); yout = yy(:);
angles = atan2(yout, xout);

amat = zeros(length(angles), length(thetas)+1);
for i = 1:numel(angles)
    idx = sum((angles(i) - thetas) * tdir >= 0); % idx of largest theta <= angle(i)
    if idx <= 0
        amat(i,1) = 1;
        continue;
    else
        % remember constant term at front
        d = (angles(i) - thetas(idx)) / tdelta;
        amat(i,1:idx) = 1;
        amat(i,idx+1) = 0.5*(2-d)*d + 0.5;
        if idx < size(amat, 2) - 1
            amat(i,idx+2) = 0.5*d^2;
        end
    end
end

params.nsamples = nsamples;
params.rs = rs;
amat_gt = getAmat(params);

figure;
subplot(131); imagesc(amat);
subplot(132); imagesc(amat_gt);
subplot(133); imagesc(amat_gt - amat);
