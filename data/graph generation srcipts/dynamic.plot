#set terminal pngcairo dashed
#set term postscript eps size 500,500 enhanced color black "Helvetica" 14
set terminal postscript eps enhanced color font 'Helvetica,18'
set output 'dynamic.eps'
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set style func linespoints
set xlabel 'Time (in minutes)'
set ylabel 'Predicted cache size'
set title 'Predicting working set size'
#set xrange[0:600000]

plot "res1" using 1:2  lw 5 with lines title "Predicted SSD cache size"
