function harmonic_fit_extra
    filename1 = 'flute/flute_A4_15_forte_normal.mp3';
    filename2 = 'flute/flute_B4_15_forte_normal.mp3';
    filename3 = 'flute/flute_Cs5_15_forte_normal.mp3';
    filename4 = 'flute/flute_Ds5_15_forte_normal.mp3';
    filename5 = 'flute/flute_F5_15_forte_normal.mp3';
    filename6 = 'flute/flute_G5_15_forte_normal.mp3';
    filename7 = 'flute/flute_A5_15_forte_normal.mp3';
    
    filename8 = 'tuba/tuba_A2_05_forte_normal.mp3';
    filename9 = 'tuba/tuba_B2_05_forte_normal.mp3';
    filename10 = 'tuba/tuba_Cs3_05_forte_normal.mp3';
    filename11 = 'tuba/tuba_Ds3_05_forte_normal.mp3';
    filename12 = 'tuba/tuba_F3_05_forte_normal.mp3';
    filename13 = 'tuba/tuba_G3_05_forte_normal.mp3';
    filename14 = 'tuba/tuba_A3_05_forte_normal.mp3';    
   
    filename15 = 'violin/violin_A4_05_forte_arco-normal.mp3';
    filename16 = 'violin/violin_B4_05_forte_arco-normal.mp3';
    filename17 = 'violin/violin_Cs5_05_forte_arco-normal.mp3';
    filename18 = 'violin/violin_Ds5_05_forte_arco-normal.mp3';
    filename19 = 'violin/violin_F5_05_forte_arco-normal.mp3';
    filename20 = 'violin/violin_G5_05_forte_arco-normal.mp3';
    filename21 = 'violin/violin_A5_05_forte_arco-normal.mp3';
    
    filename22 = 'trumpet/trumpet_A3_05_forte_normal.mp3';
    filename23 = 'trumpet/trumpet_B3_05_forte_normal.mp3';
    filename24 = 'trumpet/trumpet_Cs4_05_forte_normal.mp3';
    filename25 = 'trumpet/trumpet_Ds4_05_forte_normal.mp3';
    filename26 = 'trumpet/trumpet_F4_05_forte_normal.mp3';
    filename27 = 'trumpet/trumpet_G4_05_forte_normal.mp3';
    filename28 = 'trumpet/trumpet_A4_05_forte_normal.mp3';

    n = 8;
    number_of_files = 5;
    A = zeros(number_of_files,n,1);
    ff_array = zeros(number_of_files, 1);
    f1 = 110;
    for i = 1 : number_of_files
        ff_array(i) = f1*(2^(1/12))^(2*(i-1));
    end
    for i = 1 : number_of_files
        flute = i;
        tuba = i + 7;
        violin = tuba + 7;
        trumpet = violin + 7;
        filename = eval(strcat('filename',num2str(trumpet)));       
        [y, Fs] = audioread(filename);
        N = 2^nextpow2(length(y));
        Y = fft(y,N);
        Y = Y*100/max(Y(1:length(Y)/8));
        peaks_and_indexes = get_peaks(Y,Fs,ff_array(i), n);
        peaks_and_indexes*(Fs/N);
        A(i,:) = peaks_and_indexes(:,1); %amps        
    end
    
    degree = 2;
    coefficients = zeros(n,degree+1);
    for i = 1 : n
        coefficients(i,:) = polyfit(ff_array,A(:,i),degree);
    end
    
    
    %{
    TEST, PLOT
    for i = 1 : number_of_files
        flute = i;
        tuba = i + 7;
        violin = tuba + 7;
        trumpet = violin + 7;
        filename = eval(strcat('filename',num2str(flute)));
        [y, Fs] = audioread(filename);
        N = 2^nextpow2(length(y));
        Y = fft(y,N);          
        Y = Y*100/max(Y(1:length(Y)/8));
        peaks_and_indexes = get_peaks(Y,Fs,f1*(2^(1/12))^(2*(i-1)), n);
        figure
        plot((1:length(Y)/8)*(Fs/N), real(Y(1:length(Y)/8)))
        %hold on
        %peaks_and_indexes(1,2)*(Fs/N)
        %scatter(peaks_and_indexes(:,2)*(Fs/N), peaks_and_indexes(:,1))        
        %hold on                     
        for j = 1 : n                        
            fx(j) = polyval(coefficients(j,:),ff_array(i));     
        end
        scatter(peaks_and_indexes(:,2)*(Fs/N), fx)        
        hold on
        %peaks_and_indexes(:,1) = fx;
        %duration = 1;
        %S = generate_signal(y, Fs, peaks_and_indexes, N, duration);
        %sound(S,Fs);
        %pause('on');
        %pause(duration);                
    end
    %}
    ff_array = zeros(4, 1);
    for i = 1 : length(ff_array)        
         ff_array(i) = f1*(2^(1/12))^((i-1));
         fx = zeros(n,1);
         for j = 1 : n                        
            fx(j) = polyval(coefficients(j,:),ff_array(i));     
         end
         frequencies = zeros(n,1);
         for j = 1 : n                        
            frequencies(j) = ff_array(i)*j;
         end
         fx_and_indexes = cat(2, fx, (frequencies/(Fs/N)));
         duration = 1;
        S = generate_signal(y, Fs, fx_and_indexes, N, duration);
        sound(S,Fs);
        pause('on');
        pause(duration); 
    end
    
    for j = 1 : n
        fx(j) = polyval(coefficients(j,:),f1);
    end
    
    scaler = 1/(2*max(fx));
    coefficients*scaler;
    %write_to_file('TRUMPET.txt', coefficients, n, degree, scaler);    
