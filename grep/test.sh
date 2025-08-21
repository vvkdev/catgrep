#!/bin/bash
clear
rm -f log.txt
success=0
failed=0
rez=rez.txt
myrez=myrez.txt


compare() {
    "./grep" $1 > $myrez 2>/dev/null
    "grep" $1 > $rez 2>/dev/null
    cmp="'$(eval "diff -s $myrez $rez" 2>&1)'"
    if [[ $cmp == *"identical"* || $cmp == *"идентичны"* ]]
    then
      printf "%05d SUCCESS = %s = %s\n" $((++success)) "$1" "$cmp"
      #printf "%05d SUCCESS = %s = %s\n" $((success)) "$1" "$cmp" >> log.txt
    else
      printf "%05d FAIL = %s\n" $((failed)) "$1" >> log.txt
      printf "%05d FAIL = %s\n" $((++failed)) "$1"
      #echo ./grep "$1" && echo grep "$1"
      #echo "./grep $1 > $myrez & grep $1 > $rez & diff -s $rez $myrez"
    fi
}

for f1 in "-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-f" "-o"; do
  for f2 in "" "-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-f" "-o"; do

    opt=$(echo "$f1" "$f2" str file | tr -s ' ') && compare "$opt"
    opt=$(echo str file "$f1" "$f2" | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" BAE file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" VERT file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [a-z] file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [[:digit:]] file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [a-z] file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [[:digit:]] file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -e [[:digit:]] file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns2 file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns file | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns -f patterns2 file | tr -s ' ') && compare "$opt"

    opt=$(echo "$f1" "$f2" str file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo str file file2 "$f1" "$f2" | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" BAE file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" VERT file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [a-z] file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [[:digit:]] file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [a-z] file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [[:digit:]] file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -e [[:digit:]] file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns2 file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns file file2 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns -f patterns2 file file2 | tr -s ' ') && compare "$opt"

    opt=$(echo "$f1" "$f2" str file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo str file file3 "$f1" "$f2" | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" BAE file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" VERT file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [a-z] file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" [[:digit:]] file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [a-z] file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e [[:digit:]] file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -e [[:digit:]] file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -f patterns2 file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns file file3 | tr -s ' ') && compare "$opt"
    opt=$(echo "$f1" "$f2" -e str -f patterns -f patterns2 file file3 | tr -s ' ') && compare "$opt"

  done
done

echo SUCCESS=$((success)) FAILED=$((failed))
rm -f $myrez $rez
clang-format -n *.c *.h
cppcheck --enable=all --suppress=missingIncludeSystem ./*.c
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./grep
