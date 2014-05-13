#!/bin/bash

### Description: AndroGuard Functionality
###		This was refactored out of the toolScript to make that script cleaner

### Todo: put Name of SQLite DB into a String
###		Test Log files

logLocation=logs/androguard.log
date1=$(date +"%s")
echo "AndroGuard Start:" `date` >> $logLocation

## Create the log
touch $logLocation

echo "Starting Androguard"

mkdir -p logs/AndroRiskOutput

# Check to make sure that an argument is actually passed in
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ] 
then
	echo "Androguard requires 1 argument, the path to the location of the apk files"
fi

#The $1 is the given argument for the location of the APKs
FILES=$(find $1 -type f -name '*.apk')
for f in $FILES
do
    fileName=$(basename $f) 
  
	echo "****Starting AndroGuard for:" $fileName `date` >> $logLocation
  
	OUTPUT_FILE=./logs/AndroRiskOutput/$fileName_AndroRisk.log

	pushd ./tools/androguard

	./androrisk.py -m -i ../../$f &>> $OUTPUT_FILE 

	while read line;
	do
	
		if [[ $line == *VALUE* ]]
		then
			echo FUZZY RISK VALUE ${line#VALUE}
			cd ../../

			APKFile=$(basename $f)
			APKFile=${APKFile//.apk/""} ### Remove the apk exension from the apkID
	
			#select from apk information the row id where apkid = filename
			rowid=`sqlite3 Evolution\ of\ Android\ Applications.sqlite  "SELECT rowid FROM ApkInformation WHERE ApkId='$APKFile';"`
			
			fuzzyRiskNum=${line#VALUE}   #I am truncating the fuzzy risk number and making it an int
  			fuzzyRiskfloat=${fuzzyRiskNum/.*}
  			fuzzyRiskint=$((fuzzyRiskfloat))
  			
			echo "****Fuzzy Risk :" ${f#$PATH_TWO} ":" $fuzzyRiskint `date` >> $logLocation
			
			### Check to see if the APKID exists in tool results  		
			APKToolResultsCount=`sqlite3 Evolution\ of\ Android\ Applications.sqlite  "SELECT count(*) FROM ToolResults WHERE rowid='$rowid';"`
  			if [[ $APKToolResultsCount -eq 0 ]]; then
				sqlite3 Evolution\ of\ Android\ Applications.sqlite  "INSERT INTO ToolResults (ApkId,FuzzyRiskValue) VALUES ($rowid,$fuzzyRiskint);"
  			else
      			sqlite3 Evolution\ of\ Android\ Applications.sqlite  "UPDATE ToolResults SET FuzzyRiskValue=$fuzzyRiskint WHERE ApkId=$rowid;"
       		fi
			cd ./tools/androguard
		elif [[ $line == ERROR* ]]
		then

			echo FUZZY RISK $line
		fi
	done < $OUTPUT_FILE
	
        echo "********AndroAPKInfo for ${f#$PATH_TWO} ***********"
        ./androapkinfo.py -i $f &>> $OUTPUT_FILE 
	echo
done

echo "AndroGuard Completed"

date2=$(date +"%s")
diff=$(($date2-$date1))
echo "AndroGuard Total Running Time $(($diff / 60)) minutes and $(($diff % 60)) seconds."  >> ../../$logLocation
echo "AndoGuard End:" `date` >> ../../$logLocation