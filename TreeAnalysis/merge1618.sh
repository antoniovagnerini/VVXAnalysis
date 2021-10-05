#!/bin/bash

##################################################
#  Merges results from all the years             #
#                                                #
#  Author: A. Mecca (alberto.mecca@cern.ch)      #
#  Co-author: E. Racca (eleonora.racca@cern.ch)  #
##################################################

# ~~~~~ Setting options
haddOpt="-k -f -j $(grep processor /proc/cpuinfo | wc -l)"
rmOpt="-r -f"


cd results


# ~~~~~ Removing previous files
rm $rmOpt 1618

for y in 2016 2017 2018 ; do
	rm $rmOpt $y/ZZjjAnalyzer_CR
done


# ~~~~~ Hadd-ing results
# array of analyses and regions names
exist_an_reg=$(echo $(ls 2016) $(ls 2017) $(ls 2018) | tr ' ' '\n' | sort -u | tr '\n' ' ')
#echo $exist_an_reg


# hadd-ing ggTo4l results and removing hadd-ed files
echo "--- Hadd-ing gg -> ZZ results ---"
echo "  " 

for y in 2016 2017 2018 ; do	
	for an_reg in $exist_an_reg ; do
		
		#echo "--- + ---" $y " -> " $an_reg
		[ -d $y/$an_reg ] || continue
		rm $rmOpt $y/$an_reg/gg.root
		rm $rmOpt $y/$an_reg/WZ.root
		
		hadd $haddOpt $y/$an_reg/gg.root $(find $y/$an_reg -type f -name "ggTo*.root")
	done
done


# array of results file names
exist_sam=$(find ./201*/ -type f -name "*.root" | grep -oP "[^/]+\.root" | grep -oP "[^\.]+(?=\.root)" | grep -v "ggTo" | sort | uniq)
#echo $exist_sam


# hadd-ing results by year and by analysis
echo "  " 
echo "--- Hadd-ing results into 1618 ---"
echo "  " 
for an_reg in $exist_an_reg ; do
	mkdir -p 1618/$an_reg
 
	for sam in $exist_sam ; do
		hadd $haddOpt 1618/$an_reg/$sam.root 2016/$an_reg/$sam.root 2017/$an_reg/$sam.root 2018/$an_reg/$sam.root
	done
done


# hadd-ing control regions
echo "  " 
echo "--- Creating CR for all years ---"
echo "  " 
for y in 2016 2017 2018 1618 ; do
	mkdir -p $y/ZZjjAnalyzer_CR

	for sam in $exist_sam ; do
		hadd $haddOpt $y/ZZjjAnalyzer_CR/$sam.root $y/ZZjjAnalyzer_CR3P1F/$sam.root $y/ZZjjAnalyzer_CR2P2F/$sam.root
	done
done


cd ..
