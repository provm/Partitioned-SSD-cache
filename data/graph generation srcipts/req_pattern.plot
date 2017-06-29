#set terminal png size 600,500 enhanced font "Helvetica,15"
set terminal postscript eps enhanced color font 'Helvetica,24'
set output image.'.eps'
unset key
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set ylabel 'Frequency of access'
set xlabel 'Block number accessed'
set title 'IO Request pattern with sigma '.sigma
set xrange[0:2500000000]
bin_width = 5;
bin_number(x) = floor((x)/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 5 )

plot filename using (rounded($1)):(1) smooth frequency with boxes



