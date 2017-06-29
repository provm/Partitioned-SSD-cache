#set terminal pngcairo dashed
#set terminal pngcairo  size 600,500 enhanced font "Helvetica,13" 
set terminal postscript eps enhanced color font 'Helvetica,24'
unset key
set output 'HRC2.eps'
#set output 'latency.eps'
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set style func linespoints
set ylabel 'Cache hit rate'
#set ylabel 'Completion time (in seconds)'
set xlabel 'Cache size (in MB)'
#set xtics rotate by 45 offset -0.8,-1.8
set title 'Hit rate curve'
#set title 'Latency curve'
#set xrange[0:600000]

plot "miss_rate2" using 1:2 lw 5  with lines 

#plot "miss_rate2" using 1:3  lw 5 with lines 


