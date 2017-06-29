#set terminal pngcairo dashed
#set term postscript eps size 500,500 enhanced color black "Helvetica" 14
set terminal postscript eps enhanced color font 'Helvetica,18'
set output 'expt1_delay.eps'
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set style func linespoints
set ylabel 'Workload completion time (in seconds)'
set xlabel 'Sigma of IO requests'
set xtics rotate by 45 offset -0.7,-2.3
set title 'Unified vs partitioned SSD cache with varying workload'
#set xrange[0:600000]

plot "delay_0" using 1:2  lw 5 with lines title "Partitioned SSD cache"  , "delay_0"  using 1:3 lw 5 with lines title "Unified SSD cache"

#plot "delay_0" using 1:2 lt -1 pi -6 pt 7  with lines title "Partitioned SSD cache"  , "delay_0"  using 1:3 pi -6 pt 7 with lines title "Unified SSD cache", "delay_1000" using 1:2  with lines title "Partitioned SSD cache1", "delay_1000" using 1:3  with lines title "Unified SSD cache1" 


