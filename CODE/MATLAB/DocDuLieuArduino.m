function DocDuLieuArduino(port, baud)
% READ_WEATHER_SERIAL  Đọc dữ liệu nhiệt độ/độ ẩm từ Arduino và in ra Command Window.
% 
%   Cách dùng:
%       DocDuLieuArduino("COM6", 9600)   % chỉ định port & baud cụ thể
%       DocDuLieuArduino()               % chọn port & baud từ menu
%
%   Arduino gửi dạng:
%       Serial.print("T:");
%       Serial.print(t,1);
%       Serial.print(",H:");
%       Serial.println(h,1);  % println => CR/LF

    % --- Chọn cổng COM & Baud rate ---
    if nargin < 1 || isempty(port)
        port = Chon_cong_serial();
    end

    if nargin < 2 || isempty(baud)
        baud = Chon_Baud_rate();
    end

    % --- Mở cổng Serrial ---
    sp = [];
    try
        sp = serialport(port, baud);
        configureTerminator(sp, "CR/LF");   % đổi "LF" nếu Arduino chỉ gửi '\n'
        flush(sp);
        c = onCleanup(@() cleanup(sp));

        fprintf('\nĐã kết nối tới %s @ %d baud.\n', port, baud);
        fprintf('Nhấn Ctrl+C để dừng đọc dữ liệu.\n\n');

        % --- Đọc dữ liệu ---
        t0 = datetime('now');
        while true
            if sp.NumBytesAvailable > 0
                line = strtrim(string(readline(sp)));         % đọc 1 dòng
                data = sscanf(char(line), 'T:%f,H:%f');       % Tách chuỗi "T:xx,H:yy"
                if numel(data) == 2
                    T = data(1); H = data(2);
                    t = seconds(datetime('now') - t0);
                    fprintf('[%6.1fs]  Nhiệt độ: %5.1f °C   |   Độ ẩm: %5.1f %%\n', t, T, H);
                end
            else
                pause(0.05); % Tránh bị chiếm CPU
            end
        end

    catch ME
        if ~isempty(sp) && isvalid(sp)
            cleanup(sp);
        end
        rethrow(ME);
    end
end

% --- Hàm chọn cổng COM ---
function port = Chon_cong_serial()
    available = serialportlist("available");
        if isempty(available)
            error('Không thấy cổng COM khả dụng.');
        end
        fprintf('\n=== DANH SÁCH CỔNG KHẢ DỤNG ===\n');
        for k = 1:numel(available)
            fprintf('  %d) %s\n', k, available(k));
        end
        idx = input(sprintf('Chọn cổng (1-%d): ', numel(available)));
        if isempty(idx) || idx < 1 || idx > numel(available)
            error('Lựa chọn không hợp lệ.');
        end
        port = available(idx);
    end

% --- Hàm chọn Baud Rate ---
function baud = Chon_Baud_rate()
    list = [9600, 19200, 38400, 57600, 115200];
    fprintf('\n=== CHỌN BAUD RATE ===\n');
    for i = 1:numel(list)
        fprintf('  %d) %d\n', i, list(i));
    end

    idx = input(sprintf('Chọn (1-%d): ', numel(list)));
    if isempty(idx) || idx < 1 || idx > numel(list)
        baud = 9600;
        fprintf('Giá trị Baud không hợp lệ, dùng mặc định %d\n', baud);
    else
        baud = list(idx);
    end
end

% --- Đóng cổng Serial ---
function cleanup(sp)
    if ~isempty(sp) && isvalid(sp)
        fprintf('Đang đóng cổng serial...\n');
        delete(sp);
    end
end