end

function write_to_file(name, coefficients, n, degree, scaler)
    %code to write data to a text
    fileID = fopen(name,'wt');
    for i = 1 : n
       for j = 1 : degree+1 
            fprintf(fileID,'%0.10f',coefficients(i,j)*scaler);      
            if j == degree+1
                fprintf(fileID,',');                      
            else
                fprintf(fileID,' ');      
            end                        
       end
    end
    fclose(fileID);
end

function S = generate_signal(y, Fs, peaks_and_indexes, N, duration)
    S = 0;
    scaler = real(max(y))/max(peaks_and_indexes(:,1));
    T = 1/Fs;             % Sampling period
    L = Fs*duration;             % Length of signal, number of samples
    t = (0:L-1)*T;        % Time vector

    l = length(peaks_and_indexes);
    for j = 1 : l
        amplifier = 4;    
        A = real(peaks_and_indexes(j,1)*scaler*amplifier);
        f = peaks_and_indexes(j,2)*(Fs/N);
        S = S + A*sin(2*pi*f*t);        
    end 
end

function peaks_and_indexes = get_peaks(Y,Fs,ff,n)
    N = length(Y);
    error_margin_freq = 50;
    error_margin = round(error_margin_freq/(Fs/N));
    ff_index = round(ff/(Fs/N));    
    peaks_and_indexes = zeros(n,1);
    %get the local maximum indexes for the next n harmonics
    for i = 1 : n
        peaks_and_indexes(i,2) = real(local_max_index(Y, i*ff_index, error_margin));
        peaks_and_indexes(i,1) = real(Y(peaks_and_indexes(i,2)));
    end 
    
    %figure
    %plot(1:length(Y)/8, Y(1:length(Y)/8))
    %hold on
    %scatter(peaks_and_indexes(:,2), peaks_and_indexes(:,1))
end

function max_index=local_max_index(array, index, margin)
    max_amp = 0;
    max_index = index;
    for i = index-margin : (index+margin)
        if array(i) > max_amp
            max_amp = array(i);
            max_index = i;
        end
    end
end
