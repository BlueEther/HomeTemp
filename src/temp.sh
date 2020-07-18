#!/bin/bash
head -1 /var/www/wordpress/HomeTemp/temp.csv > /var/www/wordpress/HomeTemp/temp60min.csv
tail -120 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp60min.csv

head -1 /var/www/wordpress/HomeTemp/temp.csv > /var/www/wordpress/HomeTemp/latest.csv
tail -1 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/latest.csv

sed -i 1,3d /var/www/wordpress/HomeTemp/temp2.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp2.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp2.csv

sed -i 1,3d /var/www/wordpress/HomeTemp/temp6.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp6.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp6.csv

sed -i 1,3d /var/www/wordpress/HomeTemp/temp12.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp12.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp12.csv

sed -i 1,3d /var/www/wordpress/HomeTemp/temp24.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp24.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp24.csv

sed -i 1,3d /var/www/wordpress/HomeTemp/temp48.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp48.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp48.csv


sed -i 1,3d /var/www/wordpress/HomeTemp/temp7d.csv
sed -i '1 i\Time,Deck,Lounge,BaseStation,Ground_air,Ground_30cm,Bed_Cady,Bed_Ismay,Bed_Master,Ceiling' /var/www/wordpress/HomeTemp/temp7d.csv
tail -2 /var/www/wordpress/HomeTemp/temp.csv >> /var/www/wordpress/HomeTemp/temp7d.csv