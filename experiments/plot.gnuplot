set terminal pdfcairo enhanced font 'serif,13' size 8,4
set output 'performance.pdf'
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set key outside
# set xtics rotate by 90
set ylabel 'Normalized speedup'
set yrange [0:4]
# set title "Figure 8. Performance"

# Read data from CSV file
set datafile separator ","
plot 'performance.csv' using ($2/$2):xtic(1) title 'INORDER', \
     '' using ($2/$3) title 'DAE', \
     '' using ($2/$4) title 'SPEC_DAE', \
     '' using ($2/$5) title 'ORACLE'
     