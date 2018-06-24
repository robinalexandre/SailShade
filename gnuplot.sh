cd Results
rm results.txt
ls *.gpd >> results.txt



cat results.txt | while read line
do
   name_file=$line
   name_plot=${line%.*}

   name_best=$name_plot"best"
   name_average=$name_plot"average"
   name_worst=$name_plot"worst"

   name_plot=$name_plot".jpg"

   rm plot.gps

   echo "set terminal jpeg" >> plot.gps
   echo "set output \"$name_plot\"" >> plot.gps
   echo "set title ' Fitness en fonction du nombre itérations '" >> plot.gps
   echo "set xlabel ' nombre itérations'" >> plot.gps
   echo "set ylabel ' Fitness '" >> plot.gps
   echo "plot \"$name_file\" using 1:2 with lines title \"best\", \"$name_file\" using 1:3 with lines title \"average\", \"$name_file\" using 1:4 with lines title \"worst\"" >> plot.gps

   gnuplot plot.gps
done

