N=length(initial_data);

%%use kde_dct to reproduce matlab
in_data_n=initial_data;
in_data_n=in_data_n/sqrt(2*N);
y1=dct(initial_data);
y2=kde_dct1d(in_data_n);
y2(1)=y2(1)*sqrt(2);
figure(1)
plot(y1,'-r')
hold on
plot(y2,'-b')
hold off

dct_error.y=abs(y1-y2)./abs(y1);
dct_error.mean=mean(dct_error.y);

dct_error.max=max(dct_error.y);

dct_error

figure(2)
plot(dct_error.y)

%% use matlab to reproduce kde_dct
% in_data_n=initial_data*sqrt(2*N);
% z1=dct(in_data_n);
% z1(1)=z1(1)/sqrt(2);
in = fspecial('gaussian',[1 64],2);
%in = initial_data;
z1=kde_dct_with_matlab(in);
z2=kde_dct1d(in);

figure(3)
plot(z1,'-r')
hold on
plot(z2,'-b')
hold off

dct_error.y=abs(z1-z2+eps)./abs(z1+eps);
dct_error.mean=mean(dct_error.y);

dct_error.max=max(dct_error.y);

dct_error

figure(4)
plot(dct_error.y)

%%

%%
%in=[1.8618    1.1547    0.4476];
%in=[0.0545    0.2442    0.4026    0.2442    0.0545]';
in = fspecial('gaussian',[128 1],5);

out_kde_dct=kde_dct1d(in)
out_matlab=kde_dct_with_matlab(in)
norm(out_kde_dct-out_matlab)
figure(4)
plot(out_kde_dct,'-r')
hold on
plot(out_matlab,'-b')
hold off
