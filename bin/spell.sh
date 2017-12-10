#!/bin/bash

VERSION=0.1.0
SUBJECT=some-unique-id
USAGE="use: spell.sh -h to know more about this command"

NARGS=$#
args=("$@") 

# ---- functions ------------------------

display_help(){
	echo "./spell.sh -h"
	echo ""
	echo "       Usage: ./spell.sh TARGET [TARGET [...]]"
	echo "       TARGET can be either a file or a directory. If a directory is"
	echo "       given as a TARGET, spell will run on every (non-code) ASCII"
	echo "       file in that directory"
	echo ""
	echo "              -h: display this help"
	echo "              -r: if TARGET is a directory and the -r option is used,"
	echo "                  spell will recurse through directories too"
	echo "              -v: be more verbose"
	echo ""
	echo ""
	exit 1
}

verif(){

	path=$1
	if [[ -d $path ]]; then
	for ff in $path/*
    	do      
	       	echo $path":    not a regular text file, skipping" ;
	       	cd $path
	       	verif $ff
	       	cd ..
	   	done
	else
		if [[ $path =~ "../rsc/words_en.txt" || $path =~ "./../rsc/words_en.txt" ]]; then
			echo $path":    Skipping dictionnary"
		else
				attachmenttype=$(file $path | cut -d\  -f2)
			if [[ $attachmenttype = "ASCII" ]]; 
			then 
				echo $path":    Added for parsing ..." ;
			else 
				echo $path":    not a regular text file, skipping" ;
			fi
		fi
	fi
}

list_files(){

	echo $1
	for f in ${args[*]}
	do
		if [[ $f != "-vr" && $f != "-rv" && $f != "./spell.sh" && $f != "spell.sh" ]]; then
			p=$f
			path=$(pwd)"/"$p

			verif $path
		fi
		


	#			path=$(pwd)"/"${!i}
	#			if [[ -d $path ]]; then
	#				#statements
	#				for k in *
 	#				do
	#					cd ${!k}
	#					list_files $k
	#				done
	#			else
	#				for j in *
 	#				do
 	#					if test -d $j #if dictionary
 	#					then 
 	#						list_files $j #recursively list files
	#						cd ..
 	#					else
 	#						if [[ ${!j} = "../rsc/words_en.txt" || ${!j} = "./../rsc/words_en.txt" ]]; then
	#							echo ${!j}":    Skipping dictionnary"
	#						else
	#							attachment=$(pwd)"/"${!j}
	#							attachmenttype=$(file $attachment | cut -d\  -f2)
	#							if [[ $attachmenttype = "ASCII" ]]; 
	#							then 
	#								echo ${!j}":    Added for parsing ..." ;
	#							else 
	#								echo ${!j}":    not a regular text file, skipping" ;
	#							fi
	#						fi
	#					fi
	#				done
	#			fi
	done
}

# --- Options processing -------------------------------------------
if [ $# == 0 ] ; then
    echo $USAGE
    exit 1;
fi

while :
do
    case "$1" in

    	-h | --help)
          display_help  # Call your function
          exit 0
          ;;

        -v)
			for i in $(seq 2 $#); do
				if [[ ${!i} = "../rsc/words_en.txt" || ${!i} = "./../rsc/words_en.txt" ]]; then
					echo ${!i}":    Skipping dictionnary"
				else
					attachment=$(pwd)"/"${!i}
					attachmenttype=$(file $attachment | cut -d\  -f2)
					if [[ $attachmenttype = "ASCII" ]]; 
					then 
						echo ${!i}":    Added for parsing ..." ;
					else 
						echo ${!i}":    not a regular text file, skipping" ;
					fi
				fi
			done

		  exit 1
		  ;;

		 -rv | -vr)
			list_files
		  exit 1
		  ;;

        --) # End of all options
          shift
          break
          ;;
      
      	-*)
          echo "Error: Unknown option: $1" >&2
          ## or call function display_help
          exit 1 
          ;;
      	
      	*)  # No more options

          break
          ;;
    esac
done