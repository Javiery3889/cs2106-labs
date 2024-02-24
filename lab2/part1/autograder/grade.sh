#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
    echo "Usage: ./grade.sh <filename>"
    exit 1
fi

executable_name=$1

# Delete temporary files
out_files=$(ls ref/*.out 2> /dev/null)
if [[ -n "$out_files" ]]; then
    rm ref/*.out
fi

# Compile the reference program
gcc ref/*.c -o "ref/$executable_name"

# Generate reference output files
no_of_files=0
for input in ref/*.in; do
    ref/fun < $input > "$input.out"
    ((no_of_files++))
done

# Now mark submissions
#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#
# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file
    # Generate output from C code using *.in files in ref
    # Compare with reference output files  and award 1 mark if they are identical
# print score for student
# print total files marked.
result_file="results.out"
day=$(date +%A)
date=$(date +"%d %B %Y")
time=$(date +%T)
echo -e "Test date and time: $day, $date, $time\n" > $result_file 

max_score=$no_of_files
for dir in subs/*
do
    score=0
    student_no=${dir##*/}   # get everything after the final "/"
    gcc $dir/*.c -o "$dir/$1" 2> /dev/null

    if [[ $? -ne 0 ]]; then
        echo "Directory $student_no has a compile error." >> $result_file 
        echo "Directory $student_no score 0 / $max_score" >> $result_file
        continue
    fi

    for input in ref/*.in; do
        input_filename=${input##*/}
        student_outfile="ref/$student_no"_"$input_filename.out"
        "$dir/$executable_name" < $input > $student_outfile
        cmp -s "$input.out" $student_outfile
        if [[ $? -eq 0 ]]; then
            ((score++))
        fi
    done

    echo "Directory $student_no score $score / $max_score" >> $result_file
done

echo -e "\nProcessed $no_of_files files." >> $result_file

