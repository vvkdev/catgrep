#!/bin/bash
clear
rm -f log.txt
success=0
failed=0
rez=rez.txt
myrez=myrez.txt

## NO ARGS
cmd="cat"
out="'$(eval "./$cmd" 2>&1)'"
expected="''"
if [ "$out" = "$expected" ]
then
  printf "%05d SUCCESS = %s\n" $((success++)) $cmd
else
  ((failed++))
  echo "FAIL = $cmd"$'\n'"OUT   : $out"$'\n'"EXPECTED: $expected"$'\n'
  exit
fi

for file1 in "" files/*; do
  for file2 in files/* "fileX"; do
    for b in "" "-b" "--number-nonblank"; do
      for e in "" "-e"; do
        for E in "" "-E"; do
          for n in "" "-n" "--number"; do
            for s in "" "-s" "--squeeze-blank"; do
              for t in "" "-t"; do
                for T in "" "-T"; do
                  #for v in "" "-v" "--show-nonprinting"; do
                    opt=$(echo "$b $e $E $n $s $t $T $v $file1 $file2" | tr -s ' ')
                    ./cat $opt > $myrez 2>/dev/null
                    cat $opt > $rez 2>/dev/null
                    cmp="'$(eval "diff -s $myrez $rez" 2>&1)'"
                    if [[ $cmp == *"identical"* || $cmp == *"идентичны"* ]]
                    then
                      #printf "%05d SUCCESS = %s = %s\n" $((success)) "$opt" "$cmp" >> log.txt
                      printf "%05d SUCCESS = %s = %s\n" $((++success)) "$opt" "$cmp"
                    else
                      printf "%05d FAIL = %s\n" $((failed)) "$opt" >> log.txt
                      printf "%05d FAIL = %s\n" $((++failed)) "$opt"
                      echo "./cat $opt > $myrez & cat $opt > $rez & diff -s $rez $myrez"
                    fi
                  #done
                done
              done
            done
          done
        done
      done
    done
  done
done

echo SUCCESS=$((success)) FAILED=$((failed))
rm -f $myrez $rez
clang-format -n *.c *.h
cppcheck --enable=all --suppress=missingIncludeSystem ./*.c
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./cat
