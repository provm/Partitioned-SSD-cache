#set terminal pngcairo dashed
#set terminal pngcairo  size 600,500 enhanced font "Helvetica,13" 
set terminal postscript eps enhanced color font 'Helvetica,18'
set output 'expt2_delay_last.eps'
set boxwidth 0.05 absolute
set style fill solid 1.0 noborder
set style func linespoints
set ylabel 'Workload completion time (in seconds)'
set xlabel 'IOPS'
#set xtics rotate by 45 offset -0.8,-1.8
set title 'Unified vs partitioned SSD cache with varying IOPS'
#set xrange[0:600000]
set offsets graph 0.05, 0, 0, 0
plot "new" using 4:xtic(1)  lw 5 with lines title "Partitioned SSD cache"  , "new"  using 5:xtic(1) lw 5 with lines title "Unified SSD cache"

#plot "delay_0" using 1:2 lt -1 pi -6 pt 7  with lines title "Partitioned SSD cache"  , "delay_0"  using 1:3 pi -6 pt 7 with lines title "Unified SSD cache", "delay_1000" using 1:2  with lines title "Partitioned SSD cache1", "delay_1000" using 1:3  with lines title "Unified SSD cache1" 


