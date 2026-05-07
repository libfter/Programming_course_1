set terminal pngcairo size 1000,700 enhanced
set output 'matrix_time.png'
set grid
set title "Зависимость времени перемножения матриц от размера и числа потоков"
set xlabel "Размер матрицы N"
set ylabel "Время (мс)"
set key left top
set yrange [0:140000]

plot "data.txt" using 1:2 with linespoints title "1 поток", \
     "data.txt" using 1:3 with linespoints title "2 потока", \
     "data.txt" using 1:4 with linespoints title "4 потока", \
     "data.txt" using 1:5 with linespoints title "8 потоков", \
     "data.txt" using 1:6 with linespoints title "16 потоков", \
     "data.txt" using 1:7 with linespoints title "32 потока", \
     "data.txt" using 1:8 with linespoints title "64 потока", \
     "data.txt" using 1:9 with linespoints title "128 потоков"
