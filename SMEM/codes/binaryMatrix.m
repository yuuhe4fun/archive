clear;
num0 = 0;
num1 = 0;
num2 = 0;
num3 = 0;
num4 = 0;
num5 = 0;
num6 = 0;
num7 = 0;
% LSB
% a = [1 1 0 1 0 0 1 1 0 1 0 0 1 1 0 1 0 0 1];
a = 0b1101001101001101001;
% b = [0 1 1 1 0 1 0 1 1 1 0 1 0 1 1 1 0 1 0];
b = 0b0111010111010111010;
% MSB
% c = [0 1 1 0 1 0 0 1 1 0 1 0 0 1 1 0 1 0 0];
c = 0b0110100110100110100;
% for
for i = 0b0000000000000000000:0b1111111111111111111
    % LSB
    addr0 = bitget(bitand(i,a),19:-1:1);
    addr0_xor = addr0(1);
    for j = 2:19
        addr0_xor = bitxor(addr0(j),addr0_xor);
    end
    
    addr1 = bitget(bitand(i,b),19:-1:1);
    addr1_xor = addr1(1);
    for j = 2:19
        addr1_xor = bitxor(addr1(j),addr1_xor);
    end
    
    % MSB
    addr2 = bitget(bitand(i,c),19:-1:1);
    addr2_xor = addr2(1);
    for j = 2:19
        addr2_xor = bitxor(addr2(j),addr2_xor);
    end
    
    select = addr2_xor * 2^2 + addr1_xor * 2 +addr0_xor;
    switch select
        case 0
            num0 = num0 + 1;
        case 1
            num1 = num1 + 1;
        case 2
            num2 = num2 + 1;
        case 3
            num3 = num3 + 1;
        case 4
            num4 = num4 + 1;
        case 5
            num5 = num5 + 1;
        case 6
            num6 = num6 + 1;
        case 7
            num7 = num7 + 1;
    end
end