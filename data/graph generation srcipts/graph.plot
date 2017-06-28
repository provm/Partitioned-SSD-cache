set terminal png size 600,500 enhanced font "Helvetica,10"
set output '/home/shyamli/request_pattern.png'
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set ylabel 'Frequency'
set xlabel 'Blocks'
set title 'Request pattern'
set xrange[0:3000000000]
bin_width = 10;
bin_number(x) = floor((x)/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 10 )

plot 'requests' using (rounded($1)):(1) smooth frequency with boxes
