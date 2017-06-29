set terminal postscript eps enhanced color font 'Helvetica,18'
set output '10000.eps' 
set key left top
set style data histograms
set style histogram rowstacked
set boxwidth 1
set style fill solid 1 border -0.75
set ylabel "Cache Occupancy (in blocks)"
set xlabel "Time (in seconds)"
set xtics font ", 11"
set yrange[0:280000]
plot '10000' using 2 t "Container 1" , '' using 3:xtic(1) t "Container 2" 
