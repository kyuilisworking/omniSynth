
function fft_and_ifft
    %import mp3 files 
    %filename = 'violin_A4_15_fortissimo_arco-normal.mp3';
    %filename = 'flute_A4_05_piano_normal.mp3';
    filename = 'trumpet_A4_15_fortissimo_normal.mp3';
    %filename = 'tuba_A2_15_forte_normal.mp3';
    %filename = 'cello_A3_1_forte_arco-normal.mp3';
    %filename = 'french-horn_A4_1_mezzo-forte_normal.mp3';
    %filename = 'saxophone_A4_1_forte_normal.mp3';    
    
    [y, Fs] = audioread(filename);
    N = 2^nextpow2(length(y));
    Y = fft(y,N);
    
    %Plot
    f = (1:N) * (Fs/N);
    plot(f(1:length(Y)/5), Y(1:length(Y)/5))

    %get peaks
    peaks = get_peaks(Y,Fs);
    n = length(peaks);
    
    %create new signal
    Y2 = zeros(length(Y), 1);
    
    for i = 2 : n
        peaks(i,2);
        Y2(peaks(i,2)) = peaks(i,1)*30;
    end
    %sound(y,Fs);
    %sound(real(ifft(Y2)), Fs);
end

function peaks = get_peaks(Y,Fs)
    N = length(Y);
    n=30;
    f_threshold = 100;
    hz_limit = 20000;
    m = round(f_threshold/(Fs/N));
    f = (1:N) * (Fs/N);
    limit = round(N*hz_limit/f(N));
    f_array = Y(1:limit);

    %Define matrix of max values
    len = round((length(f_array)/m)-1);
    max_matrix = zeros(len,2);

    %get the local peaks out of m element parts
    for i = 1 : len
       for j = 1 : m
           ind = (i-1)*m+j;
           if f_array(ind) > max_matrix(i,1)
               if (ind > 1) && (ind < length(f_array))
                   if f_array(ind) > f_array(ind-1) && f_array(ind) > f_array(ind+1)                 
                       max_matrix(i, 1) = f_array(ind);
                       max_matrix(i, 2) = ind;
                   end
               end       
           end
       end
    end
    max_matrix = sortrows(max_matrix, 2);
    l = length(max_matrix);
    %filter the values lower than the freq threshold
    for i = 1 : l
        if i > 1
            if abs(max_matrix(i,2) - max_matrix(i-1,2)) < m
                if max_matrix(i,1) > max_matrix(i-1,1)                
                    max_matrix(i-1,2) = 0;
                    max_matrix(i-1,1) = 0;
                else
                    max_matrix(i,2) = 0;
                    max_matrix(i,1) = 0;
                end
            end
        end
    end

    %sort the peaks according to first column
    max_matrix = sortrows(max_matrix, 1);

    %get the last n rows
    max_matrix_rows_length = length(max_matrix(:, 1));
    n_max_matrix = zeros(n, 2);
    for i = 1 : n
       n_max_matrix(i, 1) = max_matrix(max_matrix_rows_length+1-i, 1);
       n_max_matrix(i, 2) = max_matrix(max_matrix_rows_length+1-i, 2);
    end
    %now the last n rows give the n highest peak values and indexes, 
    %in descending order
    %sort according to the indexes for plotting, and return
    n_max_matrix = sortrows(n_max_matrix, 2);

    [maximum, index] = max(n_max_matrix(:,1));
    index_f = n_max_matrix(index,2)*(Fs/N)/2;

    for i = 1 : n
        difference = mod(n_max_matrix(i, 2)*(Fs/N), index_f);
        if difference > index_f/2
            difference = index_f - difference;
        end
        if difference > index_f/9        
            n_max_matrix(i, 1) = 0;
        end 
        if n_max_matrix(i, 2)*(Fs/N) < index_f
            n_max_matrix(i, 1) = 0;
        end
    end

    %sort according to the amps
    n_max_matrix = sortrows(n_max_matrix, 1);
    temp = n_max_matrix;
    for i = 1 : n
        if n_max_matrix(i,1) == 0
            temp = n_max_matrix(i+1:end,:);   
        end
    end
    n_peaks = temp;
    peaks = n_peaks;
    
    for i = 1 : length(n_peaks);
        if n_peaks(i,1) == 0
            peaks = n_peaks(i:end, :);
        end
    end
end