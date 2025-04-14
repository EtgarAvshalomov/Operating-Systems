echo "Start"
mkdir $1
touch $1/etgar.txt
head -n 10 $2 > $1/etgar.txt
tail -n 10 $2 >> $1/etgar.txt
chmod a-w $1/etgar.txt
echo "End"
