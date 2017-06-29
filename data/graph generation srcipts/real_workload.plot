set terminal postscript eps enhanced color font 'Helvetica,18'
set output 'unified.eps' 
set key out
#set size square 1.5,1
set style data histograms
set style histogram rowstacked
set boxwidth 0.8
set style fill solid border -3
set ylabel "Cache Occupancy (in blocks)"
set xlabel "Time (in seconds)"
set xtics font ", 11"
set yrange[0:280000]
set xtics 30

plot 'ouput' using 2 t "Container 1" lt rgb "#99ccff" , '' using 3 t "Container 2" lt rgb "#333399", '' using 4 t "Container 3"  lt rgb "#ffd11a", '' using 5:xtic(1) t "Container 4"  lt rgb "#009933" 
