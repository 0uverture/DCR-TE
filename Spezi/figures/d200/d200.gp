set xlabel 'cluster size'
set ylabel 'exec/s'

unset key
set grid
set tics out nomirror
set border 3 front

set xrange [0:6]
set yrange [0:3]

set datafile separator ";"
set terminal pdf
set output 'd200.pdf'
plot 'd200.csv' using 1:2 pointtype 2
